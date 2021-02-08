#include "Data/ModelLoader.hpp"
#include "Data/DescriptorCache.hpp"

#include "QosmeticsLogger.hpp"
#include "Types/Qosmetic/QosmeticItem.hpp"
#include <thread>

#define INFO(value...) QosmeticsLogger::GetContextLogger("Model Provider").info(value);
#define ERROR(value...) QosmeticsLogger::GetContextLogger("Model Provider").error(value);

using namespace UnityEngine;

namespace Qosmetics
{
    void ModelLoader::LoadBundle(bool alsoLoadAssets)
    {
        ItemType fromItem = this->item->get_type();
        ItemType fromdescriptor = this->item->get_descriptor().get_type();
        
        bs_utils::AssetBundle::LoadFromFileAsync(item->get_descriptor().get_filePath(), [&, alsoLoadAssets](bs_utils::AssetBundle* bundle){
            this->bundle = bundle;
            if (this->bundle) INFO("Bundle Loaded!");
        });
        if (alsoLoadAssets)
        {
            std::thread assetLoad([&]{
                while(!this->bundle) usleep(1000);
                INFO("Loading assets directly from bundle load")
                this->LoadAssets();
            });

            assetLoad.detach();
        }
    }

    void ModelLoader::LoadAssets()
    {
        INFO("Began loading assets for bundle %s", item->get_descriptor().get_filePath().c_str());
        if (!this->bundle) 
        {
            ERROR("Bundle was nullptr");
            return;
        }
        
        std::string assetName;

        switch (item->get_type())
        {
            case ItemType::saber:
                assetName = "_CustomSaber";
                break;
            case ItemType::note:
                assetName = "_CustomBloq";
                break;
            case ItemType::wall:
                assetName = "_CustomWall";
                break;
            case ItemType::pointer:
                assetName = "_CustomPointer";
                break;
            case ItemType::platform:
                assetName = "_CustomPlatform";
                break;
            default:
                ERROR("Item was invalid type");
                return;
        }

        if (!GameObjectType) GameObjectType = il2cpp_utils::GetSystemType("UnityEngine", "GameObject");
        if (!TextAssetType) TextAssetType = il2cpp_utils::GetSystemType("UnityEngine", "TextAsset");
        
        INFO("Asset name to load is %s", assetName.c_str());
        bundle->LoadAssetAsync(assetName, [&](bs_utils::Asset* asset){
            item->GameObjectCallback((GameObject*)asset);
            objectLoaded = true;
            this->OnComplete();
        }, GameObjectType);
        bundle->LoadAssetAsync("config", [&](bs_utils::Asset* asset){
            item->ConfigCallback((TextAsset*)asset);
            configLoaded = true;
            this->OnComplete();
        }, TextAssetType);
        bundle->LoadAssetAsync("descriptor", [&](bs_utils::Asset* asset){
            item->DescriptorCallback((TextAsset*)asset);
            descriptorLoaded = true;
            this->OnComplete();
        }, TextAssetType);
    }

    void ModelLoader::OnComplete()
    {
        if (!get_complete()) return;
        INFO("On Complete");
        DescriptorCache::Save();
        //UnloadBundle();
    }

    void ModelLoader::UnloadBundle()
    {
        INFO("Unloading bundle, while assets will stay loaded");
        ((UnityEngine::AssetBundle*)this->bundle)->Unload(false);
    }
}