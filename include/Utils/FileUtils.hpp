#pragma once
#include <string>
#include <vector>
#include "UnityEngine/Sprite.hpp"

class FileUtils
{
    public:
        /// @brief gets the file names with a given extension in a given directory, outputs to a vector given by reference
        /// @param extension the extension to look for
        /// @param directory full path to the directory (starts with /sdcard probably)
        /// @param fileNames output vector
        static bool getFileNamesInDir(std::string extension, std::string directory, std::vector<std::string> &fileNames);

        /// @brief gets the full file name from a file path
        /// @param FilePath the filepath to get the file name from
        /// @returns File name
        static std::string GetFileName(const std::string& FilePath);

        static UnityEngine::Sprite* SpriteFromFile(const std::string& filePath, int width, int height);

        static void DownloadFileToPath(const std::string& url, std::string& filePath, bool overWrite = false);
};