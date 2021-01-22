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

#include "Qosmetic/QuestNote.hpp"

#include "Logging/UILogger.hpp"
#include "UI/Note/NotePreviewViewController.hpp"

#define INFO(value...) UILogger::GetLogger().WithContext("Note Settings").info(value)
#define ERROR(value...) UILogger::GetLogger().WithContext("Note Settings").error(value)

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
        SaveConfig();
    }

    void NoteSettingsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            get_gameObject()->AddComponent<Touchable*>();
            GameObject* container = BeatSaberUI::CreateScrollableSettingsContainer(get_transform());
            
            ExternalComponents* externalComponents = container->GetComponent<ExternalComponents*>();
            RectTransform* scrollTransform = externalComponents->Get<RectTransform*>();
            scrollTransform->set_sizeDelta(UnityEngine::Vector2(0.0f, 0.0f));
            
            BeatSaberUI::CreateToggle(container->get_transform(), "Override Note Size", config.noteConfig.overrideNoteSize, il2cpp_utils::MakeDelegate<UnityAction_1<bool>*>(classof(UnityAction_1<bool>*), this, +[](NoteSettingsViewController* view, bool value) { 
                    config.noteConfig.overrideNoteSize = value;
                    SaveConfig();
                    QuestNote::SelectionDefinitive();
                    NotePreviewViewController* previewController = Object::FindObjectOfType<NotePreviewViewController*>();//
                    if (previewController) previewController->UpdatePreview();
                    else ERROR("Couldn't find preview controller");
                }));
            BeatSaberUI::AddHoverHint(noteSizeToggle->get_gameObject(), "The overridden size of the notes compared to default, set to 1.00 for default size");
            QuestUI::IncrementSetting* noteSizeSetting = BeatSaberUI::CreateIncrementSetting(container->get_transform(), "Note Size", 2, 0.05f, config.noteConfig.noteSize, 0.0f, 10.0f, il2cpp_utils::MakeDelegate<UnityAction_1<float>*>(classof(UnityAction_1<float>*), this, +[](NoteSettingsViewController* view, float value) {
                    if (value >= 0.0f) config.noteConfig.noteSize = value;
                    SaveConfig();
                    QuestNote::SelectionDefinitive();
                    NotePreviewViewController* previewController = Object::FindObjectOfType<NotePreviewViewController*>();//
                    if (previewController) previewController->UpdatePreview();
                    else ERROR("Couldn't find preview controller");
                }));
            BeatSaberUI::AddHoverHint(noteSizeSetting->get_gameObject(), "The overridden size of the notes compared to default, set to 1.00 for default size");
            UI::Toggle* hitboxSize = BeatSaberUI::CreateToggle(container->get_transform(), "Change Hitbox Sizes", config.noteConfig.alsoChangeHitboxes, il2cpp_utils::MakeDelegate<UnityAction_1<bool>*>(classof(UnityAction_1<bool>*), this, +[](NoteSettingsViewController* view, bool value) { 
                    config.noteConfig.alsoChangeHitboxes = value;
                    SaveConfig();
                }));
            BeatSaberUI::AddHoverHint(hitboxSize->get_gameObject(), "Whether or not to let the hitbox size change along with the note size, turning this on will disable scores");
            BeatSaberUI::CreateToggle(container->get_transform(), "Force Default Bombs", config.noteConfig.forceDefaultBombs, il2cpp_utils::MakeDelegate<UnityAction_1<bool>*>(classof(UnityAction_1<bool>*), this, +[](NoteSettingsViewController* view, bool value) { 
                    config.noteConfig.forceDefaultBombs = value;
                    SaveConfig();
                    QuestNote::SelectionDefinitive();
                    NotePreviewViewController* previewController = Object::FindObjectOfType<NotePreviewViewController*>();//
                    if (previewController) previewController->UpdatePreview();
                    else ERROR("Couldn't find preview controller");
                }));
            BeatSaberUI::CreateToggle(container->get_transform(), "Force Default Debris", config.noteConfig.forceDefaultDebris, il2cpp_utils::MakeDelegate<UnityAction_1<bool>*>(classof(UnityAction_1<bool>*), this, +[](NoteSettingsViewController* view, bool value) { 
                    config.noteConfig.forceDefaultDebris = value;
                    SaveConfig();
                    QuestNote::SelectionDefinitive();
                    NotePreviewViewController* previewController = Object::FindObjectOfType<NotePreviewViewController*>();//
                    if (previewController) previewController->UpdatePreview();
                    else ERROR("Couldn't find preview controller");
                }));
        }
    }
}