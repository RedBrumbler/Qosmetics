#pragma once
#include "custom-types/shared/macros.hpp"

#include "HMUI/ViewController.hpp"
#include "custom-types/shared/coroutine.hpp"

struct donatorInfo;

DECLARE_CLASS_CODEGEN(Qosmetics::UI, PatronViewController, HMUI::ViewController,
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_INSTANCE_METHOD(void, Init);

    public:
        custom_types::Helpers::Coroutine AddDonators(donatorInfo* info);
)