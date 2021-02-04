#include "Types/Saber/SaberManager.hpp"
#include "Types/Saber/SaberItem.hpp"
#include "Data/DescriptorCache.hpp"
#include "QosmeticsLogger.hpp"
#include "UnityEngine/Transform.hpp"

DEFINE_CLASS(Qosmetics::SaberManager);

#define INFO(value...) QosmeticsLogger::GetContextLogger("Saber Manager").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("Saber Manager").error(value)

using namespace UnityEngine;

static inline Il2CppString* leftSaberName = nullptr;
static inline Il2CppString* rightSaberName = nullptr;

namespace Qosmetics
{
    GameObject* SaberManager::GetActivePrefab()
    {
        if (!activeItem) return nullptr;
        return activeItem->get_prefab();
    }

    void SaberManager::SetActiveModel(Il2CppString* csname)
    {
        if (!csname) return;
        if (getenv("saberlocked")) return;
        std::string name = to_utf8(csstrtostr(csname));
        internalSetActiveModel(name, true);
    }

    void SaberManager::SetDefault()
    {
        if (getenv("saberlocked")) return;
        if (activeItem) delete (activeItem);
        activeItem = new SaberItem(DescriptorCache::GetDescriptor(""));
    }

    void SaberManager::internalSetActiveModel(std::string name, bool load)
    {
        INFO("Setting active Saber %s", name.c_str());
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
        activeItem = new SaberItem(newItem, load);
        INFO("Active Item Set!");
    }

    UnityEngine::GameObject* SaberManager::get_rightSaber()
    {
        if (!activeItem) return nullptr;
        GameObject* prefab = activeItem->get_prefab();
        if (!prefab) return nullptr;
        Transform* object = prefab->get_transform()->Find(get_rightSaberName());
        if (!object) return nullptr;
        return Object::Instantiate(object)->get_gameObject();
    }

    UnityEngine::GameObject* SaberManager::get_leftSaber()
    {
        if (!activeItem) return nullptr;
        GameObject* prefab = activeItem->get_prefab();
        if (!prefab) return nullptr;
        Transform* object = prefab->get_transform()->Find(get_leftSaberName());
        if (!object) return nullptr;
        return Object::Instantiate(object)->get_gameObject();
    }

    Il2CppString* SaberManager::get_leftSaberName()
    {
        if (!leftSaberName) leftSaberName = il2cpp_utils::createcsstr("LeftSaber", il2cpp_utils::StringType::Manual);
        return leftSaberName;
    }

    Il2CppString* SaberManager::get_rightSaberName()
    {
        if (!rightSaberName) rightSaberName = il2cpp_utils::createcsstr("RightSaber", il2cpp_utils::StringType::Manual);
        return rightSaberName;
    }

    void SaberManager::SetActiveSaber(std::string name, bool load)
    {
        internalSetActiveModel(name, load);
    }

    ItemType SaberManager::get_type()
    {
        return activeItem->get_type();
    }
    
    SaberItem& SaberManager::get_item()
    {
        CRASH_UNLESS(activeItem);
        return *this->activeItem;
    }
}