#include "TypeRegisterer.hpp"

#include "Types/Trail/QosmeticsTrail.hpp"
#include "Types/Qosmetic/ModelManager.hpp"
#include "Types/Saber/SaberManager.hpp"
#include "Types/Note/NoteManager.hpp"
#include "Types/Wall/WallManager.hpp"
#include "Types/Pointer/PointerManager.hpp"

#include "Types/Colors/ColorManager.hpp"
#include "Types/Colors/ColorScheme.hpp"

#include "Types/Qosmetic/Qosmetic.hpp"
#include "Types/Saber/Saber.hpp"
#include "Types/Note/Note.hpp"
#include "Types/Note/Debris.hpp"
#include "Types/Note/Bomb.hpp"
#include "Types/Wall/Wall.hpp"
#include "Types/Pointer/Pointer.hpp"
#include "Types/Utils/FuckYouGC.hpp"

#include "UI/Saber/SaberPreviewElement.hpp"
#include "UI/Saber/SaberPreviewViewController.hpp"
#include "UI/Saber/SaberSelectionElement.hpp"
#include "UI/Saber/SaberSettingsViewController.hpp"
#include "UI/Saber/SaberSwitcherViewController.hpp"

#include "UI/Note/NotePreviewElement.hpp"
#include "UI/Note/NotePreviewViewController.hpp"
#include "UI/Note/NoteSelectionElement.hpp"
#include "UI/Note/NoteSettingsViewController.hpp"
#include "UI/Note/NoteSwitcherViewController.hpp"

#include "UI/Wall/WallPreviewElement.hpp"
#include "UI/Wall/WallPreviewViewController.hpp"
#include "UI/Wall/WallSelectionElement.hpp"
#include "UI/Wall/WallSettingsViewController.hpp"
#include "UI/Wall/WallSwitcherViewController.hpp"

#include "UI/General/PatronViewController.hpp"
#include "UI/General/FloorLogoViewController.hpp"
#include "UI/General/QosmeticsFlowCoordinator.hpp"
#include "UI/General/QosmeticsViewController.hpp"

#include "Installers/GameInstaller.hpp"
#include "Installers/UIInstaller.hpp"

#include "Containers/FlowCoordinatorContainer.hpp"

#include "custom-types/shared/register.hpp"

using namespace Qosmetics;
using namespace Qosmetics::UI;

void RegisterTypes()
{
    custom_types::Register::RegisterType<FuckYouGC>();
    custom_types::Register::RegisterType<ModelManager>();
    custom_types::Register::RegisterTypes<SaberManager, NoteManager, WallManager, PointerManager>();
    custom_types::Register::RegisterTypes<ColorManager, ColorScheme>();
    custom_types::Register::RegisterTypes<Qosmetic, QosmeticsTrail>();
    custom_types::Register::RegisterTypes<Saber, Wall, Note, Bomb, Debris, Pointer>();
    
    
    custom_types::Register::RegisterTypes<SaberPreviewElement, NotePreviewElement, WallPreviewElement>();
    custom_types::Register::RegisterTypes<SaberPreviewViewController, NotePreviewViewController, WallPreviewViewController>();
    
    custom_types::Register::RegisterTypes<SaberSwitcherViewController, NoteSwitcherViewController, WallSwitcherViewController>();
    custom_types::Register::RegisterTypes<SaberSettingsViewController, NoteSettingsViewController, WallSettingsViewController>();
    custom_types::Register::RegisterTypes<SaberSelectionElement, NoteSelectionElement, WallSelectionElement>();
    custom_types::Register::RegisterTypes<QosmeticsViewController, PatronViewController, FloorLogoViewController>();

    custom_types::Register::RegisterType<QosmeticsFlowCoordinator>();

    custom_types::Register::RegisterType<FlowCoordinatorContainer>();

    custom_types::Register::RegisterType<GameInstaller>();
    custom_types::Register::RegisterType<UIInstaller>();
}