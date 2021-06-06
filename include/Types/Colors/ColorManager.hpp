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
    DECLARE_METHOD(void, SetColorSchemeFromBase, GlobalNamespace::ColorScheme* base);
    DECLARE_METHOD(void, SetColorSchemeFromCustom, Qosmetics::ColorScheme* base);

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
    DECLARE_METHOD(void, Init);
    
    DECLARE_METHOD(UnityEngine::Color, ColorForNoteType, GlobalNamespace::ColorType type);
    DECLARE_METHOD(UnityEngine::Color, ColorForSaberType, GlobalNamespace::ColorType type);
    DECLARE_METHOD(UnityEngine::Color, ColorForTrailType, GlobalNamespace::ColorType type);
    DECLARE_METHOD(UnityEngine::Color, ColorForObstacle);

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


    REGISTER_FUNCTION(
        REGISTER_METHOD(SetColorSchemeFromBase);
        REGISTER_METHOD(SetColorSchemeFromCustom);
        REGISTER_METHOD(ctor);

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
        REGISTER_METHOD(Init);

        REGISTER_METHOD(ColorForSaberType);
        REGISTER_METHOD(ColorForNoteType);
        REGISTER_METHOD(ColorForTrailType);
        REGISTER_METHOD(ColorForObstacle);
        
        REGISTER_FIELD(colorScheme);
    )
)