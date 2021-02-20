#pragma once

#include "custom-types/shared/macros.hpp"
#include "Types/Saber/SaberManager.hpp"
#include "Types/Colors/ColorManager.hpp"
#include "GlobalNamespace/SaberType.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "zenjeqt/shared/Zenjeqtor.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, Saber, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::SaberManager*, modelManager, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::ColorManager*, colorManager, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(GlobalNamespace::SaberType, saberType, GlobalNamespace::SaberType::SaberA);
    DECLARE_INSTANCE_FIELD_DEFAULT(bool, replaced, false);
    
    DECLARE_METHOD(void, Awake);
    DECLARE_METHOD(void, Replace);
    DECLARE_METHOD(void, UpdateModel, bool firstActivation = false);
    DECLARE_METHOD(void, UpdateColors);
    DECLARE_METHOD(void, Restore);
    DECLARE_METHOD(void, Init, SaberManager* modelManager, ColorManager* colorManager);
    DECLARE_METHOD(void, SetupTrails);

    REGISTER_FUNCTION(Saber,
        REGISTER_FIELD(modelManager);
        REGISTER_FIELD(colorManager);
        REGISTER_FIELD(saberType);
        REGISTER_FIELD(replaced);

        REGISTER_METHOD(Awake);
        REGISTER_METHOD(Replace);
        REGISTER_METHOD(UpdateModel);
        REGISTER_METHOD(UpdateColors);
        REGISTER_METHOD(Restore);
        REGISTER_METHOD_INJECT(Init);
        REGISTER_METHOD(SetupTrails);
    )
)