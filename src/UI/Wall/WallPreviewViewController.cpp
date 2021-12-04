#include "Config.hpp"

#include "HMUI/Touchable.hpp"
#include "UI/Wall/WallPreviewElement.hpp"
#include "UI/Wall/WallPreviewViewController.hpp"
#include "questui/shared/BeatSaberUI.hpp"

#include "TMPro/TextMeshProUGUI.hpp"
#include "Types/Wall/WallItem.hpp"
#include "Utils/DateUtils.hpp"
#include "Utils/TextUtils.hpp"
#include "Utils/UIUtils.hpp"
#include "Utils/UnityUtils.hpp"

#include "HMUI/CurvedCanvasSettingsHelper.hpp"
#include "HMUI/ImageView.hpp"
#include "QosmeticsLogger.hpp"
#include "UnityEngine/GameObject.hpp"

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

#define INFO(value...) QosmeticsLogger::GetContextLogger("Wall Preview").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("Wall Preview").error(value)

DEFINE_TYPE(Qosmetics::UI, WallPreviewViewController);

using namespace Qosmetics;
using namespace UnityEngine;

namespace Qosmetics::UI
{
    void WallPreviewViewController::DidDeactivate(bool removedFromHierarchy, bool screenSystemDisabling)
    {
        WallPreviewElement* previewElement = GetComponentInChildren<WallPreviewElement*>();
        if (previewElement)
            previewElement->ClearPreview();
    }

    void WallPreviewViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            //UIUtils::SetupViewController(this);
            get_gameObject()->AddComponent<Touchable*>();
            title = UIUtils::AddHeader(get_transform(), "WallNameHere", Color::get_magenta());

            GameObject* preview = GameObject::New_ctor();
            preview->get_transform()->SetParent(get_transform());
            WallPreviewElement* previewElement = preview->AddComponent<WallPreviewElement*>();
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

    void WallPreviewViewController::Init(WallManager* wallManager, ColorManager* colorManager)
    {
        this->modelManager = wallManager;
        this->colorManager = colorManager;
    }

    void WallPreviewViewController::ShowLoading()
    {
        TextMeshProUGUI* title = GetComponentInChildren<TextMeshProUGUI*>();
        WallPreviewElement* previewElement = GetComponentInChildren<WallPreviewElement*>();
        title->set_text(il2cpp_utils::newcsstr("<i>Loading Wall...</i>"));
        previewElement->ClearPreview();
    }

    void WallPreviewViewController::UpdatePreview(bool reinstantiate)
    {
        INFO("Updating preview");
        if (!modelManager)
        {
            ERROR("model Manager was nullptr, returning!");
            return;
        }

        WallPreviewElement* previewElement = GetComponentInChildren<WallPreviewElement*>();
        WallItem& item = modelManager->get_item();

        if (item.get_descriptor().isValid())
        {
            std::string itemName = DateUtils::get_isMonth(6) ? "<i>" + TextUtils::rainbowify(item.get_descriptor().get_name()) + "</i>" : "<i>" + item.get_descriptor().get_name() + "</i>";
            title->set_text(il2cpp_utils::newcsstr(itemName));
            previewElement->ClearPreview();
            previewElement->UpdatePreview(reinstantiate);
        }
        else // default wall
        {
            std::string itemName = DateUtils::get_isMonth(6) ? "<i>" + TextUtils::rainbowify("Default Wall (no preview)") + "</i>" : "<i>Default Wall (no preview)</i>";
            title->set_text(il2cpp_utils::newcsstr(itemName));
            previewElement->ClearPreview();
        }
    }
}