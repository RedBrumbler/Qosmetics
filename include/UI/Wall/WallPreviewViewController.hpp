#pragma once
#include "custom-types/shared/macros.hpp"

#include "HMUI/ViewController.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics::UI, WallPreviewViewController, HMUI::ViewController,
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    REGISTER_FUNCTION(WallPreviewViewController,
        REGISTER_METHOD(DidActivate);
    )
)