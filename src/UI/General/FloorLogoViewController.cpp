#include "UI/General/FloorLogoViewController.hpp"

#include "QosmeticsLogger.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "static-defines.hpp"
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

DEFINE_TYPE(Qosmetics::UI::FloorLogoViewController);

#define INFO(value...) QosmeticsLogger::GetContextLogger("Floor Logo View Controller").info(value);
#define ERROR(value...) QosmeticsLogger::GetContextLogger("Floor Logo View Controller").error(value);

extern void swapButtonSprites(Button* button, std::string normalName, std::string selectedName);

namespace Qosmetics::UI
{
    void FloorLogoViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            //UIUtils::SetupViewController(this);
            INFO("Floor logo activated!");
            std::string mainPath = UIPATH;
            UnityEngine::UI::HorizontalLayoutGroup* layout = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(get_transform());
            Button* button = QuestUI::BeatSaberUI::CreateUIButton(layout->get_transform(), "", "SettingsButton", nullptr);
            swapButtonSprites(button, string_format("%s%s", mainPath.c_str(), "Icons/GameSetupIconSelected.png"), string_format("%s%s", mainPath.c_str(), "Icons/GameSetupIconSelected.png"));
            button->get_transform()->set_localScale({2.0f, 3.0f, 2.0f});
            button->set_interactable(false);
        }
    }

    void FloorLogoViewController::Init()
    {
        set_enabled(false);
    }
}