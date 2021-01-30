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

        static std::string rainbowIfy(std::string input);
    private:
        static inline int rainbowIndex = rand() % 12;
        static inline const std::string colors[12] = {
            "#ff6060",
            "#ffa060",
            "#ffff60",
            "#a0ff60",
            "#60ff60",
            "#60ffa0",
            "#60ffff",
            "#60a0ff",
            "#6060ff",
            "#a060ff",
            "#ff60ff",
            "#ff60a0"
        };
};