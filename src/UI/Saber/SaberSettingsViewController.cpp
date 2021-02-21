#include "UI/Saber/SaberSettingsViewController.hpp"

DEFINE_CLASS(Qosmetics::UI::SaberSettingsViewController);

#include "HMUI/Touchable.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/ExternalComponents.hpp"
#include "questui/shared/CustomTypes/Data/CustomDataType.hpp"

#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/Events/UnityAction_1.hpp"
#include "UnityEngine/WaitUntil.hpp"
#include "System/Collections/IEnumerator.hpp"
#include "System/Func_1.hpp"

#include "Config.hpp"
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

static std::vector<Il2CppString*> trailText = {};

extern config_t config;

namespace Qosmetics::UI
{
    struct settingsInfo {
        int counter;
        GameObject* container;
        SaberSettingsViewController* self;
        settingsInfo(GameObject* container, SaberSettingsViewController* self) : container(container), self(self), counter(0) {}
    };

    struct enumInfo {
        SaberSettingsViewController* view;
        IncrementSetting* self;

        enumInfo(SaberSettingsViewController* view, IncrementSetting* self) : view(view), self(self) {}
    };

    bool coro(settingsInfo* info);

    void SaberSettingsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            get_gameObject()->AddComponent<Touchable*>();
            
            UIUtils::AddHeader(get_transform(), "Saber Settings", Color::get_red());

            GameObject* container = CreateScrollableSettingsContainer(get_transform());

            ExternalComponents* externalComponents = container->GetComponent<ExternalComponents*>();
            RectTransform* scrollTransform = externalComponents->Get<RectTransform*>();
            scrollTransform->set_sizeDelta(UnityEngine::Vector2(0.0f, 0.0f));

            CustomDataType* wrapper = CRASH_UNLESS(il2cpp_utils::New<CustomDataType*, il2cpp_utils::CreationType::Manual>());
            wrapper->data = new settingsInfo(container, this);

            auto coroutine = WaitUntil::New_ctor(il2cpp_utils::MakeDelegate<System::Func_1<bool>*>(classof(System::Func_1<bool>*), wrapper, 
                +[](CustomDataType* data){
                    settingsInfo* info = (settingsInfo*)data->data;
                    if (coro(info)) 
                    {
                        free(info);
                        free(data);
                        return true;
                    }
                    return false;
                }));
            StartCoroutine(reinterpret_cast<System::Collections::IEnumerator*>(coroutine));   
        }
    }

    void SaberSettingsViewController::Init(SaberPreviewViewController* previewViewController)
    {
        this->previewViewController = previewViewController;
    }

    bool coro(settingsInfo* info)
    {
        GameObject* container = info->container;
        IncrementSetting* incrSetting;
        IncrementSetting* lastIncrSettting = nullptr;
        Toggle* toggleSetting;
        CustomDataType* wrapper;
        SaberSettingsViewController* view = info->self;
        switch (info->counter)
        {
            case 0: // saber width
                incrSetting = BeatSaberUI::CreateIncrementSetting(container->get_transform(), "Saber Width", 2, 0.05f, config.saberConfig.saberWidth, 0.0f, 10.0f, [view](float value) {
                        config.saberConfig.saberWidth = value;
                        view->previewViewController->UpdatePreview();
                        SaveConfig();
                    });
                BeatSaberUI::AddHoverHint(incrSetting->get_gameObject(), "Makes the saber more or less thick");
                break;
            case 1: // override trail length
                BeatSaberUI::CreateToggle(container->get_transform(), "Override Trail Length", config.saberConfig.overrideTrailLength, [view](bool value) { 
                        config.saberConfig.overrideTrailLength = value;
                        view->previewViewController->UpdatePreview();
                        SaveConfig();
                    });
                break;
            case 2: // trail length
                incrSetting = BeatSaberUI::CreateIncrementSetting(container->get_transform(), "Trail Length", 0, 1.0f, config.saberConfig.trailLength, 0.0f, 1000.0f, [view](float value) {
                        config.saberConfig.trailLength = value;
                        view->previewViewController->UpdatePreview();
                        SaveConfig();
                    });
                BeatSaberUI::AddHoverHint(incrSetting->get_gameObject(), "The overridden length of the trail");
                break;
            case 3: // white trails
                toggleSetting = BeatSaberUI::CreateToggle(container->get_transform(), "White Trail", config.saberConfig.whiteTrail, [view](bool value) { 
                        config.saberConfig.whiteTrail = value;
                        view->previewViewController->UpdatePreview();
                        SaveConfig();
                    });
                BeatSaberUI::AddHoverHint(toggleSetting->get_gameObject(), "Makes the Trail completely white, if you turn it back off the next menu/song load the color will be correct again");
                break;
            case 4: // override whitestep
                BeatSaberUI::CreateToggle(container->get_transform(), "Override White Step Value", config.saberConfig.overrideWhiteStep, [view](bool value) { 
                        config.saberConfig.overrideWhiteStep = value;
                        view->previewViewController->UpdatePreview();
                        SaveConfig();
                    });
                break;
            case 5:
                incrSetting = BeatSaberUI::CreateIncrementSetting(container->get_transform(), "White Step Amount", 2, 0.05f, config.saberConfig.whiteStep, 0.0f, 1.00f, [view](float value) {
                        config.saberConfig.whiteStep = value;
                        view->previewViewController->UpdatePreview();
                        SaveConfig();
                    });
                BeatSaberUI::AddHoverHint(incrSetting->get_gameObject(), "Override the duration of the \"white\" part of the saber trail, set to 0 for off");
                break;
            case 6:
                BeatSaberUI::CreateToggle(container->get_transform(), "Override Trail Width Value", config.saberConfig.overrideTrailWidth, [view](bool value) { 
                        config.saberConfig.overrideTrailWidth = value;
                        view->previewViewController->UpdatePreview();
                        SaveConfig();
                    });
                break;
            case 7:
                incrSetting = BeatSaberUI::CreateIncrementSetting(container->get_transform(), "Trail Width", 2, 0.05f, config.saberConfig.trailWidth, 0.0f, 1.0f, [view](float value) {
                        config.saberConfig.trailWidth = value;
                        view->previewViewController->UpdatePreview();
                        SaveConfig();
                    });
                BeatSaberUI::AddHoverHint(incrSetting->get_gameObject(), "Shrinks each trail towards its top");
                break;
            case 8:
                #warning change implemented message of enable menu pointer
                BeatSaberUI::CreateToggle(container->get_transform(), "Enable Menu Pointer (Not implemented)", config.saberConfig.enableMenuPointer, [view](bool value) { 
                        config.saberConfig.enableMenuPointer = value;
                        view->previewViewController->UpdatePreview();
                        SaveConfig();
                    });
                break;
            case 9:
                incrSetting = BeatSaberUI::CreateIncrementSetting(container->get_transform(), "Menu Pointer Size (not implemented)", 2, 0.05f, config.saberConfig.menuPointerSize, 0.0f, 10.0f, [view](float value) {
                        config.saberConfig.menuPointerSize = value;
                        view->previewViewController->UpdatePreview();
                        SaveConfig();
                    });
                BeatSaberUI::AddHoverHint(incrSetting->get_gameObject(), "Size of the menu pointer");
                break;
            case 10:
                trailText.push_back(il2cpp_utils::createcsstr("<color=#0066ff>Custom</color>", il2cpp_utils::StringType::Manual));
                trailText.push_back(il2cpp_utils::createcsstr("<color=#33ff66>Default</color>", il2cpp_utils::StringType::Manual));
                trailText.push_back(il2cpp_utils::createcsstr("<color=#ff5555>None</color>", il2cpp_utils::StringType::Manual));
                break;
            case 11:
                lastIncrSettting = BeatSaberUI::CreateIncrementSetting(container->get_transform(), "Trail Type", 0, 1.0f, (int)config.saberConfig.trailType, [lastIncrSettting, view](float value){
                        IncrementSetting* self = lastIncrSettting;
                        int val = (int)value;
                        if (val < 0) 
                        {
                            self->CurrentValue = 2.0f;
                            val = 2;
                        }
                        val %= 3;
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
                        self->Text->SetText(trailText[val]);
                        view->previewViewController->UpdatePreview();
                        SaveConfig();
                    });
                lastIncrSettting->Text->SetText(trailText[(int)config.saberConfig.trailType]);
                BeatSaberUI::AddHoverHint(lastIncrSettting->get_gameObject(), "Sets the trail type you want to use");
                break;
            default:
                return true;
        }
        info->counter++;
        return false;
    }
    
}