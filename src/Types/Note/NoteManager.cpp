#include "Types/Note/NoteManager.hpp"
#include "Types/Note/NoteItem.hpp"
#include "Data/DescriptorCache.hpp"
#include "UnityEngine/Transform.hpp"
#include "QosmeticsLogger.hpp"

DEFINE_CLASS(Qosmetics::NoteManager);

#define INFO(value...) QosmeticsLogger::GetContextLogger("Note Manager").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("Note Manager").error(value)

using namespace UnityEngine;

static inline Il2CppString* leftArrowName = nullptr;
static inline Il2CppString* rightArrowName = nullptr;
static inline Il2CppString* leftDotName = nullptr;
static inline Il2CppString* rightDotName = nullptr;
static inline Il2CppString* leftDebrisName = nullptr;
static inline Il2CppString* rightDebrisName = nullptr;
static inline Il2CppString* bombName = nullptr;

namespace Qosmetics
{
    GameObject* NoteManager::GetActivePrefab()
    {
        if (!activeItem) return nullptr;
        return activeItem->get_prefab();
    }

    void NoteManager::SetActiveModel(Il2CppString* csname)
    {
        if (!csname) return;
        std::string name = to_utf8(csstrtostr(csname));
        internalSetActiveModel(name, true);
    }

    void NoteManager::SetDefault()
    {
        if (activeItem) delete (activeItem);
        activeItem = new NoteItem(DescriptorCache::GetDescriptor(""));
    }

    void NoteManager::internalSetActiveModel(std::string name, bool load)
    {
        INFO("Setting active Note %s", name.c_str());
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
        activeItem = new NoteItem(newItem, load);
        INFO("Active Item Set!");
    }

    NoteItem& NoteManager::get_item()
    {
        CRASH_UNLESS(activeItem);
        return *activeItem;
    }
    
    ItemType NoteManager::get_type()
    {
        return ItemType::note; 
    }

    GameObject* NoteManager::get_leftArrow()
    {
        if (!activeItem) return nullptr;
        GameObject* prefab = activeItem->get_prefab();
        if (!prefab) return nullptr;
        Transform* object = prefab->get_transform()->Find(get_leftArrowName());
        if (!object) return nullptr;
        return Object::Instantiate(object)->get_gameObject();
    }

    GameObject* NoteManager::get_rightArrow()
    {
        if (!activeItem) return nullptr;
        GameObject* prefab = activeItem->get_prefab();
        if (!prefab) return nullptr;
        Transform* object = prefab->get_transform()->Find(get_rightArrowName());
        if (!object) return nullptr;
        return Object::Instantiate(object)->get_gameObject();
    }

    GameObject* NoteManager::get_leftDot()
    {
        if (!activeItem) return nullptr;
        GameObject* prefab = activeItem->get_prefab();
        if (!prefab) return nullptr;
        Transform* object = prefab->get_transform()->Find(get_leftDotName());
        if (!object) return nullptr;
        return Object::Instantiate(object)->get_gameObject();
    }

    GameObject* NoteManager::get_rightDot()
    {
        if (!activeItem) return nullptr;
        GameObject* prefab = activeItem->get_prefab();
        if (!prefab) return nullptr;
        Transform* object = prefab->get_transform()->Find(get_rightDotName());
        if (!object) return nullptr;
        return Object::Instantiate(object)->get_gameObject();
    }

    GameObject* NoteManager::get_leftDebris()
    {
        if (!activeItem) return nullptr;
        GameObject* prefab = activeItem->get_prefab();
        if (!prefab) return nullptr;
        Transform* object = prefab->get_transform()->Find(get_leftDebrisName());
        if (!object) return nullptr;
        return Object::Instantiate(object)->get_gameObject();
    }

    GameObject* NoteManager::get_rightDebris()
    {
        if (!activeItem) return nullptr;
        GameObject* prefab = activeItem->get_prefab();
        if (!prefab) return nullptr;
        Transform* object = prefab->get_transform()->Find(get_rightDebrisName());
        if (!object) return nullptr;
        return Object::Instantiate(object)->get_gameObject();
    }

    GameObject* NoteManager::get_bomb()
    {
        if (!activeItem) return nullptr;
        GameObject* prefab = activeItem->get_prefab();
        if (!prefab) return nullptr;
        Transform* object = prefab->get_transform()->Find(get_bombName());
        if (!object) return nullptr;
        return Object::Instantiate(object)->get_gameObject();
    }

    Il2CppString* NoteManager::get_leftArrowName()
    {
        if (!leftArrowName) leftArrowName = il2cpp_utils::createcsstr("LeftArrow", il2cpp_utils::StringType::Manual);
        return leftArrowName;
    }   

    Il2CppString* NoteManager::get_rightArrowName()
    {
        if (!rightArrowName) rightArrowName = il2cpp_utils::createcsstr("RightArrow", il2cpp_utils::StringType::Manual);
        return rightArrowName;
    }   

    Il2CppString* NoteManager::get_leftDotName()
    {
        if (!leftDotName) leftDotName = il2cpp_utils::createcsstr("LeftDot", il2cpp_utils::StringType::Manual);
        return leftDotName;
    }

    Il2CppString* NoteManager::get_rightDotName()
    {
        if (!rightDotName) rightDotName = il2cpp_utils::createcsstr("RightDot", il2cpp_utils::StringType::Manual);
        return rightDotName;
    }

    Il2CppString* NoteManager::get_leftDebrisName()
    {
        if (!leftDebrisName) leftDebrisName = il2cpp_utils::createcsstr("LeftDebris", il2cpp_utils::StringType::Manual);
        return leftDebrisName;
    }

    Il2CppString* NoteManager::get_rightDebrisName()
    {
        if (!rightDebrisName) rightDebrisName = il2cpp_utils::createcsstr("RightDebris", il2cpp_utils::StringType::Manual);
        return rightDebrisName;
    }

    Il2CppString* NoteManager::get_bombName()
    {
        if (!bombName) bombName = il2cpp_utils::createcsstr("Bomb", il2cpp_utils::StringType::Manual);
        return bombName;
    }

}