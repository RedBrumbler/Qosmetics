#include "Config.hpp"

#include "HMUI/Touchable.hpp"
#include "UI/Saber/SaberPreviewElement.hpp"
#include "UI/Saber/SaberPreviewViewController.hpp"
#include "questui/shared/BeatSaberUI.hpp"

#include "TMPro/TextMeshProUGUI.hpp"
#include "Types/Pointer/Pointer.hpp"
#include "Types/Saber/SaberItem.hpp"
#include "Utils/DateUtils.hpp"
#include "Utils/TextUtils.hpp"
#include "Utils/UIUtils.hpp"
#include "Utils/UnityUtils.hpp"

#include "HMUI/CurvedCanvasSettingsHelper.hpp"
#include "HMUI/ImageView.hpp"
#include "QosmeticsLogger.hpp"
#include "UnityEngine/GameObject.hpp"

#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"

DEFINE_TYPE(Qosmetics::UI, SaberPreviewViewController);

using namespace HMUI;
using namespace UnityEngine;
using namespace UnityEngine::Events;
using namespace UnityEngine::UI;
using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;
using namespace Qosmetics;
using namespace Qosmetics::UI;
using namespace TMPro;

#define INFO(value...) QosmeticsLogger::GetContextLogger("Saber Preview").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("Saber Preview").error(value)

namespace Qosmetics::UI
{
    void SaberPreviewViewController::DidDeactivate(bool removedFromHierarchy, bool screenSystemDisabling)
    {
        SaberPreviewElement* previewElement = GetComponentInChildren<SaberPreviewElement*>();
        if (previewElement)
            previewElement->ClearPreview();
    }

    void SaberPreviewViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            //UIUtils::SetupViewController(this);
            get_gameObject()->AddComponent<Touchable*>();
            title = UIUtils::AddHeader(get_transform(), "SaberNameHere", Color::get_red());

            GameObject* preview = GameObject::New_ctor();
            preview->get_transform()->SetParent(get_transform());
            SaberPreviewElement* previewElement = preview->AddComponent<SaberPreviewElement*>();
            previewElement->Init(modelManager, colorManager);

            //GameObject* container = CreateScrollableSettingsContainer(get_transform());
            VerticalLayoutGroup* layout = CreateVerticalLayoutGroup(get_transform());
            LayoutElement* layoutelem = UnityUtils::GetAddComponent<LayoutElement*>(layout->get_gameObject());
            layoutelem->set_preferredWidth(100.0f);

            Backgroundable* bg = UnityUtils::GetAddComponent<Backgroundable*>(layout->get_gameObject());
            bg->ApplyBackgroundWithAlpha(il2cpp_utils::newcsstr("title-gradient"), 1.0f);

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

        UpdatePreview(firstActivation);
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
        title->set_text(il2cpp_utils::newcsstr("<i>Loading Saber...</i>"));
        previewElement->ClearPreview();
    }

    void SaberPreviewViewController::UpdatePreview(bool reinstantiate)
    {
        INFO("Updating preview");
        if (!modelManager)
        {
            ERROR("model Manager was nullptr, returning!");
            return;
        }

        SaberPreviewElement* previewElement = GetComponentInChildren<SaberPreviewElement*>();
        SaberItem& item = modelManager->get_item();

        if (item.get_descriptor().isValid())
        {
            std::string itemName = DateUtils::get_isMonth(6) ? "<i>" + TextUtils::rainbowify(item.get_descriptor().get_name()) + "</i>" : "<i>" + item.get_descriptor().get_name() + "</i>";
            title->set_text(il2cpp_utils::newcsstr(itemName));
            previewElement->UpdatePreview(reinstantiate);
        }
        else // default Saber
        {
            std::string itemName = DateUtils::get_isMonth(6) ? "<i>" + TextUtils::rainbowify("Default Saber (no preview)") + "</i>" : "<i>Default Saber (no preview)</i>";
            title->set_text(il2cpp_utils::newcsstr(itemName));
            previewElement->ClearPreview();
        }
        Pointer::UpdateAll(reinstantiate);
    }
}