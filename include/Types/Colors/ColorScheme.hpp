#pragma once
#include "custom-types/shared/macros.hpp"

#include "UnityEngine/Color.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "GlobalNamespace/ColorScheme.hpp"
#include "GlobalNamespace/ColorType.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, ColorScheme, GlobalNamespace::ColorScheme, 
    DECLARE_INSTANCE_FIELD_DEFAULT(UnityEngine::Color, noteAColor, UnityEngine::Color(1, 1, 1, 1));
    DECLARE_INSTANCE_FIELD_DEFAULT(UnityEngine::Color, noteBColor, UnityEngine::Color(1, 1, 1, 1));
    DECLARE_INSTANCE_FIELD_DEFAULT(UnityEngine::Color, trailAColor, UnityEngine::Color(1, 1, 1, 1));
    DECLARE_INSTANCE_FIELD_DEFAULT(UnityEngine::Color, trailBColor, UnityEngine::Color(1, 1, 1, 1));
    DECLARE_INSTANCE_METHOD(void, CopyFromBaseGame, GlobalNamespace::ColorScheme* orig);
    DECLARE_INSTANCE_METHOD(void, CopyFromCustom, Qosmetics::ColorScheme* orig);
    DECLARE_INSTANCE_METHOD(void, SetLeftColor, UnityEngine::Color color);
    DECLARE_INSTANCE_METHOD(void, SetRightColor, UnityEngine::Color color);
    DECLARE_CTOR(ctor);
    DECLARE_CTOR(ctorbasegame, GlobalNamespace::ColorScheme* scheme);
    DECLARE_CTOR(ctorcustom, Qosmetics::ColorScheme* scheme);
)