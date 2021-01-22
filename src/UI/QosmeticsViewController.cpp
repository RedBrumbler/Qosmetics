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
#include "HMUI/ButtonSpriteSwap.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/ExternalComponents.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"

#include "UI/MenuButtonClickData.hpp"
#include "Logging/GenericLogger.hpp"

#include "static-defines.hpp"

using namespace QuestUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace UnityEngine::Events;
using namespace HMUI;

#define getLogger() Qosmetics::GenericLogger::GetLogger() 
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

void spriteSwapSetup(Button* button, std::string normalName, std::string selectedName)
{
    UnityEngine::Sprite* highlighted = QuestUI::BeatSaberUI::FileToSprite(selectedName, 266, 259);
    UnityEngine::Sprite* pressed = highlighted;
    UnityEngine::Sprite* selected = QuestUI::BeatSaberUI::FileToSprite(normalName, 266, 259);
    UnityEngine::Sprite* disabled = selected;
    HMUI::ButtonSpriteSwap* spriteSwap = button->get_gameObject()->GetComponent<HMUI::ButtonSpriteSwap*>();
    spriteSwap->normalStateSprite = selected;
    spriteSwap->highlightStateSprite = highlighted;
    spriteSwap->pressedStateSprite = pressed;
    spriteSwap->disabledStateSprite = disabled;
}

namespace Qosmetics
{
    void QosmeticsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if(addedToHierarchy && firstActivation) 
        {
            get_gameObject()->AddComponent<HMUI::Touchable*>();
            UnityEngine::UI::VerticalLayoutGroup* vertical = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(get_transform());
            UnityEngine::UI::HorizontalLayoutGroup* layout = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(get_transform());

            title = BeatSaberUI::CreateText(vertical->get_transform(), GetScoresDisabled() ? "Scores are: Disabled" : "Scores are: Enabled");

            layout->set_spacing(-64.0f);

            UnityEngine::RectTransform* rectTransform = layout->GetComponent<UnityEngine::RectTransform*>();
            std::string mainpath = UIPATH;
            Button* saberButton = QuestUI::BeatSaberUI::CreateUIButton(rectTransform, "Sabers", "SettingsButton", il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(classof(UnityEngine::Events::UnityAction*), CRASH_UNLESS(il2cpp_utils::New<QuestUI::CustomDataType*>(classof(QuestUI::CustomDataType*)))->SetData(MenuButtonClickData{this, saber}), OnSubMenuButtonClick));            
            spriteSwapSetup(saberButton, string_format("%s%s", mainpath.c_str(), "Icons/SaberIcon.png"), string_format("%s%s", mainpath.c_str(), "Icons/SaberIconSelected.png"));

            Button* noteButton = QuestUI::BeatSaberUI::CreateUIButton(rectTransform, "Bloqs", "SettingsButton", il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(classof(UnityEngine::Events::UnityAction*), CRASH_UNLESS(il2cpp_utils::New<QuestUI::CustomDataType*>(classof(QuestUI::CustomDataType*)))->SetData(MenuButtonClickData{this, note}), OnSubMenuButtonClick));
            spriteSwapSetup(noteButton, string_format("%s%s", mainpath.c_str(), "Icons/NoteIcon.png"), string_format("%s%s", mainpath.c_str(), "Icons/NoteIconSelected.png"));
            
            Button* wallButton = QuestUI::BeatSaberUI::CreateUIButton(rectTransform, "Walls", "SettingsButton", il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(classof(UnityEngine::Events::UnityAction*), CRASH_UNLESS(il2cpp_utils::New<QuestUI::CustomDataType*>(classof(QuestUI::CustomDataType*)))->SetData(MenuButtonClickData{this, wall}), OnSubMenuButtonClick));
            spriteSwapSetup(wallButton, string_format("%s%s", mainpath.c_str(), "Icons/WallIcon.png"), string_format("%s%s", mainpath.c_str(), "Icons/WallIconSelected.png"));
            
        }
        title = get_transform()->Find(il2cpp_utils::createcsstr("QuestUIVerticalLayoutGroup/QuestUIText"))->get_gameObject()->GetComponent<TMPro::TextMeshProUGUI*>();
        title->SetText(il2cpp_utils::createcsstr(string_format("Scores are: %s", GetScoresDisabled() ? "<color=\"red\">Disabled</color>" : "<color=\"green\">Enabled</color>")));
    }
}
