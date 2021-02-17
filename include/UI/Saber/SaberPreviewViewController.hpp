#pragma once
#include "custom-types/shared/macros.hpp"
#include "Types/Saber/SaberManager.hpp"
#include "HMUI/ViewController.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics::UI, SaberPreviewViewController, HMUI::ViewController,
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_METHOD(void, Init, Qosmetics::SaberManager* saberManager);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::SaberManager*, modelManager, nullptr);
    REGISTER_FUNCTION(SaberPreviewViewController,
        REGISTER_METHOD(DidActivate);
        REGISTER_FIELD(modelManager);
    )
)