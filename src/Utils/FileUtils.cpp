#include "../include/Utils/FileUtils.hpp"
#include <dirent.h>
#include <stdio.h>

std::string GetFileExtension(const std::string& FileName)
{
    if(FileName.find_last_of(".") != std::string::npos)
        return FileName.substr(FileName.find_last_of(".")+1);
    return "";
}

 bool FileUtils::getFileNamesInDir(std::string extension, std::string dir, std::vector<std::string> &fileNames)
{
    bool foundExtension = false; 
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
                foundExtension = true; 
            }
                
        }
        closedir(fileDir);
        if(!foundExtension) return false; 
        return true;
    } else return false;

}