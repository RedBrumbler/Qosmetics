#pragma once
#include "custom-types/shared/macros.hpp"

#include "HMUI/ViewController.hpp"
#include "zenjeqt/shared/Zenjeqtor.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics::UI, WallSettingsViewController, HMUI::ViewController,
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_METHOD(void, Init);
    REGISTER_FUNCTION(WallSettingsViewController,
        REGISTER_METHOD(DidActivate);
        REGISTER_METHOD_INJECT(Init);
    )
)