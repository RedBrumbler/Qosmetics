using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using UnityEngine;

namespace Qosmetics
{
    [ExecuteInEditMode]
    public class EventReferences : MonoBehaviour
    {
        [Tooltip("Qosmetics events for use with the Qosmetics Mod")]
        public Events events = new Events();

        [HideInInspector] public bool hideAddMenu = false;
        [HideInInspector] public bool hideEventsList = false;
    }

    [Serializable]
    [DisplayName("Qosmetics Events")]
    public class Events
    {
        /*[HideInInspector]*/public List<QosmeticsEvent> events = new List<QosmeticsEvent>();
    }


    [Serializable]
    public class QosmeticsEvent
    {
        public QosmeticsEvent() { }
        public QosmeticsEvent (QosmeticsEvent toCopy)
        {
            objectPath = toCopy.objectPath;
            theObject = toCopy.theObject;
            effect = toCopy.effect;
            type = toCopy.type;
            boolean = toCopy.boolean;
            integer = toCopy.integer;
            nameToSet = toCopy.nameToSet;
        }

        public string objectPath = "";
        private GameObject TheObject = null;

        public GameObject theObject
        {
            get { return TheObject; }
            set { TheObject = value; }
        }

        public enum EffectType
        {
            [InspectorName("Toggle GameObject Event")]
            GameObject = 0,
            [InspectorName("Bool Event")]
            Bool = 1,
            [InspectorName("Trigger Event")]
            Trigger = 2
        }

        public EffectType effect = 0;

        public enum Event
        {
            [InspectorName("On Slice Bloq")]
            OnSlice,
            [InspectorName("On Combo Number")]
            OnCombo,
            [InspectorName("On Every Nth Combo")]
            OnEveryNCombo,
            [InspectorName("On Combo Break")]
            OnComboBreak,
            [InspectorName("On Multiplier Up")]
            MultiplierUp,
            [InspectorName("On Saber Start Colliding")]
            SaberStartColliding,
            [InspectorName("On Saber Stop Colliding")]
            SaberStopColliding,
            [InspectorName("On Level Start")]
            OnLevelStart,
            [InspectorName("On Level Fail")]
            OnLevelFail,
            [InspectorName("On Level End")]
            OnLevelEnded,
            [InspectorName("On Left Light On")]
            OnLeftLightOn,
            [InspectorName("On Left Light Off")]
            OnLeftLightOff,
            [InspectorName("On Right Light On")]
            OnRightLightOn,
            [InspectorName("On Right Light Off")]
            OnRightLightOff
        }

        public Event type = 0;

        public bool boolean = true;
        public int integer = 0;
        public string nameToSet = "";


    }
}
