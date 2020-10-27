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
        public Events events;

        [HideInInspector] public bool hideAddMenu = false;
    }

    [Serializable]
    [DisplayName("Qosmetics Events")]
    public class Events
    {
        [HideInInspector]public List<QosmeticsEvent> events;
    }


    [Serializable]
    public class QosmeticsEvent
    {
        public string objectPath;
        
        public enum EventType
        {
            [InspectorName("Dis- or Enable GameObjectEvent")]
            GameObject = 0,
            [InspectorName("BoolEvent")]
            Bool = 1,
            [InspectorName("TriggerEvent")]
            Trigger = 2,
            [ObsoleteAttribute]
            All = ~0,
        }

        public EventType type;

        public enum Event
        {
            [InspectorName("On Slice Bloq")]
            OnSlice,
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
            [InspectorName("On Red Light On")]
            OnLeftLightOn,
            [InspectorName("On Blue Light On")]
            OnRightLightOn,
            [Obsolete]
            All = ~0,
        }

        public Event objectEvent;

        public enum ValueHandle
        {
            [InspectorName("<")]
            Smaller,
            [InspectorName("<=")]
            SmallerOrEqual,
            [InspectorName("==")]
            Equal,
            [InspectorName(">=")]
            GreaterOrEqual,
            [InspectorName(">")]
            Greater,
            [Obsolete]
            All = ~0,
        }

        public ValueHandle HandleValue;
        public bool boolean;
        public int integer;
        public string nameToSet;
    }
}
