#pragma once

#include "custom-types/shared/macros.hpp"
#include "Types/Qosmetic/Qosmetic.hpp"
#include "GlobalNamespace/SaberType.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, Pointer, Qosmetics::Qosmetic,
    DECLARE_INSTANCE_FIELD_DEFAULT(GlobalNamespace::SaberType, saberType, 0);

    public:
        

    REGISTER_FUNCTION(Pointer,
        //REGISTER_FIELD(modelManager);
        REGISTER_FIELD(saberType);
    )
)