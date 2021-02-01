#pragma once
#include "Trail/TrailConfig.hpp"

#include "GlobalNamespace/SaberTrail.hpp"

#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, QosmetcsTrail, GlobalNamespace::SaberTrail,
    

    public:
        void SetTrailConfig(TrailConfig* config)
        {
            trailConfig = config;
        }

    private: 
        TrailConfig* trailConfig = nullptr;
    REGISTER_FUNCTION(QosmeticsTrail,
        
    )
)