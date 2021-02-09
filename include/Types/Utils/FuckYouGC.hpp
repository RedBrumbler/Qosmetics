#pragma once

#include "custom-types/shared/macros.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/MonoBehaviour.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, FuckYouGC, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD_DEFAULT(UnityEngine::GameObject*, prefab, nullptr);
    DECLARE_METHOD(void, Awake);
    
    REGISTER_FUNCTION(FuckYouGC,
        REGISTER_METHOD(Awake);
        REGISTER_FIELD(prefab); 
    )
)