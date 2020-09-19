#pragma once
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/Object.hpp"
#include <string>
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"


namespace Qosmetics
{
    class Descriptor 
    {
        public:
            Descriptor(const rapidjson::Document &config)
            {
                this->name = config["objectName"].GetString();
                author = config["authorName"].GetString();
                description = config["description"].GetString();
            }

            Descriptor(const std::string &name, const std::string &author, const std::string &description, UnityEngine::Texture2D* image = nullptr)
            {
                this->name = name;
                this->author = author;
                this->description = description;
                this->coverImage = image; 
            }

            void SetCoverImage(UnityEngine::Texture2D* image)
            {
                coverImage = image;
                coverImage->DontDestroyOnLoad(coverImage);
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

        private:

            std::string name;
            std::string author;
            std::string description;
            UnityEngine::Texture2D* coverImage;
    };
}
