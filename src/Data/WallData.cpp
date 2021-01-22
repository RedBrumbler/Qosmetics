#include "Data/WallData.hpp"
#include "Data/QosmeticsDescriptorCache.hpp"
#include "Utils/MaterialUtils.hpp"

namespace Qosmetics
{
    void WallData::LoadBundle(bool alsoLoadAssets)
    {
        if (this->bundleLoading || this->bundle) 
        {
            getLogger().info("Was already loading bundle, not loading again");
            if (this->bundle && alsoLoadAssets) LoadAssets();
            return;
        }
        this->bundleLoading = true;
        getLogger().info("Loading Bundle");
        bs_utils::AssetBundle::LoadFromFileAsync(wallDescriptor->get_filePath(), [&](bs_utils::AssetBundle* bundle){ 
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

    void WallData::LoadAssets()
    {
        if (isLoading) return;
        if (!this->bundle) 
        {
            getLogger().info("bundle %s was null, not loading assets", wallDescriptor->get_filePath().c_str());
            if (!get_isLoading() && wallDescriptor->get_filePath() != "") LoadBundle();
            return;
        }
        isLoading = true;
        getLogger().info("Loading assets");

        bundle->LoadAssetAsync("_CustomWall", [this](bs_utils::Asset* asset){
            this->OnWallLoadComplete((UnityEngine::GameObject*)asset, true);
            this->OnComplete();
        }, il2cpp_utils::GetSystemType("UnityEngine", "GameObject"));

        bundle->LoadAssetAsync("config", [this](bs_utils::Asset* asset){
            this->OnConfigLoadComplete((UnityEngine::TextAsset* )asset);
            this->OnComplete();
        }, il2cpp_utils::GetSystemType("UnityEngine", "TextAsset"));

        bundle->LoadAssetAsync("descriptor", [this](bs_utils::Asset* asset){
            this->OnDescriptorLoadComplete((UnityEngine::TextAsset* )asset);
            this->OnComplete();
        }, il2cpp_utils::GetSystemType("UnityEngine", "TextAsset"));

        bundle->LoadAssetAsync("thumbnail", [this](bs_utils::Asset* asset){
            this->OnTextureLoadComplete((UnityEngine::Texture2D*)asset);
        }, il2cpp_utils::GetSystemType("UnityEngine", "Texture2D"));
    }

    void WallData::OnWallLoadComplete(UnityEngine::GameObject* wall, bool DontDestroyOnLoad)
    {
        if (wall == nullptr) 
        {
            getLogger().error("Loading _CustomWall object returned nullptr, marking object load as finished");
            objectComplete = true;
            return;
        }
        UnityEngine::GameObject* instantiated = UnityEngine::Object::Instantiate<UnityEngine::GameObject*>(wall);

        if (DontDestroyOnLoad) instantiated->DontDestroyOnLoad(instantiated);

        instantiated->SetActive(false);

        this->wallPrefab = instantiated;
        objectComplete = true;

        MaterialUtils::PreWarmAllShadersOnObj(this->wallPrefab);
        getLogger().info("Loaded Wall prefab");
    }

    void WallData::OnConfigLoadComplete(UnityEngine::TextAsset* configAsset)
    {
        if (configAsset == nullptr) 
        {
            getLogger().error("Loading wall config returned nullptr, marking  config load as finished");
            configComplete = true;
            return;
        }
        Il2CppString* jsonCS = configAsset->get_text();

        std::string json = to_utf8(csstrtostr(jsonCS));
        rapidjson::Document d;

        d.Parse(json.c_str());

        this->wallConfig = new Qosmetics::WallConfig(d);
        configComplete = true;
        getLogger().info("succesfully loaded config");
    }
            
    void WallData::OnDescriptorLoadComplete(UnityEngine::TextAsset* descriptorAsset)
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

        if (this->wallDescriptor) this->wallDescriptor->Copy(Descriptor(d, this->filePath, wall));
        else this->wallDescriptor = new Qosmetics::Descriptor(d, this->filePath, wall);
        DescriptorCache::AddToWallCache(this->wallDescriptor);

        descriptorComplete = true;
        getLogger().info("succesfully loaded descriptor");
    }

    void WallData::OnTextureLoadComplete(UnityEngine::Texture2D* texture)
    {
        if (texture == nullptr)
        {
            getLogger().error("Loading wall thumbnail returned nullptr");
            return;
        }
        while (this->wallDescriptor == nullptr){sleep(1);}
        this->wallDescriptor->SetCoverImage(texture);
        getLogger().info("loaded texture");
    }

    void WallData::UnloadBundle()
    {
        getLogger().info("Unloading bundle for wall %s", this->wallDescriptor->get_name());
        il2cpp_utils::RunMethod(this->bundle, "Unload", false);
    }

    UnityEngine::Mesh* WallData::get_coreMesh()
    {   
        if (get_core() == nullptr) 
        {
            getLogger().error("Tried getting core mesh but core was null, returning nullptr");
            return nullptr;
        }
        if (activeCoreMesh != nullptr) return activeCoreMesh;

        UnityEngine::MeshFilter* meshFilter = UnityUtils::GetComponent<UnityEngine::MeshFilter*>(core, "MeshFilter");
        if (meshFilter == nullptr)
        {
            getLogger().error("Finding core meshfilter returned nullptr, returning nullptr");
            return nullptr;
        } 
        
        activeCoreMesh = meshFilter->get_sharedMesh();
        return activeCoreMesh;
    }

    UnityEngine::Mesh* WallData::get_frameMesh()
    {
        if (get_frame() == nullptr) 
        {
            getLogger().error("Tried getting frame mesh but frame was null, returning nullptr");
            return nullptr;
        }
        if (activeFrameMesh != nullptr) return activeFrameMesh;

        UnityEngine::MeshFilter* meshFilter = UnityUtils::GetComponent<UnityEngine::MeshFilter*>(frame, "MeshFilter");
        if (meshFilter == nullptr)
        {
            getLogger().error("Finding frame meshfilter returned nullptr, returning nullptr");
            return nullptr;
        } 

        activeFrameMesh = meshFilter->get_sharedMesh();
        return activeFrameMesh;
    }

    UnityEngine::Material* WallData::get_coreMaterial()
    {
        if (get_core() == nullptr) 
        {
            getLogger().error("Tried getting core material but core was null, returning nullptr");
            return nullptr;
        }
        if (activeCoreMat != nullptr) return activeCoreMat;

        UnityEngine::MeshRenderer* meshRenderer = UnityUtils::GetComponent<UnityEngine::MeshRenderer*>(core, "MeshRenderer");
       
        if (meshRenderer == nullptr)
        {
            getLogger().error("Finding core meshrenderer returned nullptr, returning nullptr");
            return nullptr;
        } 

        activeCoreMat = meshRenderer->get_material();
        return activeCoreMat;
    }

    UnityEngine::Material* WallData::get_frameMaterial()
    {
        if (get_frame() == nullptr) 
        {
            getLogger().error("Tried getting frame material but frame was null, returning nullptr");
            return nullptr;
        }
        if (activeFrameMat != nullptr) return activeFrameMat;

        UnityEngine::MeshRenderer* meshRenderer = UnityUtils::GetComponent<UnityEngine::MeshRenderer*>(frame, "MeshRenderer");
        
        if (meshRenderer == nullptr)
        {
            getLogger().error("Finding frame meshrenderer returned nullptr, returning nullptr");
            return nullptr;
        } 
        
        activeFrameMat = meshRenderer->get_material();
        return activeFrameMat;
    }

    Array<UnityEngine::Material*>* WallData::get_coreSharedMaterials()
    {
        if (get_core() == nullptr) 
        {
            getLogger().error("Tried getting core material array but core was null, returning nullptr");
            return nullptr;
        }
        if (coreSharedMats != nullptr) return coreSharedMats;

        UnityEngine::MeshRenderer* meshRenderer = UnityUtils::GetComponent<UnityEngine::MeshRenderer*>(core, "MeshRenderer");
        if (meshRenderer == nullptr)
        {
            getLogger().error("Finding core meshrenderer returned nullptr, returning nullptr");
            return nullptr;
        } 

        coreSharedMats = meshRenderer->get_sharedMaterials();

        return coreSharedMats;
    }

    Array<UnityEngine::Material*>* WallData::get_frameSharedMaterials()
    {
        if (get_frame() == nullptr) 
        {
            getLogger().error("Tried getting frame material array but frame was null, returning nullptr");
            return nullptr;
        }
        if (frameSharedMats != nullptr) return frameSharedMats;

        UnityEngine::MeshRenderer* meshRenderer = UnityUtils::GetComponent<UnityEngine::MeshRenderer*>(frame, "MeshRenderer");
        if (meshRenderer == nullptr)
        {
            getLogger().error("Finding frame meshrenderer returned nullptr, returning nullptr");
            return nullptr;
        } 
        
        frameSharedMats = meshRenderer->get_sharedMaterials();

        return frameSharedMats;
    }

    UnityEngine::MeshRenderer* WallData::get_coreRenderer()
    {
        if (get_core() == nullptr) 
        {
            getLogger().error("Tried getting core renderer but core was null, returning nullptr");
            return nullptr;
        }
        if (coreRenderer != nullptr) return coreRenderer;

        coreRenderer = UnityUtils::GetComponent<UnityEngine::MeshRenderer*>(core, "MeshRenderer");

        return coreRenderer;
    }

    UnityEngine::MeshRenderer* WallData::get_frameRenderer()
    {
        if (get_frame() == nullptr) 
        {
            getLogger().error("Tried getting frame renderer but frame was null, returning nullptr");
            return nullptr;
        }
        if (frameRenderer != nullptr) return frameRenderer;

        frameRenderer = UnityUtils::GetComponent<UnityEngine::MeshRenderer*>(frame, "MeshRenderer");

        return frameRenderer;
    }

    void WallData::OnComplete()
    {
        if (!get_complete()) return;
        std::string name = string_format("%s%s", this->wallDescriptor->get_name().c_str(), this->wallDescriptor->get_author().c_str());
        this->wallPrefab->set_name(il2cpp_utils::createcsstr(name));
    }

    void WallData::FindPrefab()
    {
        if (!get_complete()) return;
        std::string name = string_format("%s%s", this->wallDescriptor->get_name().c_str(), this->wallDescriptor->get_author().c_str());
        UnityEngine::GameObject* prefab = UnityEngine::GameObject::Find(il2cpp_utils::createcsstr(name));

        if (prefab) this->wallPrefab = prefab; 
    }
}