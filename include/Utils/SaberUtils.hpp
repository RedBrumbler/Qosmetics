#pragma once
#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include <vector>

#include "GlobalNamespace/Saber.hpp"
#include "GlobalNamespace/SaberType.hpp"
#include "GlobalNamespace/ColorManager.hpp"
#include "GlobalNamespace/SaberModelController.hpp"

#include "UnityEngine/MeshFilter.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/Shader.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/XR/XRNode.hpp"


#include "../Logging/SaberLogger.hpp"
#include "Utils/UnityUtils.hpp"
namespace Qosmetics
{
    class SaberData;
    class SaberUtils 
    {
        public:
            const static Logger& getLogger()
            {
                return SaberLogger::GetLogger();
            } 

            /// @brief replaces the given saber with a gameboject prefab
            /// @param gameSaber saber instance to replace
            /// @param customSaber custom saber prefab to use
            static void ReplaceSaber(GlobalNamespace::Saber* gameSaber, UnityEngine::GameObject* customSaber);

            /// @brief adds the correct saber type to the transform of saberscript
            /// @param saberScript the saber script on the normal saber
            /// @param customSaber reference to the custom saberdata object to use
            static void AddSaber(GlobalNamespace::Saber* saberScript, Qosmetics::SaberData& customSaberData);


            /// @param handles the color update "event" from the Qosmetics color manager
            /// @param customSaberData the saber data for the object
            static void HandleColorsDidUpdateEvent(Qosmetics::SaberData& customSaberData);
            
            /// @brief checks wether or not a material should have it's color changed based on CC
            /// @param mat the material to check
            static bool ShouldChangeSaberMaterialColor(UnityEngine::Material* mat);

            /// @brief Adds the correct saber as a child to the parent
            /// @param parent the parent of the new object
            /// @param isLeft wether or not this is the left saber
            /// @param saberData the custom saber data to use
            static void AddMenuPointerSaber(UnityEngine::Transform* parent, bool isLeft, SaberData& saberData);
            
            /// @brief sets the custom colors on all meshrenderers in the given transform
            /// @param transform the transform in which to get all renderers and set colors on
            /// @param saberType the color type to use for it
            static void SetCustomColor(UnityEngine::Transform* transform, GlobalNamespace::SaberType saberType);

            static void SetSaberSize(UnityEngine::Transform* object);

            static void RevertMenuPointer(UnityEngine::Transform* controller, UnityEngine::XR::XRNode node);
        private:
            /// @brief disables all the gameObjects of these meshfilters
            /// @param meshFilters array of meshfilters
            static void DisableMesh(Array<UnityEngine::MeshFilter*>* meshFilters, bool enableFakeGlow);

            /// @brief sets the custom colors on all meshrenderers in the given transform
            /// @param vector the array that will completely be set
            /// @param saberType the color type to use for it
            static void SetCustomColor(std::vector<UnityEngine::Material*>& vector, GlobalNamespace::SaberType saberType);
    };
}
