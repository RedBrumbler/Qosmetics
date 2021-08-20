#pragma once
#include "GlobalNamespace/SaberTrailRenderer.hpp"
#include "UnityEngine/Material.hpp"

class TrailUtils
{
    public:
        /// @brief creates a new trail renderer with the given material
        /// @param mat the material used on the trail renderer
        /// @return new Trail Renderer
        static GlobalNamespace::SaberTrailRenderer* NewTrailRenderer(UnityEngine::Material* mat);

        /// @brief makes trail attached to obj invisible
        /// @param obj the obj on which to remove the trail
        static void RemoveTrail(UnityEngine::Transform* obj);
};