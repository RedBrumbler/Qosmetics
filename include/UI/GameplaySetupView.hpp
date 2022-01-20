#pragma once

#include "UnityEngine/MonoBehaviour.hpp"
#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics::Core, GameplaySetupView, UnityEngine::MonoBehaviour,
                      DECLARE_INSTANCE_METHOD(void, DidActivate, bool firstActivation);
                      void FastForwardQosmeticsFlowCoordinator(System::Type* type);

)