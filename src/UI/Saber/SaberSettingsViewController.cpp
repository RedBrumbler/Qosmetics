#include "Config.hpp"
#include "UI/Saber/SaberSettingsViewController.hpp"

DEFINE_TYPE(Qosmetics::UI, SaberSettingsViewController);

#include "HMUI/Touchable.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/ExternalComponents.hpp"
#include "questui/shared/CustomTypes/Data/CustomDataType.hpp"

#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/Events/UnityAction_1.hpp"
#include "UnityEngine/WaitUntil.hpp"
#include "System/Collections/IEnumerator.hpp"
#include "System/Func_1.hpp"

#include "Utils/UIUtils.hpp"

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
        SaberSettingsViewController* self;
        settingsInfo(GameObject* container, SaberSettingsViewController* self) : container(container), self(self), counter(0) {}
    };

    bool coro(settingsInfo* info);

    void SaberSettingsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            //UIUtils::SetupViewController(this);
            get_gameObject()->AddComponent<Touchable*>();
            
            UIUtils::AddHeader(get_transform(), "Saber Settings", Color::get_red());

            GameObject* container = CreateScrollableSettingsContainer(get_transform());

            ExternalComponents* externalComponents = container->GetComponent<ExternalComponents*>();
            RectTransform* scrollTransform = externalComponents->Get<RectTransform*>();
            scrollTransform->set_sizeDelta(UnityEngine::Vector2(0.0f, 0.0f));

            StartCoroutine(reinterpret_cast<System::Collections::IEnumerator*>(custom_types::Helpers::CoroutineHelper::New(SettingsSetupRoutine(container)))); 
        }
    }

    void SaberSettingsViewController::Init(SaberPreviewViewController* previewViewController)
    {
        this->previewViewController = previewViewController;
        set_enabled(false);
    }

    custom_types::Helpers::Coroutine SaberSettingsViewController::SettingsSetupRoutine(GameObject* container)
    {
        Transform* containerT = container->get_transform();
        auto widthSetting = BeatSaberUI::CreateIncrementSetting(containerT, "Saber Width", 2, 0.05f, config.saberConfig.saberWidth, 0.0f, 10.0f, [this](float value) {
                config.saberConfig.saberWidth = value;
                this->previewViewController->UpdatePreview();
                SaveConfig();
            });
        BeatSaberUI::AddHoverHint(widthSetting->get_gameObject(), "Makes the saber more or less thick");
        co_yield nullptr;

        BeatSaberUI::CreateToggle(containerT, "Override Trail Length", config.saberConfig.overrideTrailLength, [this](bool value) { 
                config.saberConfig.overrideTrailLength = value;
                this->previewViewController->UpdatePreview();
                SaveConfig();
            });
        co_yield nullptr;

        auto trailLengthSetting = BeatSaberUI::CreateIncrementSetting(containerT, "Trail Length", 0, 1.0f, config.saberConfig.trailLength, 1.0f, 1000.0f, [this](float value) {
                config.saberConfig.trailLength = value;
                this->previewViewController->UpdatePreview();
                SaveConfig();
            });
        BeatSaberUI::AddHoverHint(trailLengthSetting->get_gameObject(), "The overridden length of the trail");
        co_yield nullptr;

        auto toggleSetting = BeatSaberUI::CreateToggle(containerT, "White Trail", config.saberConfig.whiteTrail, [this](bool value) { 
                config.saberConfig.whiteTrail = value;
                this->previewViewController->UpdatePreview();
                SaveConfig();
            });
        BeatSaberUI::AddHoverHint(toggleSetting->get_gameObject(), "Makes the Trail completely white, if you turn it back off the next menu/song load the color will be correct again");
        co_yield nullptr;

        BeatSaberUI::CreateToggle(containerT, "Override White Step Value", config.saberConfig.overrideWhiteStep, [this](bool value) { 
                config.saberConfig.overrideWhiteStep = value;
                this->previewViewController->UpdatePreview();
                SaveConfig();
            });
        co_yield nullptr;

        auto whiteStepAmountSetting = BeatSaberUI::CreateIncrementSetting(containerT, "White Step Amount", 2, 0.05f, config.saberConfig.whiteStep, 0.0f, 1.00f, [this](float value) {
                config.saberConfig.whiteStep = value;
                this->previewViewController->UpdatePreview();
                SaveConfig();
            });
        BeatSaberUI::AddHoverHint(whiteStepAmountSetting->get_gameObject(), "Override the duration of the \"white\" part of the saber trail, set to 0 for off");
        co_yield nullptr;

        BeatSaberUI::CreateToggle(containerT, "Override Trail Width Value", config.saberConfig.overrideTrailWidth, [this](bool value) { 
                config.saberConfig.overrideTrailWidth = value;
                this->previewViewController->UpdatePreview();
                SaveConfig();
            });
        co_yield nullptr;

        auto trailWidthSetting = BeatSaberUI::CreateIncrementSetting(containerT, "Trail Width", 2, 0.05f, config.saberConfig.trailWidth, 0.0f, 1.0f, [this](float value) {
                config.saberConfig.trailWidth = value;
                this->previewViewController->UpdatePreview();
                SaveConfig();
            });
        BeatSaberUI::AddHoverHint(trailWidthSetting->get_gameObject(), "Shrinks each trail towards its top");
        co_yield nullptr;

        BeatSaberUI::CreateToggle(containerT, "Enable Menu Pointer", config.saberConfig.enableMenuPointer, [this](bool value) { 
                config.saberConfig.enableMenuPointer = value;
                this->previewViewController->UpdatePreview();
                SaveConfig();
            });
        co_yield nullptr;

        auto pointerSizeSetting = BeatSaberUI::CreateIncrementSetting(containerT, "Menu Pointer Size", 2, 0.05f, config.saberConfig.menuPointerSize, 0.0f, 10.0f, [this](float value) {
                config.saberConfig.menuPointerSize = value;
                this->previewViewController->UpdatePreview();
                SaveConfig();
            });
        BeatSaberUI::AddHoverHint(pointerSizeSetting->get_gameObject(), "Size of the menu pointer");
        co_yield nullptr;

        auto trailDropdown = BeatSaberUI::CreateDropdown(containerT, "Trail Type", trailTextValues[(int)config.saberConfig.trailType], trailTextValues, [this](auto value){
                int val = 0;
                for (auto& s : trailTextValues)
                {
                    if (s.find(value) != std::string::npos) break;
                    val++;
                }

                switch (val)
                {
                    case 0:
                        config.saberConfig.trailType = TrailType::custom;
                        break;
                    case 1:
                        config.saberConfig.trailType = TrailType::baseGame;
                        break;
                    case 2:
                        config.saberConfig.trailType = TrailType::none;
                        break;
                    default:
                        break;
                }
                this->previewViewController->UpdatePreview();
                SaveConfig();
            });
        BeatSaberUI::AddHoverHint(trailDropdown->get_gameObject(), "Sets the trail type you want to use");
    }
}