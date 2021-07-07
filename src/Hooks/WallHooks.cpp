#include "Config.hpp"
#include "QosmeticsLogger.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"

#define INFO(value...) QosmeticsLogger::GetLogger().info(value);
#define ERROR(value...) QosmeticsLogger::GetLogger().error(value);

#include "GlobalNamespace/MirroredObstacleController.hpp"
#include "GlobalNamespace/ObstacleController.hpp"
#include "GlobalNamespace/ObstacleData.hpp"
#include "UnityEngine/Vector3.hpp"

#include "Types/Wall/Wall.hpp"

#include "Containers/SingletonContainer.hpp"

using namespace Qosmetics;
using namespace UnityEngine;

MAKE_HOOK_MATCH(MirroredObstacleController_Mirror, &GlobalNamespace::MirroredObstacleController::Mirror, void, GlobalNamespace::MirroredObstacleController* self, GlobalNamespace::ObstacleController* obstacleController)
{
    MirroredObstacleController_Mirror(self, obstacleController);
    
    if (config.wallConfig.disableReflections || SingletonContainer::get_wallManager()->get_item().get_type() != ItemType::invalid)  
    {
        self->set_enabled(false);
        return;
    }

    Qosmetics::Wall* wall = self->get_gameObject()->GetComponent<Qosmetics::Wall*>();

    if (!wall)
    {
        wall = self->get_gameObject()->AddComponent<Qosmetics::Wall*>();
        wall->Init(SingletonContainer::get_wallManager(), SingletonContainer::get_colorManager());
        wall->isMirror = true;
    }
    
    wall->obstacleController = obstacleController;
    wall->Replace();
}

MAKE_HOOK_MATCH(ObstacleController_Init, &GlobalNamespace::ObstacleController::Init, void, GlobalNamespace::ObstacleController* self, GlobalNamespace::ObstacleData* obstacleData, float worldRotation, Vector3 startPos, Vector3 midPos, Vector3 endPos, float move1Duration, float move2Duration, float singleLineWidth, float height)
{
    ObstacleController_Init(self, obstacleData, worldRotation, startPos, midPos, endPos, move1Duration, move2Duration, singleLineWidth, height);
    Qosmetics::Wall* wall = self->get_gameObject()->GetComponent<Qosmetics::Wall*>();

    if (!wall)
    {
        wall = self->get_gameObject()->AddComponent<Qosmetics::Wall*>();
        wall->Init(SingletonContainer::get_wallManager(), SingletonContainer::get_colorManager());
        wall->isMirror = false;
    }
    
    wall->obstacleController = self;
    wall->Replace();
}

void installWallHooks(LoggerContextObject& logger)
{
    INSTALL_HOOK(logger, ObstacleController_Init);
    INSTALL_HOOK(logger, MirroredObstacleController_Mirror);
}