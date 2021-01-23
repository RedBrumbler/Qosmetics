using System.Collections;
using System.Collections.Generic;
using UnityEngine;
// Written By RedBrumbler
namespace Qosmetics
{
    [System.Serializable]
    public class Descriptor
    {
        public Descriptor(string author, string objectName, string description)
        {
            this.authorName = author;
            this.objectName = objectName;
            this.description = description;
        }

        public string authorName;
        public string objectName;
        public string description;
    }

}
