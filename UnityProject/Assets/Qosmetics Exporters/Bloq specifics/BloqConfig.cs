using System.Collections;
using System.Collections.Generic;
using UnityEngine;
// Written By RedBrumbler
namespace Qosmetics
{
    [System.Serializable]
    public class BloqConfig
    {
        public bool disableBaseGameArrows;
        [HideInInspector] public bool hasBomb;
        [HideInInspector] public bool hasDebris;
    }
}
