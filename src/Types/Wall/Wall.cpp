#include "Config.hpp"
#include "Types/Wall/Wall.hpp"
#include "QosmeticsLogger.hpp"
#include "chroma/shared/ObstacleAPI.hpp"

#include "UnityEngine/MeshFilter.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/Renderer.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/GameObject.hpp"

#include "GlobalNamespace/StretchableObstacle.hpp"
#include "GlobalNamespace/ParametricBoxFrameController.hpp"

#include "Utils/WallUtils.hpp"

DEFINE_TYPE(Qosmetics, Wall);

#define INFO(value...) QosmeticsLogger::GetContextLogger("Wall").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("Wall").error(value)

using namespace UnityEngine;

namespace Qosmetics
{
    void Wall::Awake()
    {
        replaced = false;
    }

    void Wall::Replace()
    {
        if (replaced) 
        {
            UpdateModel();
            return;
        }

        if (!modelManager || modelManager->get_type() != ItemType::wall || modelManager->get_item().get_type() != ItemType::wall)
        {
            ERROR("Wall model manager was nullptr or itemtype was invalid");
            replaced = true;
            return;
        }

        WallConfig& wallConfig = modelManager->get_item().get_config();

        WallReferences wallreferences = WallUtils::PrepareAndGatherReferences(get_transform());
        
        Renderer* coreRenderer = wallreferences.coreRenderer;
        Renderer* frameRenderer = wallreferences.frameRenderer;
        Renderer* fakeGlowRenderer = wallreferences.fakeGlowRenderer;

        MeshFilter* coreFilter = wallreferences.coreFilter;
        MeshFilter* frameFilter = wallreferences.frameFilter;

        if (!config.wallConfig.forceCoreOff) // should the core just be gone?
        {
            if (wallConfig.get_replaceCoreMaterial() && coreRenderer) // replace core material
            {
                coreRenderer->SetMaterialArray(modelManager->get_coreMaterialArray());
            }

            if (wallConfig.get_replaceCoreMesh() && coreFilter) // whether we should replace the core mesh
            {
                coreFilter->set_mesh(modelManager->get_coreMesh());
            }
        }

        if (!(wallConfig.get_disableFrame() || config.wallConfig.forceFrameOff)) // whether we force the frame off
        {
            if (wallConfig.get_replaceFrameMaterial() && frameRenderer) // whether we should replace the frame material
            {
                frameRenderer->SetMaterialArray(modelManager->get_frameMaterialArray());
            }

            if (wallConfig.get_replaceFrameMesh() && frameFilter) // whether we should replace the frame mesh
            {
                frameFilter->set_mesh(modelManager->get_frameMesh());
            }
        }
        replaced = true;
        UpdateModel();
    }

    void Wall::UpdateModel()
    {
        if (!replaced) return;

        WallConfig& wallConfig = modelManager->get_item().get_config();

        WallReferences wallreferences = WallUtils::GatherReferences(get_transform());
        
        Renderer* coreRenderer = wallreferences.coreRenderer;
        Renderer* frameRenderer = wallreferences.frameRenderer;
        Renderer* fakeGlowRenderer = wallreferences.fakeGlowRenderer;

        MeshFilter* coreFilter = wallreferences.coreFilter;
        MeshFilter* frameFilter = wallreferences.frameFilter;

        if (config.wallConfig.forceCoreOff && coreRenderer) // should the core just be gone?
        {
            coreRenderer->set_enabled(false);
        }

        if ((wallConfig.get_disableFrame() || config.wallConfig.forceFrameOff) && frameRenderer && fakeGlowRenderer) // whether we force the frame off
        {
            frameRenderer->set_enabled(false);
            fakeGlowRenderer->set_enabled(false);
        }
        else if ((wallConfig.get_disableFakeGlow() || config.wallConfig.forceFakeGlowOff) && fakeGlowRenderer) // whether we should disable the fake glow on the frame
        {
            fakeGlowRenderer->set_enabled(false);
        }

        UpdateColors();
    }

    void Wall::UpdateColors()
    {
        auto optionalColor = Chroma::ObstacleAPI::getObstacleControllerColorSafe(obstacleController);

        Color color = optionalColor ? *optionalColor : colorManager->ColorForObstacle();

        GlobalNamespace::StretchableObstacle* stretchableObstacle = obstacleController->stretchableObstacle;
        GlobalNamespace::ParametricBoxFrameController* frameController = stretchableObstacle->obstacleFrame;

        stretchableObstacle->SetSizeAndColor(frameController->width, frameController->height, frameController->length, color);
        WallUtils::SetColors(get_gameObject(), color, isMirror ? 1955 : 0);
    }

    void Wall::Restore()
    {

    }

    void Wall::Init(WallManager* modelManager, ColorManager* colorManager)
    {
        this->modelManager = modelManager;
        this->colorManager = colorManager;
    }
}