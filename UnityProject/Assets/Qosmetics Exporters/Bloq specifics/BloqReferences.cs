using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BloqReferences : MonoBehaviour
{
    public BMBFmod.mod bmbfmod;
    public Qosmetics.BloqConfig config;

    [HideInInspector] public bool advancedInfo;
    [HideInInspector] public bool bmbfFolded;
    [HideInInspector] public bool configFolded;
}
