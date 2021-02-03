#pragma once
#include "UnityEngine/GameObject.hpp"

class SaberUtils
{
    public:
        /// @brief hides objects on object with settings from item
        /// @param object the objet on which to look
        /// @param bool should fake glow remain enabled?
        static void HideObjects(UnityEngine::GameObject* object, bool enableFakeGlow);
};