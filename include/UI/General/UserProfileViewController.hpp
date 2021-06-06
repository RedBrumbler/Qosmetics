#pragma once
#include "custom-types/shared/macros.hpp"

#include "HMUI/ViewController.hpp"

#include "Data/Descriptor.hpp"
#include "TMPro/TextMeshProUGUI.hpp"

#include "Types/Saber/SaberManager.hpp"
#include "Types/Note/NoteManager.hpp"
#include "Types/Wall/WallManager.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics::UI, UserProfileViewController, HMUI::ViewController,
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_METHOD(void, Init, Qosmetics::SaberManager* saberManager, Qosmetics::NoteManager* noteManager, Qosmetics::WallManager* wallManager);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::SaberManager*, saberManager, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::NoteManager*, noteManager, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::WallManager*, wallManager, nullptr);
    
    REGISTER_FUNCTION(
        REGISTER_METHOD(DidActivate);
        REGISTER_METHOD(Init);

        REGISTER_FIELD(saberManager);
        REGISTER_FIELD(noteManager);
        REGISTER_FIELD(wallManager);
    )
)