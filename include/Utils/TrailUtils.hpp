#pragma once
#include "Xft/XWeaponTrail.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/Object.hpp"
#include "UnityEngine/Resources.hpp"

#include <vector>

#include "Utils/UnityUtils.hpp"
#include "Data/CustomTrail.hpp"

#include "Logging/SaberLogger.hpp"

namespace Qosmetics 
{
    class TrailUtils
    {
        public:
            static const Logger& getLogger()
            {
                return SaberLogger::GetLogger();
            }
            
            /// @brief sets all found xft::weapontrails to disabled
            static void ResetTrails();

            /// @brief moves trail from basicsabermodel to customsaber
            /// @param basicSaberModel the source from which to move stuff
            /// @param customSaber the destination where stuff will get moved
            static void MoveTrail(UnityEngine::Transform* basicSaberModel, UnityEngine::Transform* customSaber);

            /// @brief adds a trail to the trail object in the customSaber object
            /// @param trail reference to the trail to set
            /// @param customSaber transform of the custom saber
            /// @param trailRendererPrefab prefab of a trail renderer to be set on the new trail
            static void AddTrail(Qosmetics::CustomTrail &trail, UnityEngine::Transform* customSaber);

            /// @brief sets the xweapontrail component disabled
            /// @param basicSaberModel transform on which to set the script to disabled
            static void RemoveTrail(UnityEngine::Transform* basicSaberModel);
    };
}