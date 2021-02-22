#include "UI/Wall/WallPreviewViewController.hpp"
#include "Utils/UIUtils.hpp" 

DEFINE_CLASS(Qosmetics::UI::WallPreviewViewController);

using namespace Qosmetics;
using namespace UnityEngine;

namespace Qosmetics::UI
{
    void WallPreviewViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            UIUtils::SetupViewController(this);

        }
    }

    void WallPreviewViewController::Init()
    {
        set_enabled(false);
    }
}