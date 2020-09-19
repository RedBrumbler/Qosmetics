using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SaberReferences : MonoBehaviour
{
    public BMBFmod.mod bmbfmod;
    public Qosmetics.SaberConfig config;

    [HideInInspector] public bool advancedInfo;
    [HideInInspector] public bool bmbfFolded;
    [HideInInspector] public bool configFolded;
    [HideInInspector] public bool addedTrails = false;
    [HideInInspector] public bool addedWallParticles;
    [HideInInspector] public bool addedSliceParticles;
}
