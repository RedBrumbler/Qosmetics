#pragma once

#include "custom-types/shared/macros.hpp"
#include "Types/Note/NoteManager.hpp"
#include "Types/Colors/ColorManager.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/Vector3.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, Debris, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::NoteManager*, modelManager, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::ColorManager*, colorManager, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(int, colorType, 0);
    DECLARE_INSTANCE_FIELD(UnityEngine::Vector3, cutPoint);
    DECLARE_INSTANCE_FIELD(UnityEngine::Vector3, cutNormal);
    
    DECLARE_METHOD(void, Awake);
    DECLARE_METHOD(void, Replace);
    DECLARE_METHOD(void, UpdateModel);
    DECLARE_METHOD(void, UpdateColors);
    DECLARE_METHOD(void, Restore);
    DECLARE_METHOD(void, SetData, int colorType, UnityEngine::Vector3 cutPoint, UnityEngine::Vector3 cutNormal);
    DECLARE_METHOD(void, Init, NoteManager* modelManager, ColorManager* colorManager);

    public:
        bool replaced[2] = {0, 0};

    REGISTER_FUNCTION(
        REGISTER_FIELD(modelManager);
        REGISTER_FIELD(colorManager);
        REGISTER_FIELD(colorType);
        REGISTER_FIELD(cutPoint);
        REGISTER_FIELD(cutNormal);

        REGISTER_METHOD(Awake);
        REGISTER_METHOD(Replace);
        REGISTER_METHOD(UpdateModel);
        REGISTER_METHOD(UpdateColors);
        REGISTER_METHOD(Restore);
        REGISTER_METHOD(Init);
        REGISTER_METHOD(SetData);
    )
)