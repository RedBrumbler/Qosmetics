#include "Data/SaberData.hpp"
#include "Utils/SaberUtils.hpp"
#include "Utils/FileUtils.hpp"
#include "Data/QosmeticsDescriptorCache.hpp"

namespace Qosmetics
{
    void SaberData::LoadBundle(std::string filePath)
    {
        this->filePath = filePath;
        this->LoadBundle();
    }

    void SaberData::LoadBundle()
    {
        bundleLoading = true;
        getLogger().info("Loading Bundle %s", filePath.c_str());
        bs_utils::AssetBundle::LoadFromFileAsync(saberDescriptor->get_filePath(), [this](bs_utils::AssetBundle* bundle){ 
            this->bundle = bundle;
            if (bundle != nullptr) getLogger().info("Bundle loaded");
            this->bundleLoading = false;
            });
    }

    void SaberData::LoadAssets()
    {
        if (bundle == nullptr) 
        {
            getLogger().info("bundle %s was null, not loading assets", filePath.c_str());
            if (!bundleLoading && filePath != "") LoadBundle();
            return;
        }
        isLoading = true;
        getLogger().info("Loading assets");

        bundle->LoadAssetAsync("_CustomSaber", [this](bs_utils::Asset* asset){
            this->OnSaberLoadComplete((UnityEngine::GameObject*)asset, true);
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

    void SaberData::OnSaberLoadComplete(UnityEngine::GameObject* saber, bool DontDestroyOnLoad)
    {
        if (saber == nullptr) 
        {
            this->objectComplete = true;
            getLogger().error("Loading _CustomSaber object returned nullptr, marking object load as finished");
            return;
        }
        UnityEngine::GameObject* instantiated = (UnityEngine::GameObject*)UnityEngine::Object::Instantiate((UnityEngine::Object*)saber);

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
            if (get_complete()) il2cpp_utils::RunMethod(this->bundle, "Unload", false);
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
        if (get_complete()) il2cpp_utils::RunMethod(this->bundle, "Unload", false);
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
            DescriptorCache::GetCache().AddToSaberCache(this->saberDescriptor);
            if (get_complete()) il2cpp_utils::RunMethod(this->bundle, "Unload", false);
            return;
        }
        Il2CppString* jsonCS = descriptorAsset->get_text();

        std::string json = to_utf8(csstrtostr(jsonCS));
        //getLogger().info("descriptor %s", json.c_str());
        rapidjson::Document d;

        d.Parse(json.c_str());
        // if descriptor already exists, overwrite with latest data
        if (this->saberDescriptor) this->saberDescriptor->Copy(Descriptor(d, this->filePath, saber));
        else this->saberDescriptor = new Qosmetics::Descriptor(d, this->filePath, saber);
        DescriptorCache::GetCache().AddToSaberCache(this->saberDescriptor);

        this->descriptorComplete = true;
        getLogger().info("succesfully loaded descriptor");
        if (get_complete()) il2cpp_utils::RunMethod(this->bundle, "Unload", false);
    }

    void SaberData::OnTextureLoadComplete(UnityEngine::Texture2D* texture)
    {
        if (texture == nullptr)
        {
            getLogger().error("Loading saber thumbnail returned nullptr");
            if (get_complete()) il2cpp_utils::RunMethod(this->bundle, "Unload", false);
            return;
        }
        while (this->saberDescriptor == nullptr){sleep(1);}
        this->saberDescriptor->SetCoverImage(texture);
        getLogger().info("loaded texture");
        if (get_complete()) il2cpp_utils::RunMethod(this->bundle, "Unload", false);
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