#pragma once

#include "custom-types/shared/macros.hpp"
#include "Types/Trail/AltTrail.hpp"
#include "Types/Trail/TrailConfig.hpp"
#include "Types/Colors/ColorManager.hpp"

#include "GlobalNamespace/SaberModelController.hpp"
#include "sombrero/shared/ColorUtils.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, TrailHelper, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD(Qosmetics::AltTrail*, trailInstance);
    DECLARE_INSTANCE_FIELD(Qosmetics::ColorManager*, colorManager);
    DECLARE_INSTANCE_FIELD(GlobalNamespace::SaberModelController*, parentModelController);
    
    DECLARE_INSTANCE_METHOD(void, GetOrAddTrail, bool remake);
    DECLARE_INSTANCE_METHOD(void, Init, Qosmetics::ColorManager* colorManager, GlobalNamespace::SaberModelController* saberModelController);
    DECLARE_INSTANCE_METHOD(void, TrailSetup);
    DECLARE_INSTANCE_METHOD(void, SetTrailActive, bool active);
    DECLARE_INSTANCE_METHOD(Sombrero::FastColor, GetColor, int colorType);
    DECLARE_INSTANCE_METHOD(void, UpdateColors);
    DECLARE_INSTANCE_METHOD(void, UpdateChromaColors, int saberType, GlobalNamespace::SaberModelController* saberModelController, Sombrero::FastColor color);

    DECLARE_CTOR(ctor);
    public:
        void SetColors(const Sombrero::FastColor& leftColor, const Sombrero::FastColor& rightColor);
        void set_trailConfig(Qosmetics::TrailConfig& trailConfig);
        int colorType;
        Sombrero::FastColor color;
        Sombrero::FastColor multiplier;
        int length;
        float whiteStep;
)