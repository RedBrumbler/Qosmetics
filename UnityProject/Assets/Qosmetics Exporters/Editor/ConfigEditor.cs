using Qosmetics;
using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using UnityEditor;
using UnityEditor.SceneManagement;
using UnityEditorInternal;
using UnityEngine;
using UnityEngine.UI;

public class ConfigEditor : EditorWindow
{
    private ExportersConfig exporterConfig;
    private string exporterConfigPath = "Assets/Qosmetics Exporters/exportersConfig.json";

    [MenuItem("Tools/Qosmetics/Config Editor")]
    public static void ShowWindow()
    {
        EditorWindow.GetWindow(typeof(ConfigEditor), false, "Qosmetics Config Editor");
    }
    
    void OnGUI()
    {
        GUILayout.Label("Object cleanup at export config", EditorStyles.boldLabel);
        exporterConfig.removeLightsAtExport = EditorGUILayout.ToggleLeft("Remove Lights at export", exporterConfig.removeLightsAtExport);
        exporterConfig.removeCamerasAtExport = EditorGUILayout.ToggleLeft("Remove Cameras at export", exporterConfig.removeCamerasAtExport);
        GUILayout.Space(5);


        GUILayout.Label("Default information config", EditorStyles.boldLabel);
        exporterConfig.defaultVersion = EditorGUILayout.TextField("Default Version", exporterConfig.defaultVersion);
        exporterConfig.defaultPageLink = EditorGUILayout.TextField("Default Page Link", exporterConfig.defaultPageLink);
        exporterConfig.defaultCoverImageFileName = EditorGUILayout.TextField("Default Cover Image File Name", exporterConfig.defaultCoverImageFileName);
        GUILayout.Space(5);

        GUILayout.Label("Misc.", EditorStyles.boldLabel);

        exporterConfig.forceAllowExport = EditorGUILayout.ToggleLeft("Force Allow Export", exporterConfig.forceAllowExport);
        GUILayout.Space(5);
        exporterConfig.allowOwnCamera = EditorGUILayout.ToggleLeft("Allow Own Camera (add QosmeticsCam script to it)", exporterConfig.allowOwnCamera);
        GUILayout.Space(5);
        exporterConfig.setCamLayerOnExport = EditorGUILayout.ToggleLeft("Set cam layer on export (only export is visible)", exporterConfig.setCamLayerOnExport);
        GUILayout.Space(5);

        if (GUILayout.Button("Save config"))
        {
            string json = JsonUtility.ToJson(exporterConfig, true);
            File.WriteAllText(exporterConfigPath, json);
            EditorUtility.DisplayDialog("Config Saved", "For changes to show up in your exports, close and reopen the exporter unity window (not the entire unity project)", "OK");
            OnFocus();

        }
    }

    void OnFocus()
    {
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
    }

}

