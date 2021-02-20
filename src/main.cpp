#include "modloader/shared/modloader.hpp"
#include "QosmeticsLogger.hpp"
#include "Data/DescriptorCache.hpp"
#include "Data/CreatorCache.hpp"
#include "Data/PatronCache.hpp"
#include "static-defines.hpp"

#include "GlobalNamespace/MainFlowCoordinator.hpp"
#include "GlobalNamespace/SaberModelContainer.hpp"
#include "GlobalNamespace/Saber.hpp"
#include "GlobalNamespace/ConditionalMaterialSwitcher.hpp"
#include "GlobalNamespace/SaberTrailRenderer.hpp"
#include "GlobalNamespace/MultiplayerModeSelectionFlowCoordinator.hpp"
#include "HMUI/ViewController_AnimationType.hpp"
#include "UnityEngine/SceneManagement/Scene.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/MeshFilter.hpp"

#include "Types/Saber/SaberItem.hpp"
#include "Types/Note/NoteItem.hpp"
#include "Types/Wall/WallItem.hpp"
#include "Types/Colors/ColorManager.hpp"

#include "TypeRegisterer.hpp"
#include "Types/Saber/SaberManager.hpp"

#include "Utils/UnityUtils.hpp"
#include "Types/Saber/Saber.hpp"
#include "Types/Qosmetic/Qosmetic.hpp"
#include "Config.hpp"

#include "UI/Saber/SaberSwitcherViewController.hpp"
#include "UI/Saber/SaberSettingsViewController.hpp"
#include "UI/General/PatronViewController.hpp"
#include "UI/General/UISetup.hpp"

#include "questui/shared/QuestUI.hpp"
#include "Installers/GameInstaller.hpp"
#include "zenjeqt/shared/Zenjeqtor.hpp"

ModInfo modInfo = {ID, VERSION};

#define INFO(value...) QosmeticsLogger::GetLogger().info(value);
#define ERROR(value...) QosmeticsLogger::GetLogger().error(value);

using namespace Qosmetics;
using namespace Qosmetics::UI;
using namespace GlobalNamespace;

extern config_t config;

bool atLeastMenu = false;
std::string activeSceneName = "";
bool getSceneName(UnityEngine::SceneManagement::Scene scene, std::string& output);
void makeFolder(std::string directory);

/*
this->modelManager = Object::FindObjectOfType<SaberManager*>();
if (!this->modelManager) 
{
    INFO("Making Model Manager!");
    this->modelManager = UnityUtils::FindAddComponent<SaberManager*>(true);
    modelManager->internalSetActiveModel("Plasma Katana.qsaber");
    Replace();
}
*/

//static Qosmetics::SaberManager* saberManager = nullptr;
//static Qosmetics::NoteManager* noteManager = nullptr;
//static Qosmetics::WallManager* wallManager = nullptr;
//static Qosmetics::ColorManager* colorManager = nullptr;

bool firstWarmup = true;
MAKE_HOOK_OFFSETLESS(SceneManager_SetActiveScene, bool, UnityEngine::SceneManagement::Scene scene)
{
    getSceneName(scene, activeSceneName);

    if (firstWarmup && activeSceneName == "ShaderWarmup")
    {
        firstWarmup = false;
        CreatorCache::Download();
        PatronCache::Download();
        //if (!saberManager) saberManager = CRASH_UNLESS(il2cpp_utils::New<Qosmetics::SaberManager*, il2cpp_utils::CreationType::Manual>());
        //if (!noteManager) noteManager = CRASH_UNLESS(il2cpp_utils::New<Qosmetics::NoteManager*, il2cpp_utils::CreationType::Manual>());
        //if (!wallManager) wallManager = CRASH_UNLESS(il2cpp_utils::New<Qosmetics::WallManager*, il2cpp_utils::CreationType::Manual>());

        //saberManager->internalSetActiveModel(config.lastActiveSaber);
        //noteManager->internalSetActiveModel(config.lastActiveNote);
        //wallManager->internalSetActiveModel(config.lastActiveWall);
        
        //saberManager->get_item().LoadBundle();
        //noteManager->get_item().LoadBundle();
        //wallManager->get_item().LoadBundle();
        //saberManager = UnityUtils::FindAddComponent<Qosmetics::SaberManager*>(true);
        //saberManager->SetActiveSaber("Plasma Katana.qsaber");
    }

    if (activeSceneName == "HealthWarning")
    {
        //saberManager->get_item().Load();
        //saberManager->get_item().LoadAssets();
        //noteManager->get_item().LoadAssets();
        //wallManager->get_item().LoadAssets();
    }

    if (activeSceneName == "MenuViewControllers" || activeSceneName == "MenuCore")
    {

    }

    if (activeSceneName == "GameCore")
    {

    }

    //if (colorManager) colorManager->ClearCallbacks();
    INFO("Found scene %s", activeSceneName.c_str());
    return SceneManager_SetActiveScene(scene);
}

MAKE_HOOK_OFFSETLESS(SaberModelContainer_Start, void, GlobalNamespace::SaberModelContainer* self)
{
    SaberModelContainer_Start(self);

    INFO("SaberModelContainer");
    if (!self->saber) 
    {
        ERROR("Saber Was nullptr");
        return;
    }

    Qosmetics::Saber* saber = atLeastMenu ? UnityUtils::GetAddComponent<Qosmetics::Saber*>(self->saber->get_gameObject()) : nullptr;
    if (saber) 
    {
        //if (!saberManager->prefab) saberManager->SetActiveSaber("Plasma Katana.qsaber");
        //colorManager->ctor();

        //saber->Init(saberManager, colorManager);
        INFO("Replacing...");
        saber->Replace();
    }
}

MAKE_HOOK_OFFSETLESS(MainFlowCoordinator_DidActivate, void, MainFlowCoordinator* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    MainFlowCoordinator_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
    if (firstActivation)
    {
        atLeastMenu = true;
        //if (!saberManager) saberManager = UnityUtils::FindAddComponent<Qosmetics::SaberManager*>(true);
        //if (!colorManager) colorManager = CRASH_UNLESS(il2cpp_utils::New<Qosmetics::ColorManager*, il2cpp_utils::CreationType::Manual>());

        //UISetup::Init(saberManager, noteManager, wallManager, colorManager);
    }
}
// fix for trail renderers not getting these set on time
MAKE_HOOK_OFFSETLESS(SaberTrailRenderer_OnEnable, void, GlobalNamespace::SaberTrailRenderer* self)
{
    if (!self->meshRenderer)self->meshRenderer = self->get_gameObject()->GetComponent<UnityEngine::MeshRenderer*>();
    if (!self->meshFilter) self->meshFilter = self->get_gameObject()->GetComponent<UnityEngine::MeshFilter*>();
    SaberTrailRenderer_OnEnable(self);
}

MAKE_HOOK_OFFSETLESS(ConditionalMaterialSwitcher_Awake, void, GlobalNamespace::ConditionalMaterialSwitcher* self)
{
    // basically QuestTrailOverlap is forced with this
    UnityEngine::Transform* trailTransform = self->get_transform();
    std::string thisName = to_utf8(csstrtostr(trailTransform->get_gameObject()->get_name()));
    if (thisName == "Trail(Clone)") return;
    ConditionalMaterialSwitcher_Awake(self);
}

bool saberSet = false;

MAKE_HOOK_OFFSETLESS(MainMenuViewController_HandleMenuButton, void, GlobalNamespace::MainMenuViewController* self, GlobalNamespace::MainMenuViewController::MenuButton menuButton)
{
    MainMenuViewController_HandleMenuButton(self, menuButton);
    INFO("Menu pressed: %d", menuButton.value);
    /*
    if (saberManager && !saberSet) 
    {
        saberSet = true;
        saberManager->SetActiveSaber(config.lastActiveSaber, true);
    }
    */
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

#define copyFile(in, out) \
{ \
    std::string inPath = in; \
    std::string outPath = out;\
    if (!fileexists(outPath.c_str()))\
        writefile(outPath.c_str(), readfile(inPath.c_str()));\
}

void CopyIcons()
{
    makeFolder(BASEPATH);
    std::string mainPath = UIPATH;
    makeFolder(mainPath.c_str());
    makeFolder(string_format("%s%s", mainPath.c_str(), "Icons"));
    
    copyFile("sdcard/BMBFData/Mods/Qosmetics/MenuIcon.png", string_format("%s%s", mainPath.c_str(), "Icons/MenuIcon.png"));
    copyFile("sdcard/BMBFData/Mods/Qosmetics/MenuIconSelected.png", string_format("%s%s", mainPath.c_str(), "Icons/MenuIconSelected.png"));

    copyFile("sdcard/BMBFData/Mods/Qosmetics/SaberIcon.png", string_format("%s%s", mainPath.c_str(), "Icons/SaberIcon.png"));
    copyFile("sdcard/BMBFData/Mods/Qosmetics/SaberIconSelected.png", string_format("%s%s", mainPath.c_str(), "Icons/SaberIconSelected.png"));

    copyFile("sdcard/BMBFData/Mods/Qosmetics/NoteIcon.png", string_format("%s%s", mainPath.c_str(), "Icons/NoteIcon.png"));
    copyFile("sdcard/BMBFData/Mods/Qosmetics/NoteIconSelected.png", string_format("%s%s", mainPath.c_str(), "Icons/NoteIconSelected.png"));

    copyFile("sdcard/BMBFData/Mods/Qosmetics/WallIcon.png", string_format("%s%s", mainPath.c_str(), "Icons/WallIcon.png"));
    copyFile("sdcard/BMBFData/Mods/Qosmetics/WallIconSelected.png", string_format("%s%s", mainPath.c_str(), "Icons/WallIconSelected.png"));

    copyFile("sdcard/BMBFData/Mods/Qosmetics/GameSetupIcon.png", string_format("%s%s", mainPath.c_str(), "Icons/GameSetupIcon.png"));
    copyFile("sdcard/BMBFData/Mods/Qosmetics/GameSetupIconSelected.png", string_format("%s%s", mainPath.c_str(), "Icons/GameSetupIconSelected.png"));
}

extern "C" void setup(ModInfo& info)
{
    info.id = ID;
    info.version = VERSION;

    QosmeticsLogger::GetContextLogger("Setup").info("idk why you are reading logs instead of playing the game, go hit bloq or something you dolt");
}

extern "C" void load()
{
    if (!LoadConfig()) SaveConfig();
    if (!DescriptorCache::Load()) DescriptorCache::Save();
    QuestUI::Init();
    LoggerContextObject logger = QosmeticsLogger::GetContextLogger("Mod Load");
    
    CopyIcons();
    
    logger.info("Installing Hooks...");
    INSTALL_HOOK_OFFSETLESS(logger, ConditionalMaterialSwitcher_Awake, il2cpp_utils::FindMethodUnsafe("", "ConditionalMaterialSwitcher", "Awake", 0)); 
    INSTALL_HOOK_OFFSETLESS(logger, MainFlowCoordinator_DidActivate, il2cpp_utils::FindMethodUnsafe("", "MainFlowCoordinator", "DidActivate", 3));
    INSTALL_HOOK_OFFSETLESS(logger, SaberModelContainer_Start, il2cpp_utils::FindMethodUnsafe("", "SaberModelContainer", "Start", 0));
    INSTALL_HOOK_OFFSETLESS(logger, SceneManager_SetActiveScene, il2cpp_utils::FindMethodUnsafe("UnityEngine.SceneManagement", "SceneManager", "SetActiveScene", 1));
    INSTALL_HOOK_OFFSETLESS(logger, SaberTrailRenderer_OnEnable, il2cpp_utils::FindMethodUnsafe("", "SaberTrailRenderer", "OnEnable", 0));
    INSTALL_HOOK_OFFSETLESS(logger, MainMenuViewController_HandleMenuButton, il2cpp_utils::FindMethodUnsafe("", "MainMenuViewController", "HandleMenuButton", 1));
    INSTALL_HOOK_OFFSETLESS(logger, OptionsViewController_DidActivate, il2cpp_utils::FindMethodUnsafe("", "OptionsViewController", "DidActivate", 3));
    INSTALL_HOOK_OFFSETLESS(logger, MultiplayerModeSelectionFlowCoordinator_TopViewControllerWillChange, il2cpp_utils::FindMethodUnsafe("", "MultiplayerModeSelectionFlowCoordinator", "TopViewControllerWillChange", 3));

    logger.info("Installed Hooks!");

    logger.info("Registering Custom types...");
    RegisterTypes();
    logger.info("Registered Custom types!");

    //QuestUI::Register::RegisterModSettingsViewController<Qosmetics::UI::SaberSwitcherViewController*>((ModInfo){"Saber Switcher", VERSION});
    //QuestUI::Register::RegisterModSettingsViewController<Qosmetics::UI::SaberSettingsViewController*>((ModInfo){"Saber Settings", VERSION});
    //QuestUI::Register::RegisterModSettingsViewController<Qosmetics::UI::PatronViewController*>((ModInfo){"Patron Credits", VERSION});

    auto zenjeqtor = new Zenjeqt::Zenjeqtor();

    zenjeqtor->OnApp<Qosmetics::GameInstaller*>();
}

bool getSceneName(UnityEngine::SceneManagement::Scene scene, std::string& output)
{
    LoggerContextObject logger = QosmeticsLogger::GetContextLogger("scene name");
    Il2CppString* csString = UnityEngine::SceneManagement::Scene::GetNameInternal(scene.m_Handle);
    RET_0_UNLESS(logger, csString);
    output = to_utf8(csstrtostr(csString));
    return true; 
}

void makeFolder(std::string directory)
{
    if (!direxists(directory.c_str()))
    {
        int makePath = mkpath(directory.data());
        if (makePath == -1)
        {
            ERROR("Failed to make path %s", directory.c_str());
        }
    }
}