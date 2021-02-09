#include "Types/Utils/FuckYouGC.hpp"

DEFINE_CLASS(Qosmetics::FuckYouGC);

void Qosmetics::FuckYouGC::Awake()
{
    this->prefab = get_gameObject();
}