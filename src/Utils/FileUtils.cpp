#include "../include/Utils/FileUtils.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/ImageConversion.hpp"
#include "UnityEngine/SpriteMeshType.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/Vector4.hpp"
#include "UnityEngine/Rect.hpp"
#include "UnityEngine/TextureFormat.hpp"
#include <dirent.h>
#include <stdio.h>
#include <fstream>
//#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "UnityEngine/Networking/UnityWebRequest.hpp"
#include "UnityEngine/Networking/UnityWebRequestAsyncOperation.hpp"
#include "Logging/GenericLogger.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"

#define INFO(value...) Qosmetics::GenericLogger::GetLogger().WithContext("File Utils").info(value)

std::string GetFileExtension(const std::string& FileName)
{
    if(FileName.find_last_of(".") != std::string::npos)
        return FileName.substr(FileName.find_last_of(".")+1);
    return "";
}

std::string FileUtils::GetFileName(const std::string& FilePath)
{
    if(FilePath.find_last_of("/") != std::string::npos)
        return FilePath.substr(FilePath.find_last_of("/")+1);
    return "";
}

 bool FileUtils::getFileNamesInDir(std::string extension, std::string dir, std::vector<std::string> &fileNames)
{
    bool foundTheExtension = false; 
    DIR* fileDir = opendir(dir.data());
    struct dirent *files;
    if(fileDir != NULL)
    {
        while((files = readdir(fileDir)) != NULL)
        {
            std::string foundExtension = GetFileExtension(files->d_name);
            if(foundExtension == extension)
            {
                fileNames.push_back(std::string(files->d_name));
                foundTheExtension = true; 
            }
        }
        closedir(fileDir);
        if(!foundTheExtension) return false; 
        return true;
    } else return false;

}

std::string FileUtils::rainbowIfy(std::string input)
{
    std::string result = "";

    for (auto c : input)
    {
        result += string_format("<color=%s>%c</color>", colors[rainbowIndex].c_str(), c);
        rainbowIndex++;
        rainbowIndex %= 12;
    }

    return result;
}