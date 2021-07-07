#include "Config.hpp"
#include "UI/Note/NotePreviewElement.hpp"

DEFINE_TYPE(Qosmetics::UI, NotePreviewElement);

#include "HMUI/Touchable.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/ExternalComponents.hpp"
#include "questui/shared/CustomTypes/Data/CustomDataType.hpp"

#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/Events/UnityAction_1.hpp"
#include "UnityEngine/WaitUntil.hpp"
#include "System/Collections/IEnumerator.hpp"
#include "System/Func_1.hpp"

#include "Utils/NoteUtils.hpp"

using namespace HMUI;
using namespace UnityEngine;
using namespace UnityEngine::Events;
using namespace UnityEngine::UI;
using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;
using namespace Qosmetics;
using namespace Qosmetics::UI;
using namespace TMPro;

#include "QosmeticsLogger.hpp"
#define INFO(value...) QosmeticsLogger::GetContextLogger("Note Preview Element").info(value);
#define ERROR(value...) QosmeticsLogger::GetContextLogger("Note Preview Element").error(value);

namespace Qosmetics::UI
{
    void NotePreviewElement::Init(NoteManager* modelManager, ColorManager* colorManager)
    {
        this->modelManager = modelManager;
        this->colorManager = colorManager;

        get_transform()->set_localScale(Vector3::get_one() * 50.0f);
        get_transform()->set_localPosition(UnityEngine::Vector3(-30.0f, 0.0f, -75.0f));
        get_transform()->set_localEulerAngles(UnityEngine::Vector3(0.0f, 330.0f, 0.0f));
    }

    void NotePreviewElement::UpdatePreview(bool reinstantiate)
    {
        INFO("Updating Preview Element");
        if (reinstantiate || !prefab)
        {
            if (!modelManager) 
            {
                ERROR("No ModelManager");
                return;
            }
            
            GameObject* prefabToInstantiate = modelManager->GetActivePrefab();
            if (!prefabToInstantiate)
            {
                ERROR("To Instantiate nullptr");
                return;
            }
            prefab = Instantiate(prefabToInstantiate, get_transform());
            prefab->SetActive(true);
        }
        
        Transform* prefabTransform = prefab->get_transform();

        //prefabTransform->set_localPosition(UnityEngine::Vector3(0.0f, 0.0f, 0.0f));
        prefabTransform->set_localPosition(UnityEngine::Vector3(0.0f, 0.0f, 0.0f));
        prefabTransform->set_localEulerAngles(UnityEngine::Vector3(0.0f, 100.0f, 0.0f));
        prefabTransform->set_localScale(UnityEngine::Vector3::get_one() * 0.25f);

        int childCount = prefabTransform->get_childCount();

        Color leftColor = colorManager->ColorForNoteType(0);
        Color rightColor = colorManager->ColorForNoteType(1);

        float distance = 1.5f * ((config.noteConfig.noteSize > 1.0f) ? config.noteConfig.noteSize : 1.0f); 

        for (int i = 0; i < childCount; i++)
        {
            Transform* child = prefabTransform->GetChild(i);

            child->set_localScale(UnityEngine::Vector3::get_one() * config.noteConfig.noteSize);

            std::string name = to_utf8(csstrtostr(child->get_gameObject()->get_name()));
            //INFO("Found Child %s", name.c_str());
            if (name == "LeftArrow")
            {
                child->set_localPosition(UnityEngine::Vector3(distance / 2.0f, distance, 0.0f));
                NoteUtils::SetColors(child->get_gameObject(), leftColor, rightColor);
            }
            else if (name == "LeftDot")
            {
                child->set_localPosition(UnityEngine::Vector3(distance / 2.0f, 0.0f, 0.0f));
                NoteUtils::SetColors(child->get_gameObject(), leftColor, rightColor);
            }
            else if (name == "LeftDebris")
            {
                child->set_localPosition(UnityEngine::Vector3(distance / 2.0f, -distance, 0.0f));
                NoteUtils::SetColors(child->get_gameObject(), leftColor, rightColor);
                child->get_gameObject()->SetActive(!config.noteConfig.forceDefaultDebris);
            }
            else if (name == "RightArrow")
            {
                child->set_localPosition(UnityEngine::Vector3(-distance / 2.0f, distance, 0.0f));
                NoteUtils::SetColors(child->get_gameObject(), rightColor, leftColor);
            }
            else if (name == "RightDot")
            {
                child->set_localPosition(UnityEngine::Vector3(-distance / 2.0f, 0.0f, 0.0f));
                NoteUtils::SetColors(child->get_gameObject(), rightColor, leftColor);
            }
            else if (name == "RightDebris")
            {
                child->set_localPosition(UnityEngine::Vector3(-distance / 2.0f, -distance, 0.0f));
                NoteUtils::SetColors(child->get_gameObject(), rightColor, leftColor);
                child->get_gameObject()->SetActive(!config.noteConfig.forceDefaultDebris);
            }
            else if (name == "Bomb")
            {
                child->set_localPosition(UnityEngine::Vector3(distance * 1.5f, 0.0f, 0.0f));
                child->get_gameObject()->SetActive(!config.noteConfig.forceDefaultBombs);
            }
            else 
            {
                INFO("Destroying %s", name.c_str());
                Object::Destroy(child);
            }
        }
    }

    void NotePreviewElement::ClearPreview()
    {
        if (prefab) Object::Destroy(prefab);
        prefab = nullptr;
    }
}