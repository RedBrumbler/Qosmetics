#include "Types/Qosmetic/ModelManager.hpp"
#include "Data/DescriptorCache.hpp"

DEFINE_CLASS(Qosmetics::ModelManager);

using namespace UnityEngine;

namespace Qosmetics
{
    GameObject* ModelManager::GetActivePrefab()
    {
        if (!activeItem) return nullptr;
        return activeItem->get_prefab();
    }

    void ModelManager::SetActiveModel(Il2CppString* csname)
    {
        if (!csname) return;
        std::string name = to_utf8(csstrtostr(csname));
        internalSetActiveModel(name);
    }

    void ModelManager::internalSetActiveModel(std::string name)
    {
        // if new set is already the active one, ignore
        if (activeItem && activeItem->get_descriptor().GetFileName() == name) return;
        Descriptor& newItem = DescriptorCache::GetDescriptor(name);
        // if descriptor doesn't exist for this thing, ignore the setactive
        if (!newItem.isValid()) return;
        delete(activeItem);
        activeItem = new QosmeticItem(newItem, true);
    }
}