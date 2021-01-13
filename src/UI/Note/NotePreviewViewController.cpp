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
#include "UnityEngine/SkinnedMeshRenderer.hpp"
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
        title = get_transform()->Find(il2cpp_utils::createcsstr("QuestUIVerticalLayoutGroup/QuestUIText"))->get_gameObject()->GetComponent<TMPro::TextMeshProUGUI*>();
        UpdatePreview();
    }

    void NotePreviewViewController::UpdatePreview()
    {
        /*
        INFO("Found");
        INFO("%s", to_utf8(csstrtostr(get_gameObject()->get_name())).c_str());
        for (int i = 0; i < get_transform()->get_childCount(); i++)
        {
            Transform* child1 = get_transform()->GetChild(i);
            std::string name1 = to_utf8(csstrtostr(child1->get_gameObject()->get_name()));
            INFO("\t%s", name1.c_str());
            for (int j = 0; child1->get_childCount(); j++)
            {
                Transform* child2 = child1->GetChild(j);
                std::string name2 = to_utf8(csstrtostr(child2->get_gameObject()->get_name()));
                INFO("\t\t%s", name2.c_str());
                for (int k = 0; child2->get_childCount(); k++)
                {
                    Transform* child3 = child2->GetChild(k);
                    std::string name3 = to_utf8(csstrtostr(child3->get_gameObject()->get_name()));
                    INFO("\t\t\t%s", name3.c_str());
                    for (int q = 0; child2->get_childCount(); q++)
                    {
                        Transform* child4 = child3->GetChild(q);
                        std::string name4 = to_utf8(csstrtostr(child4->get_gameObject()->get_name()));
                        INFO("\t\t\t\t%s", name4.c_str());

                    }
                }
            }
        }
        */

        if (QuestNote::GetActiveNote())
        {
            if (QuestNote::DidSelectDifferentNote() && previewprefab) 
            {
                Object::Destroy(previewprefab);
                previewprefab = nullptr;
            }
            NoteData& selected = *QuestNote::GetActiveNote();
            Descriptor& noteDescriptor = *selected.get_descriptor();
            bool loadComplete = selected.get_complete();
            if (!loadComplete)
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
            if (!loadComplete) return;

            selected.FindPrefab();
            if (QuestNote::DidSelectDifferentNote() || !previewprefab)
            {
                std::string name = noteDescriptor.get_name();
                if (name == "")
                {
                    name = noteDescriptor.get_fileName();
                    if (name != "" && name.find(".") != std::string::npos) name.erase(name.find_last_of("."));
                }
                title->set_text(il2cpp_utils::createcsstr(name));

                GameObject* prefab = selected.get_notePrefab();
                if (!prefab) return;
                previewprefab = Object::Instantiate(prefab, get_transform());
                previewprefab->SetActive(true);
            }

            //previewprefab->get_transform()->set_localPosition(UnityEngine::Vector3(2.1f, 1.2f, 1.1f));
            previewprefab->get_transform()->set_localPosition(UnityEngine::Vector3(-30.0f, 0.0f, -75.0f));
            previewprefab->get_transform()->set_localEulerAngles(UnityEngine::Vector3(0.0f, 60.0f, 0.0f));
            previewprefab->get_transform()->set_localScale(UnityEngine::Vector3::get_one() * 10.0f);
            
            float distance = 1.5f * ((config.noteConfig.noteSize > 1.0f) ? config.noteConfig.noteSize : 1.0f);        
            // set the child positions for a nice preview of all of them
            for (int i = 0; i < previewprefab->get_transform()->get_childCount(); i++)
            {
                Transform* child = previewprefab->get_transform()->GetChild(i);
                std::string name = to_utf8(csstrtostr(child->get_gameObject()->get_name()));

                if (noteNameToNumber.find(name) == noteNameToNumber.end()) continue;

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
                        child->get_gameObject()->SetActive(false);
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
            }
            title->set_text(il2cpp_utils::createcsstr("Default bloqs (no preview)"));
        }
    }
}