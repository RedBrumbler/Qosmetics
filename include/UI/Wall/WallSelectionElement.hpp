#pragma once

#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "Data/DescriptorCache.hpp"
#include "Types/Wall/WallManager.hpp"
#include "UI/Wall/WallPreviewViewController.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics::UI, WallSelectionElement, UnityEngine::MonoBehaviour, 
    DECLARE_INSTANCE_FIELD_DEFAULT(WallManager*, modelManager, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(WallPreviewViewController*, previewViewController, nullptr);

    DECLARE_METHOD(void, Init, WallManager* WallManager);
    public:
        void Select();
        void Delete();
        void UpdateData();
        void SetDescriptor(Descriptor* descriptor);

    protected:
        Descriptor* descriptor;

    REGISTER_FUNCTION(WallSelectionElement,
        REGISTER_FIELD(modelManager);
        REGISTER_FIELD(previewViewController);

        REGISTER_METHOD(Init);
    )
)