#include "modloader/shared/modloader.hpp"
#include "Config.hpp"
#include "QosmeticsLogger.hpp"
#include "Data/DescriptorCache.hpp"
#include "Data/CreatorCache.hpp"
#include "Data/PatronCache.hpp"
#include "static-defines.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"

#include "UnityEngine/SceneManagement/SceneManager.hpp"
#include "UnityEngine/SceneManagement/Scene.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/MeshFilter.hpp"
#include "UnityEngine/Vector3.hpp"

#include "TypeRegisterer.hpp"

#include "Utils/UnityUtils.hpp"
#include "Utils/PlayerSettings.hpp"
#include "Types/Trail/QosmeticsTrail.hpp"
#include "Types/Trail/AltTrail.hpp"

#include "Types/Qosmetic/Qosmetic.hpp"

#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "GlobalNamespace/ColorScheme.hpp"
#include "GlobalNamespace/StandardLevelScenesTransitionSetupDataSO.hpp"
#include "GlobalNamespace/BeatmapEnvironmentHelper.hpp"
#include "GlobalNamespace/EnvironmentInfoSO.hpp"
#include "GlobalNamespace/OverrideEnvironmentSettings.hpp"
#include "GlobalNamespace/ColorSchemeSO.hpp"
#include "GlobalNamespace/GameplayCoreSceneSetupData.hpp"

#include "GlobalNamespace/MenuTransitionsHelper.hpp"
#include "System/Action_1.hpp"
#include "Zenject/DiContainer.hpp"

#include "GlobalNamespace/MenuTransitionsHelper.hpp"
#include "GlobalNamespace/GameplayModifiers.hpp"
#include "GlobalNamespace/PlayerSpecificSettings.hpp"
#include "Containers/SingletonContainer.hpp"

#include "questui/shared/QuestUI.hpp"

#include "bs-utils/shared/utils.hpp"

#include "Utils/MaterialUtils.hpp"
#include "Utils/ChromaUtils.hpp"

#include "hooks.hpp"

std::vector<void (*)(Logger&)> Hooks::installFuncs;

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

extern bool atLeastMenu;
bool firstWarmup = true;
MAKE_HOOK_MATCH(SceneManager_SetActiveScene, &SceneManagement::SceneManager::SetActiveScene, bool, UnityEngine::SceneManagement::Scene scene)
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
            ChromaUtils::setNoteColoredByChroma(false);
        }
        else
        {
            ChromaUtils::setNoteColoredByChroma(true);
        }

        if (SingletonContainer::get_saberManager()->get_item().get_descriptor().isValid())
        {
            MaterialUtils::ReplaceMaterialsForGameObject(SingletonContainer::get_saberManager()->get_item().get_prefab());
        }

        if (SingletonContainer::get_wallManager()->get_item().get_descriptor().isValid())
        {
            MaterialUtils::ReplaceMaterialsForGameObject(SingletonContainer::get_wallManager()->get_item().get_prefab());
            ChromaUtils::setObstacleColoredByChroma(false);
        }
        else
        {
            ChromaUtils::setObstacleColoredByChroma(true);
        }
    }
    return result;
}
                                                                                                                       
MAKE_HOOK_FIND_CLASS_UNSAFE_INSTANCE(GameplayCoreSceneSetupData_ctor, "", "GameplayCoreSceneSetupData", ".ctor", void, GlobalNamespace::GameplayCoreSceneSetupData* self, GlobalNamespace::IDifficultyBeatmap* difficultyBeatmap, GlobalNamespace::IPreviewBeatmapLevel* previewBeatmapLevel, GlobalNamespace::GameplayModifiers* gameplayModifiers, GlobalNamespace::PlayerSpecificSettings* playerSpecificSettings, GlobalNamespace::PracticeSettings* practiceSettings, bool useTestNoteCutSoundEffects, GlobalNamespace::EnvironmentInfoSO* environmentInfo, GlobalNamespace::ColorScheme* colorScheme)
{
    SingletonContainer::get_colorManager()->SetColorSchemeFromBase(colorScheme);
    PlayerSettings::CheckForIllegalModifiers(gameplayModifiers);
    GameplayCoreSceneSetupData_ctor(self, difficultyBeatmap, previewBeatmapLevel, gameplayModifiers, playerSpecificSettings, practiceSettings, useTestNoteCutSoundEffects, environmentInfo, colorScheme);
}

// needed for reading settings lol
MAKE_HOOK_MATCH(StandardLevelScenesTransitionSetupDataSO_Init, &GlobalNamespace::StandardLevelScenesTransitionSetupDataSO::Init, void, GlobalNamespace::StandardLevelScenesTransitionSetupDataSO* self, Il2CppString* gameMode, GlobalNamespace::IDifficultyBeatmap* difficultyBeatmap, GlobalNamespace::IPreviewBeatmapLevel* previewBeatmapLevel, GlobalNamespace::OverrideEnvironmentSettings* overrideEnvironmentSettings, GlobalNamespace::ColorScheme* overrideColorScheme, GlobalNamespace::GameplayModifiers* gamePlayModifiers, GlobalNamespace::PlayerSpecificSettings* playerSpecificSettings, GlobalNamespace::PracticeSettings* practiceSettings, Il2CppString* backButtonText, bool useTestNoteCutSoundEffects)
{
    StandardLevelScenesTransitionSetupDataSO_Init(self, gameMode, difficultyBeatmap, previewBeatmapLevel, overrideEnvironmentSettings, overrideColorScheme, gamePlayModifiers, playerSpecificSettings, practiceSettings, backButtonText, useTestNoteCutSoundEffects);
    PlayerSettings::CheckForIllegalModifiers(gamePlayModifiers);
    PlayerSettings::CheckReducedDebris(playerSpecificSettings);
    Qosmetics::QosmeticsTrail::trailIntensity = Qosmetics::AltTrail::trailIntensity = playerSpecificSettings->get_saberTrailIntensity();
}

MAKE_HOOK_MATCH(MenuTransitionsHelper_RestartGame, &GlobalNamespace::MenuTransitionsHelper::RestartGame, void, GlobalNamespace::MenuTransitionsHelper* self, System::Action_1<Zenject::DiContainer*>* finishCallback)
{
    INFO("Game is soft restarting, handling it by throwing away pointers!");
    atLeastMenu = false;
    SingletonContainer::Delete();
    MenuTransitionsHelper_RestartGame(self, finishCallback);
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

    std::string path = getDataDir(modInfo);
    path = getDataDir(ID);

    Config::Init();
    
    if (!DescriptorCache::Load()) DescriptorCache::Save();
    QuestUI::Init();
    
    Logger& logger = QosmeticsLogger::GetLogger();
    
    CopyIcons();

    logger.info("Installing Hooks...");
    
    Hooks::InstallHooks(logger);

    logger.info("Installed Hooks!");

    logger.info("Registering Custom types...");
    RegisterTypes();

    logger.info("Registered Custom types!");
}

bool getSceneName(UnityEngine::SceneManagement::Scene scene, std::string& output)
{
    LoggerContextObject& logger = QosmeticsLogger::GetContextLogger("scene name");
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

void InstallMainHooks(Logger& logger)
{   
    SIMPLE_INSTALL_HOOK(SceneManager_SetActiveScene);
    SIMPLE_INSTALL_HOOK(StandardLevelScenesTransitionSetupDataSO_Init);
    SIMPLE_INSTALL_HOOK(GameplayCoreSceneSetupData_ctor);
    SIMPLE_INSTALL_HOOK(MenuTransitionsHelper_RestartGame);
}

QOS_INSTALL_HOOKS(InstallMainHooks)