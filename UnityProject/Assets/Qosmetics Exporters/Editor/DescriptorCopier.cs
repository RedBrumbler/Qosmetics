using Qosmetics;
using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

public class DescriptorCopier : EditorWindow
{
    GameObject objectToCopy;

    [MenuItem("Tools/Qosmetics/Descriptor Copy")]
    public static void ShowWindow()
    {
        EditorWindow.GetWindow(typeof(DescriptorCopier), false, "Qosmetics Descriptor copy");
    }

    void OnGUI()
    {
        GUILayout.Label("If you have any older GameObjects\nthat you want to copy the descriptor from\nput them in here and press the button", EditorStyles.boldLabel);
        objectToCopy = EditorGUILayout.ObjectField("Object to Copy Descriptor on", objectToCopy, typeof(GameObject), true) as GameObject;

        if (GUILayout.Button("Copy Descriptor Data"))
        {
            CopySaberDescriptor();
            CopyBloqDescriptor();
            CopyWallDescriptor();
        }
    }

    void CopySaberDescriptor()
    {
        SaberReferences reference = objectToCopy.GetComponent<SaberReferences>();
        if (!reference) return;

        Copy(reference.bmbfmod, reference.descriptor);
    }

    void CopyWallDescriptor()
    {
        WallReferences reference = objectToCopy.GetComponent<WallReferences>();
        if (!reference) return;

        Copy(reference.bmbfmod, reference.descriptor);
    }

    void CopyBloqDescriptor()
    {
        BloqReferences reference = objectToCopy.GetComponent<BloqReferences>();
        if (!reference) return;

        Copy(reference.bmbfmod, reference.descriptor);
    }

    void Copy(BMBFmod.mod mod, Descriptor descriptor)
    {
        descriptor.objectName = mod.name;
        descriptor.authorName = mod.author;
        if (mod.description.Count > 0)
            descriptor.description = mod.description[0];
    }
}
