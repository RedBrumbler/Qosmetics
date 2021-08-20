#pragma once

#include "custom-types/shared/macros.hpp"
#include "Types/Wall/WallManager.hpp"
#include "Types/Colors/ColorManager.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "GlobalNamespace/ObstacleController.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, Wall, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD_DEFAULT(GlobalNamespace::ObstacleController*, obstacleController, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::WallManager*, modelManager, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::ColorManager*, colorManager, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(bool, replaced, false);
    DECLARE_INSTANCE_FIELD_DEFAULT(bool, isMirror, false);
    
    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, OnEnable);
    DECLARE_INSTANCE_METHOD(void, Replace);
    DECLARE_INSTANCE_METHOD(void, UpdateModel);
    DECLARE_INSTANCE_METHOD(void, UpdateColors);
    DECLARE_INSTANCE_METHOD(void, Restore);
    DECLARE_INSTANCE_METHOD(void, Init, WallManager* modelManager, ColorManager* colorManager);
)