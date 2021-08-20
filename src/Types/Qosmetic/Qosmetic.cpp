#include "Types/Qosmetic/Qosmetic.hpp"
#include "UnityEngine/Object.hpp"

DEFINE_TYPE(Qosmetics, Qosmetic);

using namespace UnityEngine;

namespace Qosmetics
{
    void Qosmetic::Awake()
    {
        this->modelManager = Object::FindObjectOfType<ModelManager*>();
    }

    void Qosmetic::UpdateModel()
    {
        
    }

    void Qosmetic::Restore()
    {

    }

    void Qosmetic::Replace()
    {
        
    }
}