#pragma once

#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/GameObject.hpp"
#include "Types/Wall/WallManager.hpp"
#include "Types/Colors/ColorManager.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics::UI, WallPreviewElement, UnityEngine::MonoBehaviour,
        DECLARE_INSTANCE_FIELD_DEFAULT(WallManager*, modelManager, nullptr);
        DECLARE_INSTANCE_FIELD_DEFAULT(ColorManager*, colorManager, nullptr);
        DECLARE_INSTANCE_FIELD_DEFAULT(UnityEngine::GameObject*, prefab, nullptr);

        DECLARE_INSTANCE_METHOD(void, Init, WallManager* modelManager, ColorManager* colorManager);
        DECLARE_INSTANCE_METHOD(void, UpdatePreview, bool reinstantiate = false);
        DECLARE_INSTANCE_METHOD(void, ClearPreview);
)