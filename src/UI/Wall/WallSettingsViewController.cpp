#include "UI/Wall/WallSettingsViewController.hpp"
#include "Utils/UIUtils.hpp" 

DEFINE_CLASS(Qosmetics::UI::WallSettingsViewController);

using namespace Qosmetics;
using namespace UnityEngine;

namespace Qosmetics::UI
{
    void WallSettingsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            UIUtils::SetupViewController(this);

        }
    }

    void WallSettingsViewController::Init()
    {
        set_enabled(false);
    }
}