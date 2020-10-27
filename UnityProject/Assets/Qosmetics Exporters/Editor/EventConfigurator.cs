using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using Qosmetics;
using UnityEngine.UI;
using System;

public class EventConfigurator : EditorWindow
{
    private EventReferences[] questevents;
    private QosmeticsEvent newEvent = new QosmeticsEvent();

    [MenuItem("Tools/Qosmetics/Event Configurator")]
    public static void ShowWindow()
    {
        EditorWindow.GetWindow(typeof(EventConfigurator), false, "Event Configurator");
    }

    void OnGUI()
    {
        GUILayout.Label("Event Configurator", EditorStyles.boldLabel);
        foreach (var qevent in questevents)
        {
            displayWhich(qevent);
            qevent.hideAddMenu = EditorGUILayout.BeginFoldoutHeaderGroup(qevent.hideAddMenu, "Add new Event Menu");

            if (qevent.hideAddMenu)
            {
                AddEventMenu(qevent);
            }
            GUILayout.Space(10);
            try
            {
                foreach (var qosEvent in qevent.events.events)
                {
                    GUILayout.Label(qosEvent.type.ToString() + " Event " + qosEvent.objectEvent.ToString(), EditorStyles.boldLabel);
                    DisplayEvent(qevent, qosEvent);
                    if (GUILayout.Button("Remove Event"))
                    {
                        qevent.events.events.Remove(qosEvent);
                    }
                }
            }
            catch (InvalidOperationException)
            {}
        }
    }

    void AddEventMenu(EventReferences qevent)
    {
        
        DisplayEvent(qevent, newEvent);

        GUILayout.Space(5);

        if (GUILayout.Button("Add Event"))
        {
            qevent.events.events.Add(newEvent);
            newEvent = new QosmeticsEvent();
        }
    }

    void DisplayEvent(EventReferences qevent, QosmeticsEvent display)
    {
        display.type = (QosmeticsEvent.EventType)EditorGUILayout.EnumPopup("Effect type", display.type);
        display.objectEvent = (QosmeticsEvent.Event)EditorGUILayout.EnumPopup("Event Type", display.objectEvent);
        GUILayout.Space(5);

        switch (display.objectEvent)
        {
            case QosmeticsEvent.Event.OnSlice:
                ShowOnSlice(display);
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
            case QosmeticsEvent.Event.OnRightLightOn:
                ShowOnRightLightOn(display);
                break;
        }
    }

    void ShowOnSlice(QosmeticsEvent display)
    {
        switch (display.type)
        {
            case QosmeticsEvent.EventType.GameObject:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                break;
            case QosmeticsEvent.EventType.Bool:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                display.nameToSet = EditorGUILayout.TextField("Boolean to set", display.nameToSet);
                break;
            case QosmeticsEvent.EventType.Trigger:
                display.nameToSet = EditorGUILayout.TextField("Trigger to set", display.nameToSet);
                break;
        }
    }

    void ShowOnComboBreak(QosmeticsEvent display)
    {
        switch (display.type)
        {
            case QosmeticsEvent.EventType.GameObject:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                display.integer = EditorGUILayout.IntField("Combo value to compare", display.integer);
                newEvent.HandleValue = (QosmeticsEvent.ValueHandle)EditorGUILayout.EnumPopup("How to Handle Value", newEvent.HandleValue);
                break;
            case QosmeticsEvent.EventType.Bool:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                display.nameToSet = EditorGUILayout.TextField("Boolean to set", display.nameToSet);
                display.integer = EditorGUILayout.IntField("Combo value to compare", display.integer);
                newEvent.HandleValue = (QosmeticsEvent.ValueHandle)EditorGUILayout.EnumPopup("How to Handle Value", newEvent.HandleValue);
                break;
            case QosmeticsEvent.EventType.Trigger:
                display.nameToSet = EditorGUILayout.TextField("Trigger to set", display.nameToSet);
                display.integer = EditorGUILayout.IntField("Combo value to compare", display.integer);
                newEvent.HandleValue = (QosmeticsEvent.ValueHandle)EditorGUILayout.EnumPopup("How to Handle Value", newEvent.HandleValue);
                break;
        }
    }

    void ShowMultiplierUp(QosmeticsEvent display)
    {
        switch (display.type)
        {
            case QosmeticsEvent.EventType.GameObject:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                display.integer = EditorGUILayout.IntField("Multiplier value to compare", display.integer);
                newEvent.HandleValue = (QosmeticsEvent.ValueHandle)EditorGUILayout.EnumPopup("How to Handle Value", newEvent.HandleValue);
                break;
            case QosmeticsEvent.EventType.Bool:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                display.nameToSet = EditorGUILayout.TextField("Boolean to set", display.nameToSet);
                display.integer = EditorGUILayout.IntField("Multiplier value to compare", display.integer);
                newEvent.HandleValue = (QosmeticsEvent.ValueHandle)EditorGUILayout.EnumPopup("How to Handle Value", newEvent.HandleValue);
                break;
            case QosmeticsEvent.EventType.Trigger:
                display.nameToSet = EditorGUILayout.TextField("Trigger to set", display.nameToSet);
                display.integer = EditorGUILayout.IntField("Multiplier value to compare", display.integer);
                newEvent.HandleValue = (QosmeticsEvent.ValueHandle)EditorGUILayout.EnumPopup("How to Handle Value", newEvent.HandleValue);
                break;
        }
    }

    void ShowSaberStartColliding(QosmeticsEvent display)
    {
        switch (display.type)
        {
            case QosmeticsEvent.EventType.GameObject:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                break;
            case QosmeticsEvent.EventType.Bool:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                display.nameToSet = EditorGUILayout.TextField("Boolean to set", display.nameToSet);
                break;
            case QosmeticsEvent.EventType.Trigger:
                display.nameToSet = EditorGUILayout.TextField("Trigger to set", display.nameToSet);
                break;
        }
    }

    void ShowSaberStopColliding(QosmeticsEvent display)
    {
        switch (display.type)
        {
            case QosmeticsEvent.EventType.GameObject:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                break;
            case QosmeticsEvent.EventType.Bool:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                display.nameToSet = EditorGUILayout.TextField("Boolean to set", display.nameToSet);
                break;
            case QosmeticsEvent.EventType.Trigger:
                display.nameToSet = EditorGUILayout.TextField("Trigger to set", display.nameToSet);
                break;
        }
    }

    void ShowOnLevelStart(QosmeticsEvent display)
    {
        switch (display.type)
        {
            case QosmeticsEvent.EventType.GameObject:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                break;
            case QosmeticsEvent.EventType.Bool:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                display.nameToSet = EditorGUILayout.TextField("Boolean to set", display.nameToSet);
                break;
            case QosmeticsEvent.EventType.Trigger:
                display.nameToSet = EditorGUILayout.TextField("Trigger to set", display.nameToSet);
                break;
        }
    }

    void ShowOnLevelFail(QosmeticsEvent display)
    {
        switch (display.type)
        {
            case QosmeticsEvent.EventType.GameObject:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                break;
            case QosmeticsEvent.EventType.Bool:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                display.nameToSet = EditorGUILayout.TextField("Boolean to set", display.nameToSet);
                break;
            case QosmeticsEvent.EventType.Trigger:
                display.nameToSet = EditorGUILayout.TextField("Trigger to set", display.nameToSet);
                break;
        }
    }

    void ShowOnLevelEnded(QosmeticsEvent display)
    {
        switch (display.type)
        {
            case QosmeticsEvent.EventType.GameObject:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                break;
            case QosmeticsEvent.EventType.Bool:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                display.nameToSet = EditorGUILayout.TextField("Boolean to set", display.nameToSet);
                break;
            case QosmeticsEvent.EventType.Trigger:
                display.nameToSet = EditorGUILayout.TextField("Trigger to set", display.nameToSet);
                break;
        }
    }

    void ShowOnLeftLightOn(QosmeticsEvent display)
    {
        switch (display.type)
        {
            case QosmeticsEvent.EventType.GameObject:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                break;
            case QosmeticsEvent.EventType.Bool:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                display.nameToSet = EditorGUILayout.TextField("Boolean to set", display.nameToSet);
                break;
            case QosmeticsEvent.EventType.Trigger:
                display.nameToSet = EditorGUILayout.TextField("Trigger to set", display.nameToSet);
                break;
        }
    }

    void ShowOnRightLightOn(QosmeticsEvent display)
    {
        switch (display.type)
        {
            case QosmeticsEvent.EventType.GameObject:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                break;
            case QosmeticsEvent.EventType.Bool:
                display.boolean = EditorGUILayout.Toggle("Value to set", display.boolean);
                display.nameToSet = EditorGUILayout.TextField("Boolean to set", display.nameToSet);
                break;
            case QosmeticsEvent.EventType.Trigger:
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
