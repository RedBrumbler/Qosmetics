#pragma once
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Object.hpp"
#include "UnityEngine/Component.hpp"
#include "UnityEngine/Renderer.hpp"

class UnityUtils
{
    public:

        template<typename T>
        static T GetAddComponent(UnityEngine::GameObject* obj)
        {
            static_assert(std::is_convertible_v<T, UnityEngine::Component*>);
            T component = obj->GetComponent<T>();
            if (!component) return obj->AddComponent<T>();
            return component;
        }

        template<typename T>
        static T FindAddComponent(bool manual = false)
        {
            static_assert(std::is_convertible_v<T, UnityEngine::Object*>);
            T component = UnityEngine::Object::FindObjectOfType<T>();
            if (component) return component; 
            if (manual)
            {
                UnityEngine::GameObject* newObj = UnityEngine::GameObject::New_ctor<il2cpp_utils::CreationType::Manual>();
                UnityEngine::Object::DontDestroyOnLoad(newObj);
                return newObj->AddComponent<T>();
            } 
            else
            {
                UnityEngine::GameObject* newObj = UnityEngine::GameObject::New_ctor<>();
                return newObj->AddComponent<T>();
            } 
        }

        static void HideRenderersOnObject(UnityEngine::GameObject* obj, bool doHide = true);

        static void HideRenderer(UnityEngine::Renderer* renderer, bool doHide = true);
};