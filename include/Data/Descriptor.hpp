#pragma once
#include <string>
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"

namespace Qosmetics
{
    enum ItemType {
        saber,
        note,
        wall,
        pointer,
        platform,
        invalid = 1000
    };

    class Descriptor
    {
        public:
            Descriptor() {};

            Descriptor(std::string filePath, ItemType type)
            {
                this->filePath = filePath;
                this->type = GetTypeFromName(filePath);
            }

            Descriptor(std::string author, std::string name, std::string description, std::string filePath)
            {
                this->author = author;
                this->name = name;
                this->description = description;
                this->filePath = filePath;
                this->type = GetTypeFromName(filePath);
            }

            static ItemType GetTypeFromName(std::string name);
            
            std::string GetFileName();

            bool isValid() 
            {
                return type != invalid;
            }

            const std::string& get_author()
            {
                return author;
            }

            const std::string& get_name()
            {
                return name;
            }

            const std::string& get_description()
            {
                return description;
            }

            const std::string& get_filePath()
            {
                return filePath;
            }

            bool isType(ItemType type)
            {
                return this->type == type;
            }
    
            rapidjson::Value toVal(rapidjson::Document::AllocatorType& allocator);

        private:
            std::string author = "";
            std::string name = "";
            std::string description = "";
            std::string filePath = "";
            ItemType type = invalid;
    };
}