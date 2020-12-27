#include "Data/SaberData.hpp"
#include "Utils/SaberUtils.hpp"
#include "Utils/FileUtils.hpp"
#include "Data/QosmeticsDescriptorCache.hpp"
#include <thread>
namespace Qosmetics
{
    void SaberData::LoadBundle(std::string filePath)
    {
        this->filePath = filePath;
        this->LoadBundle();
    }

    void SaberData::LoadBundle(bool alsoLoadAssets)
    {
        if (this->bundleLoading || this->bundle) 
        {
            getLogger().info("Was already loading bundle, not loading again");
            if (this->bundle && alsoLoadAssets) LoadAssets();
            return;
        }

        this->bundleLoading = true;
        getLogger().info("Loading Bundle %s", saberDescriptor->get_filePath().c_str());
        bs_utils::AssetBundle::LoadFromFileAsync(saberDescriptor->get_filePath(), [&](bs_utils::AssetBundle* bundle){ 
            this->bundle = bundle;
            if (bundle != nullptr) getLogger().info("Bundle loaded");
            this->bundleLoading = false;
            });

        if (alsoLoadAssets) 
        {
            std::thread assetLoad([this]{
                while(!this->bundle) usleep(1000);
                getLogger().info("Loading assets directly from the bundle load");
                this->LoadAssets();
            });

            assetLoad.detach();
        }
    }

    void SaberData::LoadAssets()
    {
        if (isLoading) return;
        if (!this->bundle) 
        {
            getLogger().info("bundle %s was null, not loading assets", saberDescriptor->get_filePath().c_str());
            if (!get_isLoading() && saberDescriptor->get_filePath() != "") LoadBundle();
            return;
        }
        isLoading = true;
        getLogger().info("Loading assets");

        bundle->LoadAssetAsync("_CustomSaber", [&](bs_utils::Asset* asset){
            this->OnSaberLoadComplete((UnityEngine::GameObject*)asset, true);
        }, il2cpp_utils::GetSystemType("UnityEngine", "GameObject"));

        bundle->LoadAssetAsync("config", [&](bs_utils::Asset* asset){
            this->OnConfigLoadComplete((UnityEngine::TextAsset* )asset);
        }, il2cpp_utils::GetSystemType("UnityEngine", "TextAsset"));

        bundle->LoadAssetAsync("descriptor", [&](bs_utils::Asset* asset){
            this->OnDescriptorLoadComplete((UnityEngine::TextAsset* )asset);
        }, il2cpp_utils::GetSystemType("UnityEngine", "TextAsset"));

        bundle->LoadAssetAsync("thumbnail", [&](bs_utils::Asset* asset){
            this->OnTextureLoadComplete((UnityEngine::Texture2D*)asset);
        }, il2cpp_utils::GetSystemType("UnityEngine", "Texture2D"));
    }

    void SaberData::OnSaberLoadComplete(UnityEngine::GameObject* saber, bool DontDestroyOnLoad)
    {
        if (saber == nullptr) 
        {
            this->objectComplete = true;
            getLogger().error("Loading _CustomSaber object returned nullptr, marking object load as finished");
            return;
        }
        UnityEngine::GameObject* instantiated = UnityEngine::Object::Instantiate<UnityEngine::GameObject*>(saber);

        if (DontDestroyOnLoad) instantiated->DontDestroyOnLoad(instantiated);

        instantiated->SetActive(false);

        this->saberPrefab = instantiated;
        this->objectComplete = true;
        getLogger().info("Loaded Saber prefab");
    }

    void SaberData::OnConfigLoadComplete(UnityEngine::TextAsset* configAsset)
    {
        if (configAsset == nullptr) // as in, if config doesnt exist (legacy sabers)
        {
            this->saberConfig = new Qosmetics::SaberConfig();
            this->configComplete = true;
            getLogger().info("saber config did not exist, is this a legacy saber, or did someone fuck with the exporters? regardless, setting default config");
            return;
        }
        
        Il2CppString* jsonCS = configAsset->get_text();

        std::string json = to_utf8(csstrtostr(jsonCS));
        rapidjson::Document d;

        d.Parse(json.c_str());

        this->saberConfig = new Qosmetics::SaberConfig(d);
        for(auto& trail : *this->saberConfig->get_leftTrails())
        {
            trail.FindMaterial(saberPrefab->get_transform()->Find(il2cpp_utils::createcsstr("LeftSaber")));
        }
        
        for(auto& trail : *this->saberConfig->get_rightTrails())
        {
            trail.FindMaterial(saberPrefab->get_transform()->Find(il2cpp_utils::createcsstr("RightSaber")));
        }

        this->configComplete = true;

        getLogger().info("succesfully loaded config");
    }
            
    void SaberData::OnDescriptorLoadComplete(UnityEngine::TextAsset* descriptorAsset)
    {
        if (descriptorAsset == nullptr) 
        {
            this->descriptorComplete = true;
            getLogger().error("Loading saber descriptor returned nullptr, generating legacy config instead");
            std::string filename = FileUtils::GetFileName(this->filePath);

            filename.erase(filename.find_first_of("."));
            if (this->saberDescriptor) this->saberDescriptor->Copy(Descriptor(filename, "---", "legacy saber", this->filePath, saber));
            else this->saberDescriptor = new Qosmetics::Descriptor(filename, "---", "legacy saber", this->filePath, saber);
            DescriptorCache::AddToSaberCache(this->saberDescriptor);
            return;
        }
        Il2CppString* jsonCS = descriptorAsset->get_text();

        std::string json = to_utf8(csstrtostr(jsonCS));
        rapidjson::Document d;

        d.Parse(json.c_str());
        // if descriptor already exists, overwrite with latest data
        if (this->saberDescriptor) this->saberDescriptor->Copy(Descriptor(d, this->filePath, saber));
        else this->saberDescriptor = new Qosmetics::Descriptor(d, this->filePath, saber);
        DescriptorCache::AddToSaberCache(this->saberDescriptor);

        this->descriptorComplete = true;
        getLogger().info("succesfully loaded descriptor");
    }

    void SaberData::OnTextureLoadComplete(UnityEngine::Texture2D* texture)
    {
        if (texture == nullptr)
        {
            getLogger().error("Loading saber thumbnail returned nullptr");
            return;
        }
        while (this->saberDescriptor == nullptr){usleep(1);}
        this->saberDescriptor->SetCoverImage(texture);
        getLogger().info("loaded texture");
    }

    std::vector<UnityEngine::Material*>& SaberData::get_leftSaberCCmaterials()
    {
        if (leftSaberCCmats.size() > 0) return leftSaberCCmats;

        Array<UnityEngine::Renderer*>* renderers = get_leftSaber()->GetComponentsInChildren<UnityEngine::Renderer*>();

        for (int i = 0; i < renderers->Length(); i++)
        {
            Array<UnityEngine::Material*>* sharedMats = renderers->values[i]->get_sharedMaterials();
            for (int j = 0; j < sharedMats->Length(); i++)
            {
                UnityEngine::Material* newMat = sharedMats->values[j];
                bool found = false;
                for (auto mat : leftSaberCCmats)
                {
                    if (found) continue;
                    if (newMat == mat) found = true;
                }
                if (found) continue;

                if (SaberUtils::ShouldChangeSaberMaterialColor(newMat)) leftSaberCCmats.push_back(newMat);
            }
        }
        return leftSaberCCmats;
    }

    std::vector<UnityEngine::Material*>& SaberData::get_rightSaberCCmaterials()
    {
        if (rightSaberCCmats.size() > 0) return rightSaberCCmats;

        Array<UnityEngine::Renderer*>* renderers = get_rightSaber()->GetComponentsInChildren<UnityEngine::Renderer*>();

        for (int i = 0; i < renderers->Length(); i++)
        {
            Array<UnityEngine::Material*>* sharedMats = renderers->values[i]->get_sharedMaterials();
            for (int j = 0; j < sharedMats->Length(); i++)
            {
                UnityEngine::Material* newMat = sharedMats->values[j];
                bool found = false;
                for (auto mat : rightSaberCCmats)
                {
                    if (found) continue;
                    if (newMat == mat) found = true;
                }
                if (found) continue;

                if (SaberUtils::ShouldChangeSaberMaterialColor(newMat)) rightSaberCCmats.push_back(newMat);
            }
        }
        return rightSaberCCmats;
    }
}