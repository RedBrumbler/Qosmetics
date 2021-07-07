#pragma once
#include "Types/Qosmetic/ModelManager.hpp"


DECLARE_CLASS_CUSTOM(Qosmetics, PointerManager, Qosmetics::ModelManager, 
    public:
        void internalSetActiveModel(std::string name);
        void SetActivePointer(std::string name)
        {
            internalSetActiveModel(name);
        }
    protected:
)
