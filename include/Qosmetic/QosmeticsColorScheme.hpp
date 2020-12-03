#pragma once
#include "custom-types/shared/types.hpp"
#include "custom-types/shared/macros.hpp"

#include "UnityEngine/Material.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "GlobalNamespace/ColorScheme.hpp"
#include "GlobalNamespace/ColorType.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, ColorScheme, GlobalNamespace::ColorScheme, 
    DECLARE_INSTANCE_FIELD_DEFAULT(UnityEngine::Color, noteAColor, UnityEngine::Color(1, 1, 1, 1));
    DECLARE_INSTANCE_FIELD_DEFAULT(UnityEngine::Color, noteBColor, UnityEngine::Color(1, 1, 1, 1));
    DECLARE_INSTANCE_FIELD_DEFAULT(UnityEngine::Color, trailAColor, UnityEngine::Color(1, 1, 1, 1));
    DECLARE_INSTANCE_FIELD_DEFAULT(UnityEngine::Color, trailBColor, UnityEngine::Color(1, 1, 1, 1));
    DECLARE_METHOD(void, Copy, GlobalNamespace::ColorScheme* orig);

    DECLARE_METHOD(void, SetLeftColor, UnityEngine::Color color);
    DECLARE_METHOD(void, SetRightColor, UnityEngine::Color color);
    
    DECLARE_CTOR(ctor, GlobalNamespace::ColorScheme* scheme);

    REGISTER_FUNCTION(ColorScheme,
        REGISTER_FIELD(noteAColor);
        REGISTER_FIELD(noteBColor);
        REGISTER_FIELD(trailAColor);
        REGISTER_FIELD(trailBColor);
        REGISTER_METHOD(Copy);
        REGISTER_METHOD(ctor);
    )
)