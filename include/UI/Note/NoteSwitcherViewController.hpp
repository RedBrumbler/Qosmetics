#pragma once
#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "Types/Note/NoteManager.hpp"
#include "UI/Note/NotePreviewViewController.hpp"
#include "UI/Note/NoteDeletionElement.hpp"

#include "HMUI/ViewController.hpp"

struct switcherInfo;

namespace Qosmetics::UI {
    class NoteSelectionElement;
}

DECLARE_CLASS_CODEGEN(Qosmetics::UI, NoteSwitcherViewController, HMUI::ViewController,
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::NoteManager*, modelManager, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::UI::NotePreviewViewController*, previewViewController, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::UI::NoteDeletionElement*, deletionElement, nullptr);

    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_INSTANCE_METHOD(void, Init, Qosmetics::NoteManager* modelManager, Qosmetics::UI::NotePreviewViewController* previewViewController);

    public:    
        custom_types::Helpers::Coroutine SetupSelectionsRoutine(switcherInfo* info);
        void AttemptDeletion(Qosmetics::UI::NoteSelectionElement* elem);
)