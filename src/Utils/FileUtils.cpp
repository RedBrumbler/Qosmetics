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

#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"

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

UnityEngine::Sprite* FileUtils::SpriteFromFile(const std::string& filePath, int width, int height)
{
    std::ifstream instream(filePath, std::ios::in | std::ios::binary);
    std::vector<uint8_t> data((std::istreambuf_iterator<char>(instream)), std::istreambuf_iterator<char>());
    Array<uint8_t>* bytes = il2cpp_utils::vectorToArray(data);
    UnityEngine::Texture2D* texture = UnityEngine::Texture2D::New_ctor(width, height, UnityEngine::TextureFormat::RGBA32, false, false);
    if (UnityEngine::ImageConversion::LoadImage(texture, bytes, false))
        return UnityEngine::Sprite::Create(texture, UnityEngine::Rect(0.0f, 0.0f, (float)width, (float)height), UnityEngine::Vector2(0.5f,0.5f), 1024.0f, 1u, UnityEngine::SpriteMeshType::FullRect, UnityEngine::Vector4(0.0f, 0.0f, 0.0f, 0.0f), false);
    return nullptr;
}