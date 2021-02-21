#pragma once

#include "custom-types/shared/macros.hpp"
#include "Zenject/DiContainer.hpp"
#include "UnityEngine/GameObject.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, FlowCoordinatorContainer, Il2CppObject,
    DECLARE_INSTANCE_FIELD_DEFAULT(UnityEngine::GameObject*, flowCoordinatorGO, nullptr);
    DECLARE_CTOR(ctor, Zenject::DiContainer* dicontainer);
    REGISTER_FUNCTION(FlowCoordinatorContainer,
        REGISTER_METHOD(ctor);
        REGISTER_FIELD(flowCoordinatorGO);
    )
)