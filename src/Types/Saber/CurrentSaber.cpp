#include "Types/Saber/CurrentSaber.hpp"

DEFINE_TYPE(Qosmetics::CurrentSaber);

#include "UnityEngine/Object.hpp"

using namespace UnityEngine;
namespace Qosmetics
{
    SafePtr<CurrentSaber> CurrentSaber::instance;
    void CurrentSaber::Awake()
    {
        if (instance)
        {
            Object::Destroy(instance->get_gameObject());
        }
        
        instance = this;
    }

    GameObject* CurrentSaber::get_instance()
    {
        if (!instance) return nullptr;
        return instance->get_gameObject();
    }

    void CurrentSaber::OnDestroy()
    {
        if (instance->Equals(this))
        {
            instance = nullptr;
        }
    }
}