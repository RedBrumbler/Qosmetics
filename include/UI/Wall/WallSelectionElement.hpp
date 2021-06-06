#pragma once

#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "Data/DescriptorCache.hpp"
#include "Types/Wall/WallManager.hpp"
#include "UI/Wall/WallPreviewViewController.hpp"
#include "UnityEngine/UI/HorizontalLayoutGroup.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics::UI, WallSelectionElement, UnityEngine::MonoBehaviour, 
    DECLARE_INSTANCE_FIELD_DEFAULT(WallManager*, modelManager, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(WallPreviewViewController*, previewViewController, nullptr);

    DECLARE_METHOD(void, Init, WallManager* wallManager, WallPreviewViewController* previewViewController);
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