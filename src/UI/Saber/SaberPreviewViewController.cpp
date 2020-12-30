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

#define INFO(value...) UILogger::GetLogger().info(value)
extern config_t config;
DEFINE_CLASS(Qosmetics::SaberPreviewViewController);

using namespace QuestUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace UnityEngine::Events;
using namespace HMUI;

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

    void SaberPreviewViewController::UpdatePreview()
    {
        if (QuestSaber::GetActiveSaber())
        {
            if (previewprefab) 
            {
                Object::Destroy(previewprefab);
                previewprefab = nullptr;
            }
            SaberData& selected = *QuestSaber::GetActiveSaber();
            Descriptor& saberDescriptor = *selected.saberDescriptor;
            selected.FindPrefab();
            GameObject* prefab = selected.get_saberPrefab();
            
            if (!prefab)
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
                    if (previewController) previewController->UpdatePreview();
                    else INFO("Couldn't find preview controller");
                });
                waitForLoadedPrefab.detach();
                return;
            }

            if (!prefab) return;
            
            std::string name = saberDescriptor.get_name();
            if (name == "")
            {
                name = saberDescriptor.get_fileName();
                if (name != "" && name.find(".") != std::string::npos) name.erase(name.find_last_of("."));
            }
            title->set_text(il2cpp_utils::createcsstr(name));

            previewprefab = Object::Instantiate(prefab);
            previewprefab->SetActive(true);
            previewprefab->get_transform()->set_localPosition(UnityEngine::Vector3(2.1f, 1.2f, 1.1f));
            previewprefab->get_transform()->set_localEulerAngles(UnityEngine::Vector3(0.0f, 150.0f, 0.0f));
            previewprefab->get_transform()->set_localScale(UnityEngine::Vector3::get_one());
            
            Transform* leftSaber = previewprefab->get_transform()->Find(il2cpp_utils::createcsstr("LeftSaber"));
            Transform* rightSaber = previewprefab->get_transform()->Find(il2cpp_utils::createcsstr("RightSaber"));

            leftSaber->set_localScale(UnityEngine::Vector3(config.saberConfig.saberWidth, config.saberConfig.saberWidth, 1.0f));
            rightSaber->set_localScale(UnityEngine::Vector3(config.saberConfig.saberWidth, config.saberConfig.saberWidth, 1.0f));
            
            leftSaber->set_localPosition(UnityEngine::Vector3(0.0f, 0.25f, 0.4f));
            rightSaber->set_localPosition(UnityEngine::Vector3(0.0f, -0.25f, 0.4f));

            leftSaber->set_localEulerAngles(UnityEngine::Vector3::get_up() * 180.0f);
            rightSaber->set_localEulerAngles(UnityEngine::Vector3::get_up() * 180.0f);

            SaberUtils::SetCustomColor(leftSaber, 0);
            SaberUtils::SetCustomColor(rightSaber, 1);
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
        }
    }
}