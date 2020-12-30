using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public class ColorTester : EditorWindow
{
    public Color customColor = new Color(0.7843137f, 0.08627451f, 0.08627451f, 1.0f);
    public Color otherColor = new Color(0.0f, 0.4627451f, 0.8235294f, 1.0f);
    public GameObject objectToColor;

    bool isWall = false;
    [MenuItem("Tools/Qosmetics/Color Tester")]
    public static void ShowWindow()
    {
        EditorWindow.GetWindow(typeof(ColorTester), false, "Color Tester");
    }

    void OnGUI()
    {
        customColor = EditorGUILayout.ColorField("Custom Color to preview", customColor);
        otherColor = EditorGUILayout.ColorField("Other Color to preview", otherColor);
        objectToColor = EditorGUILayout.ObjectField("Object to Color", objectToColor, typeof(GameObject), true) as GameObject;

        if (objectToColor != null && objectToColor.GetComponent<WallReferences>() != null)
        {
            isWall = true;
        }
        else
        {
            isWall = false;
        }

        if (GUILayout.Button("Test Colors"))
        {
            if (objectToColor != null) SetColor();
        }

        if (GUILayout.Button("Swap Colors"))
        {
            Color temp = customColor;
            customColor = otherColor;
            otherColor = temp;
        }
    }

    void SetColor()
    {
        foreach (var renderer in objectToColor.GetComponentsInChildren<Renderer>(true))
        {
            foreach (var material in renderer.sharedMaterials)
            {
                
                if (ShouldHaveCC(material))
                {
                    if (material.HasProperty("_Color"))
                    {
                        material.SetColor("_Color", customColor);
                    }
                    if (material.HasProperty("_OtherColor") && !isWall)
                    {
                        material.SetColor("_OtherColor", otherColor);
                    }
                }
            }
        }
    }

    bool ShouldHaveCC(Material mat)
    {
        bool shouldCC = false;
        if (mat == null)
        {
            Debug.Log("Mat was null, so not setting data on it");
            return false;
        }

        if(mat.HasProperty("_CustomColors"))
        {
            float matFloat = mat.GetFloat("_CustomColors");
            if (matFloat > 0.0f) shouldCC = true;
        }
        else
        {
            if (mat.HasProperty("_Glow"))
            {
                float matFloat = mat.GetFloat("_Glow");
                if (matFloat > 0.0f) shouldCC = true;
            }
            else
            {
                if (mat.HasProperty("_Bloom"))
                {
                    float matFloat = mat.GetFloat("_Bloom");
                    if (matFloat > 0.0f) shouldCC = true;
                }
            }
        }

        return shouldCC;
    }
}
