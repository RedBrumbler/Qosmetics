using Qosmetics;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
// Written By RedBrumbler
public class SaberReferences : MonoBehaviour
{
    [HideInInspector] public BMBFmod.mod bmbfmod;
    public Descriptor descriptor = new Descriptor("YourName", "SaberName", "Description");
    public Qosmetics.SaberConfig config;
    public Texture2D thumbnail;
    
    [HideInInspector] public bool advancedInfo;
    [HideInInspector] public bool bmbfFolded;
    [HideInInspector] public bool descriptorFolded;
    [HideInInspector] public bool configFolded;
    [HideInInspector] public bool addedTrails = false;
    [HideInInspector] public bool addedWallParticles;
    [HideInInspector] public bool addedSliceParticles;
}
