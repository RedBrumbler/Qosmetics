#include "UI/General/QosmeticsViewController.hpp"

#include "questui/shared/BeatSaberUI.hpp"

#include "QosmeticsLogger.hpp"
#include "static-defines.hpp"

#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/Texture.hpp"
#include "UnityEngine/TextureWrapMode.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "HMUI/ButtonSpriteSwap.hpp"
#include "HMUI/Touchable.hpp"
#include "Config.hpp"
#include "Utils/UIUtils.hpp"

using namespace HMUI;
using namespace UnityEngine;
using namespace UnityEngine::Events;
using namespace UnityEngine::UI;
using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;
using namespace Qosmetics;
using namespace Qosmetics::UI;
using namespace TMPro;

DEFINE_CLASS(Qosmetics::UI::QosmeticsViewController);

#define INFO(value...) QosmeticsLogger::GetContextLogger("Qosmetics View Controller").info(value);
#define ERROR(value...) QosmeticsLogger::GetContextLogger("Qosmetics View Controller").error(value);

void swapButtonSprites(Button* button, std::string normalName, std::string selectedName)
{
    UnityEngine::Sprite* highlighted = QuestUI::BeatSaberUI::FileToSprite(selectedName, 266, 259);
    Texture* highl = highlighted->get_texture();
    highl->set_wrapMode(1);
    UnityEngine::Sprite* pressed = highlighted;
    UnityEngine::Sprite* selected = QuestUI::BeatSaberUI::FileToSprite(normalName, 266, 259);
    Texture* sel = selected->get_texture();
    sel->set_wrapMode(1);
    UnityEngine::Sprite* disabled = selected;
    HMUI::ButtonSpriteSwap* spriteSwap = button->get_gameObject()->GetComponent<HMUI::ButtonSpriteSwap*>();
    spriteSwap->normalStateSprite = selected;
    spriteSwap->highlightStateSprite = highlighted;
    spriteSwap->pressedStateSprite = pressed;
    spriteSwap->disabledStateSprite = disabled;
}

namespace Qosmetics::UI
{
    void QosmeticsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            UIUtils::SetupViewController(this);
            get_gameObject()->AddComponent<Touchable*>();

            UnityEngine::UI::VerticalLayoutGroup* vertical = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(get_transform());
            UnityEngine::UI::HorizontalLayoutGroup* layout = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(get_transform());

            title = BeatSaberUI::CreateText(vertical->get_transform(), GetScoresDisabled() ? "Scores are: Disabled" : "Scores are: Enabled");

            layout->set_spacing(-64.0f);

            UnityEngine::RectTransform* rectTransform = layout->GetComponent<UnityEngine::RectTransform*>();
            std::string mainpath = UIPATH;

            Button* saberButton = QuestUI::BeatSaberUI::CreateUIButton(rectTransform, "Sabers", "SettingsButton", il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(classof(UnityEngine::Events::UnityAction*), this, 
                +[](QosmeticsViewController* self)
                {
                    self->callback(ItemType::saber); 
                }));
            swapButtonSprites(saberButton, string_format("%s%s", mainpath.c_str(), "Icons/SaberIcon.png"), string_format("%s%s", mainpath.c_str(), "Icons/SaberIconSelected.png"));
        
            Button* noteButton = QuestUI::BeatSaberUI::CreateUIButton(rectTransform, "Bloqs", "SettingsButton", il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(classof(UnityEngine::Events::UnityAction*), this, 
                +[](QosmeticsViewController* self)
                {
                    self->callback(ItemType::note); 
                }));
            swapButtonSprites(noteButton, string_format("%s%s", mainpath.c_str(), "Icons/NoteIcon.png"), string_format("%s%s", mainpath.c_str(), "Icons/NoteIconSelected.png"));

            Button* wallButton = QuestUI::BeatSaberUI::CreateUIButton(rectTransform, "Walls", "SettingsButton", il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(classof(UnityEngine::Events::UnityAction*), this, 
                +[](QosmeticsViewController* self)
                {
                    self->callback(ItemType::wall); 
                }));
            swapButtonSprites(wallButton, string_format("%s%s", mainpath.c_str(), "Icons/WallIcon.png"), string_format("%s%s", mainpath.c_str(), "Icons/WallIconSelected.png"));
        }

        title = get_transform()->Find(il2cpp_utils::createcsstr("QuestUIVerticalLayoutGroup/QuestUIText"))->get_gameObject()->GetComponent<TMPro::TextMeshProUGUI*>();
        title->SetText(il2cpp_utils::createcsstr(string_format("Scores are: %s", GetScoresDisabled() ? "<color=\"red\">Disabled</color>" : "<color=\"green\">Enabled</color>")));
    }

    void QosmeticsViewController::set_selectCallback(std::function<void(ItemType)> callback)
    {
        this->callback = callback;
    }

    void QosmeticsViewController::Init()
    {
        INFO("QosmeticsViewController init");
        set_enabled(false);
    }
}