#include "Utils/UIUtils.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"

#include "HMUI/ImageView.hpp"
#include "HMUI/Touchable.hpp"
#include "Polyglot/LocalizedTextMeshProUGUI.hpp"
#include "HMUI/CurvedTextMeshPro.hpp"
#include "HMUI/CurvedCanvasSettingsHelper.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Canvas.hpp"
#include "UnityEngine/CanvasGroup.hpp"
#include "UnityEngine/CanvasRenderer.hpp"
#include "UnityEngine/Object.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/AdditionalCanvasShaderChannels.hpp"
#include "UnityEngine/RenderMode.hpp"

#include "VRUIControls/VRGraphicRaycaster.hpp"
#include "VRUIControls/PhysicsRaycasterWithCache.hpp"

#include "QosmeticsLogger.hpp"
#include "HMUI/TitleViewController.hpp"
#include "HMUI/ButtonStaticAnimations.hpp"
#include "questui/shared/ArrayUtil.hpp"

#include "Utils/TextUtils.hpp"
#include "Utils/DateUtils.hpp"

using namespace VRUIControls;
using namespace HMUI;
using namespace UnityEngine;
using namespace UnityEngine::Events;
using namespace UnityEngine::UI;
using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;
using namespace TMPro;

#define INFO(value...) QosmeticsLogger::GetContextLogger("UI Utils").info(value);
#define ERROR(value...) QosmeticsLogger::GetContextLogger("UI Utils").error(value);

namespace UIUtils
{
    TextMeshProUGUI* AddHeader(Transform* parent, std::string title)
    {
        Color color = {0.0f, 0.75f, 1.0f, 1.0f};
        return AddHeader(parent, title, color);
    }

    TextMeshProUGUI* AddHeader(Transform* parent, std::string title, Color color)
    {
        Color otherColor = color;
        otherColor.a = 0.0f;
        return AddHeader(parent, title, color, otherColor);
    }

    TextMeshProUGUI* AddHeader(Transform* parent, std::string title, Color leftColor, Color rightColor)
    {
        if (DateUtils::get_isMonth(6)) title = TextUtils::rainbowify(title);

        VerticalLayoutGroup* vertical = CreateVerticalLayoutGroup(parent);
        vertical->get_rectTransform()->set_anchoredPosition({0.0f, 45.0f});
        HorizontalLayoutGroup* horizontal = CreateHorizontalLayoutGroup(vertical->get_transform());

        TextMeshProUGUI* text = CreateText(horizontal->get_transform(), title);
        //->get_gameObject()->AddComponent<Touchable*>();
        text->set_fontSize(text->get_fontSize() * 2.0f);
        text->set_alignment(TextAlignmentOptions::Center);

        LayoutElement* layoutelem = text->get_gameObject()->AddComponent<LayoutElement*>();
        layoutelem->set_preferredHeight(10.0f);
        layoutelem->set_preferredWidth(90.0f);

        Backgroundable* background = horizontal->get_gameObject()->AddComponent<Backgroundable*>();
        background->ApplyBackgroundWithAlpha(il2cpp_utils::createcsstr("title-gradient"), 1.0f);

        ImageView* imageView = background->get_gameObject()->GetComponentInChildren<ImageView*>();
        imageView->gradient = true;
        imageView->gradientDirection = 0;
        imageView->set_color(Color::get_white());
        imageView->set_color0(leftColor);
        imageView->set_color1(rightColor);
        imageView->curvedCanvasSettingsHelper->Reset();
        return text;
    }

    void SetTitleColor(HMUI::TitleViewController* titleView, UnityEngine::Color color, bool buttonanim)
    {
        if (!titleView)
        {
            ERROR("Title View was nullptr, not setting title color");
            return;
        }

        Transform* title_T = titleView->get_transform();
        Transform* BG_T = title_T->Find(il2cpp_utils::createcsstr("BG"));
        Transform* BackButtonBG_T = title_T->Find(il2cpp_utils::createcsstr("BackButton/BG"));

        ImageView* imageView = BG_T->get_gameObject()->GetComponent<ImageView*>();
        Color oldColor = imageView->get_color();

        imageView->set_color(color);

        ImageView* buttonImageView = BackButtonBG_T->get_gameObject()->GetComponent<ImageView*>();
        buttonImageView->set_color(color);
        buttonImageView->set_color0(color);
        buttonImageView->set_color1(color * 0.9f);

        ButtonStaticAnimations* anim = BackButtonBG_T->get_parent()->get_gameObject()->GetComponent<ButtonStaticAnimations*>();
        anim->set_enabled(buttonanim);
    }

    // these 2 methods were yoinked and edited from https://github.com/darknight1050/questui/blob/master/src/BeatSaberUI.cpp
    void AddViewComponents(GameObject* go, Zenject::DiContainer* container)
    {
        Canvas* cv = go->AddComponent<Canvas*>();
        Canvas* cvCopy = ArrayUtil::First(Resources::FindObjectsOfTypeAll<Canvas*>(), [](Canvas* x) { return to_utf8(csstrtostr(x->get_name())) == "DropdownTableView";});
        cv->set_additionalShaderChannels(cvCopy->get_additionalShaderChannels());
        cv->set_overrideSorting(cvCopy->get_overrideSorting());
        cv->set_pixelPerfect(cvCopy->get_pixelPerfect());
        cv->set_referencePixelsPerUnit(cvCopy->get_referencePixelsPerUnit());
        cv->set_renderMode(cvCopy->get_renderMode());
        cv->set_scaleFactor(cvCopy->get_scaleFactor());
        cv->set_sortingLayerID(cvCopy->get_sortingLayerID());
        cv->set_sortingOrder(cvCopy->get_sortingOrder());
        cv->set_worldCamera(cvCopy->get_worldCamera());

        VRGraphicRaycaster* rayc = container->InstantiateComponent<VRGraphicRaycaster*>(go);
        if (!rayc) ERROR("Addcompponent VRGRaphicsRaycaster failed!");
        go->AddComponent<CanvasGroup*>();

        RectTransform* rectTransform = go->GetComponent<RectTransform*>();
        rectTransform->set_anchorMin(UnityEngine::Vector2(0.0f, 0.0f));
        rectTransform->set_anchorMax(UnityEngine::Vector2(1.0f, 1.0f));
        rectTransform->set_sizeDelta(UnityEngine::Vector2(0.0f, 0.0f));
        rectTransform->set_anchoredPosition(UnityEngine::Vector2(0.0f, 0.0f));
        go->SetActive(false);
    }

    void SetupViewController(ViewController* vc) 
    {
        return;
        GameObject* go = vc->get_gameObject();
        AddViewComponents(go, nullptr);
    }
}