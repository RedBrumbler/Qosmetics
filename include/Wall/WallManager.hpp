#pragma once
#include "Data/ModelManager.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, WallManager, Qosmetics::ModelManager, 
    public:
        void internalSetActiveModel(std::string name);
        void SetActiveWall(std::string name)
        {
            internalSetActiveModel(name);
        }
    protected:

    REGISTER_FUNCTION(WallManager,

    )
)