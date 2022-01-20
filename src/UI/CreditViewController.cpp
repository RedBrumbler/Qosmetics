#include "UI/CreditViewController.hpp"
#include "Data/Patrons.hpp"
#include "Utils/UIUtils.hpp"
#include "logging.hpp"
#include "questui/shared/ArrayUtil.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"
#include "questui/shared/CustomTypes/Components/ExternalComponents.hpp"
#include "static-defines.hpp"

#include "GlobalNamespace/SharedCoroutineStarter.hpp"

#include "HMUI/Touchable.hpp"

#include "UnityEngine/Application.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/Networking/DownloadHandler.hpp"
#include "UnityEngine/Networking/UnityWebRequest.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Shader.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/UI/ContentSizeFitter.hpp"

DEFINE_TYPE(Qosmetics::Core, CreditViewController);

using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;

HorizontalLayoutGroup* make_patron_display(UnityEngine::Transform* parent, std::vector<std::string>& names, std::string title, UnityEngine::Color headercolor)
{
    auto banner = CreateHorizontalLayoutGroup(parent->get_transform());
    auto bannerText = CreateText(banner->get_transform(), title);
    auto bannerBG = banner->get_gameObject()->AddComponent<QuestUI::Backgroundable*>();
    bannerBG->ApplyBackgroundWithAlpha(il2cpp_utils::newcsstr("title-gradient"), 1.0f);
    bannerText->set_alignment(TMPro::TextAlignmentOptions::_get_Center());

    auto contentSizeFitter = banner->get_gameObject()->GetComponent<ContentSizeFitter*>();
    if (!contentSizeFitter)
        contentSizeFitter = banner->get_gameObject()->AddComponent<ContentSizeFitter*>();
    contentSizeFitter->set_horizontalFit(ContentSizeFitter::FitMode::Unconstrained);

    auto layoutElement = bannerText->get_gameObject()->GetComponent<LayoutElement*>();
    if (!layoutElement)
        layoutElement = banner->get_gameObject()->AddComponent<LayoutElement*>();
    layoutElement->set_preferredWidth(50.0f);
    layoutElement->set_preferredHeight(5.0f);

    HMUI::ImageView* imageView =
        bannerBG->get_gameObject()->GetComponentInChildren<HMUI::ImageView*>();
    imageView->dyn__gradient() = true;
    imageView->dyn__gradientDirection() = 1;
    imageView->set_color(headercolor);
    imageView->set_color0(Color(1.1f, 1.1f, 1.1f, 1.0f));
    imageView->set_color1(Color(0.9f, 0.9f, 0.9f, 1.0f));

    for (auto& n : names)
    {
        auto t = CreateText(parent, n);
        t->set_alignment(TMPro::TextAlignmentOptions::_get_Center());
    }

    return banner;
}

#define SETUP_WRAPPER()                                                             \
    auto horizontal = CreateHorizontalLayoutGroup(container->get_transform());      \
    auto vertical = CreateVerticalLayoutGroup(horizontal->get_transform());         \
    auto bg = vertical->get_gameObject()->AddComponent<QuestUI::Backgroundable*>(); \
    bg->ApplyBackgroundWithAlpha(il2cpp_utils::newcsstr("title-gradient"), 0.5f);   \
    HMUI::ImageView* imageView =                                                    \
        bg->get_gameObject()->GetComponentInChildren<HMUI::ImageView*>();           \
    imageView->dyn__gradient() = false;                                             \
    imageView->dyn__gradientDirection() = 1;                                        \
    imageView->set_color({0, 0, 0, 0.5f})

namespace Qosmetics::Core
{
    void CreditViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            get_gameObject()->AddComponent<HMUI::Touchable*>();
            container = CreateScrollableSettingsContainer(get_transform());
            ExternalComponents* components = container->GetComponent<ExternalComponents*>();
            RectTransform* rect = components->Get<RectTransform*>();
            rect->set_sizeDelta({0.0f, 0.0f});

            auto vertical = container->GetComponentInChildren<VerticalLayoutGroup*>();
            vertical->set_spacing(5.0f);

            UIUtils::AddHeader(get_transform(), "Patrons & Donators", qosmetics_purple);
            auto explHorizontal = CreateHorizontalLayoutGroup(container->get_transform());
            auto explVertical = CreateVerticalLayoutGroup(explHorizontal->get_transform());

            auto mainText = CreateText(explVertical->get_transform(), "<color=#ff4040><size=6><b>Qosmetics Patreon Supporters & Donators</b></size></color>\n<size=3>These Patrons and Donators have donated to show their support,\n and have received a place here to thank them for this.\n This support is greatly appreciated and will help justify time spent on developing Qosmetics & other mods.\n A massive thanks goes out to all these people!\n\n If you'd also like to support development, the patreon can be found at:</size>");
            mainText->get_gameObject()->AddComponent<LayoutElement*>()->set_preferredHeight(40.f);
            mainText->set_alignment(TMPro::TextAlignmentOptions::_get_Center());

            auto patreonText = CreateClickableText(explVertical->get_transform(), "Patreon.com/Qosmetics", true, []()
                                                   { Application::OpenURL(il2cpp_utils::newcsstr(patreon_url)); });
            patreonText->set_alignment(TMPro::TextAlignmentOptions::_get_Center());
            patreonText->set_defaultColor(Color(1.0f, 0.25f, 0.25f, 1.0f));
            patreonText->set_highlightColor(Color(1.0f, 0.5f, 0.5f, 1.0f));

            AddHoverHint(patreonText->get_gameObject(), "Open in Browser");

            auto patronParent = CreateHorizontalLayoutGroup(container->get_transform());
            patronTexts = CreateVerticalLayoutGroup(patronParent->get_transform());
            auto placeholderText = CreateText(patronTexts->get_transform(), "Fetching patreon supporters...");

            StartCoroutine(reinterpret_cast<System::Collections::IEnumerator*>(custom_types::Helpers::CoroutineHelper::New(GetPatreonSupporters())));
        }
    }

    custom_types::Helpers::Coroutine CreditViewController::GetPatreonSupporters()
    {
        UnityEngine::Networking::UnityWebRequest* www = UnityEngine::Networking::UnityWebRequest::Get(il2cpp_utils::newcsstr(patron_url));
        co_yield reinterpret_cast<System::Collections::IEnumerator*>(www->SendWebRequest());
        auto downloadHandler = www->get_downloadHandler();
        auto text = to_utf8(csstrtostr(downloadHandler->get_text()));
        auto patrons = Qosmetics::Core::Patrons::Parse(text);

        auto patronTextsT = patronTexts->get_transform()->get_parent();
        Object::DestroyImmediate(patronTextsT->get_gameObject());
        if (!patrons.any())
        {
            auto patronParent = CreateHorizontalLayoutGroup(container->get_transform());
            patronTexts = CreateVerticalLayoutGroup(patronParent->get_transform());
            auto placeholderText = CreateText(patronTexts->get_transform(), "There were no patrons found, you can be the first to appear here!");
        }
        else
        {
            if (patrons.legendary.size() > 0)
            {
                SETUP_WRAPPER();
                auto banner = make_patron_display(vertical->get_transform(), patrons.legendary, "<color=#000000>Legendary patrons</color> <color=#222222><size=2>(Tier 4)</size></color>", Color(0.9f, 0.75f, 0.25f, 1.0f));
                auto bannerimageView = banner->get_gameObject()->GetComponentInChildren<HMUI::ImageView*>();
                auto origMat = QuestUI::ArrayUtil::First(UnityEngine::Resources::FindObjectsOfTypeAll<UnityEngine::Material*>(),
                                                         [](UnityEngine::Material* x)
                                                         {
                                                             return to_utf8(csstrtostr(x->get_name())) == "AnimatedButton";
                                                         });
                auto dupe = Object::Instantiate(origMat);
                dupe->SetColor(Shader::PropertyToID(il2cpp_utils::newcsstr("_ShineColor")), Color(0.9f, 0.75f, 0.25f, 1.0f));
                bannerimageView->set_material(dupe);
            }

            if (patrons.amazing.size() > 0)
            {
                SETUP_WRAPPER();
                make_patron_display(vertical->get_transform(), patrons.amazing, "<color=#000000>Amazing patrons</color> <color=#222222><size=2>(Tier 3)</size></color>", Color(0.4f, 0.45f, 0.8f, 1.0f));
            }

            if (patrons.enthusiastic.size() > 0)
            {
                SETUP_WRAPPER();
                make_patron_display(vertical->get_transform(), patrons.enthusiastic, "<color=#000000>Enthusiastic patrons</color> <color=#222222><size=2>(Tier 2)</size></color>", Color(0.5f, 0.55f, 0.9f, 1.0f));
            }

            if (patrons.paypal.size() > 0)
            {
                SETUP_WRAPPER();
                make_patron_display(vertical->get_transform(), patrons.paypal, "<color=#000000>Paypal donators</color>", Color(0.0f, 0.6f, 0.85f, 1.0f));
            }
        }
        co_return;
    }
}