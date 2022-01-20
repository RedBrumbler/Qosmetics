#pragma once
#include "beatsaber-hook/shared/config/rapidjson-utils.hpp"

#include <string>

namespace Qosmetics::Core
{
    class Descriptor
    {
    public:
        Descriptor();
        Descriptor(const Descriptor& other);
        Descriptor(const rapidjson::Value& val, std::string_view filePath);
        Descriptor(const rapidjson::Value& val);

        std::string_view get_author();
        std::string_view get_name();
        std::string_view get_description();
        std::string_view get_filePath();

    private:
        std::string author = "";
        std::string name = "";
        std::string description = "";
        std::string filePath = "";
    };
}