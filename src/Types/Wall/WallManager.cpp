#include "Config.hpp"
#include "Types/Wall/WallManager.hpp"
#include "Types/Wall/WallItem.hpp"
#include "Data/DescriptorCache.hpp"
#include "QosmeticsLogger.hpp"
#include "UnityEngine/Transform.hpp"
#include "Utils/MaterialUtils.hpp"

DEFINE_TYPE(Qosmetics, WallManager);

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
return object

#define GetName(identifier, content) \
if (!identifier) identifier = il2cpp_utils::createcsstr(content, il2cpp_utils::StringType::Manual); \
return identifier

namespace Qosmetics
{
    void WallManager::ctor()
    {
        this->activeItem = nullptr;
        if (config.lastActiveWall != "")
        {
            Descriptor& descriptor = DescriptorCache::GetDescriptor(config.lastActiveWall);
            if (descriptor.isValid() && fileexists(descriptor.get_filePath()))
            {
                SetActiveWall(config.lastActiveWall, true);    
                return;
            }
        }
        SetDefault();
    }
    
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

    void WallManager::FromFilePath(std::string path, bool load)
    {
        if (getenv("walllocked")) return;
        if (this->activeItem && this->activeItem->get_descriptor().get_filePath() == path) return;
        
        Descriptor* desc = new Descriptor(path);
        
        // if descriptor doesn't exist for this thing, ignore the setactive
        if (!desc->isValid())
        {
            ERROR("Item was invalid!");
            return;  
        } 

        if (this->activeItem) delete(this->activeItem);
        this->activeItem = new WallItem(*desc, false);
        if (load) this->activeItem->Load();
        INFO("Active Item Set!");
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
        activeItem = new WallItem(newItem, false);
        activeItem->Load();
        INFO("Active Item Set!");
    }

    WallItem& WallManager::get_item()
    {
        CRASH_UNLESS(activeItem);
        return *activeItem;
    }
    
    ItemType WallManager::get_type()
    {
        INFO("this ptr: %p, item ptr: %p", this, activeItem);
        if (!activeItem) return ItemType::invalid;
        return activeItem->get_type();
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

    UnityEngine::Mesh* WallManager::get_coreMesh()
    {
        MeshFilter* core = get_coreFilter();
        if (!core) return nullptr;
        return core->get_sharedMesh();
    }

    UnityEngine::Mesh* WallManager::get_frameMesh()
    {
        MeshFilter* frame = get_frameFilter();
        if (!frame) return nullptr;
        return frame->get_sharedMesh();
    }
    
    Transform* WallManager::get_core()
    {
        GetNameInPrefab(get_coreName());
    }

    Transform* WallManager::get_frame()
    {
        GetNameInPrefab(get_frameName());
    }

    UnityEngine::MeshFilter* WallManager::get_coreFilter()
    {
        Transform* core = get_core();
        if (!core) return nullptr;
        return core->get_gameObject()->GetComponent<MeshFilter*>();
    }

    UnityEngine::MeshFilter* WallManager::get_frameFilter()
    {
        Transform* frame = get_frame();
        if (!frame) return nullptr;
        return frame->get_gameObject()->GetComponent<MeshFilter*>();
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

    void WallManager::SetActiveWall(std::string name, bool load)
    {
        internalSetActiveModel(name, load);
    }
}