#pragma once
#include "custom-types/shared/macros.hpp"
#include "Types/Wall/WallManager.hpp"

#include "HMUI/ViewController.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics::UI, WallSwitcherViewController, HMUI::ViewController,
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    REGISTER_FUNCTION(WallSwitcherViewController,
        REGISTER_METHOD(DidActivate);
    )
)