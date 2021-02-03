#include "modloader/shared/modloader.hpp"
#include "QosmeticsLogger.hpp"
#include "Data/DescriptorCache.hpp"
#include "static-defines.hpp"

#include "GlobalNamespace/MainFlowCoordinator.hpp"
#include "GlobalNamespace/SaberModelContainer.hpp"
#include "GlobalNamespace/Saber.hpp"
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

MAKE_HOOK_OFFSETLESS(SaberModelContainer_Start, void, GlobalNamespace::SaberModelContainer* self)
{
    SaberModelContainer_Start(self);
    return;

    INFO("SaberModelContainer");
    if (!self->saber) 
    {
        ERROR("Saber Was nullptr");
        return;
    }

    Qosmetics::Saber* saber = atLeastMenu ? UnityUtils::GetAddComponent<Qosmetics::Saber*>(self->saber->get_gameObject()) : nullptr;
    if (saber) 
    {
        if (!manager->prefab) manager->SetActiveSaber("Plasma Katana.qsaber");
        saber->Init(manager);
        INFO("Replacing...");
        //saber->modelManager = manager;
        saber->Replace();
    }
}

MAKE_HOOK_OFFSETLESS(Saber_Start, void, GlobalNamespace::Saber* self)
{
    Saber_Start(self);
    Qosmetics::Saber* saber = UnityUtils::GetAddComponent<Qosmetics::Saber*>(self->get_gameObject());
    saber->Init(manager);
    saber->Replace();
}

MAKE_HOOK_OFFSETLESS(MainFlowCoordinator_DidActivate, void, MainFlowCoordinator* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    MainFlowCoordinator_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
    atLeastMenu = true;
    /*
    Descriptor& descriptor = DescriptorCache::GetDescriptor("Scoresaber Notes.qbloq");
    if (descriptor.isValid())
        new NoteItem(descriptor, true);
    else ERROR("Invalid Descriptor!");
    */
    if (!manager) manager = UnityUtils::FindAddComponent<Qosmetics::SaberManager*>();
    if (!manager->prefab) manager->SetActiveSaber("Plasma Katana.qsaber");
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
    INSTALL_HOOK_OFFSETLESS(logger, Saber_Start, il2cpp_utils::FindMethodUnsafe("", "Saber", "Start", 0));
    logger.info("Installed Hooks!");

    logger.info("Registering Custom types...");
    RegisterTypes();
    logger.info("Registered Custom types!");
}