using Qosmetics;
using System;
using UnityEditor;
using UnityEngine;

public class EventConfigurator : EditorWindow
{
    private EventReferences[] questevents;
    private QosmeticsEvent newEvent = new QosmeticsEvent();
    Vector2 minorScrollPos;
    [MenuItem("Tools/Qosmetics/Event Configurator")]
    public static void ShowWindow()
    {
        EditorWindow.GetWindow(typeof(EventConfigurator), false, "Event Configurator");
    }

    void OnGUI()
    {
        GUILayout.Label("Event Configurator", EditorStyles.boldLabel);
        minorScrollPos = GUILayout.BeginScrollView(minorScrollPos);
        try
        {
            foreach (var qevent in questevents)
            {
                displayWhich(qevent);
                qevent.hideAddMenu = EditorGUILayout.BeginFoldoutHeaderGroup(qevent.hideAddMenu, "Add new Event Menu");

                if (qevent.hideAddMenu)
                {
                    AddEventMenu(qevent);
                }
                EditorGUILayout.EndFoldoutHeaderGroup();

                qevent.hideEventsList = EditorGUILayout.BeginFoldoutHeaderGroup(qevent.hideEventsList, "Events List");
                if (qevent.hideEventsList)
                {
                    foreach (var qosEvent in qevent.events.events)
                    {
                        GUILayout.Space(10);
                        GUILayout.Label(qosEvent.effect.ToString() + " Event " + qosEvent.type.ToString(), EditorStyles.boldLabel);
                        DisplayEvent(qevent, qosEvent);
                        if (GUILayout.Button("Remove Event"))
                        {
                            qevent.events.events.Remove(qosEvent);
                        }
                    }
                }

                EditorGUILayout.EndFoldoutHeaderGroup();

                GUILayout.Space(10);
                if (GUILayout.Button("Log JSON"))
                {
                    foreach (var qosEvent in qevent.events.events)
                    {
                        qosEvent.objectPath = GoToSaber(qosEvent.theObject.transform);
                    }
                    Debug.Log(JsonUtility.ToJson(qevent.events, true));

                }
            }
        }
        catch (InvalidOperationException)
        { }
        catch (MissingReferenceException ex)
        { 
            OnFocus();
            throw ex;
        }
        GUILayout.EndScrollView();
    }

    string GoToSaber(Transform eventObject)
    {
        string result = eventObject.name;
        Transform current = eventObject.parent;
        while (current.name != "LeftSaber" && current.name != "RightSaber" && current.parent != null)
        {
            result = current.name + "/" + result;
            current = current.parent;
        } 

        return result;
    }

    void AddEventMenu(EventReferences qevent)
    {
        DisplayEvent(qevent, newEvent);

        GUILayout.Space(5);
        if (newEvent.theObject == null) GUILayout.Label("GameObject to use is null, can't make an event like this!", EditorStyles.boldLabel);
        else if (GUILayout.Button("Add Event") && newEvent.theObject != null)
        {
            newEvent.objectPath = GoToSaber(newEvent.theObject.transform);
            qevent.events.events.Add(newEvent);
            newEvent = new QosmeticsEvent(newEvent);
        }
        GUILayout.Space(10);
    }

    void DisplayEvent(EventReferences qevent, QosmeticsEvent display)
    {
        display.theObject = EditorGUILayout.ObjectField("GameObject to use", display.theObject, typeof(GameObject), true) as GameObject;
        display.effect = (QosmeticsEvent.EffectType)EditorGUILayout.EnumPopup("Effect type", display.effect);
        display.type = (QosmeticsEvent.Event)EditorGUILayout.EnumPopup("Event Type", display.type);
        GUILayout.Space(5);

        switch (display.type)
        {
            case QosmeticsEvent.Event.OnSlice:
                ShowOnSlice(display);
                break;
            case QosmeticsEvent.Event.OnCombo:
                ShowOnCombo(display);
                break;
            case QosmeticsEvent.Event.OnEveryNCombo:
                ShowOnEveryNCombo(display);
                break;
            case QosmeticsEvent.Event.OnComboBreak:
                ShowOnComboBreak(display);
                break;
            case QosmeticsEvent.Event.MultiplierUp:
                ShowMultiplierUp(display);
                break;
            case QosmeticsEvent.Event.SaberStartColliding:
                ShowSaberStartColliding(display);
                break;
            case QosmeticsEvent.Event.SaberStopColliding:
                ShowSaberStopColliding(display);
                break;
            case QosmeticsEvent.Event.OnLevelStart:
                ShowOnLevelStart(display);
                break;
            case QosmeticsEvent.Event.OnLevelFail:
                ShowOnLevelFail(display);
                break;
            case QosmeticsEvent.Event.OnLevelEnded:
                ShowOnLevelEnded(display);
                break;
            case QosmeticsEvent.Event.OnLeftLightOn:
                ShowOnLeftLightOn(display);
                break;
            case QosmeticsEvent.Event.OnLeftLightOff:
                ShowOnLeftLightOff(display);
                break;
            case QosmeticsEvent.Event.OnRightLightOn:
                ShowOnRightLightOn(display);
                break;
            case QosmeticsEvent.Event.OnRightLightOff:
                ShowOnRightLightOff(display);
                break;
        }
    }

    void ShowOnSlice(QosmeticsEvent display)
    {
        
        switch (display.effect)
        {
            case QosmeticsEvent.EffectType.GameObject:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                break;
            case QosmeticsEvent.EffectType.Bool:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                display.nameToSet = EditorGUILayout.TextField("Boolean to set", display.nameToSet);
                break;
            case QosmeticsEvent.EffectType.Trigger:
                display.nameToSet = EditorGUILayout.TextField("Trigger to set", display.nameToSet);
                break;
        }
    }

    void ShowOnEveryNCombo(QosmeticsEvent display)
    {
        switch (display.effect)
        {
            case QosmeticsEvent.EffectType.GameObject:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                display.integer = EditorGUILayout.IntField("Combo Value to Compare", display.integer);
                break;
            case QosmeticsEvent.EffectType.Bool:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                display.nameToSet = EditorGUILayout.TextField("Boolean to set", display.nameToSet);
                display.integer = EditorGUILayout.IntField("Combo Value to Compare", display.integer);
                break;
            case QosmeticsEvent.EffectType.Trigger:
                display.nameToSet = EditorGUILayout.TextField("Trigger to set", display.nameToSet);
                display.integer = EditorGUILayout.IntField("Combo Value to Compare", display.integer);
                break;
        }
    }

    void ShowOnCombo(QosmeticsEvent display)
    {
        switch (display.effect)
        {
            case QosmeticsEvent.EffectType.GameObject:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                display.integer = EditorGUILayout.IntField("Combo Value to Compare", display.integer);
                
                break;
            case QosmeticsEvent.EffectType.Bool:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                display.nameToSet = EditorGUILayout.TextField("Boolean to set", display.nameToSet);
                display.integer = EditorGUILayout.IntField("Combo Value to Compare", display.integer);
                
                break;
            case QosmeticsEvent.EffectType.Trigger:
                display.nameToSet = EditorGUILayout.TextField("Trigger to set", display.nameToSet);
                display.integer = EditorGUILayout.IntField("Combo Value to Compare", display.integer);
                
                break;
        }
    }

    void ShowOnComboBreak(QosmeticsEvent display)
    {
        switch (display.effect)
        {
            case QosmeticsEvent.EffectType.GameObject:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
               
                break;
            case QosmeticsEvent.EffectType.Bool:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                display.nameToSet = EditorGUILayout.TextField("Boolean to set", display.nameToSet);
                break;
            case QosmeticsEvent.EffectType.Trigger:
                display.nameToSet = EditorGUILayout.TextField("Trigger to set", display.nameToSet);
                break;
        }
    }
    private enum multiplier
    {
        [InspectorName("2x")]
        Two = 2,
        [InspectorName("4x")]
        Four = 4,
        [InspectorName("8x")]
        Eight = 8
    }


    void ShowMultiplierUp(QosmeticsEvent display)
    {
        if (!(display.integer == 2 ||
            display.integer == 4 ||
            display.integer == 8))
            display.integer = 2;
        multiplier temp = (multiplier)display.integer;

        switch (display.effect)
        {
            
            case QosmeticsEvent.EffectType.GameObject:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                display.integer = (int)(multiplier)EditorGUILayout.EnumPopup("Value to Compare", temp);
                break;
            case QosmeticsEvent.EffectType.Bool:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                display.nameToSet = EditorGUILayout.TextField("Boolean to set", display.nameToSet);
                display.integer = (int)(multiplier)EditorGUILayout.EnumPopup("Value to Compare", temp);
                break;
            case QosmeticsEvent.EffectType.Trigger:
                display.nameToSet = EditorGUILayout.TextField("Trigger to set", display.nameToSet);
                display.integer = (int)(multiplier)EditorGUILayout.EnumPopup("Value to Compare", temp);
                break;
        }
    }

    void ShowSaberStartColliding(QosmeticsEvent display)
    {
        switch (display.effect)
        {
            case QosmeticsEvent.EffectType.GameObject:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                break;
            case QosmeticsEvent.EffectType.Bool:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                display.nameToSet = EditorGUILayout.TextField("Boolean to set", display.nameToSet);
                break;
            case QosmeticsEvent.EffectType.Trigger:
                display.nameToSet = EditorGUILayout.TextField("Trigger to set", display.nameToSet);
                break;
        }
    }

    void ShowSaberStopColliding(QosmeticsEvent display)
    {
        switch (display.effect)
        {
            case QosmeticsEvent.EffectType.GameObject:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                break;
            case QosmeticsEvent.EffectType.Bool:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                display.nameToSet = EditorGUILayout.TextField("Boolean to set", display.nameToSet);
                break;
            case QosmeticsEvent.EffectType.Trigger:
                display.nameToSet = EditorGUILayout.TextField("Trigger to set", display.nameToSet);
                break;
        }
    }

    void ShowOnLevelStart(QosmeticsEvent display)
    {
        switch (display.effect)
        {
            case QosmeticsEvent.EffectType.GameObject:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                break;
            case QosmeticsEvent.EffectType.Bool:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                display.nameToSet = EditorGUILayout.TextField("Boolean to set", display.nameToSet);
                break;
            case QosmeticsEvent.EffectType.Trigger:
                display.nameToSet = EditorGUILayout.TextField("Trigger to set", display.nameToSet);
                break;
        }
    }

    void ShowOnLevelFail(QosmeticsEvent display)
    {
        switch (display.effect)
        {
            case QosmeticsEvent.EffectType.GameObject:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                break;
            case QosmeticsEvent.EffectType.Bool:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                display.nameToSet = EditorGUILayout.TextField("Boolean to set", display.nameToSet);
                break;
            case QosmeticsEvent.EffectType.Trigger:
                display.nameToSet = EditorGUILayout.TextField("Trigger to set", display.nameToSet);
                break;
        }
    }

    void ShowOnLevelEnded(QosmeticsEvent display)
    {
        switch (display.effect)
        {
            case QosmeticsEvent.EffectType.GameObject:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                break;
            case QosmeticsEvent.EffectType.Bool:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                display.nameToSet = EditorGUILayout.TextField("Boolean to set", display.nameToSet);
                break;
            case QosmeticsEvent.EffectType.Trigger:
                display.nameToSet = EditorGUILayout.TextField("Trigger to set", display.nameToSet);
                break;
        }
    }

    void ShowOnLeftLightOn(QosmeticsEvent display)
    {
        switch (display.effect)
        {
            case QosmeticsEvent.EffectType.GameObject:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                break;
            case QosmeticsEvent.EffectType.Bool:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                display.nameToSet = EditorGUILayout.TextField("Boolean to set", display.nameToSet);
                break;
            case QosmeticsEvent.EffectType.Trigger:
                display.nameToSet = EditorGUILayout.TextField("Trigger to set", display.nameToSet);
                break;
        }
    }

    void ShowOnRightLightOn(QosmeticsEvent display)
    {
        switch (display.effect)
        {
            case QosmeticsEvent.EffectType.GameObject:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                break;
            case QosmeticsEvent.EffectType.Bool:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                display.nameToSet = EditorGUILayout.TextField("Boolean to set", display.nameToSet);
                break;
            case QosmeticsEvent.EffectType.Trigger:
                display.nameToSet = EditorGUILayout.TextField("Trigger to set", display.nameToSet);
                break;
        }
    }

    void ShowOnLeftLightOff(QosmeticsEvent display)
    {
        switch (display.effect)
        {
            case QosmeticsEvent.EffectType.GameObject:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                break;
            case QosmeticsEvent.EffectType.Bool:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                display.nameToSet = EditorGUILayout.TextField("Boolean to set", display.nameToSet);
                break;
            case QosmeticsEvent.EffectType.Trigger:
                display.nameToSet = EditorGUILayout.TextField("Trigger to set", display.nameToSet);
                break;
        }
    }

    void ShowOnRightLightOff(QosmeticsEvent display)
    {
        switch (display.effect)
        {
            case QosmeticsEvent.EffectType.GameObject:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                break;
            case QosmeticsEvent.EffectType.Bool:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                display.nameToSet = EditorGUILayout.TextField("Boolean to set", display.nameToSet);
                break;
            case QosmeticsEvent.EffectType.Trigger:
                display.nameToSet = EditorGUILayout.TextField("Trigger to set", display.nameToSet);
                break;
        }
    }

    void displayWhich(EventReferences qevent)
    {
        string objectName = qevent.gameObject.name;
        string parentName = qevent.gameObject.transform.parent.gameObject.name;

        string msg = "Event reference on " + parentName + "/" + objectName;
        GUILayout.Label(msg, EditorStyles.boldLabel);
    }

    private void OnFocus()
    {
        questevents = FindObjectsOfType<EventReferences>();
    }
}
