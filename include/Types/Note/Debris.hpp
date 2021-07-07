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
    
    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, Replace);
    DECLARE_INSTANCE_METHOD(void, UpdateModel);
    DECLARE_INSTANCE_METHOD(void, UpdateColors);
    DECLARE_INSTANCE_METHOD(void, Restore);
    DECLARE_INSTANCE_METHOD(void, SetData, int colorType, UnityEngine::Vector3 cutPoint, UnityEngine::Vector3 cutNormal);
    DECLARE_INSTANCE_METHOD(void, Init, NoteManager* modelManager, ColorManager* colorManager);

    public:
        bool replaced[2] = {0, 0};
)