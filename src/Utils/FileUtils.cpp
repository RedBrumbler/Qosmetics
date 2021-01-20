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
#include "httplib.h"
#include "UnityEngine/UnityWeb"
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

void FileUtils::DownloadFileToPath(const std::string& url, std::string& filePath, bool overWrite)
{  
    std::string mainURL = url.substr(0, url.find_last_of('/'));
    std::string fileName = url.substr(url.find_last_of('/'));
    
    INFO("Main: %s, fileName: %s", mainURL.c_str(), fileName.c_str());

    httplib::Client client(mainURL.c_str());

    //if (fileexists(filePath) && overWrite) deletefile(filePath);
    std::ofstream outstream(filePath, std::ofstream::out | std::ofstream::binary | (overWrite ? std::ofstream::trunc : 0));
    auto res = client.Get(fileName.c_str(), [&](const char* data, size_t data_length){
        if (!data) 
        {
            return false;
        }
        INFO("Got: %s", data);
        outstream.write(data, data_length);
        return true;
    });

    outstream.close();
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