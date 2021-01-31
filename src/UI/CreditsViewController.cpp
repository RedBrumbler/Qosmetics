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

#define INFO(value...) Qosmetics::UILogger::GetLogger().WithContext("Patreon Credits").info(value)
#define ERROR(value...) Qosmetics::UILogger::GetLogger().WithContext("Patreon Credits").error(value)
extern config_t config;
DEFINE_CLASS(Qosmetics::CreditsViewController);

using namespace QuestUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace UnityEngine::Events;
using namespace HMUI;

static int list = 0;

static Qosmetics::CreditsViewController* self;
static GameObject* container;

struct donatorInfo {
        Transform* layout;
        VerticalLayoutGroup* group;
        std::string header;
        const std::vector<std::string>& names;
        int index;
        int size;
        Qosmetics::CreditsViewController* self;

        donatorInfo(const std::vector<std::string>& names, std::string header, VerticalLayoutGroup* layout, Qosmetics::CreditsViewController* self) : names(names)
        {
            this->header = header;
            index = 0;
            size = names.size();
            this->group = layout;
            this->layout = layout->get_transform();
            this->self = self;
        }
    };

void AddDonators(donatorInfo* info);
void DoNext();

void DoNext()
{
    HorizontalLayoutGroup* hor = nullptr;
    VerticalLayoutGroup* layout = nullptr;
    donatorInfo* info = nullptr;
    list++;
    switch (list)
    {
        case 1:
            hor = BeatSaberUI::CreateHorizontalLayoutGroup(container->get_transform());
            layout = BeatSaberUI::CreateVerticalLayoutGroup(hor->get_transform());
            info = new donatorInfo(Qosmetics::PatreonStorage::get_legendary(), "<color=#e4c13c>== Legendary Patrons ==</color>", layout, self);
            if (info->size > 0) AddDonators(info);
            else  DoNext();
            break;
        case 2:
            hor = BeatSaberUI::CreateHorizontalLayoutGroup(container->get_transform());
            layout = BeatSaberUI::CreateVerticalLayoutGroup(hor->get_transform());
            info = new donatorInfo(Qosmetics::PatreonStorage::get_amazing(), "<color=#6573cc>-- Amazing Patrons --</color>", layout, self);
            if (info->size > 0) AddDonators(info);
            else  DoNext();
            break;
        case 3:
            hor = BeatSaberUI::CreateHorizontalLayoutGroup(container->get_transform());
            layout = BeatSaberUI::CreateVerticalLayoutGroup(hor->get_transform());
            info = new donatorInfo(Qosmetics::PatreonStorage::get_enthusiastic(), "<color=#818de2>-- Enthusiastic Patrons --</color>", layout, self);
            if (info->size > 0) AddDonators(info);
            else  DoNext();
            break;
        case 4:
            hor = BeatSaberUI::CreateHorizontalLayoutGroup(container->get_transform());
            layout = BeatSaberUI::CreateVerticalLayoutGroup(hor->get_transform());
            info = new donatorInfo(Qosmetics::PatreonStorage::get_paypal(), "<color=#0095d9>-- Paypal Donators --</color>", layout, self);
            if (info->size > 0) AddDonators(info);
            else  DoNext();
            break;
    }
}

void AddDonators(donatorInfo* info)
{
    info->group->set_childAlignment(TextAnchor::MiddleCenter);
    //info->group->get_gameObject()
    //info->group->set_spacing(3.0f);
    RectTransform* rect = (RectTransform*)info->layout;
    rect->set_sizeDelta({0.0f, 0.0f});
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
                    DoNext();
                    return true;
                }
                BeatSaberUI::CreateText(info->layout, info->names[info->index]);
                info->index++;
                return false;
        }));
    info->self->StartCoroutine(reinterpret_cast<System::Collections::IEnumerator*>(coroutine));
}

namespace Qosmetics
{
    
    void CreditsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            self = this;
            get_gameObject()->AddComponent<Touchable*>();
            container = BeatSaberUI::CreateScrollableSettingsContainer(get_transform());
            ExternalComponents* components = container->GetComponent<ExternalComponents*>();
            RectTransform* rect = components->Get<RectTransform*>();
            rect->set_sizeDelta({0.0f, 0.0f});

            //VerticalLayoutGroup* main = BeatSaberUI::CreateVerticalLayoutGroup(container->get_transform());
            if (PatreonStorage::get_loaded() && PatreonStorage::get_atLeastOne())
            {
                VerticalLayoutGroup* main = container->GetComponent<VerticalLayoutGroup*>();
                main->set_childAlignment(TextAnchor::MiddleCenter);
                BeatSaberUI::CreateText(main->get_transform(), "This mod was made possible by all these amazing patrons and donators!");
                DoNext();
            }
            else if (!PatreonStorage::get_loaded())
            {
                BeatSaberUI::CreateText(container->get_transform(), "Sorry, but the list of donators and patreons could not be loaded");
            }
            else ERROR("No patreons loaded");
        }
    }
}
