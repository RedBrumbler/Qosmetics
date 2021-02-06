#pragma once

#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "Data/DescriptorCache.hpp"
#include "Types/Saber/SaberManager.hpp"
#include "UI/Saber/SaberPreviewViewController.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics::UI, SaberSelectionElement, UnityEngine::MonoBehaviour, 
    DECLARE_INSTANCE_FIELD_DEFAULT(SaberManager*, modelManager, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(SaberPreviewViewController*, previewViewController, nullptr);

    DECLARE_METHOD(void, Init, SaberManager* SaberManager);
    public:
        void Select();
        void Delete();
        void UpdateData();
        void SetDescriptor(Descriptor* descriptor);

    protected:
        Descriptor* descriptor;

    REGISTER_FUNCTION(SaberSelectionElement,
        REGISTER_FIELD(modelManager);
        REGISTER_FIELD(previewViewController);

        REGISTER_METHOD(Init);
    )
)