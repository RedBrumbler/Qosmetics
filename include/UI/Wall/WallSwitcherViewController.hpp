#pragma once
#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "Types/Wall/WallManager.hpp"
#include "UI/Wall/WallPreviewViewController.hpp"
#include "UI/Wall/WallDeletionElement.hpp"

#include "HMUI/ViewController.hpp"

struct switcherInfo;
namespace Qosmetics::UI {
    class WallSelectionElement;
}

DECLARE_CLASS_CODEGEN(Qosmetics::UI, WallSwitcherViewController, HMUI::ViewController,
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::WallManager*, modelManager, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::UI::WallPreviewViewController*, previewViewController, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::UI::WallDeletionElement*, deletionElement, nullptr);

    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_INSTANCE_METHOD(void, Init, Qosmetics::WallManager* modelManager, Qosmetics::UI::WallPreviewViewController* previewViewController);

    public:    
        custom_types::Helpers::Coroutine SetupSelectionsRoutine(switcherInfo* info);
        void AttemptDeletion(Qosmetics::UI::WallSelectionElement* elem);
)