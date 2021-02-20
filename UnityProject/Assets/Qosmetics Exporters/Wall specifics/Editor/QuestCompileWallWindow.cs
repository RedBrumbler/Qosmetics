using Qosmetics;
using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Runtime.Serialization;
using UnityEditor;
using UnityEditor.SceneManagement;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using UnityScript.Steps;
// Written By RedBrumbler
public class QuestCompileWallWindow : EditorWindow
{
    //Developed by RedBrumbler, originally very much based on the PC Beat Saber custom saber exporter project

    private WallReferences[] questwalls;
    private string extension = "qwall";
    private string prefabName = "_CustomWall";
    private string questPath = "/sdcard/ModData/com.beatgames.beatsaber/Mods/Qosmetics/walls/";
    private ExportersConfig exporterConfig;
    private string exporterConfigPath = "Assets/Qosmetics Exporters/exportersConfig.json";
    private Camera cam;

    private GameObject wallTemplate;

    [MenuItem("Tools/Qosmetics/Exporters/QuestWall")]
    public static void ShowWindow()
    {
        EditorWindow.GetWindow(typeof(QuestCompileWallWindow), false, "QuestWall Exporter");
    }

    void OnGUI()
    {
        GUILayout.BeginHorizontal();
        GUILayout.Label("Quest Walls", EditorStyles.boldLabel);

        if (GUILayout.Button("Go to wall scene"))
        {
            EditorSceneManager.OpenScene("Assets/scenes/questwalls.unity");
            OnFocus();
        }

        if (GUILayout.Button("Add New Wall Template"))
        {
            AddWallTemplate();
        }

        GUILayout.EndHorizontal();


        GUILayout.Space(5);
        if (GUILayout.Button("Export all Active walls"))
        {
            ExportAll();
        }
        if (GUILayout.Button("Export and Upload all Active walls"))
        {
            ExportAllAndUpload();
        }

        GUILayout.Space(10);

        foreach (var wall in questwalls)
        {
            if (wall == null) continue;
            GUILayout.Label("GameObject : " + wall.gameObject.name, EditorStyles.boldLabel);
            wall.descriptorFolded = EditorGUILayout.BeginFoldoutHeaderGroup(wall.descriptorFolded, "Wall Descriptor");

            if (wall.descriptorFolded)
            {
                //wall.advancedInfo = EditorGUILayout.ToggleLeft("Advanced BMBF mod info", wall.advancedInfo);
                GUILayout.Space(5);
                DescriptorDisplay(wall);
            }
            EditorGUILayout.EndFoldoutHeaderGroup();

            GUILayout.Space(5);

            wall.configFolded = EditorGUILayout.BeginFoldoutHeaderGroup(wall.configFolded, "Wall config");
            if (wall.configFolded)
            {
                WallConfigDisplay(wall);
            }
            EditorGUILayout.EndFoldoutHeaderGroup();

            EditorGUILayout.Space(10);

            bool disableExport = shouldDisableExport(wall);

            if (disableExport && exporterConfig.forceAllowExport) GUILayout.Label("WARNING! force allow export is set to true so this wall file is unlikely to work correctly in game!", EditorStyles.boldLabel);
            EditorGUI.BeginDisabledGroup(disableExport && !exporterConfig.forceAllowExport);
            if (GUILayout.Button("Export " + wall.descriptor.objectName + "." + extension))
            {
                SetRendererLengthBools(wall);
                ExportWallFile(wall);
            }

            if (GUILayout.Button("Export and Upload " + wall.descriptor.objectName + "." + extension))
            {
                SetRendererLengthBools(wall);
                ExportAndUpload(wall);
            }
            EditorGUI.EndDisabledGroup();
        }
    }

    bool shouldDisableExport(WallReferences wall)
    {
        bool disableExport = wall.transform.Find("Core") == null || (wall.transform.Find("Frame") == null && !wall.config.disableFrame);
        return disableExport;
    }

    void ExportAll()
    {
        string path = EditorUtility.SaveFolderPanel("Select folder to export all " + extension + " files", "", "");
        if (path == "")
        {
            EditorUtility.DisplayDialog("Export Failed", "Path was invalid", "OK");
            return;
        }
        path += "/";
        List<string> skippedWalls = new List<string>();
        bool wallsWereSkipped = false;
        bool atLeastOneSuccessful = false;
        foreach (var wall in questwalls)
        {
            bool disableExport = shouldDisableExport(wall);

            if (!disableExport)
            {
                atLeastOneSuccessful = true;
                ExportWallFile(wall, path);
            }
            else
            {
                wallsWereSkipped = true;
                skippedWalls.Add(wall.descriptor.objectName);
            }
        }

        if (wallsWereSkipped)
        {
            string message = "These Walls were skipped due to exporter issues: \n";
            foreach (var wallName in skippedWalls)
            {
                message += wallName + "\n";
            }
            EditorUtility.DisplayDialog("Walls Were skipped...", message, "OK");
        }
        else
        {
            EditorUtility.DisplayDialog("Wall exports successful!", "Exported all walls successfully!", "OK");

        }
        if (atLeastOneSuccessful) EditorUtility.RevealInFinder(path);
    }

    void ExportAllAndUpload()
    {
        string path = EditorUtility.SaveFolderPanel("Select folder to export all ." + extension + " files", "", "");
        if (path == "")
        {
            EditorUtility.DisplayDialog("Export Failed", "Path was invalid", "OK");
            return;
        }

        path += "/";
        List<string> skippedWalls = new List<string>();
        bool wallsWereSkipped = false;
        bool atLeastOneSuccessful = false;

        if (exporterConfig.ipAddress != "") QosmeticUtils.adb("connect " + exporterConfig.ipAddress + ":5555");
        QosmeticUtils.adb("shell am force-stop com.beatgames.beatsaber");

        foreach (var wall in questwalls)
        {
            bool disableExport = shouldDisableExport(wall);

            if (!disableExport)
            {
                atLeastOneSuccessful = true;
                ExportWallFile(wall, path);
                string wallName = wall.descriptor.objectName + "." + extension;
                QosmeticUtils.adb("push\"" + path + wallName + "\" \"" + questPath + wallName + "\"");
            }
            else
            {
                wallsWereSkipped = true;
                skippedWalls.Add(wall.descriptor.objectName);
            }
        }

        if (wallsWereSkipped)
        {
            string message = "These Walls were skipped due to exporter issues: \n";
            foreach (var wallName in skippedWalls)
            {
                message += wallName + "\n";
            }
            EditorUtility.DisplayDialog("Sabers Were skipped...", message, "OK");
        }
        else
        {
            EditorUtility.DisplayDialog("Wall exports successful!", "Exported all walls successfully!", "OK");
        }
        if (atLeastOneSuccessful)
        {
            QosmeticUtils.adb("shell am start com.beatgames.beatsaber/com.unity3d.player.UnityPlayerActivity");
            EditorUtility.RevealInFinder(path);
        }
    }

    bool GetMoreThan1Material(Renderer renderer)
    {
        if (renderer == null) return false;
        return renderer.sharedMaterials.Length > 1;
    }

    void SetRendererLengthBools(WallReferences wall)
    {
        if (wall.config.replaceCoreMaterial) wall.config.moreThan1Core = GetMoreThan1Material(wall.transform.Find("Core")?.gameObject.GetComponent<MeshRenderer>());
        if (wall.config.replaceFrameMaterial) wall.config.moreThan1Frame = GetMoreThan1Material(wall.transform.Find("Frame")?.gameObject.GetComponent<MeshRenderer>());
    }

    void AddWallTemplate()
    {
        if (wallTemplate != null)
        {
            GameObject temp = Instantiate(wallTemplate, null);
            temp.name = "New WallTemplate";
            EditorSceneManager.MarkSceneDirty(EditorSceneManager.GetActiveScene());
            OnFocus();
        }
        else
        {
            Debug.LogError("Wall template was null!");
        }
    }

    /// <summary>
    /// Runs the code to display the descriptor settings in the GUI
    /// </summary>
    /// <param name="wall"></param>
    void DescriptorDisplay(WallReferences wall)
    {
        wall.descriptor.objectName = EditorGUILayout.TextField("Wall name", wall.descriptor.objectName);
        wall.descriptor.authorName = EditorGUILayout.TextField("Author name", wall.descriptor.authorName);
        wall.descriptor.description = EditorGUILayout.TextField("Description", wall.descriptor.description);
    }

    /// <summary>
    /// Runs the code to display the bmbf input options in the GUI
    /// </summary>
    /// <param name="wall"></param>
    void BMBFConfigDisplay(WallReferences wall)
    {
        QosmeticUtils.addIfNull(wall.bmbfmod);

        wall.bmbfmod.components[0].type = "FileCopyMod";
        wall.bmbfmod.components[0].targetRootedPathAndFileName = questPath + "testWall." + extension;

        if (wall.advancedInfo) wall.bmbfmod.coverImageFilename = EditorGUILayout.TextField("Cover image name", wall.bmbfmod.coverImageFilename);
        else if (wall.bmbfmod.coverImageFilename == "" && exporterConfig.defaultCoverImageFileName != "") wall.bmbfmod.coverImageFilename = exporterConfig.defaultCoverImageFileName;
        wall.bmbfmod.icon = wall.bmbfmod.coverImageFilename;

        if (wall.advancedInfo) wall.bmbfmod.version = EditorGUILayout.TextField("Wall version", wall.bmbfmod.version);
        else if (wall.bmbfmod.version == "" && exporterConfig.defaultVersion != "") wall.bmbfmod.version = exporterConfig.defaultVersion;

        if (wall.advancedInfo) wall.bmbfmod.links.pageLink = EditorGUILayout.TextField("Page link", wall.bmbfmod.links.pageLink);
        else if (wall.bmbfmod.links.pageLink == "" && exporterConfig.defaultPageLink != "") wall.bmbfmod.links.pageLink = exporterConfig.defaultPageLink;

        wall.bmbfmod.gameVersion = EditorGUILayout.TextField("Game version", wall.bmbfmod.gameVersion);
        wall.bmbfmod.description[0] = EditorGUILayout.TextField("Description", wall.bmbfmod.description[0]);
        wall.bmbfmod.platform = "Quest";
        wall.bmbfmod.name = EditorGUILayout.TextField("Wall name", wall.bmbfmod.name);
        wall.bmbfmod.author = EditorGUILayout.TextField("Author name", wall.bmbfmod.author);
        wall.bmbfmod.category = "Other";

        wall.bmbfmod.components[0].sourceFileName = wall.bmbfmod.name + "." + extension;

        string modID = wall.bmbfmod.name + wall.bmbfmod.author + "V" + wall.bmbfmod.version;
        modID = QosmeticUtils.sanitizeString(modID);

        wall.bmbfmod.id = modID;
    }

    /// <summary>
    /// Runs the code to display the wall input options in the GUI
    /// </summary>
    /// <param name="wall"></param>
    void WallConfigDisplay(WallReferences wall)
    {
        bool coreMeshFound;
        bool coreMaterialFound;
        bool frameMeshFound;
        bool frameMaterialFound;

        try { coreMeshFound = wall.transform.Find("Core")?.GetComponent<MeshFilter>()?.sharedMesh != null; }
        catch (MissingComponentException) { coreMeshFound = false; }

        try { coreMaterialFound = wall.transform.Find("Core")?.GetComponent<MeshRenderer>()?.sharedMaterial != null; }
        catch (MissingComponentException) { coreMaterialFound = false; }

        try { frameMeshFound = wall.transform.Find("Frame")?.GetComponent<MeshFilter>()?.sharedMesh != null; }
        catch (MissingComponentException) { frameMeshFound = false; }

        try { frameMaterialFound = wall.transform.Find("Frame")?.GetComponent<MeshRenderer>()?.sharedMaterial != null; }
        catch (MissingComponentException) { frameMaterialFound = false; }

        if (!coreMaterialFound)
        {
            GUILayout.Label("No Material was found on the Core Object, changing replaceCoreMaterial to false", EditorStyles.boldLabel);
            wall.config.replaceCoreMaterial = false;
        }

        EditorGUI.BeginDisabledGroup(!coreMaterialFound);
        wall.config.replaceCoreMaterial = EditorGUILayout.ToggleLeft("Replace Core Material", wall.config.replaceCoreMaterial);
        EditorGUI.EndDisabledGroup();

        if (!coreMeshFound)
        {
            GUILayout.Label("No Mesh was found on the Core Object, changing replaceCoreMesh to false", EditorStyles.boldLabel);
            wall.config.replaceCoreMesh = false;
        }

        EditorGUI.BeginDisabledGroup(!coreMeshFound);
        wall.config.replaceCoreMesh = EditorGUILayout.ToggleLeft("Replace Core Mesh", wall.config.replaceCoreMesh);
        EditorGUI.EndDisabledGroup();

        wall.config.disableFrame = EditorGUILayout.ToggleLeft("Disable Frame", wall.config.disableFrame);
        wall.config.disableFakeGlow = EditorGUILayout.ToggleLeft("Disable Fake Glow", wall.config.disableFakeGlow);

        GUILayout.Space(5);
        if (wall.config.disableFrame)
        {
            GUILayout.Label("Frame is disabled, these settings will be ignored in game:", EditorStyles.boldLabel);
        }

        if (!frameMaterialFound)
        {
            GUILayout.Label("No Material was found on the Frame Object, changing replaceFrameMaterial to false", EditorStyles.boldLabel);
            wall.config.replaceFrameMaterial = false;
        }

        EditorGUI.BeginDisabledGroup(!frameMaterialFound || wall.config.disableFrame);
        wall.config.replaceFrameMaterial = EditorGUILayout.ToggleLeft("Replace Frame Material", wall.config.replaceFrameMaterial);
        EditorGUI.EndDisabledGroup();

        if (!frameMeshFound)
        {
            GUILayout.Label("No Mesh was found on the Frame Object, changing replaceFrameMesh to false", EditorStyles.boldLabel);
            wall.config.replaceFrameMesh = false;
        }

        EditorGUI.BeginDisabledGroup(!frameMeshFound || wall.config.disableFrame);
        wall.config.replaceFrameMesh = EditorGUILayout.ToggleLeft("Replace Frame Mesh", wall.config.replaceFrameMesh);
        EditorGUI.EndDisabledGroup();


        GUILayout.Space(5);
        wall.config.scoreSubmissionDisabled = EditorGUILayout.ToggleLeft("Score Submission Disabled", wall.config.scoreSubmissionDisabled);
    }

    /// <summary>
    /// Exports just a qwall file
    /// </summary>
    /// <param name="wall"></param>
    void ExportWallFile(WallReferences wall, string path = "")
    {
        /*
        if(EditorUserBuildSettings.activeBuildTarget.ToString() != "Android")
        {
            EditorUtility.DisplayDialog("Exportation Failed!", "Your projects build target is not Android.", "OK");
            return;
        }
        */

        bool batch = path != "";
        GameObject wallObject = wall.gameObject;
        TextAsset config = new TextAsset(JsonUtility.ToJson(wall.config, true));

        //string bmbfmodJson = JsonUtility.ToJson(wall.bmbfmod, true);
        //TextAsset bmbfmod = new TextAsset(bmbfmodJson);

        string descriptorString = JsonUtility.ToJson(wall.descriptor, true);
        TextAsset descriptor = new TextAsset(descriptorString);
        string wallName = wall.descriptor.objectName + "." + extension;
        if (!batch) path = EditorUtility.SaveFilePanel("Save " + extension + " file", "", wallName, extension);
        else path += wallName;

        if (path == "")
        {
            EditorUtility.DisplayDialog("Exportation Failed!", "Invalid path", "OK");
            return;
        }

        string fileName = Path.GetFileName(path);
        string folderPath = Path.GetDirectoryName(path);

        string workingDir = Application.temporaryCachePath + "/QuestWall";

        if (File.Exists(path)) File.Delete(path);
        if (!Directory.Exists(workingDir)) Directory.CreateDirectory(workingDir);

        if (exporterConfig.removeCamerasAtExport) QosmeticUtils.PurgeCameras(wallObject);
        if (exporterConfig.removeLightsAtExport) QosmeticUtils.PurgeLights(wallObject);

        Selection.activeObject = wallObject;
        EditorUtility.SetDirty(wall);
        EditorSceneManager.MarkSceneDirty(wallObject.scene);
        EditorSceneManager.SaveScene(wallObject.scene);

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
            exportCam.transform.localPosition = new Vector3(-5f, 2.1f, 5.5f);
            exportCam.transform.rotation = Quaternion.Euler(22f, 130f, 0.0f);
            exportCam.clearFlags = CameraClearFlags.SolidColor;
            exportCam.backgroundColor = new Color(0.0f, 0.0f, 25.0f / 256.0f);
        }
        exportCam.nearClipPlane = 0.01f;

        int oldCamMask = exportCam.cullingMask;
        int oldObjectMask = wall.gameObject.layer;
        int newMask = 31;
        int newCamMask = 1 << 31;

        // moved prefab forward so that the layermask isnt fucked up on the actual exported file
        PrefabUtility.SaveAsPrefabAsset(Selection.activeGameObject as GameObject, "Assets/" + prefabName + ".prefab");

        if (exporterConfig.setCamLayerOnExport)
        {
            exportCam.cullingMask = newCamMask;
            QosmeticUtils.SetLayerRecursively(exportCam.gameObject, newMask);
            QosmeticUtils.SetLayerRecursively(wall.gameObject, newMask);
        }

        Texture2D thumbnail = wall.thumbnail;
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
        BuildPipeline.BuildAssetBundles(workingDir, new AssetBundleBuild[] { assetBundleBuild }, 0, BuildTarget.Android);
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
            QosmeticUtils.SetLayerRecursively(wall.gameObject, oldObjectMask);
        }
        if (!batch)
        {
            EditorUtility.DisplayDialog("Exportation Successful!", "Exportation Successful!", "OK");
            EditorUtility.RevealInFinder(path);
        }
    }

    void ExportAndUpload(WallReferences wall)
    {
        string wallName = wall.descriptor.objectName + "." + extension;
        string path = EditorUtility.SaveFilePanel("Save " + extension + " file", "", wallName, extension);
        if (path == "")
        {
            EditorUtility.DisplayDialog("Exportation Failed!", "Invalid path", "OK");
            return;
        }

        string folderPath = Path.GetDirectoryName(path) + "/";

        if (exporterConfig.ipAddress != "") QosmeticUtils.adb("connect " + exporterConfig.ipAddress + ":5555");

        QosmeticUtils.adb("shell am force-stop com.beatgames.beatsaber");

        ExportWallFile(wall, folderPath);

        QosmeticUtils.adb("push \"" + folderPath + wallName + "\" \"" + questPath + wallName + "\"");
        QosmeticUtils.adb("shell am start com.beatgames.beatsaber/com.unity3d.player.UnityPlayerActivity");

        EditorUtility.DisplayDialog("Exportation Successful!", "If the game did not turn off or the wall is not showing up, make sure your quest is connected through adb", "OK");
        EditorUtility.RevealInFinder(folderPath + wallName);
    }

    /// <summary>
    /// Exports a complete Zip package ready to be loaded into bmbf
    /// </summary>
    /// <param name="wall"></param>
    void ExportWallZip(WallReferences wall, string path = "")
    {
        /*
        if (EditorUserBuildSettings.activeBuildTarget.ToString() != "Android")
        {
            EditorUtility.DisplayDialog("Exportation Failed!", "Your projects build target is not Android.", "OK");
            return;
        }
        */
        bool batch = path != "";
        GameObject wallObject = wall.gameObject;
        TextAsset config = new TextAsset(JsonUtility.ToJson(wall.config, true));

        string bmbfmodJson = JsonUtility.ToJson(wall.bmbfmod, true);
        TextAsset bmbfmod = new TextAsset(bmbfmodJson);

        string descriptorString = JsonUtility.ToJson(wall.descriptor, true); ;
        TextAsset descriptor = new TextAsset(descriptorString);

        string zipname = wall.bmbfmod.name + "V" + wall.bmbfmod.version + ".zip";
        zipname = QosmeticUtils.sanitizeString(zipname);

        if (!batch) path = EditorUtility.SaveFilePanel("Save " + "." + extension + " zip", "", zipname, "zip");
        else path += zipname;

        if (path == "")
        {
            EditorUtility.DisplayDialog("Exportation Failed!", "Invalid path", "OK");
            return;
        }

        string fileName = wall.bmbfmod.name + "." + extension;
        string folderPath = Path.GetDirectoryName(path);

        string workingDir = Application.temporaryCachePath + "/QuestWall";

        if (File.Exists(path)) File.Delete(path);
        if (!Directory.Exists(workingDir)) Directory.CreateDirectory(workingDir);

        if (exporterConfig.removeCamerasAtExport) QosmeticUtils.PurgeCameras(wallObject);
        if (exporterConfig.removeLightsAtExport) QosmeticUtils.PurgeLights(wallObject);

        Selection.activeObject = wallObject;
        EditorUtility.SetDirty(wall);
        EditorSceneManager.MarkSceneDirty(wallObject.scene);
        EditorSceneManager.SaveScene(wallObject.scene);

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
            exportCam.transform.localPosition = new Vector3(-5f, 2.1f, 5.5f);
            exportCam.transform.rotation = Quaternion.Euler(22f, 130f, 0.0f);
            exportCam.clearFlags = CameraClearFlags.SolidColor;
            exportCam.backgroundColor = new Color(0.0f, 0.0f, 25.0f / 256.0f);
        }

        int oldCamMask = exportCam.cullingMask;
        int oldObjectMask = wall.gameObject.layer;
        int newMask = 31;
        int newCamMask = 1 << 31;

        // moved prefab forward so that the layermask isnt fucked up on the actual exported file
        PrefabUtility.SaveAsPrefabAsset(Selection.activeGameObject as GameObject, "Assets/" + prefabName + ".prefab");

        if (exporterConfig.setCamLayerOnExport)
        {
            exportCam.cullingMask = newCamMask;
            QosmeticUtils.SetLayerRecursively(exportCam.gameObject, newMask);
            QosmeticUtils.SetLayerRecursively(wall.gameObject, newMask);
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

        QosmeticUtils.exportImage(thumbnail, workingDir + "/tempzip/" + wall.bmbfmod.coverImageFilename);
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
            QosmeticUtils.SetLayerRecursively(wall.gameObject, oldObjectMask);
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
        questwalls = FindObjectsOfType<WallReferences>();
        wallTemplate = AssetDatabase.LoadAssetAtPath("Assets/Qosmetics Exporters/Wall specifics/Prefabs and Models/wallTemplate.prefab", typeof(UnityEngine.Object)) as GameObject;
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
