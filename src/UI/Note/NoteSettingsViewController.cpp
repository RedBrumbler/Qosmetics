#include "UI/Note/NoteSettingsViewController.hpp"
#include "config.hpp"
#include "Data/Descriptor.hpp"

#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Rect.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/UI/Image.hpp"
#include "UnityEngine/UI/Toggle.hpp"
#include "UnityEngine/UI/Toggle_ToggleEvent.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/Events/UnityAction_1.hpp"
#include "HMUI/ScrollView.hpp"
#include "HMUI/ModalView.hpp"
#include "HMUI/Touchable.hpp"
#include "HMUI/InputFieldView.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/ExternalComponents.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"

#include "Logging/UILogger.hpp"
#define INFO(value...) UILogger::GetLogger().info(value)
extern config_t config;

DEFINE_CLASS(Qosmetics::NoteSettingsViewController);

using namespace QuestUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace UnityEngine::Events;
using namespace HMUI;

namespace Qosmetics
{
    void NoteSettingsViewController::DidDeactivate(bool removedFromHierarchy, bool screenSystemDisabling)
    {

    }

    void NoteSettingsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            get_gameObject()->AddComponent<Touchable*>();
            GameObject* container = BeatSaberUI::CreateScrollableSettingsContainer(get_transform());
            BeatSaberUI::CreateToggle(container->get_transform(), "Override Note Size", config.noteConfig.overrideNoteSize, il2cpp_utils::MakeDelegate<UnityAction_1<bool>*>(classof(UnityAction_1<bool>*), this, +[](NoteSettingsViewController* view, bool value) { 
                    config.noteConfig.overrideNoteSize = value;
                    SaveConfig();
                }));
            QuestUI::IncrementSetting* trailLengthSetting = BeatSaberUI::CreateIncrementSetting(container->get_transform(), "Note Size", 2, 0.05f, config.noteConfig.noteSize, il2cpp_utils::MakeDelegate<UnityAction_1<float>*>(classof(UnityAction_1<float>*), this, +[](NoteSettingsViewController* view, float value) {
                    if (value >= 0.0f) config.noteConfig.noteSize = value;
                    SaveConfig();
                }));
            BeatSaberUI::AddHoverHint(trailLengthSetting->get_gameObject(), "The overridden length of the trail, values below 0 get ignored");
            BeatSaberUI::CreateToggle(container->get_transform(), "Force Default Bombs", config.noteConfig.forceDefaultBombs, il2cpp_utils::MakeDelegate<UnityAction_1<bool>*>(classof(UnityAction_1<bool>*), this, +[](NoteSettingsViewController* view, bool value) { 
                    config.noteConfig.forceDefaultBombs = value;
                    SaveConfig();
                }));
            BeatSaberUI::CreateToggle(container->get_transform(), "Force Default Debris", config.noteConfig.forceDefaultDebris, il2cpp_utils::MakeDelegate<UnityAction_1<bool>*>(classof(UnityAction_1<bool>*), this, +[](NoteSettingsViewController* view, bool value) { 
                    config.noteConfig.forceDefaultDebris = value;
                    SaveConfig();
                }));
        }
    }
}