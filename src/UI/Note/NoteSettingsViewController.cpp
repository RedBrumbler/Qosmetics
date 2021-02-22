#include "UI/Note/NoteSettingsViewController.hpp"
#include "Utils/UIUtils.hpp" 

DEFINE_CLASS(Qosmetics::UI::NoteSettingsViewController);

using namespace Qosmetics;
using namespace UnityEngine;

namespace Qosmetics::UI
{
    void NoteSettingsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            UIUtils::SetupViewController(this);
        }
    }

    void NoteSettingsViewController::Init()
    {
        set_enabled(false);
    }
}