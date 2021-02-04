#pragma once
#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/Color.hpp"

#include "GlobalNamespace/ColorManager.hpp"
#include "Types/Colors/ColorScheme.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, ColorManager, UnityEngine::MonoBehaviour, 
    DECLARE_METHOD(void, SetColorScheme, GlobalNamespace::ColorScheme*);
    DECLARE_METHOD(void, SetColorScheme, Qosmetics::ColorScheme*);
    DECLARE_METHOD(void, Awake);

    DECLARE_METHOD(void, SetLeftSaberColor, UnityEngine::Color color);
    DECLARE_METHOD(void, SetRightSaberColor, UnityEngine::Color color);

    DECLARE_METHOD(void, SetLeftTrailColor, UnityEngine::Color color);
    DECLARE_METHOD(void, SetRightTrailColor, UnityEngine::Color color);

    DECLARE_METHOD(void, SetLeftNoteColor, UnityEngine::Color color);
    DECLARE_METHOD(void, SetRightNoteColor, UnityEngine::Color color);

    DECLARE_METHOD(void, SetLeftColor, UnityEngine::Color color);
    DECLARE_METHOD(void, SetRightColor, UnityEngine::Color color);

    DECLARE_METHOD(void, SetObstacleColor, UnityEngine::Color color);
    DECLARE_METHOD(void, UpdateAllColors);
    
    DECLARE_METHOD(UnityEngine::Color, ColorForNoteType, GlobalNamespace::ColorType type);
    DECLARE_METHOD(UnityEngine::Color, ColorForSaberType, GlobalNamespace::ColorType type);
    DECLARE_METHOD(UnityEngine::Color, ColorForTrailType, GlobalNamespace::ColorType type);
    DECLARE_METHOD(UnityEngine::Color, ColorForObstacle);

    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::ColorScheme*, colorScheme, nullptr);

    REGISTER_FUNCTION(ColorManager,
        REGISTER_METHOD(SetColorScheme);
        REGISTER_METHOD(Awake);

        REGISTER_METHOD(SetLeftSaberColor);
        REGISTER_METHOD(SetRightSaberColor);

        REGISTER_METHOD(SetLeftTrailColor);
        REGISTER_METHOD(SetRightTrailColor);

        REGISTER_METHOD(SetLeftNoteColor);
        REGISTER_METHOD(SetRightNoteColor);

        REGISTER_METHOD(SetLeftColor);
        REGISTER_METHOD(SetRightColor);

        REGISTER_METHOD(SetObstacleColor);
        REGISTER_METHOD(UpdateAllColors);

        REGISTER_METHOD(ColorForSaberType);
        REGISTER_METHOD(ColorForNoteType);
        REGISTER_METHOD(ColorForTrailType);
        REGISTER_METHOD(ColorForObstacle);
        
        REGISTER_FIELD(colorScheme);
    )
)