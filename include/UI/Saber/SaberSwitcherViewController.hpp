#pragma once
#include "custom-types/shared/macros.hpp"

#include "HMUI/ViewController.hpp"
#include "Types/Saber/SaberManager.hpp"
#include "UI/Saber/SaberPreviewViewController.hpp"
#include "Types/Colors/ColorManager.hpp"
#include "Zenject/DiContainer.hpp"
#include "zenjeqt/shared/Zenjeqtor.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics::UI, SaberSwitcherViewController, HMUI::ViewController,
    DECLARE_INSTANCE_FIELD_DEFAULT(Zenject::DiContainer*, container, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::SaberManager*, modelManager, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::UI::SaberPreviewViewController*, previewViewController, nullptr);
    
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_METHOD(void, Init, Qosmetics::SaberManager* modelManager, Qosmetics::UI::SaberPreviewViewController* previewViewController, Zenject::DiContainer* container);
    REGISTER_FUNCTION(SaberSwitcherViewController,
        REGISTER_METHOD(DidActivate);
        REGISTER_METHOD_INJECT(Init);
        REGISTER_FIELD(container);
        REGISTER_FIELD(modelManager);
        REGISTER_FIELD(previewViewController);
    )
)