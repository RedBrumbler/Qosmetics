#include "modloader/shared/modloader.hpp"
#include "Logging.hpp"
#include "Data/DescriptorCache.hpp"

ModInfo modInfo;

using namespace Qosmetics;

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

    logger.info("Installed Hooks!");

    logger.info("Registering Custom types...");

    logger.info("Registered Custom types!");
}