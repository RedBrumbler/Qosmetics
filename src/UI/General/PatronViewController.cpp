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

DEFINE_CLASS(Qosmetics::UI::PatronViewController);

#define INFO(value...) QosmeticsLogger::GetContextLogger("Patron View").info(value);
#define ERROR(value...) QosmeticsLogger::GetContextLogger("Patron View").error(value);

struct donatorInfo {
    std::string header;
    const std::vector<std::string>& names;
    std::vector<std::string>::const_iterator it;
    VerticalLayoutGroup* layout;
    PatronViewController* self;
    donatorInfo(PatronViewController* self, VerticalLayoutGroup* layout, const std::vector<std::string>& names, std::string header) : self(self), layout(layout), names(names), it(names.begin()),  header(header) {}
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

void AddDonators(donatorInfo* info)
{
    QuestUI::CustomDataType* wrapper = CRASH_UNLESS(il2cpp_utils::New<QuestUI::CustomDataType*, il2cpp_utils::CreationType::Manual>(classof(QuestUI::CustomDataType*)));
    wrapper->data = info;
    CreateHeader(info->layout, info->header);
    auto coroutine = UnityEngine::WaitUntil::New_ctor(il2cpp_utils::MakeDelegate<System::Func_1<bool>*>(classof(System::Func_1<bool>*), wrapper,
        +[](QuestUI::CustomDataType* wrapper){
            donatorInfo* info = (donatorInfo*)wrapper->data;
            if (info->it == info->names.end())
            {
                free(info);
                free(wrapper);
                return true;
            }
            AddDonator(info->layout, *info->it);
            info->it++;
            return false;
        }));
    info->self->StartCoroutine(reinterpret_cast<System::Collections::IEnumerator*>(coroutine));
}

namespace Qosmetics::UI
{
    void PatronViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation && PatronCache::get_atLeastOne())
        {
            get_gameObject()->AddComponent<Touchable*>();
            VerticalLayoutGroup* titleWrapper = CreateVerticalLayoutGroup(get_transform());
            HorizontalLayoutGroup* titleLayout = CreateHorizontalLayoutGroup(titleWrapper->get_transform());
            GameObject* container = CreateScrollableSettingsContainer(get_transform());
            ExternalComponents* components = container->GetComponent<ExternalComponents*>();
            RectTransform* rect = components->Get<RectTransform*>();
            rect->set_sizeDelta({0.0f, 0.0f});

            GameObject* header = GameObject::Find(il2cpp_utils::createcsstr("ReleaseInfoViewController"));
            if (header)
            {
                INFO("Found header GO");
                header = Object::Instantiate(header->get_transform()->Find(il2cpp_utils::createcsstr("HeaderPanel"))->get_gameObject(), titleLayout->get_transform());
                header->SetActive(true);
                LayoutElement* headerelem = header->AddComponent<LayoutElement*>();
                headerelem->set_preferredHeight(10.0f);
                headerelem->set_preferredWidth(90.0f);
                
                ImageView* imageView = header->GetComponentInChildren<ImageView*>();
                imageView->set_color0(Color::get_red());
                imageView->set_color1(Color::get_blue());

                //header->get_transform()->SetParent(get_transform());
                Polyglot::LocalizedTextMeshProUGUI* localizedtext = header->GetComponentInChildren<Polyglot::LocalizedTextMeshProUGUI*>();
                localizedtext->set_enabled(false);

                HMUI::CurvedTextMeshPro* text = header->GetComponentInChildren<HMUI::CurvedTextMeshPro*>();

                text->set_text(il2cpp_utils::createcsstr("Patrons & Donators"));
                Array<TextMeshProUGUI*>* texts = header->GetComponentsInChildren<TextMeshProUGUI*>();

                for (int i = 0; i < texts->Length(); i++)
                {
                    if (texts->values[i] != text) texts->values[i]->set_enabled(false);
                }
                RectTransform* titleRect = titleWrapper->get_rectTransform();

                titleRect->set_anchoredPosition({0.0f, 45.0f});
            }
            else ERROR("Header GO not found");

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
                info = new donatorInfo(this, layout, legendary, "<color=#e4c13c>== Legendary Patrons ==</color>\n<color=#666666><size=2>(Tier 4)</size></color>");
                AddDonators(info);
            }

            const std::vector<std::string>& amazing = PatronCache::get_amazing();
            if (amazing.size() > 0)
            {
                middle = CreateHorizontalLayoutGroup(container->get_transform());
                layout = CreateVerticalLayoutGroup(middle->get_transform());
                info = new donatorInfo(this, layout, amazing, "<color=#6573cc>-- Amazing Patrons --</color>\n<color=#666666><size=2>(Tier 3)</size></color>");
                AddDonators(info);
            }

            const std::vector<std::string>& enthusiastic = PatronCache::get_enthusiastic();
            if (enthusiastic.size() > 0)
            {
                middle = CreateHorizontalLayoutGroup(container->get_transform());
                layout = CreateVerticalLayoutGroup(middle->get_transform());
                info = new donatorInfo(this, layout, enthusiastic, "<color=#818de2>-- Enthusiastic Patrons --</color>\n<color=#666666><size=2>(Tier 2)</size></color>");
                AddDonators(info);
            }

            const std::vector<std::string>& paypal = PatronCache::get_paypal();
            if (paypal.size() > 0)
            {
                middle = CreateHorizontalLayoutGroup(container->get_transform());
                layout = CreateVerticalLayoutGroup(middle->get_transform());
                info = new donatorInfo(this, layout, paypal, "<color=#0095d9>-- Paypal Donators --</color>");
                AddDonators(info);
            }
        }
    }
}