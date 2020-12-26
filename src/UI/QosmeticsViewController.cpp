#include "UI/QosmeticsViewController.hpp"
#include "config.hpp"
#include "Config/NoteConfig.hpp"
#include "Data/Descriptor.hpp"

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

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/ExternalComponents.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"

#include "UI/MenuButtonClickData.hpp"
#include "Logging/GenericLogger.hpp"

using namespace QuestUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace UnityEngine::Events;
using namespace HMUI;

#define getLogger() Qosmetics::GenericLogger::GetLogger() 
DEFINE_CLASS(Qosmetics::QosmeticsViewController);

DEFINE_EVENT(Qosmetics::QosmeticsViewController, System::Action_1<QuestUI::CustomDataType*>*, openSubMenu);

void OnSubMenuButtonClick(QuestUI::CustomDataType* data, UnityEngine::UI::Button* button) {
    if (!data) return;
    Qosmetics::MenuButtonClickData& clickData = data->GetData<Qosmetics::MenuButtonClickData>();
    std::string typeString = "";
    switch (clickData.type)
    {
        case saber:
            typeString = "saber";
            break;
        case note:
            typeString = "note";
            break;
        case wall:
            typeString = "wall";
            break;
        case menupointer:
            typeString = "menu pointer";
            break;
        case platform:
            typeString = "platform";
            break;
        default:
            typeString = "invalid";
            break;
    }
    getLogger().info("Went into %s menu", typeString.c_str());
    if(clickData.viewController->openSubMenu)
        clickData.viewController->openSubMenu->Invoke(data);
}

namespace Qosmetics
{
    void QosmeticsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if(addedToHierarchy && firstActivation) 
        {
            get_gameObject()->AddComponent<HMUI::Touchable*>();
            UnityEngine::UI::GridLayoutGroup* layout = QuestUI::BeatSaberUI::CreateGridLayoutGroup(get_transform());
            
            layout->set_constraint(UnityEngine::UI::GridLayoutGroup::Constraint::Flexible);
            layout->set_cellSize(UnityEngine::Vector2(48.0f, 10.0f));
            layout->set_spacing(UnityEngine::Vector2(3.0f, 3.0f));
            layout->set_childAlignment(UnityEngine::TextAnchor::MiddleCenter);
            layout->set_startAxis(1);

            UnityEngine::RectTransform* rectTransform = layout->GetComponent<UnityEngine::RectTransform*>();
            QuestUI::BeatSaberUI::CreateUIButton(rectTransform, "Qosmetics Sabers", il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(classof(UnityEngine::Events::UnityAction*), CRASH_UNLESS(il2cpp_utils::New<QuestUI::CustomDataType*>(classof(QuestUI::CustomDataType*)))->SetData(MenuButtonClickData{this, saber}), OnSubMenuButtonClick));
            QuestUI::BeatSaberUI::CreateUIButton(rectTransform, "Qosmetics Bloqs", il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(classof(UnityEngine::Events::UnityAction*), CRASH_UNLESS(il2cpp_utils::New<QuestUI::CustomDataType*>(classof(QuestUI::CustomDataType*)))->SetData(MenuButtonClickData{this, note}), OnSubMenuButtonClick));
            QuestUI::BeatSaberUI::CreateUIButton(rectTransform, "Qosmetics Walls", il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(classof(UnityEngine::Events::UnityAction*), CRASH_UNLESS(il2cpp_utils::New<QuestUI::CustomDataType*>(classof(QuestUI::CustomDataType*)))->SetData(MenuButtonClickData{this, wall}), OnSubMenuButtonClick));
        }
    }
}
