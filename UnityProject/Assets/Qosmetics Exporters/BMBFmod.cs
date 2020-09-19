using System.Collections;
using System.Collections.Generic;
using UnityEngine;
// Written By RedBrumbler
namespace BMBFmod
{
    [System.Serializable]
    public class Component
    {
        [HideInInspector] public string type;
        [HideInInspector] public string sourceFileName;
        [HideInInspector] public string targetRootedPathAndFileName;
    }
    [System.Serializable]
    public class Links
    {
        public string pageLink;
    }
    [System.Serializable]
    public class mod
    {
        public string coverImageFilename;
        [HideInInspector] public string icon;
        [HideInInspector] public List<Component> components;
        public string version;
        public Links links;
        public List<string> description;
        public string gameVersion;
        [HideInInspector] public string platform;
        [HideInInspector] public string id;
        public string name;
        public string author;
        [HideInInspector] public string category;
    }
}