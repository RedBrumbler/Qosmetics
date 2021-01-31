#include "UI/CreditsViewController.hpp"
#include "config.hpp"

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
#include "UnityEngine/Coroutine.hpp"
#include "UnityEngine/WaitUntil.hpp"
#include "System/Func_1.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/ExternalComponents.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"
#include "questui/shared/CustomTypes/Data/CustomDataType.hpp"
#include "Data/PatreonStorage.hpp"

#include "Logging/UILogger.hpp"
#define INFO(value...) UILogger::GetLogger().WithContext("Patreon Credits").info(value)
#define ERROR(value...) UILogger::GetLogger().WithContext("Patreon Credits").error(value)
extern config_t config;
DEFINE_CLASS(Qosmetics::CreditsViewController);

using namespace QuestUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace UnityEngine::Events;
using namespace HMUI;
struct donatorInfo {
        Transform* layout;
        std::string header;
        const std::vector<std::string>& names;
        int index;
        int size;

        donatorInfo(const std::vector<std::string>& names, std::string header, Transform* layout) : names(names)
        {
            this->header = header;
            index = 0;
            size = names.size();
            this->layout = layout;
        }
    };

void AddDonators(donatorInfo* info)
{
    BeatSaberUI::CreateText(info->layout, info->header);
    QuestUI::CustomDataType* wrapper = CRASH_UNLESS(il2cpp_utils::New<QuestUI::CustomDataType*, il2cpp_utils::CreationType::Manual>(classof(QuestUI::CustomDataType*)));
    wrapper->data = info;
    auto coroutine = UnityEngine::WaitUntil::New_ctor(il2cpp_utils::MakeDelegate<System::Func_1<bool>*>(classof(System::Func_1<bool>*), wrapper,
            +[](QuestUI::CustomDataType* wrapper){
                donatorInfo* info = (donatorInfo*)wrapper->data;
                if (info->index >= info->size)
                {
                    // we are now done, we can free the structs and custom datatype
                    free(info);
                    free(wrapper);
                    return true;
                }
                BeatSaberUI::CreateText(info->layout, info->names[info->index]);
                info->index++;
                return false;
        }));
}

namespace Qosmetics
{
    
    void CreditsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            GameObject* container = BeatSaberUI::CreateScrollableSettingsContainer(get_transform());
            if (PatreonStorage::get_loaded() && PatreonStorage::get_atLeastOne())
            {
                container->GetComponent<VerticalLayoutGroup*>()->set_childAlignment(TextAnchor::MiddleCenter);
                BeatSaberUI::CreateText(container->get_transform(), "This mod was made possible by all these amazing patreons and donators!");
                // legendary tier
                VerticalLayoutGroup* layout = BeatSaberUI::CreateVerticalLayoutGroup(container->get_transform());
                layout->set_childAlignment(TextAnchor::MiddleCenter);
                donatorInfo* info = new donatorInfo(PatreonStorage::get_legendary(), "<color=#e4c13c>== Legendary Patrons ==</color>", layout->get_transform());
                if (info->size > 0) AddDonators(info);
                // amazing tier
                layout = BeatSaberUI::CreateVerticalLayoutGroup(container->get_transform());
                layout->set_childAlignment(TextAnchor::MiddleCenter);
                info = new donatorInfo(PatreonStorage::get_amazing(), "<color=#6573cc>-- Amazing Patrons --</color>", layout->get_transform());
                if (info->size > 0) AddDonators(info);
                // enthusiastic tier
                layout = BeatSaberUI::CreateVerticalLayoutGroup(container->get_transform());
                layout->set_childAlignment(TextAnchor::MiddleCenter);
                info = new donatorInfo(PatreonStorage::get_enthusiastic(), "<color=#818de2>-- Enthusiastic Patrons --</color>", layout->get_transform());
                if (info->size > 0) AddDonators(info);
                // paypal donators
                layout = BeatSaberUI::CreateVerticalLayoutGroup(container->get_transform());
                layout->set_childAlignment(TextAnchor::MiddleCenter);
                info = new donatorInfo(PatreonStorage::get_paypal(), "<color=#0095d9>-- Paypal Donators --</color>", layout->get_transform());
                if (info->size > 0) AddDonators(info);
            }
            else if (!PatreonStorage::get_loaded())
            {
                BeatSaberUI::CreateText(container->get_transform(), "Sorry, but the list of donators and patreons could not be loaded");
            }
        }
    }
}
