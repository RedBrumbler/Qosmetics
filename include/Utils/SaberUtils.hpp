#pragma once
#include "Types/Saber/SaberItem.hpp"

class SaberUtils
{
    /// @brief hides objects on object with settings from item
    /// @param item gets the saberconfig into the method for checking the fake Glow disable
    /// @param object the objet on which to look
    void HideObjects(Qosmetics::SaberItem& item, UnityEngine::GameObject* object);
};