#pragma once

#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "Data/DescriptorCache.hpp"
#include "Types/Note/NoteManager.hpp"
#include "UI/Note/NotePreviewViewController.hpp"
#include "UnityEngine/UI/HorizontalLayoutGroup.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics::UI, NoteSelectionElement, UnityEngine::MonoBehaviour, 
    DECLARE_INSTANCE_FIELD_DEFAULT(NoteManager*, modelManager, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(NotePreviewViewController*, previewViewController, nullptr);

    DECLARE_METHOD(void, Init, NoteManager* noteManager, NotePreviewViewController* previewViewController);
    public:
        void Select();
        void SetupButtons();
        void Delete();
        void UpdateData();
        void SetDescriptor(Descriptor* descriptor);
        custom_types::Helpers::Coroutine ButtonSetupRoutine();
        
        Descriptor& get_descriptor();
        
    protected:
        Descriptor* descriptor;

    REGISTER_FUNCTION(
        REGISTER_FIELD(modelManager);
        REGISTER_FIELD(previewViewController);

        REGISTER_METHOD(Init);
    )
)