#include "Types/Qosmetic/QosmeticItem.hpp"
#include "QosmeticsLogger.hpp"
#include "Utils/MaterialUtils.hpp"
#include "Utils/UnityUtils.hpp"
#include "Types/Utils/FuckYouGC.hpp"

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
        UnityUtils::SanitizePrefab(instantiated);
        Object::DontDestroyOnLoad(instantiated);
        MaterialUtils::ReplaceMaterialsForGameObject(instantiated);
        
        this->prefabWrapper = *il2cpp_utils::New<FuckYouGC*>();
        this->prefabWrapper->prefab = instantiated;
        this->prefabWrapper->prefab->SetActive(false);

        MaterialUtils::PrewarmAllShadersOnObject(this->prefabWrapper->prefab);
        //this->prefabWrapper->prefab->SetActive(false);
        GOloaded = true;
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
        INFO("Descriptor loaded!");
        DescriptorLoaded = true;
    }
    
    void QosmeticItem::ConfigCallback(TextAsset* textAsset)
    {
        ERROR("Qosmetic item config callback was called? this shouldn't happen...");
    }
}