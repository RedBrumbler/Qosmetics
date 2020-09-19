using System.Collections;
using System.Collections.Generic;
using UnityEngine;
// Written By RedBrumbler
namespace Qosmetics
{
    [System.Serializable]
    public class WallConfig
    {
        public bool replaceCoreMaterial;
        public bool replaceCoreMesh;
        public bool disableFrame;
        public bool disableFakeGlow;
        
        public bool replaceFrameMaterial;
        public bool replaceFrameMesh;
        
        public bool scoreSubmissionDisabled;

        [HideInInspector] public bool moreThan1Core;
        [HideInInspector] public bool moreThan1Frame;
    }
}