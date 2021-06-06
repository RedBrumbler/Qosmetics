#include "Config.hpp"
#include "UI/Note/NotePreviewViewController.hpp"
#include "UI/Note/NotePreviewElement.hpp"
#include "questui/shared/BeatSaberUI.hpp" 
#include "HMUI/Touchable.hpp"

#include "TMPro/TextMeshProUGUI.hpp"
#include "Types/Note/NoteItem.hpp"
#include "Utils/UIUtils.hpp"
#include "Utils/UnityUtils.hpp"
#include "Utils/TextUtils.hpp"
#include "Utils/DateUtils.hpp"

#include "UnityEngine/GameObject.hpp"
#include "QosmeticsLogger.hpp"
#include "HMUI/ImageView.hpp"
#include "HMUI/CurvedCanvasSettingsHelper.hpp"

#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"

using namespace HMUI;
using namespace UnityEngine;
using namespace UnityEngine::Events;
using namespace UnityEngine::UI;
using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;
using namespace Qosmetics;
using namespace Qosmetics::UI;
using namespace TMPro;

#define INFO(value...) QosmeticsLogger::GetContextLogger("Note Preview").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("Note Preview").error(value)

DEFINE_TYPE(Qosmetics::UI::NotePreviewViewController);

using namespace Qosmetics;
using namespace UnityEngine;

namespace Qosmetics::UI
{
    void NotePreviewViewController::DidDeactivate(bool removedFromHierarchy, bool screenSystemDisabling)
    {
        NotePreviewElement* previewElement = GetComponentInChildren<NotePreviewElement*>();
        if (previewElement) previewElement->ClearPreview();
    }

    void NotePreviewViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            //UIUtils::SetupViewController(this);
            get_gameObject()->AddComponent<Touchable*>();
            title = UIUtils::AddHeader(get_transform(), "NoteNameHere", Color::get_blue());

            GameObject* preview = GameObject::New_ctor();
            preview->get_transform()->SetParent(get_transform());
            NotePreviewElement* previewElement = preview->AddComponent<NotePreviewElement*>();
            previewElement->Init(modelManager, colorManager);

            //GameObject* container = CreateScrollableSettingsContainer(get_transform());
            VerticalLayoutGroup* layout = CreateVerticalLayoutGroup(get_transform());
            LayoutElement* layoutelem = UnityUtils::GetAddComponent<LayoutElement*>(layout->get_gameObject());
            layoutelem->set_preferredWidth(100.0f);

            Backgroundable* bg = UnityUtils::GetAddComponent<Backgroundable*>(layout->get_gameObject());
            bg->ApplyBackgroundWithAlpha(il2cpp_utils::createcsstr("title-gradient"), 1.0f);

            ImageView* imageView = bg->get_gameObject()->GetComponent<ImageView*>();
            imageView->gradient = true;
            imageView->gradientDirection = 1;
            imageView->set_color(Color::get_white());
            Color color = Color::get_black();
            color.a = 0.3f;
            imageView->set_color0(color);
            color.a = 0.7f;
            imageView->set_color1(color);
            imageView->curvedCanvasSettingsHelper->Reset();
        }
        
        UpdatePreview(masterConfig.saberConfigRedo);
        masterConfig.saberConfigRedo = false;
    }

    void NotePreviewViewController::Init(NoteManager* noteManager, ColorManager* colorManager)
    {
        this->modelManager = noteManager;
        this->colorManager = colorManager;
    }

    void NotePreviewViewController::ShowLoading()
    {   
        TextMeshProUGUI* title = GetComponentInChildren<TextMeshProUGUI*>();
        NotePreviewElement* previewElement = GetComponentInChildren<NotePreviewElement*>();
        title->set_text(il2cpp_utils::createcsstr("<i>Loading Note...</i>"));
        previewElement->ClearPreview();
    }

    void NotePreviewViewController::UpdatePreview(bool reinstantiate)
    {
        INFO("Updating preview");
        if (!modelManager)
        {
            ERROR("model Manager was nullptr, returning!");
            return;
        }
        
        NotePreviewElement* previewElement = GetComponentInChildren<NotePreviewElement*>();
        NoteItem& item = modelManager->get_item();

        if (item.get_descriptor().isValid())
        {
            std::string itemName = DateUtils::get_isMonth(6) ? "<i>" + TextUtils::rainbowify(item.get_descriptor().get_name()) + "</i>" : "<i>" + item.get_descriptor().get_name() + "</i>";
            title->set_text(il2cpp_utils::createcsstr(itemName));
            previewElement->ClearPreview();
            previewElement->UpdatePreview(reinstantiate);
        }
        else // default wall
        {
            std::string itemName = DateUtils::get_isMonth(6) ? "<i>" + TextUtils::rainbowify("Default Notes (no preview)") + "</i>" : "<i>Default Notes (no preview)</i>";
            title->set_text(il2cpp_utils::createcsstr(itemName));
            previewElement->ClearPreview();
        }
    }
}