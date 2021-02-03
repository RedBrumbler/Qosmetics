#pragma once

#include "Types/Qosmetic/ModelManager.hpp"
#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, Qosmetic, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD_DEFAULT(ModelManager*, modelManager, nullptr);
    DECLARE_METHOD(virtual void, Awake);
    DECLARE_METHOD(virtual void, Replace);
    DECLARE_METHOD(virtual void, UpdateModel);
    DECLARE_METHOD(virtual void, Restore);
    
    REGISTER_FUNCTION(Qosmetic,
        REGISTER_FIELD(modelManager);
        REGISTER_METHOD(Awake);
        REGISTER_METHOD(Replace);
        REGISTER_METHOD(UpdateModel);
        REGISTER_METHOD(Restore);
    )
)