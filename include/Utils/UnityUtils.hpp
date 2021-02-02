#pragma once
#include "UnityEngine/GameObject.hpp"

class UnityUtils
{
    public:
        template<type T>
        T GetAddComponent(UnityEngine::GameObject* obj)
        {
            static_assert(std::is_convertible_v<T, UnityEngine::Component*>);
            T component = obj->GetComponent<T>();
            if (!component) return obj->AddComponent<T>();
            return component;
        }
}