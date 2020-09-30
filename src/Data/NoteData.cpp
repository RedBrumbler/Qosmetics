#include "Data/NoteData.hpp"
namespace Qosmetics
{
    void NoteData::LoadBundle()
    {
        bundleLoading = true;
        getLogger().info("Loading Bundle");
        bs_utils::AssetBundle::LoadFromFileAsync(filePath, [this](bs_utils::AssetBundle* bundle){ 
            this->bundle = bundle;
            if (bundle != nullptr) getLogger().info("Bundle loaded");
            this->bundleLoading = false;
            });
    }

    void NoteData::LoadAssets()
    {
        if (bundle == nullptr) 
        {
            getLogger().info("bundle %s was null, not loading assets", filePath.c_str());
            if (!bundleLoading && filePath != "") LoadBundle();
            return;
        }
        isLoading = true;
        getLogger().info("Loading assets");

        bundle->LoadAssetAsync("_CustomBloq", [this](bs_utils::Asset* asset){
            this->OnNoteLoadComplete((UnityEngine::GameObject*)asset, true);
        }, il2cpp_utils::GetSystemType("UnityEngine", "GameObject"));

        bundle->LoadAssetAsync("config", [this](bs_utils::Asset* asset){
            this->OnConfigLoadComplete((UnityEngine::TextAsset* )asset);
        }, il2cpp_utils::GetSystemType("UnityEngine", "TextAsset"));

        bundle->LoadAssetAsync("descriptor", [this](bs_utils::Asset* asset){
            this->OnDescriptorLoadComplete((UnityEngine::TextAsset* )asset);
        }, il2cpp_utils::GetSystemType("UnityEngine", "TextAsset"));

        bundle->LoadAssetAsync("thumbnail", [this](bs_utils::Asset* asset){
            this->OnTextureLoadComplete((UnityEngine::Texture2D*)asset);
        }, il2cpp_utils::GetSystemType("UnityEngine", "Texture2D"));
    }

    void NoteData::OnNoteLoadComplete(UnityEngine::GameObject* note, bool DontDestroyOnLoad)
    {
        if (note == nullptr) 
        {   
            getLogger().error("Loading _CustomBloq object returned nullptr, marking object load as finished");
            objectComplete = true;
            return;
        }

        UnityEngine::GameObject* instantiated = (UnityEngine::GameObject*)UnityEngine::Object::Instantiate((UnityEngine::Object*)note);

        if (DontDestroyOnLoad) instantiated->DontDestroyOnLoad(instantiated);

        instantiated->SetActive(false);

        this->notePrefab = instantiated;
        objectComplete = true;
        getLogger().info("Loaded Bloq prefab");
    }

    void NoteData::OnConfigLoadComplete(UnityEngine::TextAsset* configAsset)
    {
        if (configAsset == nullptr) 
        {
            getLogger().error("Loading bloq config returned nullptr, marking  config load as finished");
            configComplete = true;
            return;
        }
        
        Il2CppString* jsonCS = configAsset->get_text();

        std::string json = to_utf8(csstrtostr(jsonCS));
        rapidjson::Document d;

        d.Parse(json.c_str());

        this->noteConfig = new Qosmetics::NoteConfig(d);
        configComplete = true;
        getLogger().info("succesfully loaded config");
    }
            
    void NoteData::OnDescriptorLoadComplete(UnityEngine::TextAsset* descriptorAsset)
    {
        if (descriptorAsset == nullptr) 
        {
            getLogger().error("Loading wall descriptor returned nullptr, marking descriptor load as finished");
            descriptorComplete = true;
            return;
        }
        Il2CppString* jsonCS = descriptorAsset->get_text();

        std::string json = to_utf8(csstrtostr(jsonCS));
        rapidjson::Document d;

        d.Parse(json.c_str());

        this->noteDescriptor = new Qosmetics::Descriptor(d);
        descriptorComplete = true;
        getLogger().info("succesfully loaded descriptor");
    }

    void NoteData::OnTextureLoadComplete(UnityEngine::Texture2D* texture)
    {
        if (texture == nullptr)
        {
            getLogger().error("Loading note thumbnail returned nullptr");
            return;
        }
        while (this->noteDescriptor == nullptr){sleep(1);}
        this->noteDescriptor->SetCoverImage(texture);
        getLogger().info("loaded texture");
    }
}