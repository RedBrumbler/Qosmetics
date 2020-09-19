using Qosmetics;
using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEditor;
using UnityEditor.SceneManagement;
using UnityEngine;
using UnityEngine.UI;

public class QuestCompileBloqWindow : EditorWindow
{
    //Developed by RedBrumbler, originally very much based on the PC Beat Saber custom saber exporter project

    private BloqReferences[] questbloqs;
    private string extension = "qbloq";
    private string prefabName = "_CustomBloq";
    private string questPath = "/sdcard/Qosmetics/notes/";
    private ExportersConfig exporterConfig;
    private string exporterConfigPath = "Assets/Qosmetics Exporters/exportersConfig.json";
    private Camera cam;

    private GameObject bloqTemplate;

    [MenuItem("Tools/Qosmetics/Exporters/QuestBloq")]
    public static void ShowWindow()
    {
        EditorWindow.GetWindow(typeof(QuestCompileBloqWindow), false, "QuestBloq Exporter");
    }

    void OnGUI()
    {
        GUILayout.BeginHorizontal();

        GUILayout.Label("Quest Bloqs", EditorStyles.boldLabel);
        if (GUILayout.Button("Go to Bloq scene"))
        {
            EditorSceneManager.OpenScene("Assets/scenes/questnotes.unity");
            OnFocus();
        }

        if (GUILayout.Button("Add New Bloq Template"))
        {
            AddBloqTemplate();
        }

        GUILayout.EndHorizontal();

        GUILayout.Space(5);
        if (GUILayout.Button("Export all Active bloqs"))
        {
            ExportAll();
        }

        GUILayout.Space(10);
        foreach (var bloq in questbloqs)
        {
            if (bloq == null) continue;
            GUILayout.Label("GameObject : " + bloq.gameObject.name, EditorStyles.boldLabel);
            bloq.bmbfFolded = EditorGUILayout.BeginFoldoutHeaderGroup(bloq.bmbfFolded, "BMBF mod info");

            if (bloq.bmbfFolded)
            {
                bloq.advancedInfo = EditorGUILayout.ToggleLeft("Advanced BMBF mod info", bloq.advancedInfo);
                GUILayout.Space(5);
                BMBFConfigDisplay(bloq);
            }
            EditorGUILayout.EndFoldoutHeaderGroup();

            GUILayout.Space(5);

            bloq.configFolded = EditorGUILayout.BeginFoldoutHeaderGroup(bloq.configFolded, "Bloq config");
            if (bloq.configFolded)
            {
                BloqConfigDisplay(bloq);
                
            }
            EditorGUILayout.EndFoldoutHeaderGroup();

            EditorGUILayout.Space(10);
            bool disableExport = shouldDisableExport(bloq);
            if (disableExport && exporterConfig.forceAllowExport) GUILayout.Label("WARNING! force allow export is set to true so this bloq file is unlikely to work correctly in game!", EditorStyles.boldLabel);
            EditorGUI.BeginDisabledGroup(disableExport && !exporterConfig.forceAllowExport);
            if (GUILayout.Button("Export " + bloq.bmbfmod.name + "." + extension))
            {
                GetHasBomb(bloq);
                GetHasDebris(bloq);
                ExportBloqFile(bloq);
            }

            if (GUILayout.Button("Export " + bloq.bmbfmod.name + "." + extension + " BMBF mod zip"))
            {
                GetHasBomb(bloq);
                GetHasDebris(bloq);
                ExportBloqZip(bloq);
            }
            EditorGUI.EndDisabledGroup();


            foreach (Transform child in bloq.transform)
            {
                if (QosmeticUtils.GetObjectBounds(child.gameObject).extents.x * 2.0 > 1.4)
                {
                    GUILayout.Label(child.gameObject.name + " might be too wide (x)", EditorStyles.boldLabel);
                }

                if (QosmeticUtils.GetObjectBounds(child.gameObject).extents.y * 2.0 > 1.4)
                {
                    GUILayout.Label(child.gameObject.name + " might be too tall (y)", EditorStyles.boldLabel);
                }

                if (QosmeticUtils.GetObjectBounds(child.gameObject).extents.z * 2.0 > 1.4)
                {
                    GUILayout.Label(child.gameObject.name + " might be too long (z)", EditorStyles.boldLabel);
                }
            }
        }
    }
    /// <summary>
    /// Adds new Bloq template
    /// </summary>
    void AddBloqTemplate()
    {
        if (bloqTemplate != null)
        {
            GameObject temp = Instantiate(bloqTemplate, null);
            temp.name = "New BloqTemplate";
            EditorSceneManager.MarkSceneDirty(EditorSceneManager.GetActiveScene());
            OnFocus();
        }
        else
        {
            Debug.LogError("Bloq template was null!");
        }
    }

    void ExportAll()
    {
        string path = EditorUtility.SaveFolderPanel("Select folder to export all zips", "", "");
        if (path == "")
        {
            EditorUtility.DisplayDialog("Export Failed", "Path was invalid", "OK");
            return;
        }
        path += "/";
        List<string> skippedBloqs = new List<string>();
        bool bloqsWereSkipped = false;
        bool atLeastOneSuccessful = false;
        foreach (var bloq in questbloqs)
        {
            bool disableExport = shouldDisableExport(bloq);

            if (!disableExport)
            {
                atLeastOneSuccessful = true;
                ExportBloqZip(bloq, path);
            }
            else
            {
                bloqsWereSkipped = true;
                skippedBloqs.Add(bloq.bmbfmod.name);
            }
        }

        if (bloqsWereSkipped)
        {
            string message = "These Bloqs were skipped due to exporter issues: \n";
            foreach (var bloqName in skippedBloqs)
            {
                message += bloqName + "\n";
            }
            EditorUtility.DisplayDialog("Bloqs Were skipped...", message, "OK");
        }
        else
        {
            EditorUtility.DisplayDialog("Bloq exports successful!", "Exported all bloqs succesfully'!", "OK");

        }
        if (atLeastOneSuccessful) EditorUtility.RevealInFinder(path);
    }

    bool shouldDisableExport(BloqReferences bloq)
    {
        bool disableExport = bloq.transform.Find("LeftArrow") == null || bloq.transform.Find("RightArrow") == null || bloq.transform.Find("LeftDot") == null || bloq.transform.Find("RightDot") == null;
        return disableExport;
    }

    /// <summary>
    /// Runs the code to display the bmbf input options in the GUI
    /// </summary>
    /// <param name="bloq"></param>
    void BMBFConfigDisplay(BloqReferences bloq)
    {
        QosmeticUtils.addIfNull(bloq.bmbfmod);

        bloq.bmbfmod.components[0].type = "FileCopyMod";
        bloq.bmbfmod.components[0].targetRootedPathAndFileName = questPath + "testBloq." + extension;

        if (bloq.advancedInfo) bloq.bmbfmod.coverImageFilename = EditorGUILayout.TextField("Cover image name", bloq.bmbfmod.coverImageFilename);
        else if (bloq.bmbfmod.coverImageFilename == "" && exporterConfig.defaultCoverImageFileName != "") bloq.bmbfmod.coverImageFilename = exporterConfig.defaultCoverImageFileName;
        bloq.bmbfmod.icon = bloq.bmbfmod.coverImageFilename;

        if (bloq.advancedInfo) bloq.bmbfmod.version = EditorGUILayout.TextField("Bloq version", bloq.bmbfmod.version);
        else if (bloq.bmbfmod.version == "" && exporterConfig.defaultVersion != "") bloq.bmbfmod.version = exporterConfig.defaultVersion;

        if (bloq.advancedInfo) bloq.bmbfmod.links.pageLink = EditorGUILayout.TextField("Page link", bloq.bmbfmod.links.pageLink);
        else if (bloq.bmbfmod.links.pageLink == "" && exporterConfig.defaultPageLink != "") bloq.bmbfmod.links.pageLink = exporterConfig.defaultPageLink;

        bloq.bmbfmod.gameVersion = EditorGUILayout.TextField("Game version", bloq.bmbfmod.gameVersion);
        bloq.bmbfmod.description[0] = EditorGUILayout.TextField("Description", bloq.bmbfmod.description[0]);
        bloq.bmbfmod.platform = "Quest";
        bloq.bmbfmod.name = EditorGUILayout.TextField("Bloq name", bloq.bmbfmod.name);
        bloq.bmbfmod.author = EditorGUILayout.TextField("Author name", bloq.bmbfmod.author);
        bloq.bmbfmod.category = "Bloq";

        bloq.bmbfmod.components[0].sourceFileName = bloq.bmbfmod.name + "." + extension;

        string modID = bloq.bmbfmod.name + bloq.bmbfmod.author + "V" + bloq.bmbfmod.version;
        modID = QosmeticUtils.sanitizeString(modID);

        bloq.bmbfmod.id = modID;
    }

    /// <summary>
    /// Runs the code to display the bloq input options in the GUI
    /// </summary>
    /// <param name="bloq"></param>
    void BloqConfigDisplay(BloqReferences bloq)
    {
        bloq.config.disableBaseGameArrows = EditorGUILayout.ToggleLeft("Disable Base Game Arrows", bloq.config.disableBaseGameArrows);
    }

    void GetHasBomb(BloqReferences bloq)
    {
        bloq.config.hasBomb = bloq.transform.Find("Bomb") != null;
    }

    void GetHasDebris(BloqReferences bloq)
    {
        bool hasLeft, hasRight;

        hasLeft = bloq.transform.Find("LeftDebris") != null;
        hasRight = bloq.transform.Find("RightDebris") != null;

        bloq.config.hasDebris = hasLeft && hasRight;
    }

    /// <summary>
    /// Exports just a qbloq file
    /// </summary>
    /// <param name="bloq"></param>
    void ExportBloqFile(BloqReferences bloq)
    {

        GameObject bloqObject = bloq.gameObject;
        TextAsset config = new TextAsset(JsonUtility.ToJson(bloq.config, true));

        string bmbfmodJson = JsonUtility.ToJson(bloq.bmbfmod, true);
        TextAsset bmbfmod = new TextAsset(bmbfmodJson);

        string descriptorString = JsonUtility.ToJson(new Descriptor(bloq.bmbfmod.author, bloq.bmbfmod.name, bloq.bmbfmod.description[0]), true);
        TextAsset descriptor = new TextAsset(descriptorString);

        string path = EditorUtility.SaveFilePanel("Save " + extension + " file", "", bloq.bmbfmod.name + "." + extension, extension);

        if (path == "")
        {
            EditorUtility.DisplayDialog("Exportation Failed!", "Invalid path", "OK");
            return;
        }

        string fileName = Path.GetFileName(path);
        string folderPath = Path.GetDirectoryName(path);

        string workingDir = Application.temporaryCachePath + "/QuestBloq";

        if (File.Exists(path)) File.Delete(path);
        if (!Directory.Exists(workingDir)) Directory.CreateDirectory(workingDir);

        if (exporterConfig.removeCamerasAtExport) QosmeticUtils.PurgeCameras(bloqObject);
        if (exporterConfig.removeLightsAtExport) QosmeticUtils.PurgeLights(bloqObject);

        Selection.activeObject = bloqObject;
        EditorUtility.SetDirty(bloq);
        EditorSceneManager.MarkSceneDirty(bloqObject.scene);
        EditorSceneManager.SaveScene(bloqObject.scene);

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
            exportCam.transform.localPosition = new Vector3(2.2f, 2.6f, -6.4f);
            exportCam.transform.rotation = Quaternion.Euler(20.0f, -20.0f, 0.0f);
            exportCam.clearFlags = CameraClearFlags.SolidColor;
            exportCam.backgroundColor = new Color(0.0f, 0.0f, 25.0f / 256.0f);
        }
        exportCam.nearClipPlane = 0.01f;

        int oldCamMask = exportCam.cullingMask;
        int oldObjectMask = bloq.gameObject.layer;
        int newMask = 31;
        int newCamMask = 1 << 31;

        // moved prefab forward so that the layermask isnt fucked up on the actual exported file
        PrefabUtility.SaveAsPrefabAsset(Selection.activeGameObject as GameObject, "Assets/" + prefabName + ".prefab");

        if (exporterConfig.setCamLayerOnExport)
        {
            exportCam.cullingMask = newCamMask;
            QosmeticUtils.SetLayerRecursively(exportCam.gameObject, newMask);
            QosmeticUtils.SetLayerRecursively(bloq.gameObject, newMask);
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

        //Asset cleanup
        AssetDatabase.DeleteAsset("Assets/" + prefabName + ".prefab");
        AssetDatabase.DeleteAsset("Assets/config.asset");
        AssetDatabase.DeleteAsset("Assets/BMBFmod.asset");
        AssetDatabase.DeleteAsset("Assets/thumbnail.asset");
        AssetDatabase.DeleteAsset("Assets/descriptor.asset");
        if (!exporterConfig.allowOwnCamera) DestroyImmediate(camObject);

        File.Move(workingDir + "/" + fileName, path);
        AssetDatabase.Refresh();

        clearTemp(workingDir);

        if (exporterConfig.setCamLayerOnExport)
        {
            if (exporterConfig.allowOwnCamera) exportCam.cullingMask = oldCamMask;
            if (exporterConfig.allowOwnCamera) QosmeticUtils.SetLayerRecursively(exportCam.gameObject, oldObjectMask);
            QosmeticUtils.SetLayerRecursively(bloq.gameObject, oldObjectMask);
        }
        EditorUtility.DisplayDialog("Exportation Successful!", "Exportation Successful!", "OK");
        EditorUtility.RevealInFinder(path);
    }

    /// <summary>
    /// Exports a complete Zip package ready to be loaded into bmbf
    /// </summary>
    /// <param name="bloq"></param>
    void ExportBloqZip(BloqReferences bloq, string path = "")
    {
        /*
        if (EditorUserBuildSettings.activeBuildTarget.ToString() != "Android")
        {
            EditorUtility.DisplayDialog("Exportation Failed!", "Your projects build target is not Android.", "OK");
            return;
        }
        */
        bool batch = path != "";

        GameObject bloqObject = bloq.gameObject;
        TextAsset config = new TextAsset(JsonUtility.ToJson(bloq.config, true));

        string bmbfmodJson = JsonUtility.ToJson(bloq.bmbfmod, true);
        TextAsset bmbfmod = new TextAsset(bmbfmodJson);

        string descriptorString = JsonUtility.ToJson(new Descriptor(bloq.bmbfmod.author, bloq.bmbfmod.name, bloq.bmbfmod.description[0]), true);
        TextAsset descriptor = new TextAsset(descriptorString);

        string zipname = bloq.bmbfmod.name + "V" + bloq.bmbfmod.version + ".zip";
        zipname = QosmeticUtils.sanitizeString(zipname);

        if (!batch) path = EditorUtility.SaveFilePanel("Save " + "." + extension + " zip", "", zipname, "zip");
        else path += zipname;

        if (path == "")
        {
            EditorUtility.DisplayDialog("Exportation Failed!", "Invalid path", "OK");
            return;
        }

        string fileName = bloq.bmbfmod.name + "." + extension;
        string folderPath = Path.GetDirectoryName(path);

        string workingDir = Application.temporaryCachePath + "/QuestSaber";

        if (File.Exists(path)) File.Delete(path);
        if (!Directory.Exists(workingDir)) Directory.CreateDirectory(workingDir);

        if (exporterConfig.removeCamerasAtExport) QosmeticUtils.PurgeCameras(bloqObject);
        if (exporterConfig.removeLightsAtExport) QosmeticUtils.PurgeLights(bloqObject);

        Selection.activeObject = bloqObject;
        EditorUtility.SetDirty(bloq);
        EditorSceneManager.MarkSceneDirty(bloqObject.scene);
        EditorSceneManager.SaveScene(bloqObject.scene);

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
            exportCam.transform.localPosition = new Vector3(2.2f, 2.6f, -6.4f);
            exportCam.transform.rotation = Quaternion.Euler(20.0f, -20.0f, 0.0f);
            exportCam.clearFlags = CameraClearFlags.SolidColor;
            exportCam.backgroundColor = new Color(0.0f, 0.0f, 25.0f / 256.0f);
        }
        exportCam.nearClipPlane = 0.01f;

        int oldCamMask = exportCam.cullingMask;
        int oldObjectMask = bloq.gameObject.layer;
        int newMask = 31;
        int newCamMask = 1 << 31;

        // moved prefab forward so that the layermask isnt fucked up on the actual exported file
        PrefabUtility.SaveAsPrefabAsset(Selection.activeGameObject as GameObject, "Assets/" + prefabName + ".prefab");

        if (exporterConfig.setCamLayerOnExport)
        {
            exportCam.cullingMask = newCamMask;
            QosmeticUtils.SetLayerRecursively(exportCam.gameObject, newMask);
            QosmeticUtils.SetLayerRecursively(bloq.gameObject, newMask);
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



        Directory.CreateDirectory(workingDir + "/tempzip");

        QosmeticUtils.exportImage(thumbnail, workingDir + "/tempzip/" + bloq.bmbfmod.coverImageFilename);
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
            QosmeticUtils.SetLayerRecursively(bloq.gameObject, oldObjectMask);
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
        questbloqs = FindObjectsOfType<BloqReferences>();
        bloqTemplate = AssetDatabase.LoadAssetAtPath("Assets/Qosmetics Exporters/Bloq specifics/Prefabs and Models/bloqTemplate.prefab", typeof(UnityEngine.Object)) as GameObject;
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
