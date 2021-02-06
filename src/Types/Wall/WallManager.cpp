#include "Types/Wall/WallManager.hpp"
#include "Types/Wall/WallItem.hpp"
#include "Data/DescriptorCache.hpp"
#include "QosmeticsLogger.hpp"
#include "UnityEngine/Transform.hpp"
#include "Utils/MaterialUtils.hpp"

DEFINE_CLASS(Qosmetics::WallManager);

#define INFO(value...) QosmeticsLogger::GetContextLogger("Wall Manager").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("Wall Manager").error(value)

using namespace UnityEngine;

static Il2CppString* coreName = nullptr;
static Il2CppString* frameName = nullptr;

#define GetNameInPrefab(name) \
if (!activeItem) return nullptr; \
GameObject* prefab = activeItem->get_prefab(); \
if (!prefab) return nullptr; \
Transform* object = prefab->get_transform()->Find(name); \
if (!object) return nullptr; \
return UnityEngine::Object::Instantiate(object)

#define GetName(identifier, content) \
if (!identifier) identifier = il2cpp_utils::createcsstr(content, il2cpp_utils::StringType::Manual); \
return identifier

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

    Material* WallManager::get_coreMaterial()
    {
        Renderer* core = get_coreRenderer();
        if (!core) return nullptr;
        return core->get_material();
    }

    Material* WallManager::get_frameMaterial()
    {
        Renderer* frame = get_frameRenderer();
        if (!frame) return nullptr;
        return frame->get_material();
    }

    Array<Material*>* WallManager::get_coreMaterialArray()
    {
        Renderer* core = get_coreRenderer();
        if (!core) return nullptr;
        return MaterialUtils::GetMaterials(core);
    }

    Array<Material*>* WallManager::get_frameMaterialArray()
    {
        Renderer* frame = get_frameRenderer();
        if (!frame) return nullptr;
        return MaterialUtils::GetMaterials(frame);
    }

    Array<Material*>* WallManager::get_coreSharedMaterials()
    {
        Renderer* core = get_coreRenderer();
        if (!core) return nullptr;
        return core->get_sharedMaterials();
    }

    Array<Material*>* WallManager::get_frameSharedMaterials()
    {
        Renderer* frame = get_frameRenderer();
        if (!frame) return nullptr;
        return frame->get_sharedMaterials();
    }

    Transform* WallManager::get_core()
    {
        GetNameInPrefab(get_coreName());
    }

    Transform* WallManager::get_frame()
    {
        GetNameInPrefab(get_frameName());
    }

    MeshRenderer* WallManager::get_coreRenderer()
    {
        Transform* core = get_core();
        if (!core) return nullptr;
        return core->get_gameObject()->GetComponent<MeshRenderer*>();
    }

    MeshRenderer* WallManager::get_frameRenderer()
    {
        Transform* frame = get_frame();
        if (!frame) return nullptr;
        return frame->get_gameObject()->GetComponent<MeshRenderer*>();
    }

    Il2CppString* WallManager::get_coreName()
    {
        GetName(coreName, "Core");
    }

    Il2CppString* WallManager::get_frameName()
    {
        GetName(frameName, "Frame");
    }
}