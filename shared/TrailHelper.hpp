#pragma once

#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/Color.hpp"



#if defined __has_include && __has_include ("conditional-dependencies/shared/main.hpp")
#include "conditional-dependencies/shared/main.hpp"
#define HAS_CONDEP
#endif

#if defined __has_include && __has_include ("AltTrail.hpp")
#include "AltTrail.hpp"
#endif

namespace Qosmetics
{
    class TrailHelper : UnityEngine::MonoBehaviour
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
                auto* ptr = il2cpp_utils::FindMethodUnsafe("Qosmetics", "TrailHelper", "GetOrAddTrail", 1);
                if (ptr) il2cpp_utils::RunMethod(this, ptr, remake);
            }

            void Init(Il2CppObject* colorManager)
            {
                auto* ptr = il2cpp_utils::FindMethodUnsafe("Qosmetics", "TrailHelper", "Init", 1);
                if (ptr) il2cpp_utils::RunMethod(this, ptr, colorManager);
            }

            void TrailSetup()
            {
                auto* ptr = il2cpp_utils::FindMethodUnsafe("Qosmetics", "TrailHelper", "TrailSetup", 0);
                if (ptr) il2cpp_utils::RunMethod(this, ptr);
            }

            #ifndef HAS_CONDEP
            void SetTrailActive(bool active)
            {
                auto* ptr = il2cpp_utils::FindMethodUnsafe("Qosmetics", "TrailHelper", "SetTrailActive", 1);
                if (ptr) il2cpp_utils::RunMethod(this, ptr, active);
            }
            #else
            void SetTrailActive(bool active)
            {
                static auto function = CondDep::Find<void, Il2CppObject*, bool>("questcosmetics", "SetTrailActive");
                if (!function) return;

                function.value()(this, active);
            }
            #endif

            void UpdateColors()
            {
                auto* ptr = il2cpp_utils::FindMethodUnsafe("Qosmetics", "TrailHelper", "UpdateColors", 0);
                if (ptr) il2cpp_utils::RunMethod(this, ptr);
            }

            UnityEngine::Color GetColor(int colorType)
            {
                auto* ptr = il2cpp_utils::FindMethodUnsafe("Qosmetics", "TrailHelper", "GetColor", 1);
                if (ptr) return il2cpp_utils::RunMethod<UnityEngine::Color>(this, ptr, colorType);
                else return {1.0f, 1.0f, 1.0f, 1.0f};
            }

            int colorType;
            UnityEngine::Color color;
            UnityEngine::Color multiplier;
            int length;
            float whiteStep;

    };
}

DEFINE_IL2CPP_ARG_TYPE(Qosmetics::TrailHelper*, "Qosmetics", "TrailHelper");