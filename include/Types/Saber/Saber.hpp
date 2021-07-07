#pragma once

#include "custom-types/shared/macros.hpp"
#include "Types/Saber/SaberManager.hpp"
#include "Types/Colors/ColorManager.hpp"
#include "GlobalNamespace/SaberType.hpp"
#include "GlobalNamespace/SaberModelController.hpp"
#include "UnityEngine/MonoBehaviour.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, Saber, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::SaberManager*, modelManager, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::ColorManager*, colorManager, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(GlobalNamespace::SaberType, saberType, GlobalNamespace::SaberType::SaberA);
    DECLARE_INSTANCE_FIELD_DEFAULT(bool, replaced, false);
    DECLARE_INSTANCE_FIELD(GlobalNamespace::SaberModelController*, modelController);
    
    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, OnDestroy);
    DECLARE_INSTANCE_METHOD(void, Replace);
    DECLARE_INSTANCE_METHOD(void, UpdateModel, bool firstActivation = false);
    DECLARE_INSTANCE_METHOD(void, UpdateColors);
    DECLARE_INSTANCE_METHOD(void, UpdateChromaColors, int, GlobalNamespace::SaberModelController*, UnityEngine::Color);
    DECLARE_INSTANCE_METHOD(void, Restore);
    DECLARE_INSTANCE_METHOD(void, Init, SaberManager* modelManager, ColorManager* colorManager);
    DECLARE_INSTANCE_METHOD(void, SetupTrails);
)