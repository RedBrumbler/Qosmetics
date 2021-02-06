#pragma once

#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "Data/DescriptorCache.hpp"
#include "Types/Note/NoteManager.hpp"
#include "UI/Note/NotePreviewViewController.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics::UI, NoteSelectionElement, UnityEngine::MonoBehaviour, 
    DECLARE_INSTANCE_FIELD_DEFAULT(NoteManager*, modelManager, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(NotePreviewViewController*, previewViewController, nullptr);

    DECLARE_METHOD(void, Init, NoteManager* noteManager);
    public:
        void Select();
        void Delete();
        void UpdateData();
        void SetDescriptor(Descriptor* descriptor);

    protected:
        Descriptor* descriptor;

    REGISTER_FUNCTION(NoteSelectionElement,
        REGISTER_FIELD(modelManager);
        REGISTER_FIELD(previewViewController);

        REGISTER_METHOD(Init);
    )
)