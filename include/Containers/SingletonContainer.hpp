#pragma once

#include "beatsaber-hook/shared/utils/typedefs-wrappers.hpp"
#include "custom-types/shared/macros.hpp"
#include "UI/General/QosmeticsFlowCoordinator.hpp"

#include "UI/Saber/SaberPreviewViewController.hpp"
#include "UI/Saber/SaberSettingsViewController.hpp"
#include "UI/Saber/SaberSwitcherViewController.hpp"

#include "UI/Note/NotePreviewViewController.hpp"
#include "UI/Note/NoteSettingsViewController.hpp"
#include "UI/Note/NoteSwitcherViewController.hpp"

#include "UI/Wall/WallPreviewViewController.hpp"
#include "UI/Wall/WallSettingsViewController.hpp"
#include "UI/Wall/WallSwitcherViewController.hpp"

#include "UI/General/PatronViewController.hpp"
#include "UI/General/FloorLogoViewController.hpp"
#include "UI/General/QosmeticsFlowCoordinator.hpp"
#include "UI/General/QosmeticsViewController.hpp"
#include "UI/General/UserProfileViewController.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, SingletonContainer, Il2CppObject,
    DECLARE_INSTANCE_FIELD(Qosmetics::UI::QosmeticsFlowCoordinator*, qosmeticsFlowCoordinator);
    DECLARE_INSTANCE_FIELD(Qosmetics::SaberManager*, saberManager);
    DECLARE_INSTANCE_FIELD(Qosmetics::NoteManager*, noteManager);
    DECLARE_INSTANCE_FIELD(Qosmetics::WallManager*, wallManager);
    DECLARE_INSTANCE_FIELD(Qosmetics::ColorManager*, colorManager);

    DECLARE_INSTANCE_FIELD(Qosmetics::UI::SaberSwitcherViewController*, saberSwitcherViewController);
    DECLARE_INSTANCE_FIELD(Qosmetics::UI::SaberSettingsViewController*, saberSettingsViewController);
    DECLARE_INSTANCE_FIELD(Qosmetics::UI::SaberPreviewViewController*, saberPreviewViewController);
    
    DECLARE_INSTANCE_FIELD(Qosmetics::UI::NoteSwitcherViewController*, noteSwitcherViewController);
    DECLARE_INSTANCE_FIELD(Qosmetics::UI::NoteSettingsViewController*, noteSettingsViewController);
    DECLARE_INSTANCE_FIELD(Qosmetics::UI::NotePreviewViewController*, notePreviewViewController);

    DECLARE_INSTANCE_FIELD(Qosmetics::UI::WallSwitcherViewController*, wallSwitcherViewController);
    DECLARE_INSTANCE_FIELD(Qosmetics::UI::WallSettingsViewController*, wallSettingsViewController);
    DECLARE_INSTANCE_FIELD(Qosmetics::UI::WallPreviewViewController*, wallPreviewViewController);

    DECLARE_INSTANCE_FIELD(Qosmetics::UI::QosmeticsViewController*, qosmeticsViewController);
    DECLARE_INSTANCE_FIELD(Qosmetics::UI::PatronViewController*, patronViewController);
    DECLARE_INSTANCE_FIELD(Qosmetics::UI::FloorLogoViewController*, floorLogoViewController);
    DECLARE_INSTANCE_FIELD(Qosmetics::UI::UserProfileViewController*, userProfileViewController);

    DECLARE_METHOD(static Qosmetics::SingletonContainer*, get_instance);
    DECLARE_METHOD(static Qosmetics::UI::QosmeticsFlowCoordinator*, get_qosmeticsFlowCoordinator);
    DECLARE_METHOD(static Qosmetics::SaberManager*, get_saberManager);
    DECLARE_METHOD(static Qosmetics::NoteManager*, get_noteManager);
    DECLARE_METHOD(static Qosmetics::WallManager*, get_wallManager);
    DECLARE_METHOD(static Qosmetics::ColorManager*, get_colorManager);

    DECLARE_METHOD(static Qosmetics::UI::SaberSwitcherViewController*, get_saberSwitcherViewController);
    DECLARE_METHOD(static Qosmetics::UI::SaberSettingsViewController*, get_saberSettingsViewController);
    DECLARE_METHOD(static Qosmetics::UI::SaberPreviewViewController*, get_saberPreviewViewController);
    
    DECLARE_METHOD(static Qosmetics::UI::NoteSwitcherViewController*, get_noteSwitcherViewController);
    DECLARE_METHOD(static Qosmetics::UI::NoteSettingsViewController*, get_noteSettingsViewController);
    DECLARE_METHOD(static Qosmetics::UI::NotePreviewViewController*, get_notePreviewViewController);

    DECLARE_METHOD(static Qosmetics::UI::WallSwitcherViewController*, get_wallSwitcherViewController);
    DECLARE_METHOD(static Qosmetics::UI::WallSettingsViewController*, get_wallSettingsViewController);
    DECLARE_METHOD(static Qosmetics::UI::WallPreviewViewController*, get_wallPreviewViewController);

    DECLARE_METHOD(static Qosmetics::UI::QosmeticsViewController*, get_qosmeticsViewController);
    DECLARE_METHOD(static Qosmetics::UI::PatronViewController*, get_patronViewController);
    DECLARE_METHOD(static Qosmetics::UI::FloorLogoViewController*, get_floorLogoViewController);
    DECLARE_METHOD(static Qosmetics::UI::UserProfileViewController*, get_userProfileViewController);

    DECLARE_CTOR(ctor);

    public:
        static void Init();
    private:
        static SafePtr<SingletonContainer> instance;

    REGISTER_FUNCTION(
        REGISTER_METHOD(ctor);
        REGISTER_METHOD(get_instance);
        REGISTER_METHOD(get_qosmeticsFlowCoordinator);
        REGISTER_METHOD(get_saberManager);
        REGISTER_METHOD(get_noteManager);
        REGISTER_METHOD(get_wallManager);
        REGISTER_METHOD(get_colorManager);

        REGISTER_METHOD(get_saberSwitcherViewController);
        REGISTER_METHOD(get_saberSettingsViewController);
        REGISTER_METHOD(get_saberPreviewViewController);

        REGISTER_METHOD(get_noteSwitcherViewController);
        REGISTER_METHOD(get_noteSettingsViewController);
        REGISTER_METHOD(get_notePreviewViewController);

        REGISTER_METHOD(get_wallSwitcherViewController);
        REGISTER_METHOD(get_wallSettingsViewController);
        REGISTER_METHOD(get_wallPreviewViewController);

        REGISTER_METHOD(get_qosmeticsViewController);
        REGISTER_METHOD(get_patronViewController);
        REGISTER_METHOD(get_floorLogoViewController);
        REGISTER_METHOD(get_userProfileViewController);

        REGISTER_FIELD(qosmeticsFlowCoordinator);
        REGISTER_FIELD(saberManager);
        REGISTER_FIELD(noteManager);
        REGISTER_FIELD(wallManager);
        REGISTER_FIELD(colorManager);
        
        REGISTER_FIELD(saberSwitcherViewController);
        REGISTER_FIELD(saberSettingsViewController);
        REGISTER_FIELD(saberPreviewViewController);

        REGISTER_FIELD(noteSwitcherViewController);
        REGISTER_FIELD(noteSettingsViewController);
        REGISTER_FIELD(notePreviewViewController);

        REGISTER_FIELD(wallSwitcherViewController);
        REGISTER_FIELD(wallSettingsViewController);
        REGISTER_FIELD(wallPreviewViewController);

        REGISTER_FIELD(qosmeticsViewController);
        REGISTER_FIELD(patronViewController);
        REGISTER_FIELD(floorLogoViewController);
        REGISTER_FIELD(userProfileViewController);

    )
)