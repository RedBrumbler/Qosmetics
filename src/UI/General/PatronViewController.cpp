#include "UI/General/PatronViewController.hpp"

#include "HMUI/Touchable.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/ExternalComponents.hpp"
#include "questui/shared/CustomTypes/Data/CustomDataType.hpp"

#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/Events/UnityAction_1.hpp"
#include "UnityEngine/WaitUntil.hpp"
#include "Polyglot/LocalizedTextMeshProUGUI.hpp"
#include "HMUI/CurvedTextMeshPro.hpp"
#include "System/Collections/IEnumerator.hpp"
#include "System/Func_1.hpp"
#include "Utils/TextUtils.hpp"
#include "Utils/UIUtils.hpp"
#include "Data/PatronCache.hpp"
#include "QosmeticsLogger.hpp"

#include "HMUI/ImageView.hpp"


using namespace HMUI;
using namespace UnityEngine;
using namespace UnityEngine::Events;
using namespace UnityEngine::UI;
using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;
using namespace Qosmetics;
using namespace Qosmetics::UI;
using namespace TMPro;

DEFINE_TYPE(Qosmetics::UI::PatronViewController);

#define INFO(value...) QosmeticsLogger::GetContextLogger("Patron View").info(value);
#define ERROR(value...) QosmeticsLogger::GetContextLogger("Patron View").error(value);

struct donatorInfo {
    VerticalLayoutGroup* layout;
    const std::vector<std::string>& names;
    std::vector<std::string>::const_iterator it;
    donatorInfo(VerticalLayoutGroup* layout, const std::vector<std::string>& names) : layout(layout), names(names), it(names.begin()) {}
};

void CreateHeader(VerticalLayoutGroup* layout, std::string header)
{
    TextMeshProUGUI* text = CreateText(layout->get_transform(), header);
    text->get_gameObject()->AddComponent<LayoutElement*>()->set_preferredHeight(8.0f);
    text->set_alignment(TextAlignmentOptions::_get_Midline());
}

void AddDonator(VerticalLayoutGroup* layout, std::string name)
{
    std::string newName = name;
    if (newName.find("Bobbie") != std::string::npos && newName.length() == strlen("Bobbie")) newName = TextUtils::rainbowify(newName);
    TextMeshProUGUI* text = CreateText(layout->get_transform(), newName);
    text->get_gameObject()->AddComponent<LayoutElement*>()->set_preferredHeight(5.0f);
    text->set_alignment(TextAlignmentOptions::_get_Midline());
}

namespace Qosmetics::UI
{
    custom_types::Helpers::Coroutine PatronViewController::AddDonators(donatorInfo* info)
    {
        while (info->it != info->names.end())
        {
            AddDonator(info->layout, *info->it);
            info->it++;
            co_yield nullptr;
        }

        free(info);
        co_return;
    }

    void PatronViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            //UIUtils::SetupViewController(this);
        }
        if (firstActivation && PatronCache::get_atLeastOne())
        {
            get_gameObject()->AddComponent<Touchable*>();
            Color titleColor = Color(0.3f, 0.15f, 0.6f, 1.0f);
            UIUtils::AddHeader(get_transform(), "Patrons & Donators", titleColor);
            GameObject* container = CreateScrollableSettingsContainer(get_transform());
            ExternalComponents* components = container->GetComponent<ExternalComponents*>();
            RectTransform* rect = components->Get<RectTransform*>();
            rect->set_sizeDelta({0.0f, 0.0f});


            VerticalLayoutGroup* layout;
            HorizontalLayoutGroup* middle;
            middle = CreateHorizontalLayoutGroup(container->get_transform());
            layout = CreateVerticalLayoutGroup(middle->get_transform());

            TextMeshProUGUI* text = BeatSaberUI::CreateText(layout->get_transform(), "<color=#ff0000><size=6><b>Qosmetics Patreon Supporters & Donators</b></size></color>\n<size=3>These Patrons and Donators have donated to show their support,\n and have received a place here to thank them for this.\n This support is greatly appreciated and will help justify time spent on developing Qosmetics & other mods.\n A massive thanks goes out to all these people!\n\n If you'd also like to support development, the patreon can be found at:<color=#ff0000>Patreon.com/Qosmetics</color></size>");
            text->get_gameObject()->AddComponent<LayoutElement*>()->set_preferredHeight(40.f);
            text->set_alignment(TextAlignmentOptions::_get_Center());
            
            donatorInfo* info;
            const std::vector<std::string>& legendary = PatronCache::get_legendary();
            if (legendary.size() > 0)
            {
                middle = CreateHorizontalLayoutGroup(container->get_transform());
                layout = CreateVerticalLayoutGroup(middle->get_transform());
                info = new donatorInfo(layout, legendary);
                CreateHeader(layout, "<color=#e4c13c>== Legendary Patrons ==</color>\n<color=#666666><size=2>(Tier 4)</size></color>");
                StartCoroutine(reinterpret_cast<System::Collections::IEnumerator*>(custom_types::Helpers::CoroutineHelper::New(AddDonators(info))));
            }

            const std::vector<std::string>& amazing = PatronCache::get_amazing();
            if (amazing.size() > 0)
            {
                middle = CreateHorizontalLayoutGroup(container->get_transform());
                layout = CreateVerticalLayoutGroup(middle->get_transform());
                info = new donatorInfo(layout, amazing);
                CreateHeader(layout, "<color=#6573cc>-- Amazing Patrons --</color>\n<color=#666666><size=2>(Tier 3)</size></color>");
                StartCoroutine(reinterpret_cast<System::Collections::IEnumerator*>(custom_types::Helpers::CoroutineHelper::New(AddDonators(info))));
            }

            const std::vector<std::string>& enthusiastic = PatronCache::get_enthusiastic();
            if (enthusiastic.size() > 0)
            {
                middle = CreateHorizontalLayoutGroup(container->get_transform());
                layout = CreateVerticalLayoutGroup(middle->get_transform());
                info = new donatorInfo(layout, enthusiastic);
                CreateHeader(layout, "<color=#818de2>-- Enthusiastic Patrons --</color>\n<color=#666666><size=2>(Tier 2)</size></color>");
                StartCoroutine(reinterpret_cast<System::Collections::IEnumerator*>(custom_types::Helpers::CoroutineHelper::New(AddDonators(info))));
            }

            const std::vector<std::string>& paypal = PatronCache::get_paypal();
            if (paypal.size() > 0)
            {
                middle = CreateHorizontalLayoutGroup(container->get_transform());
                layout = CreateVerticalLayoutGroup(middle->get_transform());
                info = new donatorInfo(layout, paypal);
                CreateHeader(layout, "<color=#0095d9>-- Paypal Donators --</color>");
                StartCoroutine(reinterpret_cast<System::Collections::IEnumerator*>(custom_types::Helpers::CoroutineHelper::New(AddDonators(info))));
            }
        }
    }

    void PatronViewController::Init()
    {
        set_enabled(false);
        //UIUtils::SetupViewController(this);
    }
}