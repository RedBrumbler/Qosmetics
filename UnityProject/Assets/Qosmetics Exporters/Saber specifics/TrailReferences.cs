using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[ExecuteInEditMode]
public class TrailReferences : MonoBehaviour
{
    [HideInInspector]public string trailName;

    public Material trailMaterial;
    public enum ColorType {leftSaber, rightSaber, custom }

    public ColorType colorType;

    public Color trailColor = Color.white;
    public Color multiplierColor = Color.white;

    public int length = 14;
    public int whiteStep = 0;

    void OnEnable()
    {
        GenerateName();
    }

    public void GenerateName()
    {
        string path = gameObject.name;
        string currentName = "";
        Transform currentTransform = transform;
        if (transform.parent == null) return;
        do
        {
            currentTransform = currentTransform.parent;
            currentName = currentTransform.gameObject.name;
            if (!(currentName == "LeftSaber" || currentName == "RightSaber")) path = currentName + "/" + path;
        } while (!(currentName == "LeftSaber" || currentName == "RightSaber"));

        trailName = path;
    }
}
