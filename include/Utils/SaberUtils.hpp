#pragma once
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Color.hpp"

class SaberUtils
{
    public:
        /// @brief hides objects on object with settings from item
        /// @param object the objet on which to look
        /// @param enableFakeGlow should fake glow remain enabled?
        /// @param doHide This method can also re enable objects if you pass in false
        static void HideObjects(UnityEngine::GameObject* object, bool enableFakeGlow, bool doHide = true);

        /// @brief Sets the custom colors on the given object
        /// @param color the color that gets set on the _Color property
        /// @param otherColor the color that gets set on the _OtherColor property (if present)
        static void SetColors(UnityEngine::GameObject* object, UnityEngine::Color color, UnityEngine::Color otherColor);

        /// @brief Sets the saber Width value on the given transform
        /// @param saber obj to set on
        static void SetSaberSize(UnityEngine::Transform* saber);
};