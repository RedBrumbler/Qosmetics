#include "Utils/UnityUtils.hpp"

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