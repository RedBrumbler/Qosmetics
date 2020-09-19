using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
// Written By RedBrumbler
[ExecuteInEditMode]
public class sizeParamsScript : MonoBehaviour
{
    #if (UNITY_EDITOR)
    [Header("script enable bool")]
    public bool frameSizeParams;
    void Update()
    {
        if (!frameSizeParams) return;

        try
        {
            foreach (var renderer in gameObject.GetComponentsInChildren<MeshRenderer>())
            {
                Vector4 sizeParams = new Vector4(transform.localScale.x * 0.5f, transform.localScale.y * 0.5f, transform.localScale.z * 0.5f, 0.05f);


                if (renderer.sharedMaterial.HasProperty("_SizeParams")) renderer.sharedMaterial.SetVector("_SizeParams", sizeParams);
            }
        }
        catch (NullReferenceException)
        {

        }
    }
    #endif
}
