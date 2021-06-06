#pragma once
#include "Types/Note/NoteConfig.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Color.hpp"
#include "GlobalNamespace/MaterialPropertyBlockController.hpp"

namespace NoteUtils
{
    void SetColors(UnityEngine::GameObject* object, UnityEngine::Color color, UnityEngine::Color otherColor, int renderQueue = 0);
    void SetNoteSize(UnityEngine::Transform* transform);
    void SetBombSize(UnityEngine::Transform* mesh);
    void HideBaseGameNotes(UnityEngine::Transform* noteCube, Qosmetics::NoteConfig& config, bool hide = true);
    void SetDebrisProperties(UnityEngine::GameObject* debris, UnityEngine::Vector3 cutPoint, UnityEngine::Vector3 cutNormal);
    void AddRenderersToPropertyBlockController(GlobalNamespace::MaterialPropertyBlockController* controller, UnityEngine::GameObject* obj);
}