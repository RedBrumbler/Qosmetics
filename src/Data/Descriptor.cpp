#include "Data/Descriptor.hpp"

namespace Qosmetics::Core
{
    Descriptor::Descriptor(){};

    Descriptor::Descriptor(const Descriptor& other) : name(other.name), author(other.author), description(other.description), filePath(other.filePath){};

    Descriptor::Descriptor(const rapidjson::Value& val, std::string_view filePath)
    {
        name = val["objectName"].GetString();
        author = val["author"].GetString();
        description = val["description"].GetString();
        this->filePath = filePath;
    }

    Descriptor::Descriptor(const rapidjson::Value& val)
    {
        name = val["objectName"].GetString();
        author = val["author"].GetString();
        description = val["description"].GetString();
        filePath = val["filePath"].GetString();
    }

    std::string_view Descriptor::get_author() { return author; }
    std::string_view Descriptor::get_name() { return name; }
    std::string_view Descriptor::get_description() { return description; }
    std::string_view Descriptor::get_filePath() { return filePath; }

}