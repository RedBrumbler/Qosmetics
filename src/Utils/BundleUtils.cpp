#include "Utils/BundleUtils.hpp"
#include "UnityEngine/AssetBundleCreateRequest.hpp"
#include "UnityEngine/AssetBundleRequest.hpp"

namespace Qosmetics::Core::BundleUtils
{
    custom_types::Helpers::Coroutine LoadBundleFromZipAsync(std::string_view filePath, std::string_view fileName, UnityEngine::AssetBundle*& out)
    {
        std::vector<uint8_t> bytes;
        if (!ZipUtils::GetBytesFromZipFile(filePath, fileName, bytes))
        {
            out = nullptr;
            co_return;
        }

        using AssetBundle_LoadFromMemoryAsync = function_ptr_t<UnityEngine::AssetBundleCreateRequest*, ArrayW<uint8_t>, int>;
        static AssetBundle_LoadFromMemoryAsync assetBundle_LoadFromMemoryAsync = reinterpret_cast<AssetBundle_LoadFromMemoryAsync>(il2cpp_functions::resolve_icall("UnityEngine.AssetBundle::LoadFromMemoryAsync_Internal"));

        auto byteArr = il2cpp_utils::vectorToArray(bytes);

        auto req = assetBundle_LoadFromMemoryAsync(byteArr, 0);
        req->set_allowSceneActivation(true);
        co_yield reinterpret_cast<System::Collections::IEnumerator*>(req);

        out = req->get_assetBundle();
        co_return;
    }

    custom_types::Helpers::Coroutine LoadAssetFromBundleAsync(UnityEngine::AssetBundle* bundle, std::string_view name, System::Type* type, UnityEngine::Object*& out)
    {
        auto req = bundle->LoadAssetAsync(il2cpp_utils::newcsstr(name), type);
        req->set_allowSceneActivation(true);
        co_yield reinterpret_cast<System::Collections::IEnumerator*>(req);

        out = req->get_asset();

        co_return;
    }
}