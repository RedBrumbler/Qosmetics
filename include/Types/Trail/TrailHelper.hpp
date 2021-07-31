#pragma once

#include "custom-types/shared/macros.hpp"
#include "Types/Trail/AltTrail.hpp"
#include "Types/Trail/TrailConfig.hpp"
#include "Types/Colors/ColorManager.hpp"

#include "GlobalNamespace/SaberModelController.hpp"
#include "UnityEngine/Color.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, TrailHelper, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD(Qosmetics::AltTrail*, trailInstance);
    DECLARE_INSTANCE_FIELD(Qosmetics::ColorManager*, colorManager);
    DECLARE_INSTANCE_FIELD(GlobalNamespace::SaberModelController*, parentModelController);
    
    DECLARE_INSTANCE_METHOD(void, GetOrAddTrail, bool remake);
    DECLARE_INSTANCE_METHOD(void, Init, Qosmetics::ColorManager* colorManager, GlobalNamespace::SaberModelController* saberModelController);
    DECLARE_INSTANCE_METHOD(void, TrailSetup);
    DECLARE_INSTANCE_METHOD(void, SetTrailActive, bool active);
    DECLARE_INSTANCE_METHOD(UnityEngine::Color, GetColor, int colorType);
    DECLARE_INSTANCE_METHOD(void, UpdateColors);
    DECLARE_INSTANCE_METHOD(void, UpdateChromaColors, int saberType, GlobalNamespace::SaberModelController* saberModelController, UnityEngine::Color color);

    public:
        void set_trailConfig(Qosmetics::TrailConfig& trailConfig);
    private:
        int colorType;
        UnityEngine::Color color;
        UnityEngine::Color multiplier;
        int length;
        float whiteStep;
)