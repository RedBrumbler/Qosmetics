#pragma once

#include <string>
#include <map>
#include <functional>
#include "UnityEngine/Networking/UnityWebRequest.hpp"
#include "UnityEngine/Networking/UnityWebRequestAsyncOperation.hpp"
#include "questui/shared/CustomTypes/Data/CustomDataType.hpp"

#define DEFAULT_UA "Qosmetics/"

class FileDownloader;

typedef std::function<void(const FileDownloader& downloader)> FileDownloaderCallback;

class FileDownloader
{
    public: 
        FileDownloader(std::string url, std::string filePath, std::string useragent, bool alsoDownload = true)
        {
            this->filePath = filePath;
            this->url = url;
            this->useragent = useragent;

            if (alsoDownload) Download();
        }

        FileDownloader(std::string url, std::string filePath, bool alsoDownload = true) : FileDownloader(url, filePath, DEFAULT_UA, alsoDownload) {}

        FileDownloader(std::string url, std::string filePath, FileDownloaderCallback callback, bool alsoDownload = true) : FileDownloader(url, filePath, DEFAULT_UA, callback, alsoDownload) {}

        FileDownloader(std::string url, std::string filePath, std::string useragent, FileDownloaderCallback callback, bool alsoDownload = true) : FileDownloader(url, filePath, useragent, alsoDownload)
        {
            SetCallback(callback);
        }

        void Download();

        bool get_isDone() const
        {
            return isDone;
        }

        const std::string& get_result() const
        {
            return result;
        }

    private:
        static void WebRequestComplete(QuestUI::CustomDataType* data, UnityEngine::Networking::UnityWebRequestAsyncOperation* asyncOP);

        void SetCallback(FileDownloaderCallback callback);

        bool hasCallback = false;
        UnityEngine::Networking::UnityWebRequest* request = nullptr;
        UnityEngine::Networking::UnityWebRequestAsyncOperation* asyncOP = nullptr;
        std::string filePath = "";
        std::string url = "";
        std::string useragent = "";
        std::string result = "";
        FileDownloaderCallback callback;
        bool isDone = false;
};