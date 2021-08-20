#pragma once
#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/Color.hpp"

#include "GlobalNamespace/ColorManager.hpp"
#include "Types/Colors/ColorScheme.hpp"

#include <functional>
#include <vector>
enum callbackType {
            saber,
            trail,
            note,
            wall,
            pointer
        };

DECLARE_CLASS_CODEGEN(Qosmetics, ColorManager, Il2CppObject, 
    DECLARE_INSTANCE_METHOD(void, SetColorSchemeFromBase, GlobalNamespace::ColorScheme* base);
    DECLARE_INSTANCE_METHOD(void, SetColorSchemeFromCustom, Qosmetics::ColorScheme* base);

    DECLARE_INSTANCE_METHOD(void, SetLeftSaberColor, UnityEngine::Color color);
    DECLARE_INSTANCE_METHOD(void, SetRightSaberColor, UnityEngine::Color color);

    DECLARE_INSTANCE_METHOD(void, SetLeftTrailColor, UnityEngine::Color color);
    DECLARE_INSTANCE_METHOD(void, SetRightTrailColor, UnityEngine::Color color);

    DECLARE_INSTANCE_METHOD(void, SetLeftNoteColor, UnityEngine::Color color);
    DECLARE_INSTANCE_METHOD(void, SetRightNoteColor, UnityEngine::Color color);

    DECLARE_INSTANCE_METHOD(void, SetLeftColor, UnityEngine::Color color);
    DECLARE_INSTANCE_METHOD(void, SetRightColor, UnityEngine::Color color);

    DECLARE_INSTANCE_METHOD(void, SetObstacleColor, UnityEngine::Color color);
    DECLARE_INSTANCE_METHOD(void, UpdateAllColors);
    DECLARE_INSTANCE_METHOD(void, Init);
    
    DECLARE_INSTANCE_METHOD(UnityEngine::Color, ColorForNoteType, GlobalNamespace::ColorType type);
    DECLARE_INSTANCE_METHOD(UnityEngine::Color, ColorForSaberType, GlobalNamespace::ColorType type);
    DECLARE_INSTANCE_METHOD(UnityEngine::Color, ColorForTrailType, GlobalNamespace::ColorType type);
    DECLARE_INSTANCE_METHOD(UnityEngine::Color, ColorForObstacle);

    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::ColorScheme*, colorScheme, nullptr);

    //DECLARE_CTOR(ctor, Qosmetics::ColorScheme* colorScheme);
    DECLARE_CTOR(ctor);
    
    public:
        void RegisterCallback(std::function<void()> callback, callbackType type);
        void ClearCallbacks();
        void RunCallbacks(std::vector<std::function<void()>>& callbacks);

    private:
        std::vector<std::function<void()>> saberCallbacks;
        std::vector<std::function<void()>> trailCallbacks;
        std::vector<std::function<void()>> noteCallbacks;
        std::vector<std::function<void()>> wallCallbacks;
        std::vector<std::function<void()>> pointerCallbacks;
)