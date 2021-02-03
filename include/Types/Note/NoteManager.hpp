#pragma once
#include "Types/Qosmetic/ModelManager.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, NoteManager, Qosmetics::ModelManager, 
    public:
        void internalSetActiveModel(std::string name);
        void SetActiveNote(std::string name)
        {
            internalSetActiveModel(name);
        }
    protected:

    REGISTER_FUNCTION(NoteManager,

    )
)