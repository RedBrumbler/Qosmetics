#pragma once

#include <string>
#include <vector>

namespace Qosmetics::Core::FileUtils
{
    /// @brief gets a list of files in a folder path
    /// @param extension the extension of the file
    /// @param folderPath the path of the folder to get all files in
    /// @param out reference to the output vector
    /// @return bool found any, true if found, else false
    bool GetFilesInFolderPath(std::string_view extension, std::string_view folderPath, std::vector<std::string>& out);

    /// @brief gets the filename from a path
    /// @param path the path to get the filename form
    /// @param removeExtension whether or not to also remove the extension of the file name, more just convenience
    /// @return string view to the file name
    std::string_view GetFileName(std::string_view path, bool removeExtension = false);

    /// @brief removes the extension of a file path (everything after the .)
    /// @param path the path to remove it from
    /// @return string_view to the path without an extension
    std::string_view RemoveExtension(std::string_view path);

    /// @brief Gets the file extension of a file path (evertything after the last .)
    /// @param path the path to get it from
    /// @return string_view to the extension
    std::string_view GetExtension(std::string_view path);

}