#include "Utils/DisablingUtils.hpp"

#include <vector>
#include <map>
#include "QosmeticsLogger.hpp"

#define INFO(value...) QosmeticsLogger::GetContextLogger("Disabling Utils").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("Disabling Utils").error(value)

using namespace Qosmetics;

std::string ItemTypeToStr(ItemType& type)
{
    switch(type)
    {
        case ItemType::saber: return "saber";
        case ItemType::note: return "note";
        case ItemType::wall: return "wall";
        case ItemType::pointer: return "pointer";
        case ItemType::platform: return "platform";
        default: return "invalid";
    }
}

namespace Qosmetics::Disabling
{
    std::map<ItemType, std::vector<ModInfo>> disablingModInfos = {
        { ItemType::saber, {}},
        { ItemType::note, {}},
        { ItemType::wall, {}},
        { ItemType::pointer, {}},
        { ItemType::platform, {}}
    };

    bool get_enabled(ItemType type)
    {
        return disablingModInfos[type].size() == 0;
    }

    void RegisterDisablingInfo(ModInfo info, ItemType type)
    {
        auto vecitr = disablingModInfos.find(type);
        if (vecitr == disablingModInfos.end()) 
        {
            ERROR("Invalid itemtype passed to disable function, returning");
            return;
        }

        auto it = std::find_if(vecitr->second.begin(), vecitr->second.end(), [&](auto x){ return x.id == info.id; });
        if (it == vecitr->second.end()) // not found-> this is new
        {
            INFO("Mod %s is disabling %s!", info.id.c_str(), ItemTypeToStr(type).c_str());
            vecitr->second.push_back(info);
        }
        else
        {
            INFO("Mod %s tried to disable %s multiple times!", info.id.c_str(), ItemTypeToStr(type).c_str());
        }
    }

    void UnregisterDisablingInfo(ModInfo info, ItemType type)
    {
        auto vecitr = disablingModInfos.find(type);
        if (vecitr == disablingModInfos.end()) 
        {
            ERROR("Invalid itemtype passed to disable function, returning");
            return;
        }
        auto it = std::find_if(vecitr->second.begin(), vecitr->second.end(), [&](auto x){ return x.id == info.id; });
        if (it == vecitr->second.end()) // not found -> not disabling
        {
            INFO("Mod %s was not disabling %s! ignoring", info.id.c_str(), ItemTypeToStr(type).c_str());
        }
        else
        {
            INFO("Mod %s is no longer disabling %s!", info.id.c_str(), ItemTypeToStr(type).c_str());
            vecitr->second.erase(it, it + 1);
        }
    }
}