#pragma once
#include "custom-types/shared/macros.hpp"

#include "HMUI/ViewController.hpp"
#include "Types/Saber/SaberManager.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics::UI, SaberSwitcherViewController, HMUI::ViewController,
    DECLARE_STATIC_FIELD(Qosmetics::SaberManager*, modelManager);
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    REGISTER_FUNCTION(SaberSwitcherViewController,
        REGISTER_METHOD(DidActivate);
        REGISTER_FIELD(modelManager);
    )
)