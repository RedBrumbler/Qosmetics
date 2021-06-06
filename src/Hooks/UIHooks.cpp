#include "UI/General/UISetup.hpp"

#include "GlobalNamespace/MainFlowCoordinator.hpp"
#include "GlobalNamespace/MultiplayerModeSelectionFlowCoordinator.hpp"
#include "HMUI/ViewController_AnimationType.hpp"

#include "Containers/SingletonContainer.hpp"

#include "Types/Pointer/Pointer.hpp"

#define INFO(value...) QosmeticsLogger::GetLogger().info(value);
#define ERROR(value...) QosmeticsLogger::GetLogger().error(value);

using namespace Qosmetics;
using namespace Qosmetics::UI;
using namespace UnityEngine;
using namespace HMUI;

bool atLeastMenu = false;

MAKE_HOOK_OFFSETLESS(MainMenuViewController_HandleMenuButton, void, GlobalNamespace::MainMenuViewController* self, GlobalNamespace::MainMenuViewController::MenuButton menuButton)
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

MAKE_HOOK_OFFSETLESS(MultiplayerModeSelectionFlowCoordinator_TopViewControllerWillChange, void, GlobalNamespace::MultiplayerModeSelectionFlowCoordinator* self, HMUI::ViewController* oldViewController, HMUI::ViewController* newViewController, HMUI::ViewController::AnimationType animationType)
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

MAKE_HOOK_OFFSETLESS(OptionsViewController_DidActivate, void, GlobalNamespace::OptionsViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    OptionsViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);

    if (firstActivation)
    {
        UISetup::SetupFlowCoordinatorAtSettings(self);
    }
}

MAKE_HOOK_OFFSETLESS(MainFlowCoordinator_DidActivate, void, GlobalNamespace::MainFlowCoordinator* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    MainFlowCoordinator_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
    if (firstActivation)
    {
        atLeastMenu = true;
        SingletonContainer::get_saberManager();
        SingletonContainer::get_noteManager();
        SingletonContainer::get_wallManager();

        Qosmetics::Pointer::AddToAll();
    }
}
MAKE_HOOK_OFFSETLESS(GameplaySetupViewController_DidActivate, void, GlobalNamespace::GameplaySetupViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    GameplaySetupViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
    if (firstActivation)
    {
        UISetup::SetupFlowCoordinatorAtGameplay(self);
    }
}
void installUIHooks(LoggerContextObject& logger)
{
    INSTALL_HOOK_OFFSETLESS(logger, MainMenuViewController_HandleMenuButton, il2cpp_utils::FindMethodUnsafe("", "MainMenuViewController", "HandleMenuButton", 1));
    INSTALL_HOOK_OFFSETLESS(logger, MainFlowCoordinator_DidActivate, il2cpp_utils::FindMethodUnsafe("", "MainFlowCoordinator", "DidActivate", 3));
    INSTALL_HOOK_OFFSETLESS(logger, OptionsViewController_DidActivate, il2cpp_utils::FindMethodUnsafe("", "OptionsViewController", "DidActivate", 3));
    INSTALL_HOOK_OFFSETLESS(logger, MultiplayerModeSelectionFlowCoordinator_TopViewControllerWillChange, il2cpp_utils::FindMethodUnsafe("", "MultiplayerModeSelectionFlowCoordinator", "TopViewControllerWillChange", 3));
    INSTALL_HOOK_OFFSETLESS(logger, GameplaySetupViewController_DidActivate, il2cpp_utils::FindMethodUnsafe("", "GameplaySetupViewController", "DidActivate", 3));
}
