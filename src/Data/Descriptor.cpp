#include "Data/Descriptor.hpp"
#include "Utils/FileUtils.hpp"
#include <map>

static std::map<std::string, Qosmetics::ItemType> StringToType = {
    { "qsaber", Qosmetics::ItemType::saber },
    { "qbloq", Qosmetics::ItemType::note },
    { "qwall", Qosmetics::ItemType::wall },
    { "qpointer", Qosmetics::ItemType::pointer },
    { "qplatform", Qosmetics::ItemType::platform }
};


namespace Qosmetics
{
    ItemType Descriptor::GetTypeFromName(std::string path)
    {
        std::string extension = FileUtils::GetExtension(path);
        std::map<std::string, ItemType>::iterator it = StringToType.find(extension);
        if (it != StringToType.end())
        {
            return it->second;
        }
        return invalid;
    }

    std::string Descriptor::GetFileName(bool removeExtension)
    {
        return FileUtils::GetFileName(filePath, removeExtension);
    }

    rapidjson::Value Descriptor::ToVal(rapidjson::Document::AllocatorType& allocator)
    {
        rapidjson::Value val;
        val.SetObject();

        val.AddMember("name", rapidjson::Value(name.c_str(), name.size(), allocator), allocator);
        val.AddMember("author", rapidjson::Value(author.c_str(), author.size(), allocator), allocator);
        val.AddMember("description", rapidjson::Value(description.c_str(), description.size(), allocator), allocator);
        val.AddMember("type", (int)type, allocator);
        val.AddMember("filePath", rapidjson::Value(filePath.c_str(), filePath.size(), allocator), allocator);
        return val;
    }
}