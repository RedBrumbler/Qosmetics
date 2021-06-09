#pragma once

#include "conditional-dependencies/shared/main.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include <optional>
#include <functional>

#ifndef qosm_id
#define qosm_id "questcosmetics"
#endif

namespace Qosmetics
{
    class NoteAPI
    {
        public:
            /// @brief returns a reference to the ORIGINAL prefab used to instantiate all notes Qosmetics makes, please do not mess it up
            /// @return pointer to Main copy of Note, nullopt on nonexistent, nullptr on default
            static std::optional<UnityEngine::GameObject*> GetActiveNoteModel() noexcept
            {
                auto function = CondDep::Find<UnityEngine::GameObject*>(qosm_id, "GetActiveNoteModel");

                if (!function) return std::nullopt;

                UnityEngine::GameObject* result = function.value()();
                return std::make_optional(result);
            }

            /// @brief based on input returns left/right arrow/dot
            /// @param color 0/1 for left/right
            /// @param type 0/1 for arrow/dot
            /// @return valid ptr for found, nullptr for not found, nullopt for no Qosmetics or invalid type
            static std::optional<UnityEngine::Transform*> get_note(int color, int type) noexcept
            {
                return get_note(color * 2 + type);
            }

            /// @brief based on input returns left/right arrow/dot
            /// @param type 0/1 left arrow/dot 2/3 right arrow/dot
            /// @return valid ptr for found, nullptr for not found, nullopt for no Qosmetics or invalid type
            static std::optional<UnityEngine::Transform*> get_note(int type) noexcept
            {
                if (type < 0 || type > 3) return std::nullopt;
                auto function = CondDep::Find<UnityEngine::Transform*, int>(qosm_id, "get_note");

                if (!function) return std::nullopt;
                
                UnityEngine::Transform* result = function.value()(type);
                return std::make_optional(result);
            }

            /// @brief based on type returns left/right debris
            /// @param type 0/1 for left/right
            /// @return valid ptr for found, nullptr for not found, nullopt for no qosmetics or invalid type
            static std::optional<UnityEngine::Transform*> get_debris(int type) noexcept
            {
                if (type < 0 || type > 1) return std::nullopt;
                auto function = CondDep::Find<UnityEngine::Transform*, int>(qosm_id, "get_debris");
                if (!function) return std::nullopt;
                UnityEngine::Transform* result = function.value()(type);
                return std::make_optional(result);
            }

            /// @brief gets the active bomb if available
            /// @return valid ptr for found, nullptr for not found, nullopt for no qosmetics or invalid type
            static std::optional<UnityEngine::Transform*> get_bomb() noexcept
            {
                auto function = CondDep::Find<UnityEngine::Transform*>(qosm_id, "get_bomb");
                if (!function) return std::nullopt;
                UnityEngine::Transform* result = function.value()();
                return std::make_optional(result);
            }

            /// @brief sets the default note as the active note
            static void SetDefault() noexcept
            {
                auto function = CondDep::Find<void>(qosm_id, "SetDefaultNote");
                if (!function) return;
                function.value()();
            }

            /// @brief Sets active note based on identifier (usually just the note name)
            /// @param note filename (without extension) of the note
            static void SetActiveNote(std::string note) noexcept
            {
                if (note == "") return;
                auto function = CondDep::Find<void, const char*>(qosm_id, "SetActiveNote");
                if (!function) return;
                function.value()(note.c_str());
            }

            /// @brief Sets active note based on full file path
            /// @param notePath full path to the note
            static void SetActiveNoteFromFilePath(std::string notePath) noexcept
            {
                if (notePath == "") return;
                auto function = CondDep::Find<void, const char*>(qosm_id, "SetActiveNoteFromFilePath");
                if (!function) return;
                function.value()(notePath.c_str());
            }

            /// @brief returns whether there's a custom note, even with false the trails still get moved so keep that in mind!
            /// @return true for custom, false for default, nullopt for nonexistent
            static std::optional<bool> GetNoteIsCustom() noexcept
            {
                auto function = CondDep::Find<bool>(qosm_id, "GetNoteIsCustom");
                if (!function) return std::nullopt;
                return std::make_optional(function.value()());
            }

            /// @brief returns the filepath to the Qosmetics notes folder, in case it ever gets moved
            /// @return string folder path, or nullopt for not installed
            static std::optional<std::string> GetNoteFolder()
            {
                auto function = CondDep::Find<char*>(qosm_id, "GetNoteFolder");
                if (!function) return std::nullopt;
                char* array = function.value()();
                std::string result(array);
                delete[](array);
                return std::make_optional(function.value()());
            }
    };
}

#undef qosm_id