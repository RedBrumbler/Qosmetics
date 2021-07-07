#pragma once

#include "Types/Qosmetic/ModelManager.hpp"
#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, Qosmetic, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD_DEFAULT(ModelManager*, modelManager, nullptr);
    DECLARE_INSTANCE_METHOD(virtual void, Awake);
    DECLARE_INSTANCE_METHOD(virtual void, Replace);
    DECLARE_INSTANCE_METHOD(virtual void, UpdateModel);
    DECLARE_INSTANCE_METHOD(virtual void, Restore);
)