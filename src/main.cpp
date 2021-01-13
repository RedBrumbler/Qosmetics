#include "config.hpp"

#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp" 
#include "beatsaber-hook/shared/utils/typedefs.h"

#include <string>
#include <cstdlib>

#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/SceneManagement/Scene.hpp"
#include "UnityEngine/Object.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/SendMessageOptions.hpp"

#include "System/Action.hpp"

#include "GlobalNamespace/ConditionalMaterialSwitcher.hpp"
#include "GlobalNamespace/ObstacleController.hpp"
#include "GlobalNamespace/NoteController.hpp"
#include "GlobalNamespace/BeatmapSaveData_NoteData.hpp"
#include "GlobalNamespace/BeatmapSaveData.hpp"
#include "GlobalNamespace/GameplayModifiers.hpp"
#include "GlobalNamespace/MenuTransitionsHelper.hpp"
#include "GlobalNamespace/NoteDebris.hpp"
#include "GlobalNamespace/BombNoteController.hpp"
#include "GlobalNamespace/PlayerSpecificSettings.hpp"
#include "GlobalNamespace/SettingsFlowCoordinator.hpp"
#include "GlobalNamespace/SettingsNavigationController.hpp"
#include "GlobalNamespace/ColorType.hpp"
#include "GlobalNamespace/SaberModelController.hpp"
#include "GlobalNamespace/StandardLevelScenesTransitionSetupDataSO.hpp"
#include "GlobalNamespace/SaberModelContainer.hpp"
#include "GlobalNamespace/SetSaberFakeGlowColor.hpp"
#include "GlobalNamespace/ColorScheme.hpp"
#include "GlobalNamespace/VRController.hpp"
#include "GlobalNamespace/GamePause.hpp"
#include "GlobalNamespace/MultiplayerSessionManager.hpp"
#include "GlobalNamespace/MultiplayerLobbyConnectionController.hpp"
#include "GlobalNamespace/BeatmapDifficultyMask.hpp"
#include "GlobalNamespace/SongPackMask.hpp"
#include "GlobalNamespace/INetworkPlayer.hpp"
#include "GlobalNamespace/UnifiedNetworkPlayerModel_CreatePartyConfig.hpp"
#include "GlobalNamespace/MultiplayerPlayersManager.hpp"
#include "GlobalNamespace/PauseMenuManager.hpp"
#include "GlobalNamespace/MultiplayerLocalActivePlayerInGameMenuViewController.hpp"

#include "Qosmetic/QuestSaber.hpp"
#include "Qosmetic/QuestNote.hpp"
#include "Qosmetic/QuestWall.hpp"
#include "Qosmetic/QosmeticsColorManager.hpp"
#include "Qosmetic/QosmeticsColorScheme.hpp"

#include "Logging/WallLogger.hpp"
#include "Logging/NoteLogger.hpp"
#include "Logging/SaberLogger.hpp"
#include "Logging/GenericLogger.hpp"

#include "Data/CustomTrail.hpp"
#include "Data/QosmeticsTrail.hpp"

#include "Utils/MaterialUtils.hpp"

#include "custom-types/shared/register.hpp"


bool getSceneName(UnityEngine::SceneManagement::Scene scene, std::string& output);

std::string sceneLoadedName;

static ModInfo modInfo;

Logger& getLogger() {
  static Logger* logger = new Logger(modInfo, LoggerOptions(false, true));
  return *logger;
}

bool wallsEnabled = true;
bool notesEnabled = true;
bool sabersEnabled = true;
bool shaderWarmupFirst = true;

std::string shaderWarmup = "ShaderWarmup";
std::string healthWarning = "HealthWarning";
std::string gameCore = "GameCore";
std::string menuViewControllers = "MenuViewControllers";
std::string emptyTransition = "EmptyTransition";

extern config_t config;

MAKE_HOOK_OFFSETLESS(StandardLevelScenesTransitionSetupDataSO_Init, void, GlobalNamespace::StandardLevelScenesTransitionSetupDataSO* self, Il2CppString* gameMode, Il2CppObject* difficultyBeatmap, Il2CppObject* overrideEnvironmentSettings, Il2CppObject* overrideColorScheme, GlobalNamespace::GameplayModifiers* gameplayModifiers, GlobalNamespace::PlayerSpecificSettings* playerSpecificSettings, Il2CppObject* practiceSettings, Il2CppString* backButtonText, bool useTestNoteCutSoundEffects)
{
    StandardLevelScenesTransitionSetupDataSO_Init(self, gameMode, difficultyBeatmap, overrideEnvironmentSettings, overrideColorScheme, gameplayModifiers, playerSpecificSettings, practiceSettings, backButtonText, useTestNoteCutSoundEffects);
    
    if (notesEnabled) 
    {
        Qosmetics::QuestNote::ModifierScoreDisableCheck(gameplayModifiers);
        Qosmetics::QuestNote::ReducedDebrisDisableCheck(playerSpecificSettings);
    }
    if (sabersEnabled) Qosmetics::QuestSaber::SetTrailIntensity(playerSpecificSettings->saberTrailIntensity);
}

MAKE_HOOK_OFFSETLESS(NoteDebris_Init, void, GlobalNamespace::NoteDebris * self, GlobalNamespace::ColorType colorType, UnityEngine::Vector3 notePos, UnityEngine::Quaternion noteRot, UnityEngine::Vector3 positionOffset, UnityEngine::Quaternion rotationOffset, UnityEngine::Vector3 cutPoint, UnityEngine::Vector3 cutNormal, UnityEngine::Vector3 force, UnityEngine::Vector3 torque, float lifeTime)
{
    NoteDebris_Init(self, colorType, notePos, noteRot, positionOffset, rotationOffset, cutPoint, cutNormal, force, torque, lifeTime);
    if (notesEnabled) 
    {
        UnityEngine::Transform* initTransform = UnityEngine::GameObject::New_ctor()->get_transform();//UnityEngine::Object::Instantiate<UnityEngine::GameObject*>(UnityEngine::GameObject::New_ctor())->get_transform();
        initTransform->set_localPosition(notePos);
        Qosmetics::QuestNote::NoteDebris_Init_Post(self, colorType.value, initTransform, cutPoint, cutNormal);
        UnityEngine::Object::Destroy(initTransform->get_gameObject());
    }
}

MAKE_HOOK_OFFSETLESS(BombNoteController_Init, void, GlobalNamespace::BombNoteController* self, GlobalNamespace::BeatmapSaveData::NoteData* noteData, float worldRotation, UnityEngine::Vector3 moveStartPos, UnityEngine::Vector3 moveEndPos, UnityEngine::Vector3 jumpEndPos, float moveDuration, float jumpDuration, float jumpGravity)
{
    noteData->cutDirection = GlobalNamespace::NoteCutDirection::_get_Down();
    BombNoteController_Init(self, noteData, worldRotation, moveStartPos, moveEndPos, jumpEndPos, moveDuration, jumpDuration, jumpGravity);
    if (notesEnabled) Qosmetics::QuestNote::BombController_Init_Post(self);
}

MAKE_HOOK_OFFSETLESS(NoteController_Init, void, GlobalNamespace::NoteController* self, GlobalNamespace::BeatmapSaveData::NoteData* noteData, float worldRotation, Vector3 moveStartPos, Vector3 moveEndPos, Vector3 jumpEndPos, float moveDuration, float jumpDuration, float jumpGravity, float endRotation)
{
    NoteController_Init(self, noteData, worldRotation, moveStartPos, moveEndPos, jumpEndPos, moveDuration, jumpDuration, jumpGravity, endRotation);
    if (notesEnabled) Qosmetics::QuestNote::NoteController_Init_Post(self);
}

MAKE_HOOK_OFFSETLESS(ObstacleController_set_hide, void, GlobalNamespace::ObstacleController* self, bool value)
{
    ObstacleController_set_hide(self, value);
    if (wallsEnabled) Qosmetics::QuestWall::ObstacleController_Set_Hide(self, value);
}

MAKE_HOOK_OFFSETLESS(ObstacleController_Init, void, GlobalNamespace::ObstacleController* self, Il2CppObject* obstacleData, float worldRotation, Vector3 startPos, Vector3 midPos, Vector3 endPos, float move1Duration, float move2Duration, float singleLineWidth, float height)
{
    if (wallsEnabled) Qosmetics::QuestWall::ObstacleController_Init_Pre(self);
    ObstacleController_Init(self, obstacleData, worldRotation, startPos, midPos, endPos, move1Duration, move2Duration, singleLineWidth, height);
}
bool afterHealthWarning = false;

MAKE_HOOK_OFFSETLESS(SceneManager_ActiveSceneChanged, void, UnityEngine::SceneManagement::Scene previousActiveScene, UnityEngine::SceneManagement::Scene nextActiveScene)
{
    getSceneName(nextActiveScene, sceneLoadedName);
    std::string oldScene;
    if (!getSceneName(previousActiveScene, oldScene)) oldScene = "";
    getLogger().info("Found scene %s, handle is %d", sceneLoadedName.c_str(), nextActiveScene.m_Handle);

    if(sceneLoadedName == shaderWarmup && shaderWarmupFirst)
    {
        if (sabersEnabled) sabersEnabled = Qosmetics::QuestSaber::ShaderWarmup();
        if (wallsEnabled) wallsEnabled = Qosmetics::QuestWall::ShaderWarmup();
        if (notesEnabled) notesEnabled = Qosmetics::QuestNote::ShaderWarmup();

        if (!sabersEnabled) unsetenv("qsabersenabled");
        if (!notesEnabled) unsetenv("qbloqsenabled");
        if (!wallsEnabled) unsetenv("qwallsenabled");
        
        shaderWarmupFirst = false;
    }

    if(sceneLoadedName == healthWarning)
    {
        if (sabersEnabled) Qosmetics::QuestSaber::HealthWarning();
        if (wallsEnabled) Qosmetics::QuestWall::HealthWarning();
        if (notesEnabled) Qosmetics::QuestNote::HealthWarning();
        afterHealthWarning = true;
    }

    if(sceneLoadedName == gameCore)
    {
        if (sabersEnabled) Qosmetics::QuestSaber::GameCore();
        if (wallsEnabled) Qosmetics::QuestWall::GameCore();
        if (notesEnabled) Qosmetics::QuestNote::GameCore();
    }

    if (sceneLoadedName == menuViewControllers)
    {
        if (sabersEnabled) Qosmetics::QuestSaber::MenuViewControllers();
        if (wallsEnabled) Qosmetics::QuestWall::MenuViewControllers();
        if (notesEnabled) Qosmetics::QuestNote::MenuViewControllers();

        if(sabersEnabled || wallsEnabled || notesEnabled) Qosmetics::ColorManager::Menu();
    }

    if (afterHealthWarning)
    {
        if (sabersEnabled && config.enableMenuPointer) 
        {
            Array<GlobalNamespace::VRController*>* VRControllers = UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::VRController*>();
            if (VRControllers)
            {
                std::string distantParentName = "";
                for (int i = 0; i < VRControllers->Length(); i++)
                {
                    int node = VRControllers->values[i]->node;
                    
                    if (!(node == 4 || node == 5)) continue;
                    UnityEngine::Transform* parent1 = VRControllers->values[i]->get_transform()->get_parent();
                    UnityEngine::Transform* parent2 = parent1 ? parent1->get_parent() : nullptr;
                    UnityEngine::Transform* parent3 = parent2 ? parent2->get_parent() : nullptr;
                    UnityEngine::Transform* distantParent = parent3 ? parent3 : nullptr;
                    distantParentName = distantParent ? to_utf8(csstrtostr(distantParent->get_gameObject()->get_name())) : "null";
                    if (distantParentName.find("Multiplayer") != std::string::npos ||
                        //distantParentName.find("LocalPlayerGameCore") != std::string::npos ||
                        distantParentName.find("IsActive") != std::string::npos) continue;
                        Qosmetics::QuestSaber::ReplaceMenuPointers(VRControllers->values[i]->get_transform(), node);
                }
            }
        }
    }

    SceneManager_ActiveSceneChanged(previousActiveScene, nextActiveScene);
}

MAKE_HOOK_OFFSETLESS(ConditionalMaterialSwitcher_Awake, void, GlobalNamespace::ConditionalMaterialSwitcher* self)
{
    // basically QuestTrailOverlap is forced with this
    UnityEngine::Transform* trailTransform = self->get_transform();
    std::string thisName = to_utf8(csstrtostr(trailTransform->get_gameObject()->get_name()));
    if (thisName == "Trail(Clone)") return;
    ConditionalMaterialSwitcher_Awake(self);
}

MAKE_HOOK_OFFSETLESS(SaberTrailRenderer_OnEnable, void, GlobalNamespace::SaberTrailRenderer* self)
{
    if (self->meshRenderer == nullptr)self->meshRenderer = self->get_gameObject()->GetComponent<UnityEngine::MeshRenderer*>();
    if (self->meshFilter == nullptr) self->meshFilter = self->get_gameObject()->GetComponent<UnityEngine::MeshFilter*>();
    SaberTrailRenderer_OnEnable(self);
}

MAKE_HOOK_OFFSETLESS(SaberModelContainer_Start, void, GlobalNamespace::SaberModelContainer* self)
{
    SaberModelContainer_Start(self);
    if (sabersEnabled) Qosmetics::QuestSaber::SaberStart(self->saber);
}

MAKE_HOOK_OFFSETLESS(GamePause_Pause, void, GlobalNamespace::GamePause* self)
{
    GamePause_Pause(self);
    if (afterHealthWarning)
    {
        if (sabersEnabled && config.enableMenuPointer) 
        {
            Array<GlobalNamespace::VRController*>* VRControllers = UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::VRController*>();
            if (VRControllers)
            {
                std::string distantParentName = "";
                for (int i = 0; i < VRControllers->Length(); i++)
                {
                    int node = VRControllers->values[i]->node;
                    
                    if (!(node == 4 || node == 5)) continue;
                    UnityEngine::Transform* parent1 = VRControllers->values[i]->get_transform()->get_parent();
                    UnityEngine::Transform* parent2 = parent1 ? parent1->get_parent() : nullptr;
                    UnityEngine::Transform* parent3 = parent2 ? parent2->get_parent() : nullptr;
                    UnityEngine::Transform* distantParent = parent3 ? parent3 : nullptr;
                    distantParentName = distantParent ? to_utf8(csstrtostr(distantParent->get_gameObject()->get_name())) : "null";
                    if (distantParentName.find("Multiplayer") != std::string::npos ||
                        //distantParentName.find("LocalPlayerGameCore") != std::string::npos ||
                        distantParentName.find("IsActive") != std::string::npos) continue;
                        Qosmetics::QuestSaber::ReplaceMenuPointers(VRControllers->values[i]->get_transform(), node);
                }
            }
        }
    }
}

MAKE_HOOK_OFFSETLESS(MultiplayerPlayersManager_SwitchLocalPlayerToInactive, void, GlobalNamespace::MultiplayerPlayersManager* self)
{
    MultiplayerPlayersManager_SwitchLocalPlayerToInactive(self);
    getLogger().info("Multi pause!");
}

MAKE_HOOK_OFFSETLESS(MultiplayerLocalActivePlayerInGameMenuViewController_ShowMenu, void, GlobalNamespace::MultiplayerLocalActivePlayerInGameMenuViewController* self)
{
    MultiplayerLocalActivePlayerInGameMenuViewController_ShowMenu(self);
    if (sabersEnabled && config.enableMenuPointer) 
        {
            Array<GlobalNamespace::VRController*>* VRControllers = UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::VRController*>();
            if (VRControllers)
            {
                std::string distantParentName = "";
                std::string parent1Name = "";
                std::string parent2Name = "";
                std::string parent3Name = "";
                for (int i = 0; i < VRControllers->Length(); i++)
                {
                    int node = VRControllers->values[i]->node;
                    
                    if (!(node == 4 || node == 5)) continue;
                    UnityEngine::Transform* parent1 = VRControllers->values[i]->get_transform()->get_parent();
                    UnityEngine::Transform* parent2 = parent1 ? parent1->get_parent() : nullptr;
                    UnityEngine::Transform* parent3 = parent2 ? parent2->get_parent() : nullptr;
                    UnityEngine::Transform* parent4 = parent3 ? parent3->get_parent() : nullptr;
                    UnityEngine::Transform* distantParent = parent4 ? parent4 : nullptr;
                    distantParentName = distantParent ? to_utf8(csstrtostr(distantParent->get_gameObject()->get_name())) : "null";
                    parent1Name = parent1 ? to_utf8(csstrtostr(parent1->get_gameObject()->get_name())) : "null";
                    parent2Name = parent2 ? to_utf8(csstrtostr(parent2->get_gameObject()->get_name())) : "null";
                    parent3Name = parent3 ? to_utf8(csstrtostr(parent3->get_gameObject()->get_name())) : "null";

                    if (parent2Name.find("MultiplayerLocalActivePlayerInGameMenuViewController") != std::string::npos &&
                        parent3Name.find("Multiplayer") == std::string::npos &&
                        distantParentName.find("Clone") != std::string::npos)
                    {
                        getLogger().info("Found MP Tree %s/%s/%s/%s", distantParentName.c_str(), parent3Name.c_str(), parent2Name.c_str(), parent1Name.c_str());
                        getLogger().info("Found %d vrcontroller node", node);
                        Qosmetics::QuestSaber::ReplaceMenuPointers(VRControllers->values[i]->get_transform(), node);
                    }
                }
            }
        }
}

extern "C" void setup(ModInfo& info) 
{
    info.id = ID;
    info.version = VERSION;
    modInfo = info;
    Qosmetics::GenericLogger::modInfo = info;

    Qosmetics::QuestSaber::modInfo.id = string_format("%s Sabers", ID);
    Qosmetics::QuestSaber::modInfo.version = info.version;
    Qosmetics::SaberLogger::modInfo.id = string_format("%s Sabers", ID);
    Qosmetics::SaberLogger::modInfo.version = info.version;

    Qosmetics::QuestNote::modInfo.id = string_format("%s Notes", ID);
    Qosmetics::QuestNote::modInfo.version = info.version;
    Qosmetics::NoteLogger::modInfo.id = string_format("%s Notes", ID);
    Qosmetics::NoteLogger::modInfo.version = info.version;

    Qosmetics::QuestWall::modInfo.id = string_format("%s Walls", ID);
    Qosmetics::QuestWall::modInfo.version = info.version;
    Qosmetics::WallLogger::modInfo.id = string_format("%s Walls", ID);
    Qosmetics::WallLogger::modInfo.version = info.version;
    getLogger().info("If you are reading this then idk what you're doing, go play the game, don't read logs");
    getLogger().info(info.version);
    getLogger().info(info.id);
}

void WipeAllDefinedPointers()
{
    if (sabersEnabled) Qosmetics::QuestSaber::ClearAllInternalPointers();
    if (wallsEnabled) Qosmetics::QuestWall::ClearAllInternalPointers();
    if (notesEnabled) Qosmetics::QuestNote::ClearAllInternalPointers();
}

extern "C" void load() 
{
    if (!LoadConfig()) SaveConfig();
    il2cpp_functions::Init();
    getLogger().info("Installing hooks");

    LoggerContextObject logger = getLogger().WithContext("load");
    INSTALL_HOOK_OFFSETLESS(logger, ConditionalMaterialSwitcher_Awake, il2cpp_utils::FindMethodUnsafe("", "ConditionalMaterialSwitcher", "Awake", 0)); 
    INSTALL_HOOK_OFFSETLESS(logger, SceneManager_ActiveSceneChanged, il2cpp_utils::FindMethodUnsafe("UnityEngine.SceneManagement", "SceneManager", "Internal_ActiveSceneChanged", 2));
    INSTALL_HOOK_OFFSETLESS(logger, ObstacleController_Init, il2cpp_utils::FindMethodUnsafe("", "ObstacleController", "Init", 9));
    INSTALL_HOOK_OFFSETLESS(logger, ObstacleController_set_hide, il2cpp_utils::FindMethodUnsafe("", "ObstacleController", "set_hide", 1));
    INSTALL_HOOK_OFFSETLESS(logger, NoteController_Init, il2cpp_utils::FindMethodUnsafe("", "NoteController", "Init", 9));
    INSTALL_HOOK_OFFSETLESS(logger, NoteDebris_Init, il2cpp_utils::FindMethodUnsafe("", "NoteDebris", "Init", 10));
    INSTALL_HOOK_OFFSETLESS(logger, BombNoteController_Init, il2cpp_utils::FindMethodUnsafe("", "BombNoteController", "Init", 8));
    INSTALL_HOOK_OFFSETLESS(logger, SaberTrailRenderer_OnEnable, il2cpp_utils::FindMethodUnsafe("", "SaberTrailRenderer", "OnEnable", 0));
    INSTALL_HOOK_OFFSETLESS(logger, SaberModelContainer_Start, il2cpp_utils::FindMethodUnsafe("", "SaberModelContainer", "Start", 0));
    INSTALL_HOOK_OFFSETLESS(logger, StandardLevelScenesTransitionSetupDataSO_Init, il2cpp_utils::FindMethodUnsafe("", "StandardLevelScenesTransitionSetupDataSO", "Init", 9));
    INSTALL_HOOK_OFFSETLESS(logger, GamePause_Pause, il2cpp_utils::FindMethodUnsafe("", "GamePause", "Pause", 0));
    INSTALL_HOOK_OFFSETLESS(logger, MultiplayerLocalActivePlayerInGameMenuViewController_ShowMenu, il2cpp_utils::FindMethodUnsafe("", "MultiplayerLocalActivePlayerInGameMenuViewController", "ShowMenu", 0));

    CRASH_UNLESS(custom_types::Register::RegisterType<::Qosmetics::QosmeticsTrail>());
    CRASH_UNLESS(custom_types::Register::RegisterType<::Qosmetics::ColorScheme>());
    CRASH_UNLESS(custom_types::Register::RegisterType<::Qosmetics::ColorManager>());

    std::thread WipeRoutine(
        [&]{
            int timer = 0;
            while (1) {
                WipeAllDefinedPointers();
                getLogger().info("Wiped ALL pointers...");
                timer = 0;
                while (timer < 2000)
                {
                    usleep(2000);
                    timer++;
                }
            }
        });

    WipeRoutine.detach();
    
    setenv("qsabersenabled", "1", 1);
    setenv("qbloqsenabled", "1", 1);
    setenv("qwallsenabled", "1", 1);

    getLogger().info("Hooks installed");
}

bool getSceneName(UnityEngine::SceneManagement::Scene scene, std::string& output)
{
    LoggerContextObject logger = getLogger().WithContext("scene name");
    Il2CppString* csString = UnityEngine::SceneManagement::Scene::GetNameInternal(scene.m_Handle);
    RET_0_UNLESS(logger, csString);
    output = to_utf8(csstrtostr(csString));
    return true; 
}