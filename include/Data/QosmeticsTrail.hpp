#pragma once
#include "custom-types/shared/types.hpp"
#include "custom-types/shared/macros.hpp"
#include "GlobalNamespace/SaberTrail.hpp"

#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Vector3.hpp"

#include "UnityEngine/Material.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/Color.hpp"
#include "GlobalNamespace/SaberMovementData.hpp"
#include "GlobalNamespace/IBladeMovementData.hpp"
#include "GlobalNamespace/BladeMovementDataElement.hpp"

namespace Qosmetics
{
    class CustomTrail;
}

DECLARE_CLASS_CODEGEN(Qosmetics, QosmeticsTrail, GlobalNamespace::SaberTrail,

    DECLARE_INSTANCE_FIELD_DEFAULT(UnityEngine::Transform*, topTransform, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(UnityEngine::Transform*, bottomTransform, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(UnityEngine::Material*, trailMaterial, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(int, length, 20);
    DECLARE_INSTANCE_FIELD_DEFAULT(int, whitestep, 0);
    DECLARE_INSTANCE_FIELD_DEFAULT(int, colorType, 2);
    DECLARE_INSTANCE_FIELD(UnityEngine::Color, trailColor);
    DECLARE_INSTANCE_FIELD(UnityEngine::Color, multiplierColor);
    DECLARE_INSTANCE_FIELD(GlobalNamespace::SaberMovementData*, customMovementData);
    DECLARE_STATIC_FIELD(float, trailIntensity);

    DECLARE_METHOD(void, Update);
    DECLARE_METHOD(void, Awake);
    DECLARE_METHOD(void, UpdateColors);
    DECLARE_METHOD(void, TrailSetup, int length,
                                     int granularity,
                                     int colorType,
                                     int whiteStep, 
                                     UnityEngine::Material* trailMaterial, 
                                     UnityEngine::Color trailColor, 
                                     UnityEngine::Color multiplierColor);
    
    public: static void AddTrailAndSetup(UnityEngine::Transform* trailObject, Qosmetics::CustomTrail &trail);
    
    public: static void set_trailIntensity(float intensity)
    {
        trailIntensity = intensity;
    }

    public: static GlobalNamespace::SaberTrailRenderer* NewTrailRenderer(UnityEngine::Material* material);

    public: Qosmetics::QosmeticsTrail* CopyFromBase(GlobalNamespace::SaberTrail* base);

    REGISTER_FUNCTION(QosmeticsTrail,
        REGISTER_METHOD(Update);
        REGISTER_METHOD(Awake);
        REGISTER_METHOD(TrailSetup);
        REGISTER_METHOD(UpdateColors);
        
        REGISTER_FIELD(topTransform);
        REGISTER_FIELD(bottomTransform);
        REGISTER_FIELD(trailMaterial);
        REGISTER_FIELD(length);
        REGISTER_FIELD(whitestep);
        REGISTER_FIELD(colorType);
        
        REGISTER_FIELD(trailColor);
        REGISTER_FIELD(multiplierColor);
        
        REGISTER_FIELD(customMovementData);
        REGISTER_FIELD(trailIntensity);
    )
)