using System.Collections;
using System.Collections.Generic;
using UnityEngine;
// Written By RedBrumbler
namespace Qosmetics
{
    [System.Serializable]
    public class SaberConfig
    {
        public bool enableFakeGlow;
        [HideInInspector] public bool hasCustomTrails;
        [HideInInspector] public bool hasCustomWallParticles;
        [HideInInspector] public bool hasCustomSliceParticles;

        [HideInInspector] public List<TrailConfig> leftTrails;
        [HideInInspector] public List<TrailConfig> rightTrails;
    }

    [System.Serializable]
    public class TrailConfig
    {
        public TrailConfig(TrailReferences trail)
        {
            trail.GenerateName();
            name = trail.trailName;

            colorType = (int)trail.colorType;

            trailColor = new TrailColor(trail.trailColor);
            multiplierColor = new TrailColor(trail.multiplierColor);

            length = trail.length;
            whiteStep = trail.whiteStep;
        }

        public string name;

        public int colorType;

        public TrailColor trailColor;
        public TrailColor multiplierColor;

        public int length;
        public int whiteStep;
    }

    [System.Serializable]
    public class TrailColor
    {
        public TrailColor(Color color)
        {
            R = color.r;
            G = color.g;
            B = color.b;
            A = color.a;
        }
        public float R;
        public float G;
        public float B;
        public float A;
    }
}
