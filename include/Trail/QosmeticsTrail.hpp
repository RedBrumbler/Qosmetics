#pragma once
#include "Trail/TrailConfig.hpp"

#include "GlobalNamespace/SaberTrail.hpp"
#include "GlobalNamespace/SaberTrailRenderer.hpp"
#include "GlobalNamespace/SaberMovementData.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/Transform.hpp"

#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, QosmeticsTrail, GlobalNamespace::SaberTrail,
    DECLARE_INSTANCE_FIELD_DEFAULT(UnityEngine::Transform*, topTransform, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(UnityEngine::Transform*, bottomTransform, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(UnityEngine::Transform*, customBottomTransform, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(UnityEngine::Material*, trailMaterial, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(GlobalNamespace::SaberMovementData*, customMovementData, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(int, length, 20);
    DECLARE_INSTANCE_FIELD_DEFAULT(int, whitestep, 0);
    DECLARE_INSTANCE_FIELD_DEFAULT(int, colorType, 2);
    DECLARE_INSTANCE_FIELD(UnityEngine::Color, trailColor);
    DECLARE_INSTANCE_FIELD(UnityEngine::Color, multiplierColor);
    DECLARE_INSTANCE_FIELD_DEFAULT(bool, customInited, false);
    DECLARE_STATIC_FIELD(float, trailIntensity);
    
    DECLARE_METHOD(void, Awake);
    DECLARE_METHOD(void, InitTrail, int length, int colorType, int whiteStep, UnityEngine::Material* material, UnityEngine::Color trailColor, UnityEngine::Color multiplierColor, bool ignoreOverrides);
    DECLARE_METHOD(void, InitFromDefault, UnityEngine::Transform* objToCopy);
    DECLARE_METHOD(void, Update);
    DECLARE_METHOD(void, UpdateTrail);
    DECLARE_METHOD(void, UpdateColors);
    DECLARE_METHOD(GlobalNamespace::SaberTrailRenderer*, NewTrailRenderer);

    public:
        void SetTrailConfig(TrailConfig* config);
        void Reset();

    private: 
        TrailConfig* trailConfig;

    REGISTER_FUNCTION(QosmeticsTrail,
        REGISTER_METHOD(Awake);
        REGISTER_METHOD(InitTrail);
        REGISTER_METHOD(Update);
        REGISTER_METHOD(UpdateTrail);
        REGISTER_METHOD(UpdateColors);

        REGISTER_FIELD(topTransform);
        REGISTER_FIELD(bottomTransform);
        REGISTER_FIELD(customBottomTransform);
        REGISTER_FIELD(trailMaterial);
        REGISTER_FIELD(length);
        REGISTER_FIELD(whitestep);
        REGISTER_FIELD(colorType);
        REGISTER_FIELD(customInited);
        
        REGISTER_FIELD(trailColor);
        REGISTER_FIELD(multiplierColor);
        
        REGISTER_FIELD(customMovementData);
        REGISTER_FIELD(trailIntensity);
    )
)