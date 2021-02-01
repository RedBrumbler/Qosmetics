#include "Data/DescriptorCache.hpp"
#include "Utils/FileUtils.hpp"
#include "static-defines.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"

namespace Qosmetics
{
    void DescriptorCache::Save()
    {

    }

    void DescriptorCache::Write()
    {

    }

    bool DescriptorCache::Load()
    {
        if (!fileexists(DESCRIPTORCACHE)) return false;
        std::string json = readfile(DESCRIPTORCACHE);
        if (json == "") return false;
        rapidjson::Document d;
        d.Parse(json.c_str());

        LoadDescriptorsIntoMap(d["sabers"], saberDescriptors);
        LoadDescriptorsIntoMap(d["walls"], wallDescriptors);
        LoadDescriptorsIntoMap(d["notes"], noteDescriptors);
        LoadDescriptorsIntoMap(d["pointers"], pointerDescriptors);
        LoadDescriptorsIntoMap(d["platforms"], platformDescriptors);

        return true;
    }

    Descriptor& DescriptorCache::AddDescriptorToCache(Descriptor& descriptor)
    {
        switch(descriptor.get_type())
        {
            case saber:
                return AddDescriptorToMap(descriptor, saberDescriptors);
            case note:
                return AddDescriptorToMap(descriptor, noteDescriptors);
            case wall:
                return AddDescriptorToMap(descriptor, wallDescriptors);
            case pointer:
                return AddDescriptorToMap(descriptor, pointerDescriptors);
            case platform:
                return AddDescriptorToMap(descriptor, platformDescriptors);
            default:
                return invalid;
        }
    }

    Descriptor& DescriptorCache::AddDescriptorToMap(Descriptor& descriptor, std::map<std::string, Descriptor>& map)
    {
        std::string fileName = descriptor.GetFileName();
        map[fileName] = descriptor;
        return GetDescriptorFromMap(fileName, map);
    }

    Descriptor& DescriptorCache::GetDescriptor(std::string fileName)
    {
        ItemType type = Descriptor::GetTypeFromName(fileName);
        switch (type)
        {
            case saber:
                return GetDescriptorFromMap(fileName, saberDescriptors);
            case note:
                return GetDescriptorFromMap(fileName, noteDescriptors);
            case wall:
                return GetDescriptorFromMap(fileName, wallDescriptors);
            case pointer:
                return GetDescriptorFromMap(fileName, pointerDescriptors);
            case platform:
                return GetDescriptorFromMap(fileName, platformDescriptors);
            default:
                return invalid;
        }
    }

    Descriptor& DescriptorCache::GetDescriptorFromMap(std::string& fileName, std::map<std::string, Descriptor>& map)
    {
        std::string key = FileUtils::GetFileName(fileName);
        std::map<std::string, Descriptor>::iterator it = map.find(key);
        if (it != map.end())
        {
            return it->second;
        }
        return invalid;
    }

    void DescriptorCache::LoadDescriptorsIntoMap(rapidjson::Value& val, std::map<std::string, Descriptor>& out)
    {
        for (auto& descriptor : val.GetArray())
        {
            Descriptor desc = Descriptor(descriptor);
            out[desc.GetFileName()] = desc;
        }
    }

    rapidjson::Value DescriptorCache::MapToValue(std::map<std::string, Descriptor>& map, rapidjson::Document::AllocatorType& allocator)
    {
        rapidjson::Value array(rapidjson::kArrayType);
        array.SetArray();

        for (auto& pair : map)
        {
            array.PushBack(pair.second.ToVal(allocator), allocator);
        }

        return array;
    }
}