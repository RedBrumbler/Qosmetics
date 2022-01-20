#pragma once
#include "../Data/BasicConfig.hpp"
#include "../Data/Descriptor.hpp"
#include "../Utils/ZipUtils.hpp"

namespace Qosmetics::Core
{
    /// @tparam T config for the object, requires a default & const rapidjson::Value& ctor
    template <class T = BasicConfig>
    class Manifest
    {
    public:
        Manifest(std::string filePath) : filePath(filePath)
        {
            std::vector<uint8_t> data = {};
            if (!ZipUtils::GetBytesFromZipFile(filePath, "package.json", data)) return;
            rapidjson::Document d;
            d.Parse((char*)data.data(), data.size());

            fileName = d["androidFileName"].GetString();
            descriptor = Descriptor(d["descriptor"], filePath);
            config = std::make_shared<T>(d["config"]);
        }

        /// @brief returrns a const reference to the filepath string
        virtual const std::string& get_filePath() const
        {
            return filePath;
        }

        /// @brief returns the fileName, will need appending of _platform though
        virtual const std::string& get_fileName() const
        {
            return fileName;
        }

        /// @brief returns a const reference to the descriptor
        virtual const Descriptor& get_descriptor() const
        {
            return descriptor;
        }

        /// @brief returns a const reference to the config
        virtual const T& get_config() const
        {
            return *config;
        }

    protected:
        std::string filePath;
        std::string fileName;
        Descriptor descriptor = Descriptor();
        std::shared_ptr<T> config = std::make_shared<T>();
    };
}