#pragma once

#include "beatsaber-hook/shared/utils/typedefs-wrappers.hpp"
#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/GameObject.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, CurrentSaber, UnityEngine::MonoBehaviour,
    DECLARE_STATIC_METHOD(UnityEngine::GameObject*, get_instance);
    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, OnDestroy);
    private:
        static SafePtr<CurrentSaber> instance;
);