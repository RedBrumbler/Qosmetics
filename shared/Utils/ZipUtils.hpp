#pragma once
#include "zip/shared/zip.h"
#include <string>
#include <vector>

namespace Qosmetics::Core::ZipUtils
{
    /// @brief gets bytes from a zip file into the passed vector reference
    /// @param zipPath the filePath for the zip
    /// @param fileName which file to get from the zip
    /// @param out the reference to the output vector
    /// @return true if succesful, false otherwise
    bool GetBytesFromZipFile(std::string_view zipPath, std::string_view fileName, std::vector<uint8_t>& out);
}