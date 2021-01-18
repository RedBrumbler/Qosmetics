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
#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/SpriteMeshType.hpp"
#include "UnityEngine/Rect.hpp"
#include "UnityEngine/TextureFormat.hpp"
#include "UnityEngine/ImageConversion.hpp"

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
#include "GlobalNamespace/OptionsViewController.hpp"
#include "GlobalNamespace/GameplaySetupViewController.hpp"
#include "GlobalNamespace/GameplaySetupViewController_Panel.hpp"
#include "GlobalNamespace/SoloFreePlayFlowCoordinator.hpp"
#include "GlobalNamespace/PartyFreePlayFlowCoordinator.hpp"
#include "GlobalNamespace/CampaignFlowCoordinator.hpp"
#include "GlobalNamespace/MainFlowCoordinator.hpp"
#include "GlobalNamespace/MenuDestination.hpp"
#include "GlobalNamespace/MainMenuViewController.hpp"
#include "GlobalNamespace/SelectSubMenuDestination.hpp"
#include "GlobalNamespace/GameServerLobbyFlowCoordinator.hpp"
#include "GlobalNamespace/HostGameServerLobbyFlowCoordinator.hpp"
#include "GlobalNamespace/ClientGameServerLobbyFlowCoordinator.hpp"
#include "GlobalNamespace/MultiplayerModeSelectionFlowCoordinator.hpp"
#include "GlobalNamespace/MainFlowCoordinator.hpp"

#include "HMUI/TextSegmentedControl.hpp"
#include "HMUI/SegmentedControl_IDataSource.hpp"
#include "HMUI/SegmentedControl.hpp"
#include "HMUI/SegmentedControlCell.hpp"
#include "HMUI/ButtonSpriteSwap.hpp"

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
#include "Data/Descriptor.hpp"
#include "Data/QosmeticsTrail.hpp"
#include "Data/QosmeticsDescriptorCache.hpp"

#include "Utils/MaterialUtils.hpp"

#include "custom-types/shared/register.hpp"


#include "UI/Saber/SaberSwitcherViewController.hpp"
#include "UI/Saber/SaberPreviewViewController.hpp"
#include "UI/Saber/SaberSettingsViewController.hpp"

#include "UI/Wall/WallSwitcherViewController.hpp"
#include "UI/Wall/WallPreviewViewController.hpp"
#include "UI/Wall/WallSettingsViewController.hpp"

#include "UI/Note/NoteSwitcherViewController.hpp"
#include "UI/Note/NotePreviewViewController.hpp"
#include "UI/Note/NoteSettingsViewController.hpp"

#include "UI/QosmeticsViewController.hpp"
#include "UI/QosmeticsFlowCoordinator.hpp"
//#include "UI/QosmeticsflowCoordinator.hpp"

#include "questui/shared/QuestUI.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/ViewController_AnimationType.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "Polyglot/LocalizedTextMeshProUGUI.hpp"

bool getSceneName(UnityEngine::SceneManagement::Scene scene, std::string& output);

std::string sceneLoadedName;

static ModInfo modInfo;

Logger& getLogger() {
  static Logger* logger = new Logger(modInfo, LoggerOptions(false, true));
  return *logger;
}

void makeFolder(std::string directory)
    {
        if (!direxists(directory.c_str()))
        {
            int makePath = mkpath(directory.data());
            if (makePath == -1)
            {
                getLogger().debug("Failed to make path %s", directory.c_str());
            }
        }
    }


bool shaderWarmupFirst = true;

static inline std::string shaderWarmup = "ShaderWarmup";
static inline std::string healthWarning = "HealthWarning";
static inline std::string gameCore = "GameCore";
static inline std::string menuViewControllers = "MenuViewControllers";
static inline std::string emptyTransition = "EmptyTransition";

extern config_t config;

MAKE_HOOK_OFFSETLESS(StandardLevelScenesTransitionSetupDataSO_Init, void, GlobalNamespace::StandardLevelScenesTransitionSetupDataSO* self, Il2CppString* gameMode, Il2CppObject* difficultyBeatmap, Il2CppObject* overrideEnvironmentSettings, Il2CppObject* overrideColorScheme, GlobalNamespace::GameplayModifiers* gameplayModifiers, GlobalNamespace::PlayerSpecificSettings* playerSpecificSettings, Il2CppObject* practiceSettings, Il2CppString* backButtonText, bool useTestNoteCutSoundEffects)
{
    StandardLevelScenesTransitionSetupDataSO_Init(self, gameMode, difficultyBeatmap, overrideEnvironmentSettings, overrideColorScheme, gameplayModifiers, playerSpecificSettings, practiceSettings, backButtonText, useTestNoteCutSoundEffects);
    
    Qosmetics::QuestNote::ModifierScoreDisableCheck(gameplayModifiers);
    Qosmetics::QuestNote::ReducedDebrisDisableCheck(playerSpecificSettings);
    Qosmetics::QuestSaber::SetTrailIntensity(playerSpecificSettings->saberTrailIntensity);
}

MAKE_HOOK_OFFSETLESS(NoteDebris_Init, void, GlobalNamespace::NoteDebris * self, GlobalNamespace::ColorType colorType, UnityEngine::Vector3 notePos, UnityEngine::Quaternion noteRot, UnityEngine::Vector3 positionOffset, UnityEngine::Quaternion rotationOffset, UnityEngine::Vector3 cutPoint, UnityEngine::Vector3 cutNormal, UnityEngine::Vector3 force, UnityEngine::Vector3 torque, float lifeTime)
{
    NoteDebris_Init(self, colorType, notePos, noteRot, positionOffset, rotationOffset, cutPoint, cutNormal, force, torque, lifeTime);
    if (config.noteConfig.overrideNoteSize) // apply custom size
    {
        self->get_transform()->Find(il2cpp_utils::createcsstr("NoteDebrisMesh"))->set_localScale(UnityEngine::Vector3::get_one() * (float)config.noteConfig.noteSize);
    }
    if (Qosmetics::QuestNote::GetActiveNote())
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
    Qosmetics::QuestNote::BombController_Init_Post(self);
}

MAKE_HOOK_OFFSETLESS(NoteController_Init, void, GlobalNamespace::NoteController* self, GlobalNamespace::BeatmapSaveData::NoteData* noteData, float worldRotation, Vector3 moveStartPos, Vector3 moveEndPos, Vector3 jumpEndPos, float moveDuration, float jumpDuration, float jumpGravity, float endRotation)
{
    NoteController_Init(self, noteData, worldRotation, moveStartPos, moveEndPos, jumpEndPos, moveDuration, jumpDuration, jumpGravity, endRotation);
    Qosmetics::QuestNote::NoteController_Init_Post(self);
}

MAKE_HOOK_OFFSETLESS(ObstacleController_set_hide, void, GlobalNamespace::ObstacleController* self, bool value)
{
    ObstacleController_set_hide(self, value);
    Qosmetics::QuestWall::ObstacleController_Set_Hide(self, value);
}

MAKE_HOOK_OFFSETLESS(ObstacleController_Init, void, GlobalNamespace::ObstacleController* self, Il2CppObject* obstacleData, float worldRotation, Vector3 startPos, Vector3 midPos, Vector3 endPos, float move1Duration, float move2Duration, float singleLineWidth, float height)
{
    Qosmetics::QuestWall::ObstacleController_Init_Pre(self);
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
        Qosmetics::QuestSaber::ShaderWarmup();
        Qosmetics::QuestWall::ShaderWarmup();
        Qosmetics::QuestNote::ShaderWarmup();
        shaderWarmupFirst = false;
    }

    if(sceneLoadedName == healthWarning)
    {
        Qosmetics::QuestSaber::HealthWarning();
        Qosmetics::QuestWall::HealthWarning();
        Qosmetics::QuestNote::HealthWarning();
        afterHealthWarning = true;
    }

    if(sceneLoadedName == gameCore)
    {
        Qosmetics::QuestSaber::GameCore();
        Qosmetics::QuestWall::GameCore();
        Qosmetics::QuestNote::GameCore();
    }

    if (sceneLoadedName == menuViewControllers)
    {
        Qosmetics::QuestSaber::MenuViewControllers();
        Qosmetics::QuestWall::MenuViewControllers();
        Qosmetics::QuestNote::MenuViewControllers();

        Qosmetics::ColorManager::Menu();
    }

    if (afterHealthWarning)
    {
        if (config.saberConfig.enableMenuPointer) 
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
    Qosmetics::QuestSaber::SaberStart(self->saber);
}

MAKE_HOOK_OFFSETLESS(GamePause_Pause, void, GlobalNamespace::GamePause* self)
{
    GamePause_Pause(self);
    if (afterHealthWarning)
    {
        if (config.saberConfig.enableMenuPointer) 
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
    if (config.saberConfig.enableMenuPointer) 
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

Qosmetics::QosmeticsFlowCoordinator* flowCoordinator = nullptr;
Qosmetics::PreviousFlowCoordinatorType returnFlowCoordinator = Qosmetics::PreviousFlowCoordinatorType::settings;

void OnQosmeticsButtonClick(UnityEngine::UI::Button* button) {
    getLogger().info("QosmeticsSongSelectButtonClick");
    if(!flowCoordinator)
        flowCoordinator = QuestUI::BeatSaberUI::CreateFlowCoordinator<Qosmetics::QosmeticsFlowCoordinator*>();
    flowCoordinator = UnityEngine::Object::FindObjectOfType<Qosmetics::QosmeticsFlowCoordinator*>();

    if (!flowCoordinator) return;
    HMUI::FlowCoordinator* currentCoordinator = nullptr;
    switch (returnFlowCoordinator)
    {
        case Qosmetics::PreviousFlowCoordinatorType::settings:
            currentCoordinator = reinterpret_cast<HMUI::FlowCoordinator*>(UnityEngine::Object::FindObjectOfType<GlobalNamespace::MainFlowCoordinator*>());
            getLogger().info("presenting from settings");
            break;
        case Qosmetics::PreviousFlowCoordinatorType::solo:
            currentCoordinator = reinterpret_cast<HMUI::FlowCoordinator*>(UnityEngine::Object::FindObjectOfType<GlobalNamespace::SoloFreePlayFlowCoordinator*>());
            getLogger().info("presenting from solo");
            break;
        case Qosmetics::PreviousFlowCoordinatorType::party:
            currentCoordinator = reinterpret_cast<HMUI::FlowCoordinator*>(UnityEngine::Object::FindObjectOfType<GlobalNamespace::PartyFreePlayFlowCoordinator*>());
            getLogger().info("presenting from party");
            break;
        case Qosmetics::PreviousFlowCoordinatorType::campaign:
            currentCoordinator = reinterpret_cast<HMUI::FlowCoordinator*>(UnityEngine::Object::FindObjectOfType<GlobalNamespace::CampaignFlowCoordinator*>());
            getLogger().info("presenting from campaign");
            break;
        case Qosmetics::PreviousFlowCoordinatorType::multiHost:
            currentCoordinator = reinterpret_cast<HMUI::FlowCoordinator*>(UnityEngine::Object::FindObjectOfType<GlobalNamespace::HostGameServerLobbyFlowCoordinator*>());
            getLogger().info("presenting from multiHost");
            break;
        case Qosmetics::PreviousFlowCoordinatorType::multiClient:
            currentCoordinator = reinterpret_cast<HMUI::FlowCoordinator*>(UnityEngine::Object::FindObjectOfType<GlobalNamespace::ClientGameServerLobbyFlowCoordinator*>());
            getLogger().info("presenting from multiClient");
            break;
        default:
            break;
    }

    if (currentCoordinator) currentCoordinator->PresentFlowCoordinator(flowCoordinator, nullptr, HMUI::ViewController::AnimationDirection::Horizontal, false, false);
}

MAKE_HOOK_OFFSETLESS(OptionsViewController_DidActivate, void, GlobalNamespace::OptionsViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    OptionsViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
    if(firstActivation) {
        flowCoordinator = nullptr;
        bool questUIExists = QuestUI::GetModsCount() > 0;
        QuestUI::BeatSaberUI::ClearCache();
        
        UnityEngine::UI::Button* avatarButton = self->settingsButton;
        UnityEngine::UI::Button* button = UnityEngine::Object::Instantiate(avatarButton);
        button->set_name(il2cpp_utils::createcsstr("Qosmetics Settings"));
        UnityEngine::Transform* AvatarParent = self->get_transform()->Find(il2cpp_utils::createcsstr("Wrapper"));
        button->get_transform()->SetParent(AvatarParent, false);

        if (questUIExists) // if questui has a menu button, make the base game stuff fuck off to another layout
        {
            UnityEngine::UI::HorizontalLayoutGroup* newLayout = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(self->get_transform());

            self->editAvatarButton->get_transform()->SetParent(newLayout->get_transform());
            self->playerOptionsButton->get_transform()->SetParent(newLayout->get_transform());
            self->settingsButton->get_transform()->SetParent(newLayout->get_transform());

            newLayout->get_transform()->SetAsFirstSibling();
            newLayout->set_spacing(-64.0f);
            newLayout->get_gameObject()->GetComponent<UnityEngine::RectTransform*>()->set_anchoredPosition(UnityEngine::Vector2(0.0f, -7.5f));

            UnityEngine::UI::HorizontalLayoutGroup* oldLayout = AvatarParent->get_gameObject()->GetComponent<UnityEngine::UI::HorizontalLayoutGroup*>();
            oldLayout->get_gameObject()->GetComponent<UnityEngine::RectTransform*>()->set_anchoredPosition(UnityEngine::Vector2(0.0f, 0.0f));
            button->get_transform()->SetAsLastSibling();
        }
        else button->get_transform()->SetAsFirstSibling();

        button->get_onClick()->AddListener(il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(classof(UnityEngine::Events::UnityAction*), (Il2CppObject*)nullptr, OnQosmeticsButtonClick));
        
        UnityEngine::Sprite* highlighted = FileUtils::SpriteFromFile("sdcard/Qosmetics/UI/Icons/MenuIconSelected.png", 266, 259);
        UnityEngine::Sprite* pressed = highlighted;
        UnityEngine::Sprite* selected = FileUtils::SpriteFromFile("sdcard/Qosmetics/UI/Icons/MenuIcon.png", 266, 259);
        UnityEngine::Sprite* disabled = selected;

        HMUI::ButtonSpriteSwap* spriteSwap = button->get_gameObject()->GetComponent<HMUI::ButtonSpriteSwap*>();
        spriteSwap->normalStateSprite = selected;
        spriteSwap->highlightStateSprite = highlighted;
        spriteSwap->pressedStateSprite = pressed;
        spriteSwap->disabledStateSprite = disabled;
        
        UnityEngine::Object::Destroy(button->GetComponentInChildren<Polyglot::LocalizedTextMeshProUGUI*>());

        button->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->SetText(il2cpp_utils::createcsstr("Qosmetics Settings"));
    }
}

MAKE_HOOK_OFFSETLESS(GameplaySetupViewController_DidActivate, void, GlobalNamespace::GameplaySetupViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    GameplaySetupViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
    if (firstActivation)
    {
        QuestUI::BeatSaberUI::ClearCache();
        UnityEngine::UI::HorizontalLayoutGroup* layout = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(self->get_transform());
        UnityEngine::RectTransform* rectTransform = layout->get_gameObject()->GetComponent<UnityEngine::RectTransform*>();

        rectTransform->set_anchoredPosition(UnityEngine::Vector2(52.5f, 25.6f));
        rectTransform->set_localScale(UnityEngine::Vector3::get_one() * 0.2f);
        UnityEngine::UI::Button* button = QuestUI::BeatSaberUI::CreateUIButton(rectTransform, "", "SettingsButton", il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(classof(UnityEngine::Events::UnityAction*), (Il2CppObject*)nullptr, +[](Il2CppObject* obj, UnityEngine::UI::Button* button){}));
        button->get_onClick()->AddListener(il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(classof(UnityEngine::Events::UnityAction*), (Il2CppObject*)nullptr, OnQosmeticsButtonClick));
        {
            UnityEngine::Sprite* highlighted = FileUtils::SpriteFromFile("sdcard/Qosmetics/UI/Icons/GameSetupIconSelected.png", 266, 259);
            UnityEngine::Sprite* pressed = highlighted;
            UnityEngine::Sprite* selected = FileUtils::SpriteFromFile("sdcard/Qosmetics/UI/Icons/GameSetupIcon.png", 266, 259);
            UnityEngine::Sprite* disabled = selected;
            HMUI::ButtonSpriteSwap* spriteSwap = button->get_gameObject()->GetComponent<HMUI::ButtonSpriteSwap*>();
            spriteSwap->normalStateSprite = selected;
            spriteSwap->highlightStateSprite = highlighted;
            spriteSwap->pressedStateSprite = pressed;
            spriteSwap->disabledStateSprite = disabled;
        }
    }
}

MAKE_HOOK_OFFSETLESS(MainMenuViewController_HandleMenuButton, void, GlobalNamespace::MainMenuViewController* self, GlobalNamespace::MainMenuViewController::MenuButton menuButton)
{
    MainMenuViewController_HandleMenuButton(self, menuButton);
    getLogger().info("Menu pressed: %d", menuButton.value);
    switch (menuButton.value)
    {
        case 0: // solo
            returnFlowCoordinator = Qosmetics::PreviousFlowCoordinatorType::solo;
            break;
        case 1: // party
            returnFlowCoordinator = Qosmetics::PreviousFlowCoordinatorType::party;
            break;
        case 2: // Editor
            returnFlowCoordinator = Qosmetics::PreviousFlowCoordinatorType::invalid;
            break;
        case 3: // campaign
            returnFlowCoordinator = Qosmetics::PreviousFlowCoordinatorType::campaign;
            break;
        case 4: // floorAdjust
            returnFlowCoordinator = Qosmetics::PreviousFlowCoordinatorType::invalid;
            break;
        case 5: // quit
            returnFlowCoordinator = Qosmetics::PreviousFlowCoordinatorType::invalid;
            break;
        case 6: // multi
            returnFlowCoordinator = Qosmetics::PreviousFlowCoordinatorType::invalid;
            break;
        case 7: // options
            returnFlowCoordinator = Qosmetics::PreviousFlowCoordinatorType::settings;
            break;
        case 8: // howtoplay/tutorial
            returnFlowCoordinator = Qosmetics::PreviousFlowCoordinatorType::invalid;
            break;
    }
}

MAKE_HOOK_OFFSETLESS(MultiplayerModeSelectionFlowCoordinator_TopViewControllerWillChange, void, GlobalNamespace::MultiplayerModeSelectionFlowCoordinator* self, HMUI::ViewController* oldViewController, HMUI::ViewController* newViewController, HMUI::ViewController::AnimationType animationType)
{
    MultiplayerModeSelectionFlowCoordinator_TopViewControllerWillChange(self, oldViewController, newViewController, animationType);
    if ((void*)newViewController == (void*)self->createServerViewController)
    {
        getLogger().info("User is Host");
        returnFlowCoordinator = Qosmetics::PreviousFlowCoordinatorType::multiHost;
    }
    else if ((void*)newViewController == (void*)self->serverCodeEntryViewController)
    {
        getLogger().info("User is Client");
        returnFlowCoordinator = Qosmetics::PreviousFlowCoordinatorType::multiClient;
    }
}
extern "C" void setup(ModInfo& info) 
{
    info.id = ID;
    info.version = VERSION;
    modInfo = info;
    
    getLogger().info("If you are reading this then idk what you're doing, go play the game, don't read logs");
}

void WipeAllDefinedPointers()
{
    Qosmetics::QuestSaber::ClearAllInternalPointers();
    Qosmetics::QuestWall::ClearAllInternalPointers();
    Qosmetics::QuestNote::ClearAllInternalPointers();
}

void CopyIcons()
{
    makeFolder("sdcard/Qosmetics");
    makeFolder("sdcard/Qosmetics/UI");
    makeFolder("sdcard/Qosmetics/UI/Icons");
    
    if (!fileexists("sdcard/Qosmetics/UI/Icons/MenuIcon.png"))
            writefile("sdcard/Qosmetics/UI/Icons/MenuIcon.png", readfile("sdcard/BMBFData/Mods/Qosmetics/MenuIcon.png"));
    
    if (!fileexists("sdcard/Qosmetics/UI/Icons/MenuIconSelected.png"))
            writefile("sdcard/Qosmetics/UI/Icons/MenuIconSelected.png", readfile("sdcard/BMBFData/Mods/Qosmetics/MenuIconSelected.png"));
    
    if (!fileexists("sdcard/Qosmetics/UI/Icons/SaberIconSelected.png"))
            writefile("sdcard/Qosmetics/UI/Icons/SaberIconSelected.png", readfile("sdcard/BMBFData/Mods/Qosmetics/SaberIconSelected.png"));
    
    if (!fileexists("sdcard/Qosmetics/UI/Icons/SaberIcon.png"))
            writefile("sdcard/Qosmetics/UI/Icons/SaberIcon.png", readfile("sdcard/BMBFData/Mods/Qosmetics/SaberIcon.png"));
    
    if (!fileexists("sdcard/Qosmetics/UI/Icons/NoteIconSelected.png"))
            writefile("sdcard/Qosmetics/UI/Icons/NoteIconSelected.png", readfile("sdcard/BMBFData/Mods/Qosmetics/NoteIconSelected.png"));
    
    if (!fileexists("sdcard/Qosmetics/UI/Icons/NoteIcon.png"))
            writefile("sdcard/Qosmetics/UI/Icons/NoteIcon.png", readfile("sdcard/BMBFData/Mods/Qosmetics/NoteIcon.png"));
    
    if (!fileexists("sdcard/Qosmetics/UI/Icons/WallIconSelected.png"))
            writefile("sdcard/Qosmetics/UI/Icons/WallIconSelected.png", readfile("sdcard/BMBFData/Mods/Qosmetics/WallIconSelected.png"));
    
    if (!fileexists("sdcard/Qosmetics/UI/Icons/WallIcon.png"))
            writefile("sdcard/Qosmetics/UI/Icons/WallIcon.png", readfile("sdcard/BMBFData/Mods/Qosmetics/WallIcon.png"));
    
    if (!fileexists("sdcard/Qosmetics/UI/Icons/GameSetupIconSelected.png"))
            writefile("sdcard/Qosmetics/UI/Icons/GameSetupIconSelected.png", readfile("sdcard/BMBFData/Mods/Qosmetics/GameSetupIconSelected.png"));
    
    if (!fileexists("sdcard/Qosmetics/UI/Icons/GameSetupIcon.png"))
            writefile("sdcard/Qosmetics/UI/Icons/GameSetupIcon.png", readfile("sdcard/BMBFData/Mods/Qosmetics/GameSetupIcon.png"));
}


extern "C" void load() 
{
    if (!LoadConfig()) SaveConfig();

    if (!Qosmetics::DescriptorCache::Load()) Qosmetics::DescriptorCache::Write();

    CopyIcons();
    
    QuestUI::Init();
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
    INSTALL_HOOK_OFFSETLESS(logger, OptionsViewController_DidActivate, il2cpp_utils::FindMethodUnsafe("", "OptionsViewController", "DidActivate", 3));
    INSTALL_HOOK_OFFSETLESS(logger, GameplaySetupViewController_DidActivate, il2cpp_utils::FindMethodUnsafe("", "GameplaySetupViewController", "DidActivate", 3));
    INSTALL_HOOK_OFFSETLESS(logger, MainMenuViewController_HandleMenuButton, il2cpp_utils::FindMethodUnsafe("", "MainMenuViewController", "HandleMenuButton", 1));
    INSTALL_HOOK_OFFSETLESS(logger, MultiplayerModeSelectionFlowCoordinator_TopViewControllerWillChange, il2cpp_utils::FindMethodUnsafe("", "MultiplayerModeSelectionFlowCoordinator", "TopViewControllerWillChange", 3));

    CRASH_UNLESS(custom_types::Register::RegisterType<::Qosmetics::QosmeticsTrail>());
    CRASH_UNLESS(custom_types::Register::RegisterType<::Qosmetics::ColorScheme>());
    CRASH_UNLESS(custom_types::Register::RegisterType<::Qosmetics::ColorManager>());

    CRASH_UNLESS(custom_types::Register::RegisterType<::Qosmetics::SaberPreviewViewController>());
    CRASH_UNLESS(custom_types::Register::RegisterType<::Qosmetics::SaberSettingsViewController>());
    CRASH_UNLESS(custom_types::Register::RegisterType<::Qosmetics::SaberSwitcherViewController>());

    CRASH_UNLESS(custom_types::Register::RegisterType<::Qosmetics::NotePreviewViewController>());
    CRASH_UNLESS(custom_types::Register::RegisterType<::Qosmetics::NoteSettingsViewController>());
    CRASH_UNLESS(custom_types::Register::RegisterType<::Qosmetics::NoteSwitcherViewController>());

    CRASH_UNLESS(custom_types::Register::RegisterType<::Qosmetics::WallPreviewViewController>());
    CRASH_UNLESS(custom_types::Register::RegisterType<::Qosmetics::WallSettingsViewController>());
    CRASH_UNLESS(custom_types::Register::RegisterType<::Qosmetics::WallSwitcherViewController>());

    CRASH_UNLESS(custom_types::Register::RegisterType<::Qosmetics::QosmeticsViewController>());
    CRASH_UNLESS(custom_types::Register::RegisterType<::Qosmetics::QosmeticsFlowCoordinator>());
    //CRASH_UNLESS(custom_types::Register::RegisterType<::Qosmetics::QosmeticsflowCoordinator>());
    
    //QuestUI::Register::RegisterModSettingsFlowCoordinator<Qosmetics::QosmeticsFlowCoordinator*>((ModInfo){"Qosmetics Settings", VERSION});

    //QuestUI::Register::RegisterModSettingsViewController<Qosmetics::WallSettingsViewController*>((ModInfo){"Qosmetics Wall Settings", VERSION});
    //QuestUI::Register::RegisterModSettingsViewController<Qosmetics::SaberSettingsViewController*>((ModInfo){"Qosmetics Saber Settings", VERSION});
    //QuestUI::Register::RegisterModSettingsViewController<Qosmetics::NoteSettingsViewController*>((ModInfo){"Qosmetics Bloq Settings", VERSION});
    
    
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