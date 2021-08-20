#pragma once

#include "custom-types/shared/macros.hpp"
#include "Types/Saber/SaberManager.hpp"
#include "Types/Colors/ColorManager.hpp"
#include "GlobalNamespace/SaberType.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "GlobalNamespace/VRController.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, Pointer, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD_DEFAULT(GlobalNamespace::VRController*, vrController, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::SaberManager*, modelManager, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::ColorManager*, colorManager, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(GlobalNamespace::SaberType, pointerType, GlobalNamespace::SaberType::SaberA);
    DECLARE_INSTANCE_FIELD_DEFAULT(bool, replaced, false);
    
    DECLARE_CTOR(ctor);
    DECLARE_STATIC_METHOD(void, AddToAll);
    DECLARE_STATIC_METHOD(void, UpdateAll, bool reinstantiate = false);
    DECLARE_INSTANCE_METHOD(void, Replace, bool reinstantiate = false);
    DECLARE_INSTANCE_METHOD(void, UpdateModel, bool reinstantiate = false, bool firstUpdate = false);
    DECLARE_INSTANCE_METHOD(void, UpdateColors);
    DECLARE_INSTANCE_METHOD(void, Restore);
    DECLARE_INSTANCE_METHOD(void, Init, SaberManager* modelManager, ColorManager* colorManager);
    DECLARE_INSTANCE_METHOD(void, SetupTrails);
    DECLARE_INSTANCE_METHOD(void, OnApplicationFocus, bool hasFocus);
)