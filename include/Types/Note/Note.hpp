#pragma once

#include "custom-types/shared/macros.hpp"
#include "Types/Note/NoteManager.hpp"
#include "Types/Colors/ColorManager.hpp"
#include "UnityEngine/MonoBehaviour.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, Note, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::NoteManager*, modelManager, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::ColorManager*, colorManager, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(bool, replaced, false);
    
    DECLARE_METHOD(void, Awake);
    DECLARE_METHOD(void, Replace);
    DECLARE_METHOD(void, UpdateModel);
    DECLARE_METHOD(void, UpdateColors);
    DECLARE_METHOD(void, Restore);
    DECLARE_METHOD(void, Init, NoteManager* modelManager, ColorManager* colorManager);
    DECLARE_METHOD(void, SetupTrails);

    REGISTER_FUNCTION(Note,
        REGISTER_FIELD(modelManager);
        REGISTER_FIELD(colorManager);
        REGISTER_FIELD(replaced);

        REGISTER_METHOD(Awake);
        REGISTER_METHOD(Replace);
        REGISTER_METHOD(UpdateModel);
        REGISTER_METHOD(UpdateColors);
        REGISTER_METHOD(Restore);
        REGISTER_METHOD(Init);
        REGISTER_METHOD(SetupTrails);
    )
)