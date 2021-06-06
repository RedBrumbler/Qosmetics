#pragma once
#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/coroutine.hpp"

#include "HMUI/ViewController.hpp"
#include "UI/Wall/WallPreviewViewController.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics::UI, WallSettingsViewController, HMUI::ViewController,
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_METHOD(void, Init, WallPreviewViewController* previewViewController);
    DECLARE_INSTANCE_FIELD_DEFAULT(WallPreviewViewController*, previewViewController, nullptr);
    
    public:
        custom_types::Helpers::Coroutine SettingsSetupRoutine(UnityEngine::GameObject* container);

    REGISTER_FUNCTION(
        REGISTER_METHOD(DidActivate);
        REGISTER_METHOD(Init);
        REGISTER_FIELD(previewViewController);
    )
)