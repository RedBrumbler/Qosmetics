#include "UI/Note/NoteSwitcherViewController.hpp"

#include "questui/shared/BeatSaberUI.hpp"

DEFINE_CLASS(Qosmetics::UI::NoteSwitcherViewController);

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
    void NoteSwitcherViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            TextMeshProUGUI* text = CreateText(get_transform(), "Not Implemented");
            text->set_alignment(TextAlignmentOptions::_get_Midline());
            text->set_fontSize(text->get_fontSize() * 3.0f);
        }
    }
}