#include "UI/Saber/SaberPreviewViewController.hpp"
#include "UI/Saber/SaberPreviewElement.hpp"
#include "questui/shared/BeatSaberUI.hpp" 
#include "HMUI/Touchable.hpp"

#include "TMPro/TextMeshProUGUI.hpp"
#include "Types/Saber/SaberItem.hpp"
#include "Utils/UIUtils.hpp"

#include "UnityEngine/GameObject.hpp"

DEFINE_CLASS(Qosmetics::UI::SaberPreviewViewController);

using namespace HMUI;
using namespace UnityEngine;
using namespace UnityEngine::Events;
using namespace UnityEngine::UI;
using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;
using namespace Qosmetics;
using namespace Qosmetics::UI;
using namespace TMPro;

namespace Qosmetics::UI
{
    void SaberPreviewViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            get_gameObject()->AddComponent<Touchable*>();
            UIUtils::AddHeader(get_transform(), "SaberNameHere", Color::get_red());

            GameObject* preview = GameObject::New_ctor();
            preview->get_transform()->SetParent(get_transform());
            SaberPreviewElement* previewElement = preview->AddComponent<SaberPreviewElement*>();
            previewElement->Init(modelManager, colorManager);

            GameObject* container = CreateScrollableSettingsContainer(get_transform());
            CreateText(container->get_transform(), "bitch");
        }
    }

    void SaberPreviewViewController::Init(SaberManager* saberManager, ColorManager* colorManager)
    {
        this->modelManager = saberManager;
        this->colorManager = colorManager;
    }

    void SaberPreviewViewController::ShowLoading()
    {   
        TextMeshProUGUI* title = GetComponentInChildren<TextMeshProUGUI*>();
        SaberPreviewElement* previewElement = GetComponentInChildren<SaberPreviewElement*>();
        title->set_text(il2cpp_utils::createcsstr("Loading Saber..."));
        previewElement->ClearPreview();
    }

    void SaberPreviewViewController::UpdatePreview()
    {
        TextMeshProUGUI* title = GetComponentInChildren<TextMeshProUGUI*>();
        SaberPreviewElement* previewElement = GetComponentInChildren<SaberPreviewElement*>();
        SaberItem& item = modelManager->get_item();

        if (item.get_descriptor().isValid())
        {
            title->set_text(il2cpp_utils::createcsstr(item.get_descriptor().get_name()));
            previewElement->UpdatePreview();
        }
        else // default saber
        {
            title->set_text(il2cpp_utils::createcsstr("Default Saber (no preview)"));
            previewElement->ClearPreview();
        }
    }
}