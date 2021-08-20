#pragma once
#include "custom-types/shared/macros.hpp"

#include "HMUI/ViewController.hpp"

#include <functional>

#include "Data/Descriptor.hpp"
#include "TMPro/TextMeshProUGUI.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics::UI, QosmeticsViewController, HMUI::ViewController,
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_INSTANCE_FIELD_DEFAULT(TMPro::TextMeshProUGUI*, title, nullptr);
    DECLARE_INSTANCE_METHOD(void, Init);
    
    public:
        void set_selectCallback(std::function<void(ItemType)> callback);
        std::function<void(ItemType)> callback;
)