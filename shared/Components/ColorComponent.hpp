#pragma once

#include "beatsaber-hook/shared/utils/typedefs.h"
#include "UnityEngine/MonoBehaviour.hpp"

#if defined __has_include && __has_include("sombrero/shared/ColorUtils.hpp")
#include "sombrero/shared/ColorUtils.hpp"
#define SOMBRERO
#else
#include "UnityEngine/Color.hpp"
#endif

#ifndef qosm_id
#define qosm_id "questcosmetics"
#endif

namespace Qosmetics
{
    class ColorComponent : public UnityEngine::MonoBehaviour
    {
        public:
            #ifdef SOMBRERO
            /// @brief give a left and right fastcolor to use for this object
            void SetColors(const Sombrero::FastColor& leftColor, const Sombrero::FastColor& rightColor)
            {
                static auto function = CondDep::Find<void, Il2CppObject*, Sombrero::FastColor, Sombrero::FastColor>(qosm_id, "ColorComponent_SetColors");
            #else
            /// @brief give a left and right color to use for this object
            void SetColors(const UnityEngine::Color& leftColor, const UnityEngine::Color& rightColor)
            {
                static auto function = CondDep::Find<void, Il2CppObject*, UnityEngine::Color, UnityEngine::Color>(qosm_id, "ColorComponent_SetColors");
            #endif
                if (!function) return;

                function.value()(this, leftColor, rightColor);
            }

            /// @brief call this to let qosmetics handle the colors itself
            void UpdateColors()
            {
                static auto function = CondDep::Find<void, Il2CppObject*>(qosm_id, "ColorComponent_UpdateColors");
                if (!function) return;

                function.value()(this);
            }
            // Qosmetics::ColorManager*
            Il2CppObject* colorManager;
            // 0 saber
            // 1 note
            // 2 wall
            int objectType;
            // 0 left
            // 1 right
            int colorType;
    };
}
#undef qosm_id
DEFINE_IL2CPP_ARG_TYPE(Qosmetics::ColorComponent*, "Qosmetics", "ColorComponent");
