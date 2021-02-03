#pragma once
#include "Types/Qosmetic/ModelManager.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, PointerManager, Qosmetics::ModelManager, 
    public:
        void internalSetActiveModel(std::string name);
        void SetActivePointer(std::string name)
        {
            internalSetActiveModel(name);
        }
    protected:

    REGISTER_FUNCTION(PointerManager,

    )
)