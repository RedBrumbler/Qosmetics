#include "Data/QosmeticsDescriptorCache.hpp"
#include "Utils/FileUtils.hpp"
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/StringBuffer.h"
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/PrettyWriter.h"
#include "beatsaber-hook/shared/utils/logging.hpp"

// since they are pointers, a direct comparison will directly point out if they are the exact same pointer
#define RET_IF_CONTAINS(descriptors, descriptor) \
for (auto& check : descriptors) \
{\
    if (check == descriptor) return;\
}

extern Logger& getLogger();

namespace Qosmetics
{
    bool DescriptorCache::Load()
    {
        getLogger().info("Loading Qosmetics Descriptor Cache");
        CreateCacheIfNeeded(false);
        // if file doesn't exist return false
        if (!fileexists(DESCRIPTOR_FILEPATH)) return false;

        // read data, if data empty return false
        std::string dataString = readfile(DESCRIPTOR_FILEPATH);
        if (dataString == "") return false;
        cache->cacheDocument.Parse(dataString.c_str());

        cache->LoadDescriptorsFromCacheIntoVector("sabers", cache->saberDescriptors);
        cache->LoadDescriptorsFromCacheIntoVector("notes", cache->noteDescriptors);
        cache->LoadDescriptorsFromCacheIntoVector("walls", cache->wallDescriptors);

        // everything has been loaded, return true
        getLogger().info("Qosmetics Descriptor Cache Loaded Succesfully!");
        return true;
    }

    void DescriptorCache::LoadDescriptorsFromCacheIntoVector(std::string cacheName, std::vector<Descriptor*>& cacheVector)
    {
        if (!cacheDocument.HasMember(cacheName.c_str())) return;
        const rapidjson::Value& descriptors = cache->cacheDocument[cacheName.c_str()];
        assert(descriptors.IsArray());
        for (auto& descriptor : descriptors.GetArray())
        {
            if (!fileexists(descriptor["filePath"].GetString())) continue; // if the file does not exist, don't try to add the descriptor to the cache as it is now invalid
            if (descriptor["type"].GetInt() == 1000) continue; // if the type is 1000, if it ended up in the cache, ignore it
            cacheVector.push_back(LoadDescriptorFromValue(descriptor));
        }
    }

    // Descriptor(const std::string &name, const std::string &author, const std::string &description, const std::string &filePath = "", const qosmeticsType &type = saber, UnityEngine::Texture2D* image = nullptr)
    Descriptor* DescriptorCache::LoadDescriptorFromValue(const rapidjson::Value& value)
    {
        return new Descriptor(value["name"].GetString(),
                              value["author"].GetString(),
                              value["description"].GetString(),
                              value["filePath"].GetString(),
                              (qosmeticsType)value["type"].GetInt());
    }

    void DescriptorCache::Write()
    {
        getLogger().info("Writing Qosmetics Descriptors Cache to file");
        CreateCacheIfNeeded();
        cache->cacheDocument.RemoveAllMembers();
        cache->cacheDocument.SetObject();
        rapidjson::Document::AllocatorType& allocator = cache->cacheDocument.GetAllocator();
        
        cache->AddDescriptorVectorToCache(cache->saberDescriptors, allocator, "sabers");
        cache->AddDescriptorVectorToCache(cache->noteDescriptors, allocator, "notes");
        cache->AddDescriptorVectorToCache(cache->wallDescriptors, allocator, "walls");

        cache->WriteDocument();
    }

    void DescriptorCache::WriteDocument()
    {
        // stringify document
        rapidjson::StringBuffer buffer;
        buffer.Clear();

        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
        cache->cacheDocument.Accept(writer);

        // make string out of buffer data
        std::string json(buffer.GetString(), buffer.GetSize());

        // write to file
        writefile(DESCRIPTOR_FILEPATH, json);
    }

    void DescriptorCache::AddDescriptorVectorToCache(std::vector<Descriptor*>& descriptorVector, rapidjson::Document::AllocatorType& allocator, std::string name)
    {
        // make new array value
        rapidjson::Value array(rapidjson::kArrayType);
        array.SetArray();
        // add all the descriptor data to it
        for (auto* descriptor : descriptorVector)
        {
            if (descriptor->get_type() == qosmeticsType::invalid) continue; // do not write invalid descriptors to file
            rapidjson::Value newDescriptor;
            newDescriptor.SetObject();
            AddDescriptorToValue(newDescriptor, allocator, descriptor);
            array.PushBack(newDescriptor, allocator);
        }

        // add it to the document
        rapidjson::Value cachedName(name.c_str(), name.size(), allocator);
        cacheDocument.AddMember(cachedName, array, allocator);
    }

    void DescriptorCache::AddDescriptorToValue(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator, Descriptor* descriptor)
    {
        std::string fileName = descriptor->get_fileName();
        std::string name = descriptor->get_name();
        std::string author = descriptor->get_author();
        std::string description = descriptor->get_description();
        std::string filePath = descriptor->get_filePath();
        
        value.AddMember("fileName", rapidjson::Value(fileName.c_str(), fileName.size(), allocator), allocator);
        value.AddMember("name", rapidjson::Value(name.c_str(), name.size(), allocator), allocator);
        value.AddMember("author", rapidjson::Value(author.c_str(), author.size(), allocator), allocator);
        value.AddMember("description", rapidjson::Value(description.c_str(), description.size(), allocator), allocator);
        value.AddMember("filePath", rapidjson::Value(filePath.c_str(), filePath.size(), allocator), allocator);
        value.AddMember("type", (int)descriptor->get_type(), allocator);
    }

    void DescriptorCache::AddToSaberCache(Descriptor* descriptor)
    {
        RET_IF_CONTAINS(saberDescriptors, descriptor)
        if (descriptor->get_type() != qosmeticsType::saber) 
        {
            getLogger().info("not adding to cache because descriptor was not correct");
            return;
        }
        saberDescriptors.push_back(descriptor);
    }

    void DescriptorCache::AddToNoteCache(Descriptor* descriptor)
    {
        RET_IF_CONTAINS(noteDescriptors, descriptor)
        if (descriptor->get_type() != qosmeticsType::note) 
        {
            getLogger().info("not adding to cache because descriptor was not correct");
            return;
        }
        noteDescriptors.push_back(descriptor);
    }

    void DescriptorCache::AddToWallCache(Descriptor* descriptor)
    {
        RET_IF_CONTAINS(wallDescriptors, descriptor)
        if (descriptor->get_type() != qosmeticsType::wall) 
        {
            getLogger().info("not adding to cache because descriptor was not correct");
            return;
        }
        wallDescriptors.push_back(descriptor);
    }

    Descriptor* DescriptorCache::GetDescriptor(std::string fileName, qosmeticsType type)
    {
        getLogger().info("getting requested descriptor for file %s", fileName.c_str());
        if (fileName == "") return invalid;
        int i = 0;
        switch (type)
        {
            case saber:
                for (auto* descriptor : cache->saberDescriptors)
                {
                    if (descriptor->get_fileName().find(fileName) != std::string::npos) return descriptor;
                    i++;
                }
                return invalid;
                break;
            case note:
                for (auto* descriptor : cache->noteDescriptors)
                {
                    if (descriptor->get_fileName().find(fileName) != std::string::npos) return descriptor;
                    i++;
                }
                return invalid;
                break;
            case wall:
                for (auto* descriptor : cache->wallDescriptors)
                {
                    if (descriptor->get_fileName().find(fileName) != std::string::npos) return descriptor;
                    i++;
                }
                return invalid;
                break;
            default:
                return invalid;
                break;
        }
    }
}