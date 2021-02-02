#include "Data/QosmeticItem.hpp"
#include "QosmeticsLogger.hpp"
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
            ERROR("GameObject was nullptr");
            return;
        }

        GameObject* instantiated = Object::Instantiate(gameObject);
        Object::DontDestroyOnLoad(instantiated);

        this->prefab = instantiated;
        MaterialUtils::PrewarmAllShadersOnObject(this->prefab);
        
        INFO("Prefab Loaded");
    }

    void QosmeticItem::DescriptorCallback(TextAsset* textAsset)
    {
        if (!textAsset)
        {
            ERROR("Descriptor was nullptr");
            return;
        }

        Il2CppString* descriptorstring = textAsset->get_text();
        
        std::string json = to_utf8(csstrtostr(descriptorstring));

        rapidjson::Document d;
        d.Parse(json.c_str());

        Descriptor temp = Descriptor(d, descriptor.get_filePath());
        descriptor.CopyFrom(temp);
    }
}