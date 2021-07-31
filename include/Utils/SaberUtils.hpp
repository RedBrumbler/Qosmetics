#pragma once
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Color.hpp"

namespace SaberUtils
{
    /// @brief hides objects on object with settings from item
    /// @param object the objet on which to look
    /// @param enableFakeGlow should fake glow remain enabled?
    /// @param doHide This method can also re enable objects if you pass in false
    void HideObjects(UnityEngine::GameObject* object, bool enableFakeGlow, bool doHide = true);

    /// @brief Sets the custom colors on the given object
    /// @param color the color that gets set on the _Color property
    /// @param otherColor the color that gets set on the _OtherColor property (if present)
    void SetColors(UnityEngine::GameObject* object, UnityEngine::Color color, UnityEngine::Color otherColor);

    /// @brief Sets the saber Width value on the given transform
    /// @param saber obj to set on
    void SetSaberSize(UnityEngine::Transform* saber);

    /// @brief Makes a dummy saber, meaning it creates a saber model with appropriate trails
    /// @param saberType 0 left 1 right rest nullptr
    UnityEngine::Transform* MakeDummySaber(int saberType);
};