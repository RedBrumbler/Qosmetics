#pragma once
#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/coroutine.hpp"

#include "HMUI/ViewController.hpp"
#include "Types/Saber/SaberManager.hpp"
#include "UI/Saber/SaberPreviewViewController.hpp"
#include "UI/Saber/SaberDeletionElement.hpp"

#include "Types/Colors/ColorManager.hpp"
#include "Zenject/DiContainer.hpp"

struct switcherInfo;
namespace Qosmetics::UI {
    class SaberSelectionElement;
}

DECLARE_CLASS_CODEGEN(Qosmetics::UI, SaberSwitcherViewController, HMUI::ViewController,
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::SaberManager*, modelManager, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::UI::SaberPreviewViewController*, previewViewController, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::UI::SaberDeletionElement*, deletionElement, nullptr);
    
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_INSTANCE_METHOD(void, Init, Qosmetics::SaberManager* modelManager, Qosmetics::UI::SaberPreviewViewController* previewViewController);
    
    public:
        custom_types::Helpers::Coroutine SetupSelectionsRoutine(switcherInfo* info);
        void AttemptDeletion(Qosmetics::SaberSelectionElement* elem);
)