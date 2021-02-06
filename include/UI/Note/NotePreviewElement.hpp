#pragma once

#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/GameObject.hpp"
#include "Types/Note/NoteManager.hpp"
#include "Types/Colors/ColorManager.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics::UI, NotePreviewElement, UnityEngine::MonoBehaviour,
        DECLARE_INSTANCE_FIELD_DEFAULT(NoteManager*, modelManager, nullptr);
        DECLARE_INSTANCE_FIELD_DEFAULT(ColorManager*, colorManager, nullptr);
        DECLARE_INSTANCE_FIELD_DEFAULT(UnityEngine::GameObject*, prefab, nullptr);

        DECLARE_METHOD(void, Init, NoteManager* modelManager, ColorManager* colorManager);
        DECLARE_METHOD(void, UpdatePreview);
        DECLARE_METHOD(void, ClearPreview);

    REGISTER_FUNCTION(NotePreviewElement, 
        REGISTER_FIELD(modelManager);
        REGISTER_FIELD(colorManager);

        REGISTER_METHOD(Init);
        REGISTER_METHOD(UpdatePreview);
        REGISTER_METHOD(ClearPreview);
    )
)