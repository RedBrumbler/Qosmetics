#pragma once
#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include <vector>

#include "GlobalNamespace/Saber.hpp"
#include "GlobalNamespace/SaberType.hpp"
#include "GlobalNamespace/ColorManager.hpp"

#include "UnityEngine/MeshFilter.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/Shader.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/GameObject.hpp"

#include "../Logging/SaberLogger.hpp"
#include "Utils/UnityUtils.hpp"
namespace Qosmetics
{
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
        private:
            /// @brief disables all the gameObjects of these meshfilters
            /// @param meshFilters array of meshfilters
            static void DisableMesh(Array<UnityEngine::MeshFilter*>* meshFilters);

            /// @brief sets the custom colors on all meshrenderers in the given transform
            /// @param transform the transform in which to get all renderers and set colors on
            /// @param saberType the color type to use for it
            static void setCustomColor(UnityEngine::Transform* transform, GlobalNamespace::SaberType saberType);
    };
}
