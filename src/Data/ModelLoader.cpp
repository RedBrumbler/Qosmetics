#include "Data/ModelLoader.hpp"
#include "Data/DescriptorCache.hpp"

#include "Types/Saber/SaberItem.hpp"

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
        if (bundle) return;
        
        //this->bundle = UnityEngine::AssetBundle::LoadFromFile(il2cpp_utils::createcsstr(item->get_descriptor().get_filePath()));
        std::vector<char> bytes = readbytes(item->get_descriptor().get_filePath());
        std::vector<uint8_t>& data = *((std::vector<uint8_t>*)&bytes);
        using LoadFromMemory = function_ptr_t<UnityEngine::AssetBundle*, Array<uint8_t>*, unsigned int>;
        static LoadFromMemory loadFromMemory = reinterpret_cast<LoadFromMemory>(il2cpp_functions::resolve_icall("UnityEngine.AssetBundle::LoadFromMemory_Internal"));
        Array<uint8_t>* dataArray = il2cpp_utils::vectorToArray(data);

        this->bundle = loadFromMemory(dataArray, 0);
        /*
        AssetBundle::LoadFromFileAsync(item->get_descriptor().get_filePath(), [&, alsoLoadAssets](bs_utils::AssetBundle* bundle){
            this->bundle = bundle;
            if (this->bundle) INFO("Bundle Loaded!");
        });
        */
        if (alsoLoadAssets)
        {
            this->LoadAssets();
            /*std::thread assetLoad([&]{
                while(!this->bundle) usleep(1000);
                INFO("Loading assets directly from bundle load")
                this->LoadAssets();
            });

            assetLoad.detach();
            */
        }
    }

    void ModelLoader::LoadAssets()
    {
        INFO("Began loading assets for bundle %s", item->get_descriptor().get_filePath().c_str());
        if (!bundle)
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

        INFO("Asset name to load is %s", assetName.c_str());

        
        item->GameObjectCallback(bundle->LoadAsset<GameObject*>(il2cpp_utils::createcsstr(assetName)));
        objectLoaded = true;
        this->OnComplete();

        item->ConfigCallback(bundle->LoadAsset<TextAsset*>(il2cpp_utils::createcsstr("config")));
        configLoaded = true;
        this->OnComplete();

        item->DescriptorCallback(bundle->LoadAsset<TextAsset*>(il2cpp_utils::createcsstr("descriptor")));
        descriptorLoaded = true;
        this->OnComplete();

        /*
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
        */
    }

    void ModelLoader::OnComplete()
    {
        if (!get_complete()) return;
        INFO("On Complete");
        DescriptorCache::Save();
        UnloadBundle();
    }

    void ModelLoader::UnloadBundle()
    {
        INFO("Unloading bundle, while assets will stay loaded");
        this->bundle->Unload(false);
    }
}
