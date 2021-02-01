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

        static Array<UnityEngine::Material*>* GetMaterialArray(UnityEngine::Renderer* renderer);
};