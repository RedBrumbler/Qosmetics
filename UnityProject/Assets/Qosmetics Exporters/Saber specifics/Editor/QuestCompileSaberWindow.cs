using Qosmetics;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics.Eventing.Reader;
using System.IO;
using System.Linq;
using UnityEditor;
using UnityEditor.SceneManagement;
using UnityEditorInternal;
using UnityEngine;
using UnityEngine.UI;

public class QuestCompileSaberWindow : EditorWindow
{
    //Developed by RedBrumbler, originally very much based on the PC Beat Saber custom saber exporter project

    private SaberReferences[] questsabers;
    private string extension = "qsaber";
    private string prefabName = "_CustomSaber";
    private string questPath = "/sdcard/ModData/com.beatgames.beatsaber/Mods/Qosmetics/sabers/";
    private ExportersConfig exporterConfig;
    private string exporterConfigPath = "Assets/Qosmetics Exporters/exportersConfig.json";
    private Camera cam;
    private bool debug = false;
    private bool upload = false;
    private GameObject trail;
    private GameObject saberTemplate;
    private GameObject wallParticles;
    private GameObject sliceParticles;

    [MenuItem("Tools/Qosmetics/Exporters/QuestSaber")]
    public static void ShowWindow()
    {
        EditorWindow.GetWindow(typeof(QuestCompileSaberWindow), false, "QuestSaber Exporter");
    }

    void OnGUI()
    {
        
        GUILayout.BeginHorizontal();
        GUILayout.Label("Quest Sabers", EditorStyles.boldLabel);

        if (GUILayout.Button("Go to saber scene"))
        {
            EditorSceneManager.OpenScene("Assets/scenes/questsabers.unity");
            OnFocus();
        }

        if (GUILayout.Button("Add New Saber Template"))
        {
            AddSaberTemplate();
        }
        GUILayout.EndHorizontal();

        GUILayout.Space(5);
        if (GUILayout.Button("Export all Active sabers"))
        {
            ExportAll();
        }
        if (GUILayout.Button("Export and Upload all Active sabers"))
        {
            ExportAllAndUpload();
        }

        GUILayout.Space(10);

        foreach (var saber in questsabers)
        {
            if (saber == null) continue;
            GUILayout.Label("GameObject : " + saber.gameObject.name, EditorStyles.boldLabel);
            saber.descriptorFolded = EditorGUILayout.BeginFoldoutHeaderGroup(saber.descriptorFolded, "Saber Descriptor");

            if (saber.descriptorFolded)
            {
                GUILayout.Space(5);
                DescriptorDisplay(saber);
            }
            EditorGUILayout.EndFoldoutHeaderGroup();

            GUILayout.Space(5);

            saber.configFolded = EditorGUILayout.BeginFoldoutHeaderGroup(saber.configFolded, "Saber config");
            if (saber.configFolded)
            {
                SaberConfigDisplay(saber);
            }
            EditorGUILayout.EndFoldoutHeaderGroup();

            EditorGUILayout.Space(10);
            bool disableExport = shouldDisableExport(saber);

            if (disableExport && exporterConfig.forceAllowExport) GUILayout.Label("WARNING! force allow export is set to true so this saber file is unlikely to work correctly in game!", EditorStyles.boldLabel);
            EditorGUI.BeginDisabledGroup(disableExport && !exporterConfig.forceAllowExport);
            if (GUILayout.Button("Export " + saber.descriptor.objectName + "." + extension))
            {
                ExportSaberFile(saber);
            }

            if (GUILayout.Button("Export and Upload " + saber.descriptor.objectName + "." + extension + " To Quest"))
            {
                ExportAndUpload(saber);
            }
            EditorGUI.EndDisabledGroup();

            if (QosmeticUtils.GetObjectBounds(saber.gameObject).extents.z * 2.0 > 2.0)
            {
                GUILayout.Label("The saber might be too long", EditorStyles.boldLabel);
            }

            if (QosmeticUtils.GetObjectBounds(saber.gameObject).extents.z * 2.0 < 0.5)
            {
                GUILayout.Label("The saber might be too short", EditorStyles.boldLabel);
            }

            if (QosmeticUtils.GetObjectBounds(saber.gameObject).extents.x * 2.0 > 1.0)
            {
                GUILayout.Label("The saber might be too large", EditorStyles.boldLabel);
            }

            if (debug)
            {
                if (GUILayout.Button("Log config json"))
                {
                    SetTrailConfigs(saber);

                    Debug.Log(JsonUtility.ToJson(saber.config, true));
                }
            }

        }
    }

    /// <summary>
    /// sets the hasCustomTrails bool on the saber
    /// </summary>
    void GetHasCustomTrails(SaberReferences saber)
    {
        if (saber.config.leftTrails.Count > 0 || saber.config.rightTrails.Count > 0) saber.config.hasCustomTrails = true;
    }

    void ExportAll()
    {
        string path = EditorUtility.SaveFolderPanel("Select folder to export all ." + extension + " files", "", "");
        if (path == "")
        {
            EditorUtility.DisplayDialog("Export Failed", "Path was invalid", "OK");
            return;
        }
        path += "/";
        List<string> skippedSabers = new List<string>();
        bool sabersWereSkipped = false;
        bool atLeastOneSuccessful = false;
        foreach (var saber in questsabers)
        {
            bool disableExport = shouldDisableExport(saber);

            if (!disableExport)
            {
                atLeastOneSuccessful = true;
                ExportSaberFile(saber, path);
            }
            else
            {
                sabersWereSkipped = true;
                skippedSabers.Add(saber.descriptor.objectName);
            }
        }

        if (sabersWereSkipped)
        {
            string message = "These Sabers were skipped due to exporter issues: \n";
            foreach (var saberName in skippedSabers)
            {
                message += saberName + "\n";
            }
            EditorUtility.DisplayDialog("Sabers Were skipped...", message, "OK");
        }
        else
        {
            EditorUtility.DisplayDialog("Saber exports successful!", "Exported all sabers successfully!", "OK");

        }
        if (atLeastOneSuccessful) EditorUtility.RevealInFinder(path);
    }

    /// <summary>
    /// Exports all sabers and uploads them all to the quest
    /// </summary>
    void ExportAllAndUpload()
    {
        string path = EditorUtility.SaveFolderPanel("Select folder to export all " + extension + " files", "", "");
        if (path == "")
        {
            EditorUtility.DisplayDialog("Export Failed", "Path was invalid", "OK");
            return;
        }
        path += "/";
        List<string> skippedSabers = new List<string>();
        bool sabersWereSkipped = false;
        bool atLeastOneSuccessful = false;

        if (exporterConfig.ipAddress != "") QosmeticUtils.adb("connect " + exporterConfig.ipAddress + ":5555");
        QosmeticUtils.adb("shell am force-stop com.beatgames.beatsaber");

        foreach (var saber in questsabers)
        {
            bool disableExport = shouldDisableExport(saber);

            if (!disableExport)
            {
                atLeastOneSuccessful = true;
                ExportSaberFile(saber, path);
                string saberName = saber.descriptor.objectName + "." + extension;
                QosmeticUtils.adb("push\"" + path + saberName + "\" \""+ questPath + saberName + "\"");
            }
            else
            {
                sabersWereSkipped = true;
                skippedSabers.Add(saber.descriptor.objectName);
            }
        }

        if (sabersWereSkipped)
        {
            string message = "These Sabers were skipped due to exporter issues: \n";
            foreach (var saberName in skippedSabers)
            {
                message += saberName + "\n";
            }
            EditorUtility.DisplayDialog("Sabers Were skipped...", message, "OK");
        }
        else
        {
            EditorUtility.DisplayDialog("Saber exports successful!", "Exported all sabers successfully!", "OK");
        }
        if (atLeastOneSuccessful)
        {
            QosmeticUtils.adb("shell am start com.beatgames.beatsaber/com.unity3d.player.UnityPlayerActivity");
            EditorUtility.RevealInFinder(path);
        }
    }

    bool shouldDisableExport(SaberReferences saber)
    {
        bool disableExport = saber.transform.Find("LeftSaber") == null || saber.transform.Find("RightSaber") == null;
        bool trailNotFound = false;
        bool trailNotInRoot = false;
        if (saber.config.hasCustomTrails)
        {
            if (saber.transform.Find("LeftSaber").GetComponentsInChildren<TrailReferences>().Length == 0 || saber.transform.Find("RightSaber").GetComponentsInChildren<TrailReferences>().Length == 0) trailNotFound = true;
            TrailReferences[] trails = saber.gameObject.GetComponentsInChildren<TrailReferences>();

            foreach (var trail in trails)
            {
                if (!(trail.transform.parent.gameObject.name == "LeftSaber" || trail.transform.parent.gameObject.name == "RightSaber")) trailNotInRoot = true;
            }
        }

        if (trailNotFound)
        {
            GUILayout.Label("Custom trails are enabled but there was no trail found on one or both sabers, please check that there is a trail on both sabers", EditorStyles.boldLabel);
            disableExport = true;
        }

        if (trailNotInRoot)
        {
            GUILayout.Label("One or more of your trails are not in the root LeftSaber or RightSaber object, please make sure they are", EditorStyles.boldLabel);
            disableExport = true;
        }
        return disableExport;
    }

    /// <summary>
    /// Instantiates a new saber prefab in the current scene
    /// </summary>
    void AddSaberTemplate()
    {
        if (saberTemplate != null)
        {
            GameObject temp = Instantiate(saberTemplate, null);
            temp.name = "New SaberTemplate";
            EditorSceneManager.MarkSceneDirty(EditorSceneManager.GetActiveScene());
            OnFocus();
        }
        else
        {
            Debug.LogError("Saber template was null!");
        }
    }

    /// <summary>
    /// Sets the left and right trail config arrays
    /// </summary>
    /// <param name="saber"></param>
    void SetTrailConfigs(SaberReferences saber)
    {
        // TODO add check for trailStart and TrailEnd
        TrailReferences[] leftReferences = saber.transform.Find("LeftSaber").GetComponentsInChildren<TrailReferences>();
        TrailReferences[] rightReferences = saber.transform.Find("RightSaber").GetComponentsInChildren<TrailReferences>();
        saber.config.leftTrails.Clear();
        saber.config.rightTrails.Clear();

        if (saber.config.hasCustomTrails)
        {
            if (!saber.addedTrails && trail != null)
            {

                if (leftReferences.Length == 0)
                {
                    GameObject tempTrail = Instantiate(trail);

                    tempTrail.name = "Trail";

                    tempTrail.GetComponent<TrailReferences>().colorType = TrailReferences.ColorType.leftSaber;

                    tempTrail.transform.SetParent(saber.transform.Find("LeftSaber"));
                    tempTrail.transform.localPosition = Vector3.zero;
                    tempTrail.transform.localScale = Vector3.one;
                    tempTrail.transform.localEulerAngles = Vector3.zero;
                }

                if (rightReferences.Length == 0)
                {
                    GameObject tempTrail = Instantiate(trail);

                    tempTrail.name = "Trail";

                    tempTrail.GetComponent<TrailReferences>().colorType = TrailReferences.ColorType.rightSaber;

                    tempTrail.transform.SetParent(saber.transform.Find("RightSaber"));
                    tempTrail.transform.localPosition = Vector3.zero;
                    tempTrail.transform.localScale = Vector3.one;
                    tempTrail.transform.localEulerAngles = Vector3.zero;
                }

                saber.addedTrails = true;
            }
            else if (!saber.addedTrails && trail == null)
            {
                Debug.LogError("Tried adding trail components to the custom saber but the trails prefab was null");
                saber.addedTrails = true;
            }

            foreach (var trail in leftReferences)
            {
                saber.config.leftTrails.Add(new TrailConfig(trail));
            }

            foreach (var trail in rightReferences)
            {
                saber.config.rightTrails.Add(new TrailConfig(trail));
            }
        }

        foreach (var trail in saber.gameObject.GetComponentsInChildren<TrailReferences>())
        {
            Renderer renderer = trail.gameObject.GetComponent<Renderer>();

            if (renderer == null)
            {
                renderer = trail.gameObject.AddComponent<MeshRenderer>();
            }

            renderer.sharedMaterial = trail.trailMaterial;
        }
    }

    /// <summary>
    /// Runs the code to display the descriptor settings in the GUI
    /// </summary>
    /// <param name="saber"></param>
    void DescriptorDisplay(SaberReferences saber)
    {
        saber.descriptor.objectName = EditorGUILayout.TextField("Saber name", saber.descriptor.objectName);
        saber.descriptor.authorName = EditorGUILayout.TextField("Author name", saber.descriptor.authorName);
        saber.descriptor.description = EditorGUILayout.TextField("Description", saber.descriptor.description);
    }

    /// <summary>
    /// Runs the code to display the bmbf input options in the GUI
    /// </summary>
    /// <param name="saber"></param>
    void BMBFConfigDisplay(SaberReferences saber)
    {
        QosmeticUtils.addIfNull(saber.bmbfmod);

        saber.bmbfmod.components[0].type = "FileCopyMod";
        saber.bmbfmod.components[0].targetRootedPathAndFileName = questPath + "testSaber." + extension;

        if (saber.advancedInfo) saber.bmbfmod.coverImageFilename = EditorGUILayout.TextField("Cover image name", saber.bmbfmod.coverImageFilename);
        else if (saber.bmbfmod.coverImageFilename == "" && exporterConfig.defaultCoverImageFileName != "") saber.bmbfmod.coverImageFilename = exporterConfig.defaultCoverImageFileName;
        saber.bmbfmod.icon = saber.bmbfmod.coverImageFilename;

        if (saber.advancedInfo) saber.bmbfmod.version = EditorGUILayout.TextField("Saber version", saber.bmbfmod.version);
        else if (saber.bmbfmod.version == "" && exporterConfig.defaultVersion != "") saber.bmbfmod.version = exporterConfig.defaultVersion;

        if (saber.advancedInfo) saber.bmbfmod.links.pageLink = EditorGUILayout.TextField("Page link", saber.bmbfmod.links.pageLink);
        else if (saber.bmbfmod.links.pageLink == "" && exporterConfig.defaultPageLink != "") saber.bmbfmod.links.pageLink = exporterConfig.defaultPageLink;

        saber.bmbfmod.gameVersion = EditorGUILayout.TextField("Game version", saber.bmbfmod.gameVersion);
        saber.bmbfmod.description[0] = EditorGUILayout.TextField("Description", saber.bmbfmod.description[0]);
        saber.bmbfmod.platform = "Quest";
        saber.bmbfmod.name = EditorGUILayout.TextField("Saber name", saber.bmbfmod.name);
        saber.bmbfmod.author = EditorGUILayout.TextField("Author name", saber.bmbfmod.author);
        saber.bmbfmod.category = "Saber";

        saber.bmbfmod.components[0].sourceFileName = saber.bmbfmod.name + "." + extension;

        string modID = saber.bmbfmod.name + saber.bmbfmod.author + "V" + saber.bmbfmod.version;
        modID = QosmeticUtils.sanitizeString(modID);

        saber.bmbfmod.id = modID;
    }

    /// <summary>
    /// Runs the code to display the saber input options in the GUI
    /// </summary>
    /// <param name="saber"></param>
    void SaberConfigDisplay(SaberReferences saber)
    {
        saber.config.enableFakeGlow = EditorGUILayout.ToggleLeft("Enable base game Fake Glow", saber.config.enableFakeGlow);
        saber.config.hasCustomTrails = EditorGUILayout.ToggleLeft("Has custom trails", saber.config.hasCustomTrails);
        //saber.config.hasCustomWallParticles = EditorGUILayout.ToggleLeft("Has custom Wall Particles", saber.config.hasCustomWallParticles);
        //saber.config.hasCustomSliceParticles = EditorGUILayout.ToggleLeft("Has custom Slice Particles", saber.config.hasCustomSliceParticles);

        if (saber.config.hasCustomTrails)
        {
            EditorGUILayout.Space(5);
            SetTrailConfigs(saber);

            //Some logic to display plural forms of the words correctly :)
            int leftTrailCount = saber.config.leftTrails.Count;
            int rightTrailCount = saber.config.rightTrails.Count;

            string message = "Found ";
            message += leftTrailCount.ToString();
            message += leftTrailCount > 1 ? " left Trails and " : " left Trail and ";
            message += rightTrailCount.ToString();
            message += leftTrailCount > 1 ? " right Trails" : " right Trail";

            GUILayout.Label(message, EditorStyles.boldLabel);
        }
        /*
        if (saber.config.hasCustomWallParticles && !saber.addedWallParticles)
        {
            if (saber.transform.Find("RightSaber/WallParticles")?.GetComponent<ParticleSystem>() == null)
            {
                GameObject temp = Instantiate(wallParticles);
                temp.transform.SetParent(saber.transform.Find("RightSaber"), false);
                temp.transform.localPosition = Vector3.zero;
                temp.transform.localScale = Vector3.one;
                temp.transform.localEulerAngles = Vector3.zero;
                temp.name = "WallParticles";
            }

            if (saber.transform.Find("LeftSaber/WallParticles")?.GetComponent<ParticleSystem>() == null)
            {
                GameObject temp = Instantiate(wallParticles);
                temp.transform.SetParent(saber.transform.Find("LeftSaber"), false);
                temp.transform.localPosition = Vector3.zero;
                temp.transform.localScale = Vector3.one;
                temp.transform.localEulerAngles = Vector3.zero;
                temp.name = "WallParticles";
            }

            saber.addedWallParticles = true;
        }
        else
        {
            saber.addedWallParticles = false;
        }

        if (saber.config.hasCustomSliceParticles && !saber.addedSliceParticles)
        {
            if (saber.transform.Find("RightSaber/SliceParticles")?.GetComponent<ParticleSystem>() == null)
            {
                GameObject temp = Instantiate(sliceParticles);
                temp.transform.SetParent(saber.transform.Find("RightSaber"), false);
                temp.transform.localPosition = Vector3.zero;
                temp.transform.localScale = Vector3.one;
                temp.transform.localEulerAngles = Vector3.zero;
                temp.name = "SliceParticles";
            }

            if (saber.transform.Find("LeftSaber/SliceParticles")?.GetComponent<ParticleSystem>() == null)
            {
                GameObject temp = Instantiate(sliceParticles);
                temp.transform.SetParent(saber.transform.Find("LeftSaber"), false);
                temp.transform.localPosition = Vector3.zero;
                temp.transform.localScale = Vector3.one;
                temp.transform.localEulerAngles = Vector3.zero;
                temp.name = "SliceParticles";
            }

            saber.addedSliceParticles = true;
        }
        else
        {
            saber.addedSliceParticles = false;
        }
        */
    }

    /// <summary>
    /// Exports just a qsaber file
    /// </summary>
    /// <param name="saber"></param>
    void ExportSaberFile(SaberReferences saber, string path = "")
    {
        /*
        if (EditorUserBuildSettings.activeBuildTarget.ToString() != "Android")
        {
            EditorUtility.DisplayDialog("Exportation Failed!", "Your projects build target is not Android.", "OK");
            return;
        }
        */
        bool batch = path != "";
        GameObject saberObject = saber.gameObject;
        TextAsset config = new TextAsset(JsonUtility.ToJson(saber.config, true));

        //string bmbfmodJson = JsonUtility.ToJson(saber.bmbfmod, true);
        //TextAsset bmbfmod = new TextAsset(bmbfmodJson);

        string descriptorString = JsonUtility.ToJson(saber.descriptor, true);
        TextAsset descriptor = new TextAsset(descriptorString);

        string saberName = saber.descriptor.objectName + "." + extension;
        if (!batch) path = EditorUtility.SaveFilePanel("Save " + extension + " file", "", saberName, extension);
        else path += saberName;

        if (path == "")
        {
            EditorUtility.DisplayDialog("Exportation Failed!", "Invalid path", "OK");
            return;
        }

        string fileName = Path.GetFileName(path);
        string folderPath = Path.GetDirectoryName(path);

        string workingDir = Application.temporaryCachePath + "/QuestSaber";

        if (File.Exists(path)) File.Delete(path);
        if (!Directory.Exists(workingDir)) Directory.CreateDirectory(workingDir);

        if (exporterConfig.removeCamerasAtExport) QosmeticUtils.PurgeCameras(saberObject);
        if (exporterConfig.removeLightsAtExport) QosmeticUtils.PurgeLights(saberObject);

        Selection.activeObject = saberObject;
        EditorUtility.SetDirty(saber);
        EditorSceneManager.MarkSceneDirty(saberObject.scene);
        EditorSceneManager.SaveScene(saberObject.scene);

        GameObject camObject;
        Camera exportCam = null;


        if (exporterConfig.allowOwnCamera) // manual cam
        {
            camObject = cam.gameObject;
            exportCam = cam;
            exportCam.nearClipPlane = 0.01f;
        }
        else // automatic cam:
        {
            camObject = GameObject.CreatePrimitive(PrimitiveType.Quad);
            exportCam = camObject.AddComponent<Camera>();
            exportCam.gameObject.name = "Camera";
            exportCam.transform.localPosition = new Vector3(0.6f, 0.4f, 0.05f);
            exportCam.transform.rotation = Quaternion.Euler(25f, -70f, 0.0f);
            exportCam.clearFlags = CameraClearFlags.SolidColor;
            exportCam.nearClipPlane = 0.01f;
            exportCam.backgroundColor = new Color(0.0f, 0.0f, 25.0f / 256.0f);
        }

        int oldCamMask = exportCam.cullingMask;
        int oldObjectMask = saber.gameObject.layer;
        int newMask = 31;
        int newCamMask = 1 << 31;

        if (exporterConfig.setCamLayerOnExport)
        {
            exportCam.cullingMask = newCamMask;
            QosmeticUtils.SetLayerRecursively(exportCam.gameObject, newMask);
            QosmeticUtils.SetLayerRecursively(saber.gameObject, newMask);
        }
        // moved prefab forward so that the layermask isnt fucked up on the actual exported file
        PrefabUtility.SaveAsPrefabAsset(Selection.activeGameObject as GameObject, "Assets/" + prefabName + ".prefab");

        Texture2D thumbnail = saber.thumbnail;

        if (!thumbnail) thumbnail = QosmeticUtils.getTexture2D(exportCam, 1024, 1024);
        //Start actually constructing the bundle

        AssetDatabase.CreateAsset(config, "Assets/config.asset");
        //AssetDatabase.CreateAsset(bmbfmod, "Assets/BMBFmod.asset");
        AssetDatabase.CreateAsset(thumbnail, "Assets/thumbnail.asset");
        AssetDatabase.CreateAsset(descriptor, "Assets/descriptor.asset");

        AssetBundleBuild assetBundleBuild = default(AssetBundleBuild);

        assetBundleBuild.assetNames = new string[]
        {
            "Assets/" + prefabName + ".prefab",
            "Assets/config.asset",
            //"Assets/BMBFmod.asset",
            "Assets/thumbnail.asset",
            "Assets/descriptor.asset"
        };

        assetBundleBuild.assetBundleName = fileName;

        BuildTargetGroup selectedBuildTargetGroup = EditorUserBuildSettings.selectedBuildTargetGroup;
        BuildTarget activeBuildTarget = EditorUserBuildSettings.activeBuildTarget;

        //Build Bundle
        BuildPipeline.BuildAssetBundles(workingDir, new AssetBundleBuild[] { assetBundleBuild }, 0, BuildTarget.Android);//EditorUserBuildSettings.activeBuildTarget);
        EditorPrefs.SetString("currentBuildingAssetBundlePath", folderPath);
        EditorUserBuildSettings.SwitchActiveBuildTarget(selectedBuildTargetGroup, activeBuildTarget);

        //Asset cleanup
        AssetDatabase.DeleteAsset("Assets/" + prefabName + ".prefab");
        AssetDatabase.DeleteAsset("Assets/config.asset");
        //AssetDatabase.DeleteAsset("Assets/BMBFmod.asset");
        AssetDatabase.DeleteAsset("Assets/thumbnail.asset");
        AssetDatabase.DeleteAsset("Assets/descriptor.asset");

        if (!exporterConfig.allowOwnCamera) DestroyImmediate(camObject);

        File.Move(workingDir + "/" + fileName, path);

        clearTemp(workingDir);

        AssetDatabase.Refresh();

        if (exporterConfig.setCamLayerOnExport)
        {
            if (exporterConfig.allowOwnCamera) exportCam.cullingMask = oldCamMask;
            if (exporterConfig.allowOwnCamera) QosmeticUtils.SetLayerRecursively(exportCam.gameObject, oldObjectMask);
            QosmeticUtils.SetLayerRecursively(saber.gameObject, oldObjectMask);
        }

        if (!batch)
        {
            EditorUtility.DisplayDialog("Exportation Successful!", "Exportation Successful!", "OK");
            EditorUtility.RevealInFinder(path);
        }
    }

    /// <summary>
    /// Exports and uploads the saber via adb
    /// </summary>
    /// <param name="saber"></param>
    void ExportAndUpload(SaberReferences saber)
    {
        string saberName = saber.descriptor.objectName + "." + extension;
        string path = EditorUtility.SaveFilePanel("Save " + extension + " file", "", saberName, extension);
        if (path == "")
        {
            EditorUtility.DisplayDialog("Exportation Failed!", "Invalid path", "OK");
            return;
        }

        string folderPath = Path.GetDirectoryName(path) + "/";

        if (exporterConfig.ipAddress != "") QosmeticUtils.adb("connect " + exporterConfig.ipAddress + ":5555");

        QosmeticUtils.adb("shell am force-stop com.beatgames.beatsaber");

        ExportSaberFile(saber, folderPath);

        QosmeticUtils.adb("push \"" + folderPath + saberName + "\" \"" + questPath + saberName + "\"");
        QosmeticUtils.adb("shell am start com.beatgames.beatsaber/com.unity3d.player.UnityPlayerActivity");

        EditorUtility.DisplayDialog("Exportation Successful!", "If the game did not turn off or the saber is not showing up, make sure your quest is connected through adb", "OK");
        EditorUtility.RevealInFinder(folderPath + saberName);
    }

    /// <summary>
    /// Exports a complete Zip package ready to be loaded into bmbf
    /// </summary>
    /// <param name="saber"></param>
    void ExportSaberZip(SaberReferences saber, string path = "")
    {
        /*
        if (EditorUserBuildSettings.activeBuildTarget.ToString() != "Android")
        {
            EditorUtility.DisplayDialog("Exportation Failed!", "Your projects build target is not Android.", "OK");
            return;
        }
        */
        bool batch = path != "";

        GameObject saberObject = saber.gameObject;
        TextAsset config = new TextAsset(JsonUtility.ToJson(saber.config, true));

        string bmbfmodJson = JsonUtility.ToJson(saber.bmbfmod, true);
        TextAsset bmbfmod = new TextAsset(bmbfmodJson);

        string descriptorString = JsonUtility.ToJson(saber.descriptor, true);
        TextAsset descriptor = new TextAsset(descriptorString);

        string zipname = saber.bmbfmod.name + "V" + saber.bmbfmod.version + ".zip";
        zipname = QosmeticUtils.sanitizeString(zipname);

        if (!batch) path = EditorUtility.SaveFilePanel("Save " + "." + extension + " zip", "", zipname, "zip");
        else path += zipname;

        if (path == "")
        {
            EditorUtility.DisplayDialog("Exportation Failed!", "Invalid path", "OK");
            return;
        }

        string fileName = saber.bmbfmod.name + "." + extension;
        string folderPath = Path.GetDirectoryName(path);

        string workingDir = Application.temporaryCachePath + "/QuestSaber";

        if (File.Exists(path)) File.Delete(path);
        if (!Directory.Exists(workingDir)) Directory.CreateDirectory(workingDir);

        if (exporterConfig.removeCamerasAtExport) QosmeticUtils.PurgeCameras(saberObject);
        if (exporterConfig.removeLightsAtExport) QosmeticUtils.PurgeLights(saberObject);

        Selection.activeObject = saberObject;
        EditorUtility.SetDirty(saber);
        EditorSceneManager.MarkSceneDirty(saberObject.scene);
        EditorSceneManager.SaveScene(saberObject.scene);

        GameObject camObject;
        Camera exportCam;

        if (exporterConfig.allowOwnCamera) // manual cam
        {
            camObject = cam.gameObject;
            exportCam = cam;
            
        }
        else // automatic cam:
        {
            camObject = GameObject.CreatePrimitive(PrimitiveType.Quad);
            exportCam = camObject.AddComponent<Camera>();
            exportCam.gameObject.name = "Camera";
            exportCam.transform.localPosition = new Vector3(0.6f, 0.4f, 0.05f);
            exportCam.transform.rotation = Quaternion.Euler(25f, -70f, 0.0f);
            exportCam.clearFlags = CameraClearFlags.SolidColor;
            exportCam.nearClipPlane = 0.01f;
            exportCam.backgroundColor = new Color(0.0f, 0.0f, 25.0f / 256.0f);
        }
        exportCam.nearClipPlane = 0.01f;

        int oldCamMask = exportCam.cullingMask;
        int oldObjectMask = saber.gameObject.layer;
        int newMask = 31;
        int newCamMask = 1 << 31;

        // moved prefab forward so that the layermask isnt fucked up on the actual exported file
        PrefabUtility.SaveAsPrefabAsset(Selection.activeGameObject as GameObject, "Assets/" + prefabName + ".prefab");

        if (exporterConfig.setCamLayerOnExport)
        {
            exportCam.cullingMask = newCamMask;
            QosmeticUtils.SetLayerRecursively(exportCam.gameObject, newMask);
            QosmeticUtils.SetLayerRecursively(saber.gameObject, newMask);
        }
        
        Texture2D thumbnail = QosmeticUtils.getTexture2D(exportCam, 1024, 1024);

        //Start actually constructing the bundle

        
        AssetDatabase.CreateAsset(config, "Assets/config.asset");
        AssetDatabase.CreateAsset(bmbfmod, "Assets/BMBFmod.asset");
        AssetDatabase.CreateAsset(thumbnail, "Assets/thumbnail.asset");
        AssetDatabase.CreateAsset(descriptor, "Assets/descriptor.asset");

        AssetBundleBuild assetBundleBuild = default(AssetBundleBuild);

        assetBundleBuild.assetNames = new string[]
        {
            "Assets/" + prefabName + ".prefab",
            "Assets/config.asset",
            "Assets/BMBFmod.asset",
            "Assets/thumbnail.asset",
            "Assets/descriptor.asset"
        };

        assetBundleBuild.assetBundleName = fileName;

        BuildTargetGroup selectedBuildTargetGroup = EditorUserBuildSettings.selectedBuildTargetGroup;
        BuildTarget activeBuildTarget = EditorUserBuildSettings.activeBuildTarget;

        //Build Bundle
        BuildPipeline.BuildAssetBundles(workingDir, new AssetBundleBuild[] { assetBundleBuild }, 0, BuildTarget.Android);
        EditorPrefs.SetString("currentBuildingAssetBundlePath", folderPath);
        EditorUserBuildSettings.SwitchActiveBuildTarget(selectedBuildTargetGroup, activeBuildTarget);

        thumbnail = QosmeticUtils.getTexture2D(exportCam, 1024, 512);

        Directory.CreateDirectory(workingDir + "/tempzip");

        QosmeticUtils.exportImage(thumbnail, workingDir + "/tempzip/" + saber.bmbfmod.coverImageFilename);
        File.Move(workingDir + "/" + fileName, workingDir + "/tempzip/" + fileName);
        File.WriteAllText(workingDir + "/tempzip/bmbfmod.json", bmbfmodJson);

        QosmeticUtils.createZipFromFolder(workingDir + "/tempzip", path);

        //Asset cleanup
        AssetDatabase.DeleteAsset("Assets/" + prefabName + ".prefab");
        AssetDatabase.DeleteAsset("Assets/config.asset");
        AssetDatabase.DeleteAsset("Assets/BMBFmod.asset");
        AssetDatabase.DeleteAsset("Assets/thumbnail.asset");
        AssetDatabase.DeleteAsset("Assets/descriptor.asset");

        if (!exporterConfig.allowOwnCamera) DestroyImmediate(camObject);
        
        clearTemp(workingDir);

        AssetDatabase.Refresh();

        if (exporterConfig.setCamLayerOnExport)
        {
            if (exporterConfig.allowOwnCamera) exportCam.cullingMask = oldCamMask;
            if (exporterConfig.allowOwnCamera) QosmeticUtils.SetLayerRecursively(exportCam.gameObject, oldObjectMask);
            QosmeticUtils.SetLayerRecursively(saber.gameObject, oldObjectMask);
        }

        if (!batch)
        {
            EditorUtility.DisplayDialog("Exportation Successful!", "Exportation Successful!", "OK");
            EditorUtility.RevealInFinder(path);
        }
    }

    /// <summary>
    /// Removes any leftover files and folders from the working directory
    /// </summary>
    /// <param name="path"></param>
    private void clearTemp(string path)
    {
        DirectoryInfo di = new DirectoryInfo(path);

        foreach (FileInfo file in di.GetFiles())
        {
            file.Delete();
        }
        foreach (DirectoryInfo dir in di.GetDirectories())
        {
            dir.Delete(true);
        }

        Directory.Delete(path);
    }

    public void sceneChange()
    {
        OnFocus();
    }

    private void OnFocus()
    {
        questsabers = FindObjectsOfType<SaberReferences>();
        trail = AssetDatabase.LoadAssetAtPath("Assets/Qosmetics Exporters/Saber specifics/Prefabs and Models/trailTemplate.prefab", typeof(UnityEngine.Object)) as GameObject;
        saberTemplate = AssetDatabase.LoadAssetAtPath("Assets/Qosmetics Exporters/Saber specifics/Prefabs and Models/saberTemplate.prefab", typeof(UnityEngine.Object)) as GameObject;
        wallParticles = AssetDatabase.LoadAssetAtPath("Assets/Qosmetics Exporters/Saber specifics/Prefabs and Models/wallParticles.prefab", typeof(UnityEngine.Object)) as GameObject;
        sliceParticles = AssetDatabase.LoadAssetAtPath("Assets/Qosmetics Exporters/Saber specifics/Prefabs and Models/sliceParticles.prefab", typeof(UnityEngine.Object)) as GameObject;
        try
        {
            string json = File.ReadAllText(exporterConfigPath);
            exporterConfig = JsonUtility.FromJson<ExportersConfig>(json);
        }
        catch (FileNotFoundException)
        {
            Debug.LogError("File at " + exporterConfigPath + " Could not be found! Remaking the file...");
            string newJson = JsonUtility.ToJson(new ExportersConfig(), true);
            File.WriteAllText(newJson, exporterConfigPath);
        }
        catch (DirectoryNotFoundException)
        {
            Debug.LogError("File at " + exporterConfigPath + " Could not be found! Remaking the file...");
            string newJson = JsonUtility.ToJson(new ExportersConfig(), true);
            File.WriteAllText(newJson, exporterConfigPath);
        }

        if (exporterConfig.allowOwnCamera) cam = FindObjectOfType<QosmeticsCam>()?.gameObject.GetComponent<Camera>();
    }
    
}
