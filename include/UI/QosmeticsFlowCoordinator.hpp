#pragma once
#include "HMUI/ViewController.hpp"
#include "HMUI/NavigationController.hpp"
#include "HMUI/FlowCoordinator.hpp"

#include "UI/QosmeticsViewController.hpp"

#include "UI/Saber/SaberSwitcherViewController.hpp"
#include "UI/Saber/SaberPreviewViewController.hpp"
#include "UI/Saber/SaberSettingsViewController.hpp"

#include "UI/Note/NoteSwitcherViewController.hpp"
#include "UI/Note/NotePreviewViewController.hpp"
#include "UI/Note/NoteSettingsViewController.hpp"

#include "UI/Wall/WallSwitcherViewController.hpp"
#include "UI/Wall/WallPreviewViewController.hpp"
#include "UI/Wall/WallSettingsViewController.hpp"

#include "custom-types/shared/macros.hpp"

namespace Qosmetics {
    enum PreviousFlowCoordinatorType {
        invalid,
        settings,
        solo,
        party,
        campaign,
        multiHost,
        multiClient
    };
}

DECLARE_CLASS_CODEGEN(Qosmetics, QosmeticsFlowCoordinator, HMUI::FlowCoordinator,
    
    DECLARE_INSTANCE_FIELD_DEFAULT(HMUI::ViewController*, ActiveViewController, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::QosmeticsViewController*, QosmeticsViewController, nullptr);

    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::SaberSwitcherViewController*, SaberSwitcherViewController, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::SaberSettingsViewController*, SaberSettingsViewController, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::SaberPreviewViewController*, SaberPreviewViewController, nullptr);

    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::NoteSwitcherViewController*, NoteSwitcherViewController, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::NoteSettingsViewController*, NoteSettingsViewController, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::NotePreviewViewController*, NotePreviewViewController, nullptr);

    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::WallSwitcherViewController*, WallSwitcherViewController, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::WallSettingsViewController*, WallSettingsViewController, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::WallPreviewViewController*, WallPreviewViewController, nullptr);

    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "FlowCoordinator", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_OVERRIDE_METHOD(void, BackButtonWasPressed, il2cpp_utils::FindMethodUnsafe("HMUI", "FlowCoordinator", "BackButtonWasPressed", 1), HMUI::ViewController* topViewController);

    REGISTER_FUNCTION(QosmeticsFlowCoordinator,
        REGISTER_FIELD(ActiveViewController);
        REGISTER_FIELD(QosmeticsViewController);

        REGISTER_FIELD(SaberSwitcherViewController);
        REGISTER_FIELD(SaberSettingsViewController);
        REGISTER_FIELD(SaberPreviewViewController);

        REGISTER_FIELD(NoteSwitcherViewController);
        REGISTER_FIELD(NoteSettingsViewController);
        REGISTER_FIELD(NotePreviewViewController);

        REGISTER_FIELD(WallSwitcherViewController);
        REGISTER_FIELD(WallSettingsViewController);
        REGISTER_FIELD(WallPreviewViewController);

        REGISTER_METHOD(DidActivate);
        REGISTER_METHOD(BackButtonWasPressed);
    )
)