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
        /// @param renderQueue if given, will also set the renderqueue since we're already going through all the materials anyways
        static void SetColors(UnityEngine::GameObject* object, UnityEngine::Color color, UnityEngine::Color otherColor, bool checkName = false, int renderQueue = 0);

        /// @brief sets the custom colors on the given object, and possibly checking names for noCC
        /// @param object the object on which the colors get set
        /// @param color the main color
        /// @param checkName whether to check for the names
        /// @param renderQueue if given, will also set the renderqueue since we're already going through all the materials anyways
        static void SetColors(UnityEngine::GameObject* object, UnityEngine::Color color, bool checkName = false, int renderQueue = 0);

        /// @brief sets the custom colors on the given object, and possibly checking names for noCC
        /// @param object the object on which the colors get set
        /// @param renderQueue the renderQueue to set
        static void SetRenderQueue(UnityEngine::GameObject* object, int renderQueue);

        /// @brief replaces all materials with _replace in the name on given gameobject
        static void ReplaceMaterialsForGameObject(UnityEngine::GameObject* object);

        /// @brief replaces the material on all renderers in gameobject
        static void ReplaceMaterialForGameObjectChildren(UnityEngine::GameObject* gameObject, UnityEngine::Material* material, std::string materialToReplaceName);
        
        /// @brief tries to replace all materials on given renderer
        static void ReplaceMaterialForRenderer(UnityEngine::Renderer* renderer, UnityEngine::Material* replacingMaterial, std::string materialToReplaceName);

};