#pragma once

#include "custom-types/shared/macros.hpp" 
#include "UnityEngine/MonoBehaviour.hpp"
#include "sombrero/shared/ColorUtils.hpp"
#include "Types/Colors/ColorManager.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, ColorComponent, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_METHOD(void, UpdateColors);
    DECLARE_INSTANCE_METHOD(void, SetColors, Sombrero::FastColor leftColor, Sombrero::FastColor rightColor);
    DECLARE_INSTANCE_FIELD(Qosmetics::ColorManager*, colorManager);

    void Init(Qosmetics::ColorManager* colorManager, int objectType, int colorType);
    void SetSaberColors(const Sombrero::FastColor& leftColor, const Sombrero::FastColor& rightColor);
    void SetNoteColors(const Sombrero::FastColor& leftColor, const Sombrero::FastColor& rightColor);
    void SetWallColor(const Sombrero::FastColor& color);

    void UpdateSaberColors();
    void UpdateNoteColors();
    void UpdateWallColors();

    int objectType;
    int colorType;
)
