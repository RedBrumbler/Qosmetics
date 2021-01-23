using Qosmetics;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
// Written By RedBrumbler
public class BloqReferences : MonoBehaviour
{
    [HideInInspector] public BMBFmod.mod bmbfmod;
    public Descriptor descriptor = new Descriptor("YourName", "BloqName", "Description");
    public Qosmetics.BloqConfig config;
    public Texture2D thumbnail;

    [HideInInspector] public bool advancedInfo;
    [HideInInspector] public bool bmbfFolded;
    [HideInInspector] public bool descriptorFolded;
    [HideInInspector] public bool configFolded;
}
