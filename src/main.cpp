#include "modloader/shared/modloader.hpp"
#include "QosmeticsLogger.hpp"
#include "Data/DescriptorCache.hpp"
#include "static-defines.hpp"

#include "GlobalNamespace/MainFlowCoordinator.hpp"
#include "GlobalNamespace/SaberModelContainer.hpp"
#include "GlobalNamespace/Saber.hpp"
#include "GlobalNamespace/SaberTrailRenderer.hpp"
#include "UnityEngine/SceneManagement/Scene.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/MeshFilter.hpp"

#include "Types/Saber/SaberItem.hpp"
#include "Types/Note/NoteItem.hpp"
#include "Types/Wall/WallItem.hpp"

#include "TypeRegisterer.hpp"
#include "Types/Saber/SaberManager.hpp"

#include "Utils/UnityUtils.hpp"
#include "Types/Saber/Saber.hpp"
#include "Types/Qosmetic/Qosmetic.hpp"

ModInfo modInfo = {ID, VERSION};

#define INFO(value...) QosmeticsLogger::GetLogger().info(value);
//QosmeticsLogger::GetContextLogger("main").info(value);
#define ERROR(value...) QosmeticsLogger::GetLogger().error(value);
//QosmeticsLogger::GetContextLogger("main").error(value);

using namespace Qosmetics;
using namespace GlobalNamespace;

bool atLeastMenu = false;
std::string activeSceneName = "";
bool getSceneName(UnityEngine::SceneManagement::Scene scene, std::string& output);
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

static Qosmetics::SaberManager* manager = nullptr;
bool firstWarmup = true;
MAKE_HOOK_OFFSETLESS(SceneManager_SetActiveScene, bool, UnityEngine::SceneManagement::Scene scene)
{
    getSceneName(scene, activeSceneName);

    if (firstWarmup && activeSceneName == "ShaderWarmup")
    {
        firstWarmup = false;
        //manager = UnityUtils::FindAddComponent<Qosmetics::SaberManager*>(true);
        //manager->SetActiveSaber("Plasma Katana.qsaber");
    }

    if (activeSceneName == "HealthWarning")
    {
        //manager->get_item().Load();
    }

    if (activeSceneName == "MenuViewControllers" || activeSceneName == "MenuCore")
    {

    }

    if (activeSceneName == "GameCore")
    {

    }

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
        //if (!manager->prefab) manager->SetActiveSaber("Plasma Katana.qsaber");
        saber->Init(manager);
        INFO("Replacing...");
        saber->Replace();
    }
}

MAKE_HOOK_OFFSETLESS(MainFlowCoordinator_DidActivate, void, MainFlowCoordinator* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    MainFlowCoordinator_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
    atLeastMenu = true;

    if (!manager) manager = UnityUtils::FindAddComponent<Qosmetics::SaberManager*>(true);
    manager->SetActiveSaber("Plasma Katana.qsaber", true);
}
// fix for trail renderers not getting these set on time
MAKE_HOOK_OFFSETLESS(SaberTrailRenderer_OnEnable, void, GlobalNamespace::SaberTrailRenderer* self)
{
    if (!self->meshRenderer)self->meshRenderer = self->get_gameObject()->GetComponent<UnityEngine::MeshRenderer*>();
    if (!self->meshFilter) self->meshFilter = self->get_gameObject()->GetComponent<UnityEngine::MeshFilter*>();
    SaberTrailRenderer_OnEnable(self);
}

extern "C" void setup(ModInfo& info)
{
    info.id = ID;
    info.version = VERSION;

    QosmeticsLogger::GetContextLogger("Setup").info("idk why you are reading logs instead of playing the game, go hit bloq or something you dolt");
}

extern "C" void load()
{
    if (!DescriptorCache::Load()) DescriptorCache::Save();
    
    LoggerContextObject logger = QosmeticsLogger::GetContextLogger("Mod Load");

    logger.info("Installing Hooks...");
    
    INSTALL_HOOK_OFFSETLESS(logger, MainFlowCoordinator_DidActivate, il2cpp_utils::FindMethodUnsafe("", "MainFlowCoordinator", "DidActivate", 3));
    INSTALL_HOOK_OFFSETLESS(logger, SaberModelContainer_Start, il2cpp_utils::FindMethodUnsafe("", "SaberModelContainer", "Start", 0));
    INSTALL_HOOK_OFFSETLESS(logger, SceneManager_SetActiveScene, il2cpp_utils::FindMethodUnsafe("UnityEngine.SceneManagement", "SceneManager", "SetActiveScene", 1));
    INSTALL_HOOK_OFFSETLESS(logger, SaberTrailRenderer_OnEnable, il2cpp_utils::FindMethodUnsafe("", "SaberTrailRenderer", "OnEnable", 0));

    logger.info("Installed Hooks!");

    logger.info("Registering Custom types...");
    RegisterTypes();
    logger.info("Registered Custom types!");
}

bool getSceneName(UnityEngine::SceneManagement::Scene scene, std::string& output)
{
    LoggerContextObject logger = QosmeticsLogger::GetContextLogger("scene name");
    Il2CppString* csString = UnityEngine::SceneManagement::Scene::GetNameInternal(scene.m_Handle);
    RET_0_UNLESS(logger, csString);
    output = to_utf8(csstrtostr(csString));
    return true; 
}