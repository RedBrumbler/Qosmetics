#include "modloader/shared/modloader.hpp"
#include "QosmeticsLogger.hpp"
#include "Data/DescriptorCache.hpp"

#include "GlobalNamespace/MainFlowCoordinator.hpp"
#include "Saber/SaberItem.hpp"

ModInfo modInfo = {ID, VERSION};

#define INFO(value...) QosmeticsLogger::GetLogger().info(value);
//QosmeticsLogger::GetContextLogger("main").info(value);
#define ERROR(value...) QosmeticsLogger::GetLogger().error(value);
//QosmeticsLogger::GetContextLogger("main").error(value);

using namespace Qosmetics;
using namespace GlobalNamespace;

MAKE_HOOK_OFFSETLESS(MainFlowCoordinator_DidActivate, void, MainFlowCoordinator* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    MainFlowCoordinator_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
    Descriptor& descriptor = DescriptorCache::GetDescriptor("Plasma Katana.qsaber");
    if (descriptor.isValid())
        new SaberItem(descriptor, true);
    else ERROR("Invalid Descriptor!");
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
    logger.info("Installed Hooks!");

    logger.info("Registering Custom types...");

    logger.info("Registered Custom types!");
}