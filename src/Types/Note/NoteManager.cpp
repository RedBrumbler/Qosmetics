#include "Types/Note/NoteManager.hpp"
#include "Types/Note/NoteItem.hpp"
#include "Data/DescriptorCache.hpp"

DEFINE_CLASS(Qosmetics::NoteManager);

namespace Qosmetics
{
    void NoteManager::internalSetActiveModel(std::string name)
    {
        // if new set is already the active one, ignore
        if (activeItem->get_descriptor().GetFileName() == name) return;
        Descriptor& newItem = DescriptorCache::GetDescriptor(name);
        // if descriptor doesn't exist for this thing, ignore the setactive
        if (!newItem.isValid()) return;
        delete(activeItem);
        activeItem = new NoteItem(newItem, true);
    }
}