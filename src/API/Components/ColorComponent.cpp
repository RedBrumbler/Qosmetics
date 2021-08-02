#include "Types/Colors/ColorComponent.hpp"
#include "conditional-dependencies/shared/main.hpp"

EXPOSE_API(UpdateColors, void, Il2CppObject* colorComponent) {
    ((Qosmetics::ColorComponent*)colorComponent)->UpdateColors();
}

EXPOSE_API(SetColors, void, Il2CppObject* colorComponent, UnityEngine::Color leftColor, UnityEngine::Color rightColor) {
    ((Qosmetics::ColorComponent*)colorComponent)->SetColors(leftColor, rightColor);
}