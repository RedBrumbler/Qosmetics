#pragma once
#include "custom-types/shared/macros.hpp"
#include "Types/Saber/SaberManager.hpp"
#include "Types/Colors/ColorManager.hpp"
#include "HMUI/ViewController.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "zenjeqt/shared/Zenjeqtor.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics::UI, SaberPreviewViewController, HMUI::ViewController,
    DECLARE_OVERRIDE_METHOD(void, DidDeactivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidDeactivate", 2), bool removedFromHierarchy, bool screenSystemDisabling);
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_METHOD(void, Init, Qosmetics::SaberManager* saberManager, Qosmetics::ColorManager* colorManager);
    DECLARE_METHOD(void, UpdatePreview);
    DECLARE_METHOD(void, ShowLoading);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::SaberManager*, modelManager, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::ColorManager*, colorManager, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(TMPro::TextMeshProUGUI*, title, nullptr);
    REGISTER_FUNCTION(SaberPreviewViewController,
        REGISTER_METHOD(DidActivate);
        REGISTER_METHOD(DidDeactivate);
        REGISTER_METHOD_INJECT(Init);
        REGISTER_METHOD(UpdatePreview);
        REGISTER_METHOD(ShowLoading);
        REGISTER_FIELD(modelManager);
        REGISTER_FIELD(colorManager);
        REGISTER_FIELD(title);
    )
)