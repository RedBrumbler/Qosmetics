#pragma once
#include "Types/Trail/TrailConfig.hpp"
#include "Types/Colors/ColorManager.hpp"

#include "GlobalNamespace/SaberModelController.hpp"
#include "GlobalNamespace/SaberTrail.hpp"
#include "GlobalNamespace/SaberTrailRenderer.hpp"
#include "GlobalNamespace/SaberMovementData.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/Transform.hpp"

#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, QosmeticsTrail, GlobalNamespace::SaberTrail,
    DECLARE_INSTANCE_FIELD(UnityEngine::Transform*, topTransform);
    DECLARE_INSTANCE_FIELD(UnityEngine::Transform*, bottomTransform);
    DECLARE_INSTANCE_FIELD(UnityEngine::Transform*, customBottomTransform);
    DECLARE_INSTANCE_FIELD(UnityEngine::Material*, trailMaterial);
    DECLARE_INSTANCE_FIELD(int, length);
    DECLARE_INSTANCE_FIELD(int, whitestep);
    DECLARE_INSTANCE_FIELD(int, colorType);
    DECLARE_INSTANCE_FIELD(UnityEngine::Color, trailColor);
    DECLARE_INSTANCE_FIELD(UnityEngine::Color, multiplierColor);
    DECLARE_INSTANCE_FIELD(GlobalNamespace::SaberMovementData*, customMovementData);
    DECLARE_INSTANCE_FIELD(Qosmetics::ColorManager*, colorManager);
    DECLARE_INSTANCE_FIELD(bool, customInited);
    DECLARE_INSTANCE_FIELD(GlobalNamespace::SaberModelController*, attachedSaberModelController);
    
    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, InitTrail, int length,
                                    int colorType,
                                    int whiteStep,
                                    UnityEngine::Material* material,
                                    UnityEngine::Color trailColor,
                                    UnityEngine::Color multiplierColor,
                                    bool ignoreOverrides);
                                    
    DECLARE_INSTANCE_METHOD(void, InitFromDefault, UnityEngine::Transform* objToCopy);
    DECLARE_INSTANCE_METHOD(void, Update);
    DECLARE_INSTANCE_METHOD(void, Reset);
    DECLARE_INSTANCE_METHOD(void, UpdateTrail);
    DECLARE_INSTANCE_METHOD(void, UpdateColors);
    DECLARE_INSTANCE_METHOD(void, UpdateChromaColors, int, GlobalNamespace::SaberModelController* modelController, UnityEngine::Color color);
    DECLARE_INSTANCE_METHOD(void, SetColorManager, ColorManager* colorManager);
    DECLARE_INSTANCE_METHOD(GlobalNamespace::SaberTrailRenderer*, NewTrailRenderer);

    DECLARE_CTOR(ctor);
    DECLARE_DTOR(dtor);

    public:
        void SetTrailConfig(TrailConfig* config);
        static float trailIntensity;
    private: 
        TrailConfig* trailConfig;
)