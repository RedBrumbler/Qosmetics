#include "UI/General/PatronViewController.hpp"

#include "HMUI/Touchable.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/ExternalComponents.hpp"
#include "questui/shared/CustomTypes/Data/CustomDataType.hpp"

#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/Events/UnityAction_1.hpp"
#include "UnityEngine/WaitUntil.hpp"
#include "System/Collections/IEnumerator.hpp"
#include "System/Func_1.hpp"
#include "Utils/TextUtils.hpp"
#include "Data/PatronCache.hpp"

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
    text->get_gameObject()->AddComponent<LayoutElement*>()->set_preferredHeight(5.0f);
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
            GameObject* container = CreateScrollableSettingsContainer(get_transform());
            ExternalComponents* components = container->GetComponent<ExternalComponents*>();
            RectTransform* rect = components->Get<RectTransform*>();
            rect->set_sizeDelta({0.0f, 0.0f});
            

            BeatSaberUI::CreateText(container->get_transform(), "This mod was made possible by all these amazing patrons and donators!");
            VerticalLayoutGroup* layout;
            HorizontalLayoutGroup* middle;
            donatorInfo* info;
            const std::vector<std::string>& legendary = PatronCache::get_legendary();
            if (legendary.size() > 0)
            {
                middle = CreateHorizontalLayoutGroup(container->get_transform());
                layout = CreateVerticalLayoutGroup(middle->get_transform());
                info = new donatorInfo(this, layout, legendary, "<color=#e4c13c>== Legendary Patrons ==</color>");
                AddDonators(info);
            }

            const std::vector<std::string>& amazing = PatronCache::get_amazing();
            if (amazing.size() > 0)
            {
                middle = CreateHorizontalLayoutGroup(container->get_transform());
                layout = CreateVerticalLayoutGroup(middle->get_transform());
                info = new donatorInfo(this, layout, amazing, "<color=#6573cc>-- Amazing Patrons --</color>");
                AddDonators(info);
            }

            const std::vector<std::string>& enthusiastic = PatronCache::get_enthusiastic();
            if (enthusiastic.size() > 0)
            {
                middle = CreateHorizontalLayoutGroup(container->get_transform());
                layout = CreateVerticalLayoutGroup(middle->get_transform());
                info = new donatorInfo(this, layout, enthusiastic, "<color=#818de2>-- Enthusiastic Patrons --</color>");
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