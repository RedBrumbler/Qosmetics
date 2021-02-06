#include "Utils/UnityUtils.hpp"

template<typename T>
T UnityUtils::GetAddComponent(UnityEngine::GameObject* obj)
{
    //static_assert(std::is_convertible_v<T, UnityEngine::Component*>);
    T component = obj->GetComponent<T>();
    if (!component) return obj->AddComponent<T>();
    return component;
}

template<typename T>
T UnityUtils::FindAddComponent(bool manual)
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

void UnityUtils::HideRenderersOnObject(UnityEngine::GameObject* obj, bool doHide)
{
    Array<UnityEngine::Renderer*>* renderers = obj->GetComponentsInChildren<UnityEngine::Renderer*>();
    if (!renderers) return;
    for (int i = 0; i < renderers->Length(); i++)
    {
        UnityEngine::Renderer* renderer = renderers->values[i];
        if (!renderer) continue;
        HideRenderer(renderer, doHide);
    }
}

void UnityUtils::HideRenderer(UnityEngine::Renderer* renderer, bool doHide)
{
    if (!renderer) return;
    renderer->set_enabled(!doHide);
}