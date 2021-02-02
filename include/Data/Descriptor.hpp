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

            Descriptor (ItemType type)
            {
                if (type == ItemType::invalid)
                {
                    author = "invalid";
                    name = "invalid";
                    filePath = "invalid";
                    description = "invalid";
                }

                this->type = type;
            }

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

            Descriptor(rapidjson::Value& val)
            {
                author = val["author"].GetString();
                name = val["name"].GetString();
                description = val["description"].GetString();
                type = (ItemType)val["type"].GetInt();
                filePath = val["filePath"].GetString();
            }

            Descriptor(rapidjson::Value& val, std::string filePath)
            {
                author = val["authorName"].GetString();
                name = val["objectName"].GetString();
                description = val["description"].GetString();
                type = GetTypeFromName(filePath);
                this->filePath = filePath;
            }

            static ItemType GetTypeFromName(std::string name);
            
            std::string GetFileName(bool removeExtension = false);

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

            ItemType get_type()
            {
                return type;
            }

            bool isType(ItemType type)
            {
                return this->type == type;
            }

            void CopyFrom(Descriptor& descriptor)
            {
                author = descriptor.author;
                name = descriptor.name;
                description = descriptor.description;
                filePath = descriptor.filePath;
            }
    
            rapidjson::Value ToVal(rapidjson::Document::AllocatorType& allocator);

        private:
            std::string author = "---";
            std::string name = "";
            std::string description = "legacy saber";
            std::string filePath = "";
            ItemType type = invalid;
    };
}