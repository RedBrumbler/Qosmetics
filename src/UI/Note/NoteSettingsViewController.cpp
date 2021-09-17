#include "Config.hpp"
#include "UI/Note/NoteSettingsViewController.hpp"

DEFINE_TYPE(Qosmetics::UI, NoteSettingsViewController);

#include "HMUI/Touchable.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/ExternalComponents.hpp"
#include "questui/shared/CustomTypes/Data/CustomDataType.hpp"

#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/Events/UnityAction_1.hpp"
#include "UnityEngine/WaitUntil.hpp"
#include "System/Collections/IEnumerator.hpp"
#include "System/Func_1.hpp"

#include "UnityEngine/UI/Toggle.hpp"

#include "Utils/UIUtils.hpp"
#include "Utils/DisablingUtils.hpp"

using namespace HMUI;
using namespace UnityEngine;
using namespace UnityEngine::Events;
using namespace UnityEngine::UI;
using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;
using namespace Qosmetics;
using namespace Qosmetics::UI;
using namespace TMPro;

static std::vector<std::string> trailTextValues = {
    "Custom",
    "Default",
    "None"
};

extern config_t config;

namespace Qosmetics::UI
{
    struct settingsInfo {
        int counter;
        GameObject* container;
        NoteSettingsViewController* self;
        settingsInfo(GameObject* container, NoteSettingsViewController* self) : container(container), self(self), counter(0) {}
    };

    struct enumInfo {
        NoteSettingsViewController* view;
        IncrementSetting* self;

        enumInfo(NoteSettingsViewController* view, IncrementSetting* self) : view(view), self(self) {}
    };

    bool coro(settingsInfo* info);

    void NoteSettingsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            //UIUtils::SetupViewController(this);
            get_gameObject()->AddComponent<Touchable*>();
            
            UIUtils::AddHeader(get_transform(), "Note Settings", Color::get_blue());

            GameObject* container = CreateScrollableSettingsContainer(get_transform());

            ExternalComponents* externalComponents = container->GetComponent<ExternalComponents*>();
            RectTransform* scrollTransform = externalComponents->Get<RectTransform*>();
            scrollTransform->set_sizeDelta(UnityEngine::Vector2(0.0f, 0.0f));

            StartCoroutine(reinterpret_cast<System::Collections::IEnumerator*>(custom_types::Helpers::CoroutineHelper::New(SettingsSetupRoutine(container))));   
        }
    }

    void NoteSettingsViewController::Init(NotePreviewViewController* previewViewController)
    {
        this->previewViewController = previewViewController;
        set_enabled(false);
    }

    custom_types::Helpers::Coroutine NoteSettingsViewController::SettingsSetupRoutine(GameObject* container)
    {
        Transform* containerT = container->get_transform();
        auto disableSetting = BeatSaberUI::CreateToggle(containerT, "Complete Disable", config.noteConfig.disabled, [this](bool val){
            config.noteConfig.disabled = val;
            if (config.noteConfig.disabled) Disabling::RegisterDisablingInfo({ID, VERSION}, ItemType::note);
            else Disabling::UnregisterDisablingInfo({ID, VERSION}, ItemType::note);
        });
        BeatSaberUI::AddHoverHint(disableSetting->get_gameObject(), "Completely disables all things Qosmetics does with notes");
        co_yield nullptr;
        // note size toggle
        Toggle* noteSizeToggle = BeatSaberUI::CreateToggle(containerT, "Override Note Size", config.noteConfig.overrideNoteSize, [&](bool value) { 
                config.noteConfig.overrideNoteSize = value;
                previewViewController->UpdatePreview();
                SaveConfig();
            });
        BeatSaberUI::AddHoverHint(noteSizeToggle->get_gameObject(), "Whether to override the note size");
        co_yield nullptr;

        // note size setting
        QuestUI::IncrementSetting* noteSizeSetting = BeatSaberUI::CreateIncrementSetting(containerT, "Note Size", 2, 0.05f, config.noteConfig.noteSize, 0.05f, 10.0f, [&](float value) {
                if (value >= 0.0f)config.noteConfig.noteSize = value;
                previewViewController->UpdatePreview();
                SaveConfig();
            });
        BeatSaberUI::AddHoverHint(noteSizeSetting->get_gameObject(), "The overridden size of the notes compared to default, set to 1.00 for default size");
        co_yield nullptr;

        // change hitbox
        Toggle* hitboxSize = BeatSaberUI::CreateToggle(containerT, "Change Hitbox Sizes", config.noteConfig.alsoChangeHitboxes, [](bool value) { 
                config.noteConfig.alsoChangeHitboxes = value;
                SaveConfig();
            });
        BeatSaberUI::AddHoverHint(hitboxSize->get_gameObject(), "Whether or not to let the hitbox size change along with the note size, turning this on will disable scores");
        co_yield nullptr;

        // default bombs
        Toggle* bombToggle = BeatSaberUI::CreateToggle(containerT, "Force Default Bombs", config.noteConfig.forceDefaultBombs, [&](bool value) { 
                config.noteConfig.forceDefaultBombs = value;
                previewViewController->UpdatePreview();
                SaveConfig();
            });
        BeatSaberUI::AddHoverHint(bombToggle->get_gameObject(), "Force default bombs with this turned on");
        co_yield nullptr;

        // default debris
        Toggle* debrisToggle = BeatSaberUI::CreateToggle(containerT, "Force Default Debris", config.noteConfig.forceDefaultDebris, [&](bool value) { 
                config.noteConfig.forceDefaultDebris = value;
                previewViewController->UpdatePreview();
                SaveConfig();
            });
        BeatSaberUI::AddHoverHint(debrisToggle->get_gameObject(), "Force default debris with this turned on");
        co_yield nullptr;

        // default debris
        Toggle* mirrorToggle = BeatSaberUI::CreateToggle(containerT, "Disable Reflections", config.noteConfig.disableReflections, [&](bool value) { 
                config.noteConfig.disableReflections = value;
                SaveConfig();
            });
        BeatSaberUI::AddHoverHint(mirrorToggle->get_gameObject(), "With this turned on, the note reflections will be disabled");
        co_return;
    }
}