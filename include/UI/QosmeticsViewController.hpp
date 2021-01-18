#pragma once
#include "custom-types/shared/types.hpp"
#include "custom-types/shared/macros.hpp"

#include "TMPro/TextMeshProUGUI.hpp"
#include "HMUI/ViewController.hpp"
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/Transform.hpp"
#include "questui/shared/CustomTypes/Data/CustomDataType.hpp"
#include "questui/shared/eventmacros.hpp"
#include "System/Action_1.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, QosmeticsViewController, HMUI::ViewController,
    DECLARE_EVENT(System::Action_1<QuestUI::CustomDataType*>*, openSubMenu);
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);

    REGISTER_FUNCTION(QosmeticsViewController,
        REGISTER_METHOD(DidActivate);
        REGISTER_EVENT(openSubMenu);
    )
)