#include "Types/Wall/WallManager.hpp"
#include "Types/Wall/WallItem.hpp"
#include "Data/DescriptorCache.hpp"
#include "QosmeticsLogger.hpp"

DEFINE_CLASS(Qosmetics::WallManager);

#define INFO(value...) QosmeticsLogger::GetContextLogger("Wall Manager").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("Wall Manager").error(value)

using namespace UnityEngine;

namespace Qosmetics
{
    GameObject* WallManager::GetActivePrefab()
    {
        if (!activeItem) return nullptr;
        return activeItem->get_prefab();
    }

    void WallManager::SetActiveModel(Il2CppString* csname)
    {
        if (!csname) return;
        std::string name = to_utf8(csstrtostr(csname));
        internalSetActiveModel(name, true);
    }

    void WallManager::SetDefault()
    {
        if (activeItem) delete (activeItem);
        activeItem = new WallItem(DescriptorCache::GetDescriptor(""));
    }

    void WallManager::internalSetActiveModel(std::string name, bool load)
    {
        INFO("Setting active Wall %s", name.c_str());
        // if new set is already the active one, ignore
        if (activeItem && activeItem->get_descriptor().GetFileName() == name) return;
        Descriptor& newItem = DescriptorCache::GetDescriptor(name);
        // if descriptor doesn't exist for this thing, ignore the setactive
        if (!newItem.isValid())
        {
            ERROR("Item was invalid!");
            return;  
        } 
        if (activeItem) delete(activeItem);
        activeItem = new WallItem(newItem, load);
        INFO("Active Item Set!");
    }

    WallItem& WallManager::get_item()
    {
        CRASH_UNLESS(activeItem);
        return *activeItem;
    }
    
    ItemType WallManager::get_type()
    {
        return ItemType::wall; 
    }

    UnityEngine::Material* WallManager::get_coreMaterial()
    {

    }
    
    UnityEngine::Material* WallManager::get_frameMaterial()
    {

    }
    
    UnityEngine::Material* WallManager::get_coreMaterialArray()
    {

    }
    
    UnityEngine::Material* WallManager::get_frameMaterialArray()
    {

    }
    
    UnityEngine::Material* WallManager::get_coreSharedMaterials()
    {

    }
    
    UnityEngine::Material* WallManager::get_frameSharedMaterials()
    {

    }
    

}