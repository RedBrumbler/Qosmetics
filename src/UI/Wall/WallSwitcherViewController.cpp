#include "UI/Wall/WallSwitcherViewController.hpp"
#include "Utils/UIUtils.hpp" 

#include "questui/shared/BeatSaberUI.hpp"

DEFINE_CLASS(Qosmetics::UI::WallSwitcherViewController);

using namespace HMUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;
using namespace Qosmetics;
using namespace Qosmetics::UI;
using namespace TMPro;

namespace Qosmetics::UI
{
    void WallSwitcherViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            UIUtils::SetupViewController(this);
            TextMeshProUGUI* text = CreateText(get_transform(), "Not Implemented");
            text->set_alignment(TextAlignmentOptions::_get_Midline());
            text->set_fontSize(text->get_fontSize() * 3.0f);
        }
    }

    void WallSwitcherViewController::Init()
    {
        set_enabled(false);
    }
}