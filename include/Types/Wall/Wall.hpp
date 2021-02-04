#pragma once

#include "custom-types/shared/macros.hpp"
#include "Types/Wall/WallManager.hpp"
#include "UnityEngine/MonoBehaviour.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, Wall, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::WallManager*, modelManager, nullptr);
    DECLARE_METHOD(void, Awake);
    DECLARE_METHOD(void, Replace);
    DECLARE_METHOD(void, UpdateModel);
    DECLARE_METHOD(void, Restore);
    DECLARE_METHOD(void, Init, WallManager* modelManager);
    
    REGISTER_FUNCTION(Wall,
        REGISTER_FIELD(modelManager);

        REGISTER_METHOD(Awake);
        REGISTER_METHOD(Replace);
        REGISTER_METHOD(UpdateModel);
        REGISTER_METHOD(Restore);
        REGISTER_METHOD(Init);
    )
)