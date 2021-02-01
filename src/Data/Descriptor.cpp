#include "Descriptor.hpp"
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

    std::string Descriptor::GetFileName()
    {
        return FileUtils::GetFileName(filePath, true);
    }

    rapidjson::Value Descriptor::toVal(rapidjson::Document::AllocatorType& allocator)
    {
        rapidjson::Value val;
        assert(val.IsObject());

        val.AddMember("name", name, allocator);
        val.AddMember("author", author, allocator);
        val.AddMember("description", description, allocator);
        val.AddMember("type", type, allocator);
        val.AddMember("filePath", filePath, allocator);
        return val;
    }
}