#include "Config.hpp"
#include "UI/Wall/WallSettingsViewController.hpp"

DEFINE_TYPE(Qosmetics::UI, WallSettingsViewController);

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
        WallSettingsViewController* self;
        settingsInfo(GameObject* container, WallSettingsViewController* self) : container(container), self(self), counter(0) {}
    };

    struct enumInfo {
        WallSettingsViewController* view;
        IncrementSetting* self;

        enumInfo(WallSettingsViewController* view, IncrementSetting* self) : view(view), self(self) {}
    };

    bool coro(settingsInfo* info);

    void WallSettingsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            //UIUtils::SetupViewController(this);
            get_gameObject()->AddComponent<Touchable*>();
            
            UIUtils::AddHeader(get_transform(), "Wall Settings", Color::get_magenta());

            GameObject* container = CreateScrollableSettingsContainer(get_transform());

            ExternalComponents* externalComponents = container->GetComponent<ExternalComponents*>();
            RectTransform* scrollTransform = externalComponents->Get<RectTransform*>();
            scrollTransform->set_sizeDelta(UnityEngine::Vector2(0.0f, 0.0f));

            StartCoroutine(reinterpret_cast<System::Collections::IEnumerator*>(custom_types::Helpers::CoroutineHelper::New(SettingsSetupRoutine(container))));   
        }
    }

    void WallSettingsViewController::Init(WallPreviewViewController* previewViewController)
    {
        this->previewViewController = previewViewController;
        set_enabled(false);
    }

    custom_types::Helpers::Coroutine WallSettingsViewController::SettingsSetupRoutine(GameObject* container)
    {
        Transform* containerT = container->get_transform();
        auto disableSetting = BeatSaberUI::CreateToggle(containerT, "Complete Disable", config.wallConfig.disabled, [this](bool val){
            config.wallConfig.disabled = val;
            if (config.wallConfig.disabled) Disabling::RegisterDisablingInfo({ID, VERSION}, ItemType::wall);
            else Disabling::UnregisterDisablingInfo({ID, VERSION}, ItemType::wall);
        });
        BeatSaberUI::AddHoverHint(disableSetting->get_gameObject(), "Completely disables all things Qosmetics does with walls");
        co_yield nullptr;
        // fake glow
        Toggle* fakeGlowToggle = BeatSaberUI::CreateToggle(containerT, "Force Fake Glow Off", config.wallConfig.forceFakeGlowOff, [&](bool value) { 
                config.wallConfig.forceFakeGlowOff = value;
                previewViewController->UpdatePreview();
                SaveConfig();
            });
        BeatSaberUI::AddHoverHint(fakeGlowToggle->get_gameObject(), "Whether to turn off the Fake glow at all times");
        co_yield nullptr;

        // core
        Toggle* coreToggle = BeatSaberUI::CreateToggle(containerT, "Force Core Off", config.wallConfig.forceCoreOff, [&](bool value) { 
                config.wallConfig.forceCoreOff = value;
                previewViewController->UpdatePreview();
                SaveConfig();
            });
        BeatSaberUI::AddHoverHint(coreToggle->get_gameObject(), "Whether to turn off the Wall Core at all times");
        co_yield nullptr;

        // frame
        Toggle* frameToggle = BeatSaberUI::CreateToggle(containerT, "Force Frame Off", config.wallConfig.forceFrameOff, [&](bool value) { 
                config.wallConfig.forceFrameOff = value;
                previewViewController->UpdatePreview();
                SaveConfig();
            });
        BeatSaberUI::AddHoverHint(frameToggle->get_gameObject(), "Whether to turn off the Wall Frame at all times");
        co_yield nullptr;

        // frame
        Toggle* reflectionsToggle = BeatSaberUI::CreateToggle(containerT, "Disable Reflections", config.wallConfig.forceFrameOff, [&](bool value) { 
                config.wallConfig.disableReflections = value;
                SaveConfig();
            });
        BeatSaberUI::AddHoverHint(reflectionsToggle->get_gameObject(), "Whether to make base game walls be reflected");
        co_return;
    }
}