#pragma once

#include "Data/DescriptorCache.hpp"
#include "UnityEngine/Transform.hpp"
struct switcherInfo {
    Cache& cache;
    Cache::iterator it;
    UnityEngine::Transform* layout;

    switcherInfo(Cache& cache, UnityEngine::Transform* layout) : cache(cache)
    {
        it = this->cache.begin();
        this->layout = layout;
    }
};