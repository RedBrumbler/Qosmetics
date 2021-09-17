#include "Utils/UnityUtils.hpp"
#include "UnityEngine/EventSystems/EventTrigger.hpp"
#include "UnityEngine/EventSystems/UIBehaviour.hpp"
#include "UnityEngine/Collider.hpp"
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

void UnityUtils::SanitizePrefab(GameObject* prefab)
{
    Array<UnityEngine::EventSystems::EventTrigger*>* triggers = prefab->GetComponentsInChildren<UnityEngine::EventSystems::EventTrigger*>(true);
    int triggerLength = triggers->Length();
    for (int i = 0; i < triggerLength; i++)
    {
            Object::DestroyImmediate(triggers->values[i]);
    }

    Array<UnityEngine::EventSystems::UIBehaviour*>* uiBehaviours = prefab->GetComponentsInChildren<UnityEngine::EventSystems::UIBehaviour*>(true);
    int behavioursLength = uiBehaviours->Length();
    for (int i = 0; i < behavioursLength; i++)
    {
            Object::DestroyImmediate(uiBehaviours->values[i]);
    }

    Array<UnityEngine::Collider*>* colliders = prefab->GetComponentsInChildren<UnityEngine::Collider*>(true);
    int collidersLength = colliders->Length();
    for (int i = 0; i < collidersLength; i++)
    {
            Object::DestroyImmediate(colliders->values[i]);
    }
}