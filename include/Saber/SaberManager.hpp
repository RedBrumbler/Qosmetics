#pragma once
#include "Data/ModelManager.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, SaberManager, Qosmetics::ModelManager, 
    public:
        void internalSetActiveModel(std::string name);
        void SetActiveSaber(std::string name)
        {
            internalSetActiveModel(name);
        }
    protected:

    REGISTER_FUNCTION(SaberManager,

    )
)