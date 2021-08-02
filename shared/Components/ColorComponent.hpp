#pragma once

#include "beatsaber-hook/shared/utils/typedefs.h"
#include "UnityEngine/MonoBehaviour.hpp"

#ifndef qosm_id
#define qosm_id "questcosmetics"
#endif

namespace Qosmetics
{
    class ColorComponent : public UnityEngine::MonoBehaviour
    {
        /// @brief give a left and right color to use for this object
        void SetColors(const UnityEngine::Color& leftColor, const UnityEngine::Color& rightColor)
        {
            static auto function = CondDep::Find<void, Il2CppObject*, UnityEngine::Color, UnityEngine::Color>(qosm_id, "SetColors");
            if (!function) return;

            function.value()(this, leftColor, rightColor);
        }

        /// @brief call this to let qosmetics handle the colors itself
        void UpdateColors()
        {
            static auto function = CondDep::Find<void, Il2CppObject*>(qosm_id, "UpdateColors");
            if (!function) return;

            function.value()(this);
        }
        
        Il2CppObject* colorManager;
        int objectType;
        int colorType;
    }
}
#undef qosm_id
DEFINE_IL2CPP_ARG_TYPE(Qosmetics::ColorComponent*, "Qosmetics", "ColorComponent");
