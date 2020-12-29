#pragma once
#include "custom-types/shared/types.hpp"
#include "custom-types/shared/macros.hpp"

#include "TMPro/TextMeshProUGUI.hpp"
#include "HMUI/ViewController.hpp"
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/GameObject.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, SaberPreviewViewController, HMUI::ViewController,
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_OVERRIDE_METHOD(void, DidDeactivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidDeactivate", 2), bool removedFromHierarchy, bool screenSystemDisabling);

    DECLARE_METHOD(void, UpdatePreview);
    DECLARE_INSTANCE_FIELD_DEFAULT(UnityEngine::GameObject*, previewprefab, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(TMPro::TextMeshProUGUI*, title, nullptr);

    REGISTER_FUNCTION(SaberPreviewViewController,
        REGISTER_METHOD(DidActivate);
        REGISTER_METHOD(DidDeactivate);
        REGISTER_METHOD(UpdatePreview);
        REGISTER_FIELD(previewprefab);
        REGISTER_FIELD(title);
    )
)