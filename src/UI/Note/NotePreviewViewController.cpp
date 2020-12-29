#include "UI/Note/NotePreviewViewController.hpp"
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

#include "Utils/NoteUtils.hpp"
#include "Qosmetic/QuestNote.hpp"

#define INFO(value...) UILogger::GetLogger().info(value)
extern config_t config;
DEFINE_CLASS(Qosmetics::NotePreviewViewController);

using namespace QuestUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace UnityEngine::Events;
using namespace HMUI;

std::map<std::string, int> noteNameToNumber = {
    {
        "LeftArrow",
        0
    },
    {
        "LeftDot",
        1
    },
    {
        "RightArrow",
        2
    },
    {
        "RightDot",
        3
    },
    {
        "LeftDebris",
        4
    },
    {
        "RightDebris",
        5
    },
    {
        "Bomb",
        6
    }
};

namespace Qosmetics
{
    void NotePreviewViewController::DidDeactivate(bool removedFromHierarchy, bool screenSystemDisabling)
    {
        if (previewprefab) 
        {
            Object::Destroy(previewprefab);
            previewprefab = nullptr;
        }
    }

    void NotePreviewViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            VerticalLayoutGroup* textlayout = BeatSaberUI::CreateVerticalLayoutGroup(get_transform());

            title = BeatSaberUI::CreateText(textlayout->get_transform(), "placeholder text");
            title->set_fontSize(10.0f);
        }
        UpdatePreview();
    }

    void NotePreviewViewController::UpdatePreview()
    {
        if (QuestNote::GetActiveNote())
        {
            if (previewprefab) 
            {
                Object::Destroy(previewprefab);
                previewprefab = nullptr;
            }
            NoteData& selected = *QuestNote::GetActiveNote();
            Descriptor& noteDescriptor = *selected.get_descriptor();
            GameObject* prefab = selected.get_notePrefab();

            if (!prefab)
            {
                title->set_text(il2cpp_utils::createcsstr("Loading .qbloq File"));
                std::thread waitForLoadedPrefab([]{
                    bool complete = false;
                    while (!complete)
                    {
                        if (QuestNote::GetActiveNote())
                        {
                            complete = QuestNote::GetActiveNote()->get_complete();
                        }
                        usleep(1000);
                    }
                    NotePreviewViewController* previewController = Object::FindObjectOfType<NotePreviewViewController*>();//
                    if (previewController) previewController->UpdatePreview();
                    else INFO("Couldn't find preview controller");
                });
                waitForLoadedPrefab.detach();
                return;
            }

            if (!prefab) return;

            title->set_text(il2cpp_utils::createcsstr(noteDescriptor.get_name()));

            previewprefab = Object::Instantiate(prefab);
            previewprefab->SetActive(true);
            previewprefab->get_transform()->set_localPosition(UnityEngine::Vector3(2.1f, 1.2f, 1.1f));
            previewprefab->get_transform()->set_localEulerAngles(UnityEngine::Vector3(0.0f, 60.0f, 0.0f));
            previewprefab->get_transform()->set_localScale(UnityEngine::Vector3::get_one() * 0.2f);
            
            float distance = 1.5f * ((config.noteConfig.noteSize > 1.0f) ? config.noteConfig.noteSize : 1.0f);        
            // set the child positions for a nice preview of all of them
            for (int i = 0; i < previewprefab->get_transform()->get_childCount(); i++)
            {
                Transform* child = previewprefab->get_transform()->GetChild(i);
                std::string name = to_utf8(csstrtostr(child->get_gameObject()->get_name()));
                child->set_localScale(UnityEngine::Vector3::get_one() * config.noteConfig.noteSize);
                switch(noteNameToNumber[name])
                {
                    case 0: // left arrow
                        child->set_localPosition(UnityEngine::Vector3(-distance, distance, 0.0f));
                        NoteUtils::SetColor(child, true);
                        break;
                    case 1: // left dot
                        child->set_localPosition(UnityEngine::Vector3(-distance, 0.0f, 0.0f));
                        NoteUtils::SetColor(child, true);
                        break;
                    case 2: // right arrow
                        child->set_localPosition(UnityEngine::Vector3(0.0f, distance, 0.0f));
                        NoteUtils::SetColor(child, false);
                        break;
                    case 3: // right dot
                        child->set_localPosition(UnityEngine::Vector3(0.0f, 0.0f, 0.0f));
                        NoteUtils::SetColor(child, false);
                        break;
                    case 4: // left debris
                        child->set_localPosition(UnityEngine::Vector3(-distance, -distance, 0.0f));
                        NoteUtils::SetColor(child, true);
                        child->get_gameObject()->SetActive(!config.noteConfig.forceDefaultDebris);
                        break;
                    case 5: // right debris
                        child->set_localPosition(UnityEngine::Vector3(0.0f, -distance, 0.0f));
                        NoteUtils::SetColor(child, false);
                        child->get_gameObject()->SetActive(!config.noteConfig.forceDefaultDebris);
                        break;
                    case 6: // bomb
                        child->set_localPosition(UnityEngine::Vector3(distance, 0.0f, 0.0f));
                        child->get_gameObject()->SetActive(!config.noteConfig.forceDefaultBombs);
                        break;
                    default:
                        break;
                }
            }
            
        }
        else 
        {
            INFO("there is no active note configured");
            if (previewprefab) 
            {
                Object::Destroy(previewprefab);
                previewprefab = nullptr;
                title->set_text(il2cpp_utils::createcsstr("Default bloqs (no preview)"));
            }
        }
    }
}