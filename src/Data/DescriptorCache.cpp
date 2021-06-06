#include "Data/DescriptorCache.hpp"
#include "Utils/FileUtils.hpp"
#include "static-defines.hpp"
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/prettywriter.h"
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/stringbuffer.h"
#include <thread>
#include "QosmeticsLogger.hpp"

#define INFO(value...) QosmeticsLogger::GetContextLogger("Descriptor Cache").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("Descriptor Cache").error(value)

namespace Qosmetics
{
    void DescriptorCache::Save()
    {
        //std::thread SaveToFile([]{
            INFO("Saving Descriptor Cache...");
            rapidjson::Document d;
            d.SetObject();
            rapidjson::Document::AllocatorType& allocator = d.GetAllocator();

            d.AddMember("sabers", MapToValue(descriptors[ItemType::saber], allocator), allocator);
            d.AddMember("notes", MapToValue(descriptors[ItemType::note], allocator), allocator);
            d.AddMember("walls", MapToValue(descriptors[ItemType::wall], allocator), allocator);
            d.AddMember("pointers", MapToValue(descriptors[ItemType::pointer], allocator), allocator);
            d.AddMember("platforms", MapToValue(descriptors[ItemType::platform], allocator), allocator);

            Write(d);
            INFO("Saved Descriptor Cache!");
        //});

        //SaveToFile.detach();
    }

    void DescriptorCache::Write(rapidjson::Document& d)
    {
        // stringify document
        rapidjson::StringBuffer buffer;
        buffer.Clear();

        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
        d.Accept(writer);

        // make string out of buffer data
        std::string json(buffer.GetString(), buffer.GetSize());

        // write to file
        writefile(DESCRIPTORCACHE, json);
    }

    bool DescriptorCache::Load()
    {
        INFO("Loading Descriptor Cache...");
        if (!fileexists(DESCRIPTORCACHE)) return false;
        //std::thread LoadFromFile([]{
            std::string json = readfile(DESCRIPTORCACHE);
            rapidjson::Document d;
            d.Parse(json.c_str());

            if (d.HasMember("sabers")) LoadDescriptorsIntoMap(d["sabers"], descriptors[ItemType::saber]);
            if (d.HasMember("walls")) LoadDescriptorsIntoMap(d["walls"], descriptors[ItemType::wall]);
            if (d.HasMember("notes")) LoadDescriptorsIntoMap(d["notes"], descriptors[ItemType::note]);
            if (d.HasMember("pointers")) LoadDescriptorsIntoMap(d["pointers"], descriptors[ItemType::pointer]);
            if (d.HasMember("platforms")) LoadDescriptorsIntoMap(d["platforms"], descriptors[ItemType::platform]);
            INFO("Loaded Descriptor Cache File!");

            INFO("Checking for any new files");
            bool anyNew = false;
            if (DescriptorCache::DescriptorsFromFolder(SABERPATH)) anyNew = true;
            if (DescriptorCache::DescriptorsFromFolder(WALLPATH)) anyNew = true;
            if (DescriptorCache::DescriptorsFromFolder(NOTEPATH)) anyNew = true;

            INFO("Done checking for any new files in folder paths");

            if (anyNew)
            {
                INFO("New descriptors were found, writing to file");
                Save();
            }
        //});
        //LoadFromFile.detach();
        return true;
    }

    Descriptor& DescriptorCache::AddDescriptorToCache(Descriptor& descriptor)
    {
        if (!descriptor.isValid()) return invalid;
        return AddDescriptorToMap(descriptor, descriptors[descriptor.get_type()]);
    }

    Descriptor& DescriptorCache::AddDescriptorToMap(Descriptor& descriptor, Cache& map)
    {
        std::string fileName = descriptor.GetFileName();
        map[fileName] = descriptor;
        return GetDescriptorFromMap(fileName, map);
    }

    Descriptor& DescriptorCache::GetDescriptor(std::string fileName)
    {
        ItemType type = Descriptor::GetTypeFromName(fileName);
        if (type == ItemType::invalid) return invalid;
        return GetDescriptorFromMap(fileName, descriptors[type]);

        /*
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
        */
    }

    Descriptor& DescriptorCache::GetDescriptorFromMap(std::string& fileName, Cache& map)
    {
        std::string key = FileUtils::GetFileName(fileName);
        Cache::iterator it = map.find(key);
        if (it != map.end())
        {
            return it->second;
        }
        return invalid;
    }

    void DescriptorCache::LoadDescriptorsIntoMap(rapidjson::Value& val, Cache& out)
    {
        for (auto& descriptor : val.GetArray())
        {
            Descriptor desc = Descriptor(descriptor);
            out[desc.GetFileName()] = desc;
        }
    }

    rapidjson::Value DescriptorCache::MapToValue(Cache& map, rapidjson::Document::AllocatorType& allocator)
    {
        rapidjson::Value array(rapidjson::kArrayType);
        array.SetArray();

        for (auto& pair : map)
        {
            array.PushBack(pair.second.ToVal(allocator), allocator);
        }

        return array;
    }

    bool DescriptorCache::DescriptorsFromFolder(std::string folderPath)
    {
        bool gotAny = false;
        std::vector<std::string> fileNames = {};
        INFO("making descriptors for files in folder %s", folderPath.c_str());
        if (!FileUtils::GetFilesInFolderPath("", folderPath, fileNames)) 
        {
            ERROR("No descriptors were found, returning!");
            return false;
        }

        for (auto& name : fileNames)
        {
            Descriptor& gotten = GetDescriptor(name);
            if (gotten.isValid()) 
            {
                continue;
            }
            // if it was valid (didn't exist) make a new descriptor and add to cache
            ItemType type = Descriptor::GetTypeFromName(name);
            std::string filePath = folderPath + name;
            Descriptor newDescriptor = Descriptor(filePath);
            INFO("Created descriptor with file path %s, and type %d", filePath.c_str(), (int)type);
            gotAny = true;
            AddDescriptorToCache(newDescriptor);
        }

        return gotAny;
    }
}