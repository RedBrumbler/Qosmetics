#include "Types/Colors/ColorComponent.hpp"
#include "conditional-dependencies/shared/main.hpp"

EXPOSE_API(ColorComponent_UpdateColors, void, Il2CppObject* colorComponent) {
    ((Qosmetics::ColorComponent*)colorComponent)->UpdateColors();
}

EXPOSE_API(ColorComponent_SetColors, void, Il2CppObject* colorComponent, UnityEngine::Color leftColor, UnityEngine::Color rightColor) {
    ((Qosmetics::ColorComponent*)colorComponent)->SetColors(leftColor, rightColor);
}