#include "UI/Saber/SaberPreviewViewController.hpp"
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
#include "UnityEngine/XR/XRNode.hpp"
#include "GlobalNamespace/VRController.hpp"

#include "HMUI/ScrollView.hpp"
#include "HMUI/ModalView.hpp"
#include "HMUI/Touchable.hpp"
#include "HMUI/InputFieldView.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/ExternalComponents.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"

#include "Logging/UILogger.hpp"

#include "Data/SaberData.hpp"
#include "Qosmetic/QuestSaber.hpp"
#include "Utils/SaberUtils.hpp"

#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/Shader.hpp"
#include "UnityEngine/Vector4.hpp"

#include <map>
#define INFO(value...) UILogger::GetLogger().WithContext("Saber Preview").info(value)
#define ERROR(value...) UILogger::GetLogger().WithContext("Saber Preview").error(value)
extern config_t config;
DEFINE_CLASS(Qosmetics::SaberPreviewViewController);

using namespace QuestUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace UnityEngine::Events;
using namespace HMUI;

std::map<std::string, int> saberNameToNumber = {
    {
        "LeftSaber",
        0
    },
    {
        "RightSaber",
        1
    }
};


namespace Qosmetics
{
    void SaberPreviewViewController::DidDeactivate(bool removedFromHierarchy, bool screenSystemDisabling)
    {
        if (previewprefab) 
        {
            Object::Destroy(previewprefab);
            previewprefab = nullptr;
        }
    }

    void SaberPreviewViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
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

    void SaberPreviewViewController::Update()
    {
        if (updateView)
        {
            updateView = false;
            UpdatePreview();
        }
    }

    void SaberPreviewViewController::UpdatePreview()
    {
        if (QuestSaber::GetActiveSaber())
        {
            if (QuestSaber::DidSelectDifferentSaber() && previewprefab) 
            {
                Object::Destroy(previewprefab);
                previewprefab = nullptr;
            }
            SaberData& selected = *QuestSaber::GetActiveSaber();
            Descriptor& saberDescriptor = *selected.saberDescriptor;
            bool loadComplete = selected.get_complete();
            if (!loadComplete)
            {
                title->set_text(il2cpp_utils::createcsstr("Loading .qsaber File"));
                std::thread waitForLoadedPrefab([]{
                    bool complete = false;
                    while (!complete)
                    {
                        if (QuestSaber::GetActiveSaber())
                        {
                            complete = QuestSaber::GetActiveSaber()->get_complete();
                        }
                        usleep(1000);
                    }
                    SaberPreviewViewController* previewController = Object::FindObjectOfType<SaberPreviewViewController*>();//
                    if (previewController) previewController->updateView = true;
                    else ERROR("Couldn't find preview controller");
                });
                waitForLoadedPrefab.detach();
                return;
            }
            if (!loadComplete) return;

            selected.FindPrefab();
            if (QuestSaber::DidSelectDifferentSaber() || !previewprefab)
            {
                std::string name = saberDescriptor.get_name();
                if (name == "")
                {
                    name = saberDescriptor.get_fileName();
                    if (name != "" && name.find(".") != std::string::npos) name.erase(name.find_last_of("."));
                }
                title->set_text(il2cpp_utils::createcsstr(name));
                
                GameObject* prefab = selected.get_saberPrefab();
                if (!prefab) return;

                previewprefab = Object::Instantiate(prefab, get_transform());
                previewprefab->SetActive(true);
            }

            previewprefab->get_transform()->set_localPosition(UnityEngine::Vector3(-30.0f, 0.0f, -75.0f));
            previewprefab->get_transform()->set_localEulerAngles(UnityEngine::Vector3(0.0f, 150.0f, 0.0f));
            previewprefab->get_transform()->set_localScale(UnityEngine::Vector3::get_one() * 50.0f);
        
            for (int i = 0; i < previewprefab->get_transform()->get_childCount(); i++)
            {
                Transform* child = previewprefab->get_transform()->GetChild(i);
                std::string name = to_utf8(csstrtostr(child->get_gameObject()->get_name()));

                if (saberNameToNumber.find(name) == saberNameToNumber.end()) continue;
                switch(saberNameToNumber[name])
                {
                    case 0: // left saber
                        child->set_localScale(UnityEngine::Vector3(config.saberConfig.saberWidth, config.saberConfig.saberWidth, 1.0f));
                        child->set_localPosition(UnityEngine::Vector3(0.0f, 0.25f, 0.4f));
                        child->set_localEulerAngles(UnityEngine::Vector3::get_up() * 180.0f);
                        SaberUtils::SetCustomColor(child, 0);
                        break;
                    case 1: // right saber
                        child->set_localScale(UnityEngine::Vector3(config.saberConfig.saberWidth, config.saberConfig.saberWidth, 1.0f));
                        child->set_localPosition(UnityEngine::Vector3(0.0f, -0.25f, 0.4f));
                        child->set_localEulerAngles(UnityEngine::Vector3::get_up() * 180.0f);
                        SaberUtils::SetCustomColor(child, 1);
                        break;
                    default:
                        Object::Destroy(child->get_gameObject());
                        break;
                }
            }

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
                    UnityEngine::Transform* distantParent = parent3 ? parent3 : nullptr;
                    distantParentName = distantParent ? to_utf8(csstrtostr(distantParent->get_gameObject()->get_name())) : "null";
                    if (distantParentName.find("Multiplayer") != std::string::npos ||
                        //distantParentName.find("LocalPlayerGameCore") != std::string::npos ||
                        distantParentName.find("IsActive") != std::string::npos) continue;
                        SaberUtils::RevertMenuPointer(VRControllers->values[i]->get_transform(), node);
                        if (config.saberConfig.enableMenuPointer)
                        {
                            if (QuestSaber::DidSelectDifferentSaber()) 
                            {
                                SaberUtils::RevertMenuPointer(VRControllers->values[i]->get_transform(), node);
                                Qosmetics::QuestSaber::ReplaceMenuPointers(VRControllers->values[i]->get_transform(), node);
                            }
                            else Qosmetics::QuestSaber::UpdateMenuPointers(VRControllers->values[i]->get_transform(), node);
                        }
                        //else SaberUtils::RevertMenuPointer(VRControllers->values[i]->get_transform(), node);
                }
            }
        }
        else 
        {
            INFO("there is no active saber configured");
            if (previewprefab) 
            {
                Object::Destroy(previewprefab);
                previewprefab = nullptr;
            }
            title->set_text(il2cpp_utils::createcsstr("Default sabers (no preview)"));

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
                    SaberUtils::RevertMenuPointer(VRControllers->values[i]->get_transform(), node);
                    continue;
                    UnityEngine::Transform* parent1 = VRControllers->values[i]->get_transform()->get_parent();
                    UnityEngine::Transform* parent2 = parent1 ? parent1->get_parent() : nullptr;
                    UnityEngine::Transform* parent3 = parent2 ? parent2->get_parent() : nullptr;
                    UnityEngine::Transform* distantParent = parent3 ? parent3 : nullptr;
                    distantParentName = distantParent ? to_utf8(csstrtostr(distantParent->get_gameObject()->get_name())) : "null";
                    if (distantParentName.find("Multiplayer") != std::string::npos ||
                        //distantParentName.find("LocalPlayerGameCore") != std::string::npos ||
                        distantParentName.find("IsActive") != std::string::npos) continue;
                        SaberUtils::RevertMenuPointer(VRControllers->values[i]->get_transform(), node);
                }
            }

        }
    }
}