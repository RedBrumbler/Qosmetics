#pragma once

#include "HMUI/TitleViewController.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "UnityEngine/Color.hpp"
#include <string>

namespace Qosmetics::Core::UIUtils
{

    /// @brief adds a header to the given parent with text title
    /// @param parent the parent of the title
    /// @param title the text to display
    /// @param color the color to use for the fade, goes from full color to full transparent
    /// @return the text object that is in the header
    TMPro::TextMeshProUGUI* AddHeader(UnityEngine::Transform* parent, std::string_view title, const UnityEngine::Color& color);

    /// @brief sets the color for a given title view, can turn off the button animation if needed
    /// @param titleView the titleview to set the color for
    /// @param color the color to set
    /// @param buttonanim whether the button animation should work (this prevents colors from being weird)
    void SetTitleColor(HMUI::TitleViewController* titleView, const UnityEngine::Color& color, bool buttonanim = false);
}