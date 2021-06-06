#include "Types/Utils/FuckYouGC.hpp"
#include "UnityEngine/Object.hpp"

DEFINE_TYPE(Qosmetics::FuckYouGC);

void Qosmetics::FuckYouGC::ctor()
{
    prefab = nullptr;
}

void Qosmetics::FuckYouGC::dtor()
{
    UnityEngine::Object::Destroy(prefab);
}