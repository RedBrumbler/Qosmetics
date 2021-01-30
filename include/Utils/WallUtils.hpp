#pragma once
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/MeshFilter.hpp"
#include "UnityEngine/Mesh.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/Color.hpp"

#include "GlobalNamespace/ColorManager.hpp"
#include "GlobalNamespace/SimpleColorSO.hpp"
 
#include "Utils/UnityUtils.hpp"
#include "Utils/MaterialUtils.hpp"
#include "Logging/WallLogger.hpp"

#include "Data/WallData.hpp"
#include "Config/WallConfig.hpp"

class WallUtils
{
    public:
        static Logger& getLogger()
        { 
            return Qosmetics::WallLogger::GetLogger();
        }

        /// @brief sets the obstacle colors
        /// @param customWall reference to the wall that needs it's colors set
        static void SetObstacleColors(Qosmetics::WallData &customWall);

        /// @brief disables fake glow
        /// @param obstacle root transform of the obstacle
        static void DisableFakeGlow(UnityEngine::Transform* obstacle);

        /// @brief disables the renderers that need to be disabled by default on the wall
        /// @param obstacle root transform of the obstacle
        static void DisableDefaults(UnityEngine::Transform* obstacle);

        /// @brief replaces the frame mesh in the obstacle
        /// @param obstacle root transform of the obstacle
        /// @param mesh the mesh to set
        static void ReplaceFrameMesh(UnityEngine::Transform* obstacle, UnityEngine::Mesh* mesh);

        /// @brief replaces the frame material in the obstacle
        /// @param obstacle root transform of the obstacle
        /// @param material the material to set
        static void ReplaceFrameMaterial(UnityEngine::Transform* obstacle, UnityEngine::Material* material);

        /// @brief replaces the core mesh in the obstacle
        /// @param obstacle root transform of the obstacle
        /// @param mesh the mesh to set
        static void ReplaceCoreMesh(UnityEngine::Transform* obstacle, UnityEngine::Mesh* mesh);

        /// @brief replaces the core material in the obstacle
        /// @param obstacle root transform of the obstacle
        /// @param material the material to set
        static void ReplaceCoreMaterial(UnityEngine::Transform* obstacle, UnityEngine::Material* material);

        /// @brief disables the frame meshrenderer
        /// @param obstacle root transform of the obstacle
        static void DisableFrame(UnityEngine::Transform* obstacle);

        /// @brief replaces an array of materials on the core object
        /// @param obstacle root transform of the obstacle
        /// @param materials array of materials to use
        static void ReplaceCoreSharedMaterials(UnityEngine::Transform* obstacle,  Array<UnityEngine::Material*>* materials);

        /// @brief replaces an array of materials on the frame object
        /// @param obstacle root transform of the obstacle
        /// @param materials array of materials to use
        static void ReplaceFrameSharedMaterials(UnityEngine::Transform* obstacle,  Array<UnityEngine::Material*>* materials);
        
        /// @brief sets the renderer enable bool to !value
        /// @param renderer the renderer to hide/unhide
        /// @param doHide wether to hide or to unhide, defaults to true
        static void HideRenderer(UnityEngine::Renderer* renderer, bool doHide = true)
        {
            if (renderer == nullptr)
            {
                getLogger().error("Tried disabling nullptr renderer, skipping disable");
                return;
            }
            
            renderer->set_enabled(!doHide); 
        }
        
    private:

        /// @brief sets the obstacle color to the _Color property of the given material
        /// @param material material on which the color will get set
        static void SetObstacleColor(UnityEngine::Material* material);
};