#pragma once
#include "UnityEngine/Material.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Renderer.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/Color.hpp"

class MaterialUtils
{
    public:
        /// @brief prewarms all shaders on the object
        /// @param object object on which to prewarm
        static void PrewarmAllShadersOnObject(UnityEngine::GameObject* object);
        
        /// @brief gets the material array from the passed in renderer
        /// @param renderer the renderer from which to get the array
        /// @returns material array
        static Array<UnityEngine::Material*>* GetMaterials(UnityEngine::Renderer* renderer);

        /// @brief checks whether this material should get CC
        /// @param mat the material to check out
        /// @param checkName whether to check the name as well for _noCC
        static bool ShouldCC(UnityEngine::Material* mat, bool checkName = false);

        /// @brief sets the custom colors on the given object, and possibly checking names for noCC
        /// @param object the object on which the colors get set
        /// @param color the main color
        /// @param otherColor the secondary color
        /// @param checkName whether to check for the names
        static void SetColors(UnityEngine::GameObject* object, UnityEngine::Color color, UnityEngine::Color otherColor, bool checkName = false);
};