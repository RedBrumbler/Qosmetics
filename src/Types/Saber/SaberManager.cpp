#include "Config.hpp"
#include "Types/Saber/SaberManager.hpp"
#include "Types/Saber/SaberItem.hpp"
#include "Data/DescriptorCache.hpp"
#include "QosmeticsLogger.hpp"
#include "UnityEngine/Transform.hpp"


DEFINE_TYPE(Qosmetics::SaberManager);

#define INFO(value...) QosmeticsLogger::GetContextLogger("Saber Manager").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("Saber Manager").error(value)

using namespace UnityEngine;

static inline Il2CppString* basicSaberModelName = nullptr;
static inline Il2CppString* leftSaberName = nullptr;
static inline Il2CppString* rightSaberName = nullptr;

namespace Qosmetics
{
    void SaberManager::ctor()
    {
        this->activeItem = nullptr;
        if (config.lastActiveSaber != "")
            SetActiveSaber(config.lastActiveSaber, true);    
        else SetDefault();
    }

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

    void SaberManager::FromFilePath(Il2CppString* filePath)
    {
        if (!filePath) return;
        if (getenv("saberlocked")) return;
        std::string path = to_utf8(csstrtostr(filePath));
        if (this->activeItem && this->activeItem->get_descriptor().get_filePath() == path) return;
        
        Descriptor* desc = new Descriptor(path);
        
        // if descriptor doesn't exist for this thing, ignore the setactive
        if (!desc->isValid())
        {
            ERROR("Item was invalid!");
            return;  
        } 

        if (this->activeItem) delete(this->activeItem);
        this->activeItem = new SaberItem(*desc, false);
        this->activeItem->Load();
        INFO("Active Item Set!");
    }

    void SaberManager::SetDefault()
    {
        if (getenv("saberlocked")) return;
        INFO("Active Item ptr: %p", activeItem);
        if (this->activeItem) delete (this->activeItem);
        INFO("Set Default called");
        activeItem = new SaberItem(DescriptorCache::GetDescriptor(""));
    }

    void SaberManager::internalSetActiveModel(std::string name, bool load)
    {
        INFO("Setting active Saber %s, previous ptr: %p", name.c_str(), activeItem);

        // if new set is already the active one, ignore
        if (this->activeItem && this->activeItem->get_descriptor().GetFileName() == name) return;
        Descriptor& newItem = DescriptorCache::GetDescriptor(name);
        // if descriptor doesn't exist for this thing, ignore the setactive
        if (!newItem.isValid())
        {
            ERROR("Item was invalid!");
            return;  
        } 
        if (this->activeItem) delete(this->activeItem);
        this->activeItem = new SaberItem(newItem, false);
        if (load) this->activeItem->Load();
        INFO("Active Item Set!");
    }

    UnityEngine::Transform* SaberManager::get_rightSaber()
    {
        if (!activeItem) return nullptr;
        GameObject* prefab = activeItem->get_prefab();
        if (!prefab) return nullptr;
        Transform* object = prefab->get_transform()->Find(get_rightSaberName());
        if (!object) return nullptr;
        return UnityEngine::Object::Instantiate(object);
    }

    UnityEngine::Transform* SaberManager::get_leftSaber()
    {
        if (!activeItem) return nullptr;
        GameObject* prefab = activeItem->get_prefab();
        if (!prefab) return nullptr;
        Transform* object = prefab->get_transform()->Find(get_leftSaberName());
        if (!object) return nullptr;
        return UnityEngine::Object::Instantiate(object);
    }

    Il2CppString* SaberManager::get_basicSaberModelName()
    {
        if (!basicSaberModelName) basicSaberModelName = il2cpp_utils::createcsstr("BasicSaberModel(Clone)", il2cpp_utils::StringType::Manual);
        return basicSaberModelName;
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