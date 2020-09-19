using System.Collections;
using System.Collections.Generic;
using UnityEngine;
// Written By RedBrumbler
namespace Qosmetics
{
    [System.Serializable]
    public class ExportersConfig
    {
        public ExportersConfig()
        {
            removeCamerasAtExport = false;
            removeLightsAtExport = false;

            defaultVersion = "1.0.0";
            defaultPageLink = "";
            defaultCoverImageFileName = "cover.png";

            forceAllowExport = false;

            allowOwnCamera = false;

            setCamLayerOnExport = true;
        }
        //cleanup config
        public bool removeLightsAtExport;
        public bool removeCamerasAtExport;
        
        //default information config
        public string defaultVersion;
        public string defaultPageLink;
        public string defaultCoverImageFileName;

        //recommend keeping off
        public bool forceAllowExport;

        public bool allowOwnCamera;

        public bool setCamLayerOnExport;
    }
}
