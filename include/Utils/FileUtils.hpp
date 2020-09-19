#pragma once
#include <string>
#include <vector>
class FileUtils
{
    public:
        /// @brief gets the file names with a given extension in a given directory, outputs to a vector given by reference
        /// @param extension the extension to look for
        /// @param directory full path to the directory (starts with /sdcard probably)
        /// @param fileNames output vector
        static bool getFileNamesInDir(std::string extension, std::string directory, std::vector<std::string> &fileNames);
};