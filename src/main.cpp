#include "modloader/shared/modloader.hpp"
#include "Config.hpp"
#include "QosmeticsLogger.hpp"
#include "Data/DescriptorCache.hpp"
#include "Data/CreatorCache.hpp"
#include "Data/PatronCache.hpp"
#include "static-defines.hpp"


#include "UnityEngine/SceneManagement/Scene.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/MeshFilter.hpp"
#include "UnityEngine/Vector3.hpp"

#include "TypeRegisterer.hpp"

#include "Utils/UnityUtils.hpp"
#include "Utils/PlayerSettings.hpp"
#include "Types/Trail/QosmeticsTrail.hpp"

#include "Types/Qosmetic/Qosmetic.hpp"

#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "GlobalNamespace/ColorScheme.hpp"
#include "GlobalNamespace/StandardLevelScenesTransitionSetupDataSO.hpp"
#include "GlobalNamespace/BeatmapEnvironmentHelper.hpp"
#include "GlobalNamespace/EnvironmentInfoSO.hpp"
#include "GlobalNamespace/OverrideEnvironmentSettings.hpp"
#include "GlobalNamespace/ColorSchemeSO.hpp"
#include "GlobalNamespace/GameplayCoreSceneSetupData.hpp"

#include "GlobalNamespace/GameplayModifiers.hpp"
#include "GlobalNamespace/PlayerSpecificSettings.hpp"
#include "Containers/SingletonContainer.hpp"

#include "questui/shared/QuestUI.hpp"

#include "bs-utils/shared/utils.hpp"

#include "Utils/MaterialUtils.hpp"

ModInfo modInfo = {ID, VERSION};

#define INFO(value...) QosmeticsLogger::GetLogger().info(value);
#define ERROR(value...) QosmeticsLogger::GetLogger().error(value);

#define LOG_COLOR(text, color) INFO("color %s: %.2f, %.2f, %.2f", text, color.r, color.g, color.b)

using namespace UnityEngine;
using namespace Qosmetics;
using namespace Qosmetics::UI;
using namespace GlobalNamespace;

extern config_t config;

std::string activeSceneName = "";
bool getSceneName(UnityEngine::SceneManagement::Scene scene, std::string& output);
void makeFolder(std::string directory);

bool firstWarmup = true;
MAKE_HOOK_OFFSETLESS(SceneManager_SetActiveScene, bool, UnityEngine::SceneManagement::Scene scene)
{
    getSceneName(scene, activeSceneName);
    INFO("Found scene %s", activeSceneName.c_str());
    bool result = SceneManager_SetActiveScene(scene);

    if (firstWarmup && activeSceneName == "ShaderWarmup")
    {
        // when settings get reset it goes through shaderwarmup again
        firstWarmup = false;

        // async pog
        CreatorCache::Download();
        // async pog
        PatronCache::Download();
    }

    if (activeSceneName == "GameCore")
    {
        if (GetScoresDisabled())
        {
            bs_utils::Submission::disable(modInfo);
        }
        else
        {
            bs_utils::Submission::enable(modInfo);
        }

        if (SingletonContainer::get_noteManager()->get_item().get_descriptor().isValid())
        {
            MaterialUtils::ReplaceMaterialsForGameObject(SingletonContainer::get_noteManager()->get_item().get_prefab());
        }

        if (SingletonContainer::get_saberManager()->get_item().get_descriptor().isValid())
        {
            MaterialUtils::ReplaceMaterialsForGameObject(SingletonContainer::get_saberManager()->get_item().get_prefab());
        }

        if (SingletonContainer::get_wallManager()->get_item().get_descriptor().isValid())
        {
            MaterialUtils::ReplaceMaterialsForGameObject(SingletonContainer::get_wallManager()->get_item().get_prefab());
        }
    }

    // clear all callbacks
    SingletonContainer::get_colorManager()->ClearCallbacks();
    return result;
}

MAKE_HOOK_OFFSETLESS(GameplayCoreSceneSetupData_ctor, void, GlobalNamespace::GameplayCoreSceneSetupData* self, Il2CppObject* difficultyBeatmap, Il2CppObject* previewBeatmapLevel, Il2CppObject* gameplayModifiers, Il2CppObject* playerSpecificSettings, Il2CppObject* practiceSettings, bool useTestNoteCutSoundEffects, Il2CppObject* environmentInfo, GlobalNamespace::ColorScheme* colorScheme)
{
    SingletonContainer::get_colorManager()->SetColorSchemeFromBase(colorScheme);
    GameplayCoreSceneSetupData_ctor(self, difficultyBeatmap, previewBeatmapLevel, gameplayModifiers, playerSpecificSettings, practiceSettings, useTestNoteCutSoundEffects, environmentInfo, colorScheme);
}

// needed for reading settings lol
MAKE_HOOK_OFFSETLESS(StandardLevelScenesTransitionSetupDataSO_Init, void, GlobalNamespace::StandardLevelScenesTransitionSetupDataSO* self, Il2CppString* gameMode, GlobalNamespace::IDifficultyBeatmap* difficultyBeatmap, Il2CppObject* previewBeatmapLevel, GlobalNamespace::OverrideEnvironmentSettings* overrideEnvironmentSettings, GlobalNamespace::ColorScheme* overrideColorScheme, GlobalNamespace::GameplayModifiers* gamePlayModifiers, GlobalNamespace::PlayerSpecificSettings* playerSpecificSettings, Il2CppObject* practiceSettings, Il2CppString* backButtonText, bool useTestNoteCutSoundEffects)
{
    StandardLevelScenesTransitionSetupDataSO_Init(self, gameMode, difficultyBeatmap, previewBeatmapLevel, overrideEnvironmentSettings, overrideColorScheme, gamePlayModifiers, playerSpecificSettings, practiceSettings, backButtonText, useTestNoteCutSoundEffects);
    PlayerSettings::CheckForIllegalModifiers(gamePlayModifiers);
    PlayerSettings::CheckReducedDebris(playerSpecificSettings);
    Qosmetics::QosmeticsTrail::trailIntensity = playerSpecificSettings->get_saberTrailIntensity();
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

extern void installNoteHooks(LoggerContextObject& logger);
extern void installSaberHooks(LoggerContextObject& logger);
extern void installUIHooks(LoggerContextObject& logger);
extern void installWallHooks(LoggerContextObject& logger);

extern "C" void load()
{
    if (!LoadConfig()) SaveConfig();
    Config::Init();
    
    if (!DescriptorCache::Load()) DescriptorCache::Save();
    QuestUI::Init();
    
    LoggerContextObject logger = QosmeticsLogger::GetContextLogger("Mod Load");
    
    CopyIcons();

    logger.info("Installing Hooks...");
    INSTALL_HOOK_OFFSETLESS(logger, SceneManager_SetActiveScene, il2cpp_utils::FindMethodUnsafe("UnityEngine.SceneManagement", "SceneManager", "SetActiveScene", 1));
    INSTALL_HOOK_OFFSETLESS(logger, StandardLevelScenesTransitionSetupDataSO_Init, il2cpp_utils::FindMethodUnsafe("", "StandardLevelScenesTransitionSetupDataSO", "Init", 10));
    INSTALL_HOOK_OFFSETLESS(logger, GameplayCoreSceneSetupData_ctor, il2cpp_utils::FindMethodUnsafe("", "GameplayCoreSceneSetupData", ".ctor", 8));

    installNoteHooks(logger);
    installSaberHooks(logger);
    installUIHooks(logger);
    installWallHooks(logger);

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