using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using UnityEditor.SceneManagement;
using Qosmetics;
public class SceneChanger : EditorWindow
{
    [MenuItem("Tools/Qosmetics/Scene Changer")]
    public static void ShowWindow()
    {
        EditorWindow.GetWindow(typeof(SceneChanger), false, "Qosmetics Scene Changer");
    }

    void OnGUI()
    {
        if (GUILayout.Button("Go to Wall scene"))
        {
            EditorSceneManager.OpenScene("Assets/scenes/questwalls.unity");
            EditorWindow.GetWindow(typeof(QuestCompileWallWindow));
        }

        if (GUILayout.Button("Go to Saber scene"))
        {
            EditorSceneManager.OpenScene("Assets/scenes/questsabers.unity");
            EditorWindow.GetWindow(typeof(QuestCompileSaberWindow));
        }

        if (GUILayout.Button("Go to Bloq scene"))
        {
            EditorSceneManager.OpenScene("Assets/scenes/questnotes.unity");
            EditorWindow.GetWindow(typeof(QuestCompileBloqWindow));
        }
    }
}
