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
    
    DECLARE_METHOD(void, Awake);
    DECLARE_METHOD(static void, AddToAll);
    DECLARE_METHOD(static void, UpdateAll, bool reinstantiate = false);
    DECLARE_METHOD(void, Replace, bool reinstantiate = false);
    DECLARE_METHOD(void, UpdateModel, bool reinstantiate = false, bool firstUpdate = false);
    DECLARE_METHOD(void, UpdateColors);
    DECLARE_METHOD(void, Restore);
    DECLARE_METHOD(void, Init, SaberManager* modelManager, ColorManager* colorManager);
    DECLARE_METHOD(void, SetupTrails);
    DECLARE_METHOD(void, OnApplicationFocus, bool hasFocus);

    REGISTER_FUNCTION(
        REGISTER_FIELD(vrController);
        REGISTER_FIELD(modelManager);
        REGISTER_FIELD(colorManager);
        REGISTER_FIELD(pointerType);
        REGISTER_FIELD(replaced);

        REGISTER_METHOD(Awake);
        REGISTER_METHOD(UpdateAll);
        REGISTER_METHOD(Replace);
        REGISTER_METHOD(UpdateModel);
        REGISTER_METHOD(UpdateColors);
        REGISTER_METHOD(Restore);
        REGISTER_METHOD(Init);
        REGISTER_METHOD(SetupTrails);
        REGISTER_METHOD(OnApplicationFocus);
    )
)