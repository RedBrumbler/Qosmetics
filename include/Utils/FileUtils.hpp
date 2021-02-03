#pragma once
#include "Data/Descriptor.hpp"

#include <string>
#include <vector>

class FileUtils
{
    public:
        /// @brief gets the file name optionally without extension from passed in path
        /// @param path the path to get it from
        /// @param removeExtension if the extension should be included
        /// @return string name
        static std::string GetFileName(std::string path, bool removeExtension = false);

        /// @brief removes the .extension from passed in string
        /// @param path the string to remove from
        /// @return everything up until the . in the string, if no . found just returns same value
        static std::string RemoveExtension(std::string path);

        /// @brief gets the file extension
        /// @param the string from which to get
        /// @return the extension, or empty if not found
        static std::string GetExtension(std::string path);

        /// @brief gets all files with extension in folder filePath, and outputs the names to vector out
        /// @param extension the extension to look for
        /// @param filePath the folder to look in
        /// @param out the output vector
        /// @return bool found at least 1
        static bool GetFilesInFolderPath(std::string extension, std::string filePath, std::vector<std::string>& out);        
    private:
};