#pragma once
#include "custom-types/shared/types.hpp"
#include "custom-types/shared/macros.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"

#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/Object.hpp"
#include <string>
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"

typedef enum _qosmeticsType {
    saber,          // 0
    bloq,           // 1
    wall,           // 2
    menupointer,    // 3
    platform        // 4
} qosmeticsType;

namespace Qosmetics
{
    class Descriptor 
    {
        public:
            Descriptor(const rapidjson::Document &config, const std::string &filePath = "", const qosmeticsType &type = saber)
            {
                this->name = config["objectName"].GetString();
                this->author = config["authorName"].GetString();
                this->description = config["description"].GetString();
                this->filePath = filePath;
                this->type = type;
            }

            Descriptor(const std::string &name, const std::string &author, const std::string &description, const std::string &filePath = "", const qosmeticsType &type = saber, UnityEngine::Texture2D* image = nullptr)
            {
                this->name = name;
                this->author = author;
                this->description = description;
                this->coverImage = image; 
                this->filePath = filePath;
                this->type = type;
            }

            void SetCoverImage(UnityEngine::Texture2D* image)
            {
                this->coverImage = image;
                this->coverImage->DontDestroyOnLoad(coverImage);
            }

            const qosmeticsType get_type()
            {
                return type;
            }

            const std::string get_name()
            {
                return name;
            }

            const std::string get_author()
            {
                return author;
            }

            const std::string get_description()
            {
                return description;
            }

            const UnityEngine::Texture2D* get_coverImage()
            {
                return coverImage;
            }

            const std::string get_filePath()
            {
                return filePath;
            }

            bool operator<(const Descriptor& other) const
            {
                return this->name < other.name;
            }

        private:
            std::string name;
            std::string author;
            std::string description;
            std::string filePath;
            UnityEngine::Texture2D* coverImage;
            qosmeticsType type;
    };
}

// C# wrapper type
DECLARE_CLASS_CODEGEN(Qosmetics, DescriptorWrapper, Il2CppObject,
    DECLARE_INSTANCE_FIELD_DEFAULT(void*, descriptor, nullptr);
    DECLARE_CTOR(ctor);
    REGISTER_FUNCTION(DescriptorWrapper,
        REGISTER_METHOD(ctor);
        REGISTER_FIELD(descriptor);
    )
)