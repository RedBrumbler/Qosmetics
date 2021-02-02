#pragma once
#include "Trail/TrailConfig.hpp"

#include "GlobalNamespace/SaberTrail.hpp"

#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, QosmeticsTrail, GlobalNamespace::SaberTrail,
    DECLARE_METHOD(void, Awake);
    DECLARE_METHOD(void, Update);

    public:
        void SetTrailConfig(TrailConfig* config)
        {
            trailConfig = config;
        }

        void Reset();

    private: 
        TrailConfig* trailConfig = nullptr;

    REGISTER_FUNCTION(QosmeticsTrail,
        REGISTER_METHOD(Awake);
        REGISTER_METHOD(Update);
    )
)