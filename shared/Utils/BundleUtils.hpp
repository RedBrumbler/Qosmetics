#pragma once

#include "../Data/Manifest.hpp"
#include "../Utils/ZipUtils.hpp"
#include "UnityEngine/AssetBundle.hpp"
#include "UnityEngine/Object.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "custom-types/shared/coroutine.hpp"
#include <type_traits>

namespace Qosmetics::Core::BundleUtils
{
    /// @brief Loads a bundle from a zip file as a coroutine
    /// @param filePath path of the zip
    /// @param fileName the filename within the zip to load as a bundle
    /// @param out reference to the assetbundle pointer variable
    /// @return coroutine
    custom_types::Helpers::Coroutine LoadBundleFromZipAsync(std::string_view filePath, std::string_view fileName, UnityEngine::AssetBundle*& out);

    /// @brief loads an asset from a bundle as a coroutine
    /// @param bundle the bundle to load from
    /// @param name the name of the object to load
    /// @param out reference to the object output
    /// @return coroutine
    custom_types::Helpers::Coroutine LoadAssetFromBundleAsync(UnityEngine::AssetBundle* bundle, std::string_view name, System::Type* type, UnityEngine::Object*& out);

    /// @brief loads an asset from a bundle as a coroutine
    /// @tparam T the object type
    /// @param bundle the bundle to load from
    /// @param name the name of the object to load
    /// @param out reference to the object output
    /// @return coroutine
    template <class T>
    requires(!std::is_same_v<T, UnityEngine::Object*> && std::is_base_of_v<T, UnityEngine::Object*>)
        custom_types::Helpers::Coroutine LoadAssetFromBundleAsync(UnityEngine::AssetBundle* bundle, std::string_view name, T& out)
    {
        return LoadAssetFromBundleAsync(bundle, name, reinterpret_cast<System::Type*>(csTypeOf(T)), reinterpret_cast<UnityEngine::Object*&>(out));
    }
}