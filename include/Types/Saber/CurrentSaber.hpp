#pragma once

#include "beatsaber-hook/shared/utils/typedefs-wrappers.hpp"
#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/GameObject.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, CurrentSaber, UnityEngine::MonoBehaviour,
    DECLARE_METHOD(static UnityEngine::GameObject*, get_instance);
    DECLARE_METHOD(void, Awake);
    DECLARE_METHOD(void, OnDestroy);
    private:
        static SafePtr<CurrentSaber> instance;
    REGISTER_FUNCTION(
        REGISTER_METHOD(Awake);
        REGISTER_METHOD(OnDestroy);
        REGISTER_METHOD(get_instance);
    )
);