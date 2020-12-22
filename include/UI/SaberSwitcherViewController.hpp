#pragma once
#include "custom-types/shared/types.hpp"
#include "custom-types/shared/macros.hpp"

#include "TMPro/TextMeshProUGUI.hpp"
#include "HMUI/ViewController.hpp"
#include "UnityEngine/Sprite.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, SaberSwitcherViewController, HMUI::ViewController, 
    
    DECLARE_INSTANCE_FIELD(UnityEngine::Sprite*, cookieSprite);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, cookiesTextMesh);
    DECLARE_INSTANCE_FIELD(int, cookies);


    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_OVERRIDE_METHOD(void, DidDeactivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidDeactivate", 2), bool removedFromHierarchy, bool screenSystemDisabling);

    REGISTER_FUNCTION(SaberSwitcherViewController,
        REGISTER_FIELD(cookieSprite);
        REGISTER_FIELD(cookiesTextMesh);
        REGISTER_FIELD(cookies);

        REGISTER_METHOD(DidActivate);
        REGISTER_METHOD(DidDeactivate);
    )
)