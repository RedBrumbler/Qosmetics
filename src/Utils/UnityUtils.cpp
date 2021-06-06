#include "Utils/UnityUtils.hpp"
using namespace UnityEngine;

void UnityUtils::HideRenderersOnObject(UnityEngine::GameObject* obj, bool doHide)
{
    Array<UnityEngine::Renderer*>* renderers = obj->GetComponentsInChildren<UnityEngine::Renderer*>(true);
    if (!renderers) return;
    for (int i = 0; i < renderers->Length(); i++)
    {
        UnityEngine::Renderer* renderer = renderers->values[i];
        if (!renderer) continue;
        HideRenderer(renderer, doHide);
    }
}

void UnityUtils::HideRenderersOnObject(UnityEngine::Transform* obj, bool doHide)
{
    if (!obj) return;
    HideRenderersOnObject(obj->get_gameObject(), doHide);
}

void UnityUtils::HideRenderer(UnityEngine::Renderer* renderer, bool doHide)
{
    if (!renderer) return;
    renderer->set_enabled(!doHide);
}

void UnityUtils::SetLayerRecursive(Transform* object, int layer)
{
    int childs = object->get_childCount();
    for (int i = 0; i < childs; i++)
    {
        SetLayerRecursive(object->GetChild(i), layer);
    }
}

void UnityUtils::SetLayerRecursive(GameObject* object, int layer)
{
    if (!object) return;
    object->set_layer(layer);
    SetLayerRecursive(object->get_transform(), layer);
}