using Qosmetics;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
// Written By RedBrumbler
public class WallReferences : MonoBehaviour
{
    [HideInInspector] public BMBFmod.mod bmbfmod;
    public Descriptor descriptor = new Descriptor("YourName", "WallName", "Description");
    public Qosmetics.WallConfig config;
    public Texture2D thumbnail;

    [HideInInspector] public bool advancedInfo;
    [HideInInspector] public bool bmbfFolded;
    [HideInInspector] public bool descriptorFolded;
    [HideInInspector] public bool configFolded;
}
