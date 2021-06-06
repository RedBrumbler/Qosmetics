#include "Utils/FileDownloader.hpp"
#include "UnityEngine/Networking/DownloadHandler.hpp"
#include "UnityEngine/AsyncOperation.hpp"
#include "System/Action_1.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "QosmeticsLogger.hpp"

#define INFO(value...) QosmeticsLogger::GetContextLogger("File Downloader").info(value);
#define ERROR(value...) QosmeticsLogger::GetContextLogger("File Downloader").error(value);

using namespace UnityEngine::Networking;

void FileDownloader::Download()
{
    Il2CppString* urlPath = il2cpp_utils::createcsstr(url);
    this->request = UnityWebRequest::Get(urlPath);

    this->request->SetRequestHeader(il2cpp_utils::createcsstr("User-Agent"), il2cpp_utils::createcsstr(useragent));
    this->asyncOP = this->request->SendWebRequest();

    QuestUI::CustomDataType* data = CRASH_UNLESS(il2cpp_utils::New<QuestUI::CustomDataType*>());
    data->SetDataPointer(this);

    asyncOP->add_completed(il2cpp_utils::MakeDelegate<System::Action_1<UnityEngine::AsyncOperation*>*>(classof(System::Action_1<UnityEngine::AsyncOperation*>*), data, WebRequestComplete));
}

void FileDownloader::SetCallback(FileDownloaderCallback callback)
{
    this->hasCallback = true;
    this->callback = callback; 
}

void FileDownloader::WebRequestComplete(QuestUI::CustomDataType* data, UnityWebRequestAsyncOperation* asyncOP)
{
    INFO("data ptr: %p", data);
    FileDownloader* instance = (FileDownloader*)data->data;

    data->data = nullptr;
    //data->Finalize();
    
    //data->data = nullptr;
    //delete(data);

    INFO("Request to URL %s complete!", instance->url.c_str());

    if (!instance->request)
    {
        ERROR("Request on downloader to URL %s was nullptr", instance->url.c_str());
        return;
    }

    DownloadHandler* handler = asyncOP->webRequest->get_downloadHandler();
    if (!handler)
    {
        ERROR("Download handler for URL %s was nulltptr", instance->url.c_str());
        return;
    }

    Il2CppString* resultptr = handler->GetText();

    instance->result = to_utf8(csstrtostr(resultptr));
    
    if (instance->filePath != "" && instance->result != "")
    {
        if (fileexists(instance->filePath)) deletefile(instance->filePath);
        writefile(instance->filePath, instance->result);
    }

    instance->isDone = true;
    if (instance->hasCallback) instance->callback(*instance);
    delete(instance);
}
