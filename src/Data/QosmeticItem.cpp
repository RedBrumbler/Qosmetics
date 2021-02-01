#include "Data/QosmeticItem.hpp"
#include "Logging.hpp"
#include "UnityEngine/Object.hpp"
#include "Utils/MaterialUtils.hpp"

using namespace UnityEngine;

#define INFO(value...) QosmeticsLogger::GetContextLogger("Qosmetic Item").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("Qosmetic Item").error(value)

namespace Qosmetics
{
    void QosmeticItem::GameObjectCallback(GameObject* gameObject)
    {
        if (!gameObject) 
        {
            ERROR("GameObject was null");
            return;
        }

        GameObject* instantiated = Object::Instantiate(gameObject);
        Object::DontDestroyOnLoad(instantiated);

        this->prefab = instantiated;
        MaterialUtils::PrewarmAllShadersOnObject(this->prefab);
        
        INFO("Prefab Loaded");
    }
}