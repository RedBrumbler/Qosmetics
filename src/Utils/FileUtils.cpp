#include "Utils/FileUtils.hpp"

#include <dirent.h>
#include <stdio.h>
#include <fstream>
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"

std::string FileUtils::GetFileName(std::string path, bool removeExtension)
{
    std::string result = "";
    if(path.find_last_of("/") != std::string::npos)
        result = path.substr(path.find_last_of("/")+1);
    else result = path;
    if (removeExtension) result = RemoveExtension(result);
    return result;;
}

std::string FileUtils::RemoveExtension(std::string path)
{
    if(path.find_last_of(".") != std::string::npos)
        return path.substr(0, path.find_last_of("."));
    return path;
}

std::string FileUtils::GetExtension(std::string path)
{
    if(path.find_last_of(".") != std::string::npos)
        return path.substr(path.find_last_of(".")+1);
    return "";
}

bool FileUtils::GetFilesInFolderPath(std::string extension, std::string filePath, std::vector<std::string> out)
{
    bool foundTheExtension = false; 
    struct DIR* fileDir = opendir(filePath.data());
    struct dirent *files;
    if(fileDir != NULL)
    {
        while((files = readdir(fileDir)) != NULL)
        {
            //std::string fileName = ;
            std::string foundExtension = GetExtension(files->d_name);
            if(foundExtension == extension)
            {
                out.push_back(std::string(files->d_name));
                foundTheExtension = true; 
            }
        }
        closedir(fileDir);
    }
    return foundTheExtension;
}        

