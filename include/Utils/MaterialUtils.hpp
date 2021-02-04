#pragma once
#include "UnityEngine/Material.hpp"
#include "UnityEngine/Shader.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Renderer.hpp"

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
};