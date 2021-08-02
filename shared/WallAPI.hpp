#pragma once

#include "conditional-dependencies/shared/main.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/Mesh.hpp"

#include <optional>
#include <functional>

#ifndef qosm_id
#define qosm_id "questcosmetics"
#endif

namespace Qosmetics
{
    class WallAPI
    {
        public:
            /// @brief returns a reference to the ORIGINAL prefab used to instantiate all walls Qosmetics makes, please do not mess it up
            /// @return pointer to Main copy of Wall, nullopt on nonexistent, nullptr on default
            static std::optional<UnityEngine::GameObject*> GetActiveWallModel() noexcept
            {
                static auto function = CondDep::Find<UnityEngine::GameObject*>(qosm_id, "GetActiveWallModel");
                if (!function) return std::nullopt;
                UnityEngine::GameObject* result = function.value()();
                return std::make_optional(result);
            }

            /// @brief gets the core material array on the custom wall
            /// @return valid array on custom, nullptr on default, nullopt on nonexistent
            static std::optional<Array<UnityEngine::Material*>*> get_coreMaterials() noexcept
            {
                static auto function = CondDep::Find<Array<UnityEngine::Material*>*>(qosm_id, "get_coreMaterials");
                if (!function) return std::nullopt;
                Array<UnityEngine::Material*>* result = function.value()();
                return std::make_optional(result);
            }

            /// @brief gets the frame material array on the custom wall
            /// @return valid array on custom, nullptr on default, nullopt on nonexistent
            static std::optional<Array<UnityEngine::Material*>*> get_frameMaterials() noexcept
            {
                static auto function = CondDep::Find<Array<UnityEngine::Material*>*>(qosm_id, "get_frameMaterials");
                if (!function) return std::nullopt;
                Array<UnityEngine::Material*>* result = function.value()();
                return std::make_optional(result);
            }

            /// @brief gets the core mesh on the custom wall
            /// @return valid array on custom, nullptr on default, nullopt on nonexistent
            static std::optional<UnityEngine::Mesh*> get_coreMesh() noexcept
            {
                static auto function = CondDep::Find<UnityEngine::Mesh*>(qosm_id, "get_coreMesh");
                if (!function) return std::nullopt;
                UnityEngine::Mesh* result = function.value()();
                return std::make_optional(result);
            }

            /// @brief gets the frame mesh on the custom wall
            /// @return valid array on custom, nullptr on default, nullopt on nonexistent
            static std::optional<UnityEngine::Mesh*> get_frameMesh() noexcept
            {
                static auto function = CondDep::Find<UnityEngine::Mesh*>(qosm_id, "get_frameMesh");
                if (!function) return std::nullopt;
                UnityEngine::Mesh* result = function.value()();
                return std::make_optional(result);
            }

            /// @brief sets the default wall as the active wall
            static void SetDefault() noexcept
            {
                static auto function = CondDep::Find<void>(qosm_id, "SetDefaultWall");
                if (!function) return;
                function.value()();
            }

            /// @brief Sets active wall based on identifier (usually just the wall name)
            /// @param wall filename (without extension) of the wall
            static void SetActiveWall(std::string wall) noexcept
            {
                if (wall == "") return;
                static auto function = CondDep::Find<void, const char*>(qosm_id, "SetActiveWall");
                if (!function) return;
                function.value()(wall.c_str());
            }

            /// @brief Sets active wall based on full file path
            /// @param wallPath full path to the wall
            static void SetActiveWallFromFilePath(std::string wallPath) noexcept
            {
                if (wallPath == "") return;
                static auto function = CondDep::Find<void, const char*>(qosm_id, "SetActiveWallFromFilePath");
                if (!function) return;
                function.value()(wallPath.c_str());
            }

            /// @brief returns whether there's a custom wall, even with false the trails still get moved so keep that in mind!
            /// @return true for custom, false for default/nonexistent
            static std::optional<bool> GetWallIsCustom() noexcept
            {
                static auto function = CondDep::Find<bool>(qosm_id, "GetWallIsCustom");
                if (!function) return std::nullopt;
                return std::make_optional(function.value()());
            }

            /// @brief returns the filepath to the Qosmetics notes folder, in case it ever gets moved
            /// @return string folder path, or nullopt for not installed
            static std::optional<std::string> GetWallFolder()
            {
                static auto function = CondDep::Find<void, std::string&>(qosm_id, "GetWallFolder");
                if (!function) return std::nullopt;
                std::string result = "";
                function.value()(result);
                return std::make_optional(result);
            }
    };
}