#pragma once

#include "UnityEngine/MeshFilter.hpp"
#include "UnityEngine/Renderer.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/Vector3.hpp"
#include "Types/Wall/WallConfig.hpp"

struct WallReferences {
    UnityEngine::Renderer* coreRenderer = nullptr;
    UnityEngine::Renderer* frameRenderer = nullptr;
    UnityEngine::Renderer* fakeGlowRenderer = nullptr;
    UnityEngine::MeshFilter* coreFilter = nullptr;
    UnityEngine::MeshFilter* frameFilter = nullptr;
};

namespace WallUtils
{
    WallReferences PrepareAndGatherReferences(UnityEngine::Transform* obstacle);
    WallReferences GatherReferences(UnityEngine::Transform* obstacle);
    void SetColors(UnityEngine::GameObject* obstacle, UnityEngine::Color color, int renderQueue = 0);
    void SetSizeParams(UnityEngine::GameObject* object, UnityEngine::Vector3 localScale, float edgeSize = 0);
}