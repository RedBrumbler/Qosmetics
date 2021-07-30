#include "UI/General/UISetup.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"

#include "GlobalNamespace/MainFlowCoordinator.hpp"
#include "GlobalNamespace/MultiplayerModeSelectionFlowCoordinator.hpp"
#include "HMUI/ViewController_AnimationType.hpp"
#include "questui/shared/BeatSaberUI.hpp"

#include "Containers/SingletonContainer.hpp"

#include "Types/Pointer/Pointer.hpp"

#include "hooks.hpp"

#define INFO(value...) QosmeticsLogger::GetLogger().info(value);
#define ERROR(value...) QosmeticsLogger::GetLogger().error(value);

using namespace Qosmetics;
using namespace Qosmetics::UI;
using namespace UnityEngine;
using namespace HMUI;

bool atLeastMenu = false;

MAKE_HOOK_MATCH(MainMenuViewController_HandleMenuButton, &GlobalNamespace::MainMenuViewController::HandleMenuButton, void, GlobalNamespace::MainMenuViewController* self, GlobalNamespace::MainMenuViewController::MenuButton menuButton)
{
    MainMenuViewController_HandleMenuButton(self, menuButton);

    switch (menuButton.value)
    {
        case 0: // solo
            UISetup::set_flowCoordinatorType(UISetup::solo);
            break;
        case 1: // party
            UISetup::set_flowCoordinatorType(UISetup::party);
            break;
        case 2: // Editor
            UISetup::set_flowCoordinatorType(UISetup::invalid);
            break;
        case 3: // campaign
            UISetup::set_flowCoordinatorType(UISetup::campaign);
            break;
        case 4: // floorAdjust
            UISetup::set_flowCoordinatorType(UISetup::invalid);
            break;
        case 5: // quit
            UISetup::set_flowCoordinatorType(UISetup::invalid);
            break;
        case 6: // multi
            break;
        case 7: // options
            UISetup::set_flowCoordinatorType(UISetup::settings);
            break;
        case 8: // howtoplay/tutorial
            UISetup::set_flowCoordinatorType(UISetup::invalid);
            break;
    }
}

MAKE_HOOK_MATCH(MultiplayerModeSelectionFlowCoordinator_TopViewControllerWillChange, &GlobalNamespace::MultiplayerModeSelectionFlowCoordinator::TopViewControllerWillChange, void, GlobalNamespace::MultiplayerModeSelectionFlowCoordinator* self, HMUI::ViewController* oldViewController, HMUI::ViewController* newViewController, HMUI::ViewController::AnimationType animationType)
{
    MultiplayerModeSelectionFlowCoordinator_TopViewControllerWillChange(self, oldViewController, newViewController, animationType);
    if ((void*)newViewController == (void*)self->createServerViewController)
    {
        INFO("User is Host");
        UISetup::set_flowCoordinatorType(UISetup::multiHost);
    }
    else if ((void*)newViewController == (void*)self->serverCodeEntryViewController)
    {
        INFO("User is Client");
        UISetup::set_flowCoordinatorType(UISetup::multiClient);
    }
}

MAKE_HOOK_MATCH(OptionsViewController_DidActivate, &GlobalNamespace::OptionsViewController::DidActivate, void, GlobalNamespace::OptionsViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    OptionsViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);

    if (firstActivation)
    {
        UISetup::SetupFlowCoordinatorAtSettings(self);
    }
}

MAKE_HOOK_MATCH(MainFlowCoordinator_DidActivate, &GlobalNamespace::MainFlowCoordinator::DidActivate, void, GlobalNamespace::MainFlowCoordinator* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    MainFlowCoordinator_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
    if (firstActivation)
    {
        atLeastMenu = true;
        
        // create 1 instance
        SingletonContainer::Init();

        SingletonContainer::get_saberManager();
        SingletonContainer::get_noteManager();
        SingletonContainer::get_wallManager();

        Qosmetics::Pointer::AddToAll();

        GameObject* newObj = GameObject::New_ctor();
        Object::DontDestroyOnLoad(newObj);

        UnityEngine::UI::Button* button = QuestUI::BeatSaberUI::CreateUIButton(newObj->get_transform(), "fuckoff");
        button->get_gameObject()->set_name(il2cpp_utils::newcsstr("QosmeticsTemplateButton"));
    }
}

MAKE_HOOK_MATCH(GameplaySetupViewController_DidActivate, &GlobalNamespace::GameplaySetupViewController::DidActivate, void, GlobalNamespace::GameplaySetupViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    GameplaySetupViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
    if (firstActivation)
    {
        UISetup::SetupFlowCoordinatorAtGameplay(self);
    }
}

void InstallUIHooks(Logger& logger)
{
    SIMPLE_INSTALL_HOOK(MainMenuViewController_HandleMenuButton);
    SIMPLE_INSTALL_HOOK(MainFlowCoordinator_DidActivate);
    SIMPLE_INSTALL_HOOK(OptionsViewController_DidActivate);
    SIMPLE_INSTALL_HOOK(MultiplayerModeSelectionFlowCoordinator_TopViewControllerWillChange);
    SIMPLE_INSTALL_HOOK(GameplaySetupViewController_DidActivate);
}

QOS_INSTALL_HOOKS(InstallUIHooks)