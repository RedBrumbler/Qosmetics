#pragma once

#include "beatsaber-hook/shared/utils/typedefs.h"
#include "UnityEngine/MonoBehaviour.hpp"

#if defined __has_include && __has_include("sombrero/shared/ColorUtils.hpp")
#include "sombrero/shared/ColorUtils.hpp"
#define SOMBRERO
#else
#include "UnityEngine/Color.hpp"
#endif

#if defined __has_include && __has_include ("AltTrail.hpp")
#include "AltTrail.hpp"
#endif

#ifdef SOMBRERO
#define COLOR Sombrero::FastColor
#else
#define COLOR UnityEngine::Color
#endif

namespace Qosmetics
{
    class TrailHelper : public UnityEngine::MonoBehaviour
    {
        public:
            #if defined __has_include && __has_include ("AltTrail.hpp")
            Qosmetics::AltTrail* trailInstance;
            #else
            // Qosmetics::AltTrail*
            Il2CppObject* trailInstance;
            #endif
            // Qosmetics::ColorManager*
            Il2CppObject* colorManager;
            // GlobalNamespace::SaberModelController*
            Il2CppObject* parentModelController;
            
            void GetOrAddTrail(bool remake)
            {
                static auto function = CondDep::Find<void, Il2CppObject*, bool>("questcosmetics", "TrailHelper_GetOrAddTrail");
                if (!function) return;

                function.value()(this, remake);
            }

            void Init(Il2CppObject* colorManager, Il2CppObject* saberModelController)
            {
                static auto function = CondDep::Find<void, Il2CppObject*, , Il2CppObject*, , Il2CppObject*>("questcosmetics", "TrailHelper_Init");
                if (!function) return;

                function.value()(this, colorManager, saberModelController);
            }

            void TrailSetup()
            {
                static auto function = CondDep::Find<void, Il2CppObject*>("questcosmetics", "TrailHelper_TrailSetup");
                if (!function) return;

                function.value()(this);
            }

            void SetTrailActive(bool active)
            {
                static auto function = CondDep::Find<void, Il2CppObject*, bool>("questcosmetics", "TrailHelper_SetTrailActive");
                if (!function) return;

                function.value()(this, active);
            }

            void UpdateColors()
            {
                static auto function = CondDep::Find<void, Il2CppObject*>("questcosmetics", "TrailHelper_UpdateColors");
                if (!function) return;

                function.value()(this);
            }

            COLOR GetColor(int colorType)
            {
                static auto function = CondDep::Find<void, Il2CppObject*, int, COLOR&>("questcosmetics", "TrailHelper_GetColor");
                if (!function) return;
                COLOR out;
                function.value()(this, colorType, out);
                return out;
            }

            void SetColors(const COLOR& leftColor, const COLOR& rightColor)
            {
                static auto function = CondDep::Find<void, Il2CppObject*, const COLOR&, const COLOR&>("questcosmetics", "TrailHelper_SetColors");
                if (!function) return;
                
                function.value()(this, leftColor, rightColor);
            }

            int colorType;
            COLOR color;
            COLOR multiplier;
            int length;
            float whiteStep;

    };
}

DEFINE_IL2CPP_ARG_TYPE(Qosmetics::TrailHelper*, "Qosmetics", "TrailHelper");