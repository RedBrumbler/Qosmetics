#include "Types/Saber/SaberManager.hpp"
#include "Types/Saber/SaberItem.hpp"
#include "Data/DescriptorCache.hpp"
#include "QosmeticsLogger.hpp"

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
        std::string name = to_utf8(csstrtostr(csname));
        internalSetActiveModel(name);
    }

    void SaberManager::internalSetActiveModel(std::string name)
    {
        INFO("Setting active Saber %s", name.c_str());
        // if new set is already the active one, ignore
        if (activeItem && activeItem->get_descriptor().GetFileName() == name) return;
        Descriptor& newItem = DescriptorCache::GetDescriptor(name);
        // if descriptor doesn't exist for this thing, ignore the setactive
        if (!newItem.isValid()) return;
        delete(activeItem);
        if (this->prefab) 
        {
            UnityEngine::Object::Destroy(this->prefab);
            this->prefab = nullptr;
        }
        activeItem = new SaberItem(newItem, true);
        activeItem->SetCompleteCallback([&](SaberItem& item){
            this->prefab = Object::Instantiate(item.get_prefab(), get_transform());
            this->prefab->SetActive(true);
        });
    }

    UnityEngine::Transform* SaberManager::get_rightSaber()
    {
        if (!this->prefab) return nullptr;
        return get_transform()->GetChild(0)->Find(get_rightSaberName());
    }

    UnityEngine::Transform* SaberManager::get_leftSaber()
    {
        if (!this->prefab) return nullptr;
        return get_transform()->GetChild(0)->Find(get_leftSaberName());
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

    void SaberManager::SetActiveSaber(std::string name)
    {
        internalSetActiveModel(name);
    }

    ItemType SaberManager::get_type()
    {
        return activeItem->get_type();
    }
    
    SaberItem& SaberManager::get_item()
    {
        INFO("Item ptr: %p", activeItem);
        return *this->activeItem;
    }
}