#include "UI/Note/NotePreviewViewController.hpp"
#include "Utils/UIUtils.hpp" 

DEFINE_CLASS(Qosmetics::UI::NotePreviewViewController);

using namespace Qosmetics;
using namespace UnityEngine;

namespace Qosmetics::UI
{
    void NotePreviewViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            UIUtils::SetupViewController(this);
        }
    }

    void NotePreviewViewController::Init()
    {
        set_enabled(false);
    }
}