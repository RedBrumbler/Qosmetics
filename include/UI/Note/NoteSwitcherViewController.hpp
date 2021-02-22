#pragma once
#include "custom-types/shared/macros.hpp"
#include "Types/Note/NoteManager.hpp"

#include "HMUI/ViewController.hpp"
#include "zenjeqt/shared/Zenjeqtor.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics::UI, NoteSwitcherViewController, HMUI::ViewController,
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_METHOD(void, Init);
    REGISTER_FUNCTION(NoteSwitcherViewController,
        REGISTER_METHOD(DidActivate);
        REGISTER_METHOD_INJECT(Init);
    )
)