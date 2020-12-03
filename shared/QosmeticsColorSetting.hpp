#pragma once
#include "beatsaber-hook/shared/utils/utils.h"

#ifndef HAS_CODEGEN
#error Codegen is needed to build with QosmeticsColors header! run 'qpm dependency add "codegen"' to add it to your project
#endif

#ifdef HAS_CODEGEN
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/ScriptableObject.hpp"
#include "GlobalNamespace/SimpleColorSO.hpp"
#include "GlobalNamespace/ColorScheme.hpp"

namespace Qosmetics
{
    class QosmeticsColors
    {
        public:
            /// @brief always call this method at gamecore to make sure the 
            static void GameCore()
            {
                manager = nullptr;
                FindManager();
            }

            /// @brief Sets all Left object colors: sabers, trails and bloqs
            /// @param color the color to set
            static void SetLeftColor(UnityEngine::Color color)
            {
                if (!FindManager()) return;
                colorSO->SetColor(color);
                manager->SendMessage(setLeftColor, colorSO, 1);
            }

            /// @brief Sets all Right object colors: sabers, trails and bloqs
            /// @param color the color to set
            static void SetRightColor(UnityEngine::Color color)
            {
                if (!FindManager()) return;
                colorSO->SetColor(color);
                manager->SendMessage(setRightColor, colorSO, 1);
            }

            /// @brief Sets the left note/bloq color
            /// @param color the color to set
            static void SetLeftNoteColor(UnityEngine::Color color)
            {
                if (!FindManager()) return;
                colorSO->SetColor(color);
                manager->SendMessage(setLeftNoteColor, colorSO, 1);
            }

            /// @brief Sets the right note/bloq color
            /// @param color the color to set
            static void SetRightNoteColor(UnityEngine::Color color)
            {
                if (!FindManager()) return;
                colorSO->SetColor(color);
                manager->SendMessage(setRightNoteColor, colorSO, 1);
            }

            /// @brief Sets the left saber color
            /// @param color the color to set
            static void SetLeftSaberColor(UnityEngine::Color color)
            {
                if (!FindManager()) return;
                colorSO->SetColor(color);
                manager->SendMessage(setLeftSaberColor, colorSO, 1);
            }

            /// @brief Sets the right saber color
            /// @param color the color to set
            static void SetRightSaberColor(UnityEngine::Color color)
            {
                if (!FindManager()) return;
                colorSO->SetColor(color);
                manager->SendMessage(setRightSaberColor, colorSO, 1);
            }

            /// @brief Sets the left trail color
            /// @param color the color to set
            static void SetLeftTrailColor(UnityEngine::Color color)
            {
                if (!FindManager()) return;
                colorSO->SetColor(color);
                manager->SendMessage(setLeftTrailColor, colorSO, 1);
            }

            /// @brief Sets the right trail color
            /// @param color the color to set
            static void SetRightTrailColor(UnityEngine::Color color)
            {
                if (!FindManager()) return;
                colorSO->SetColor(color);
                manager->SendMessage(setRightTrailColor, colorSO, 1);
            }

            /// @brief Sets the obstacle color
            /// @param color the color to set
            static void SetObstacleColor(UnityEngine::Color color)
            {
                if (!FindManager()) return;
                colorSO->SetColor(color);
                manager->SendMessage(setObstacleColor, colorSO, 1);
            }

            /// @brief Sets the color scheme, this would be like setting LeftColor, RightColor and ObstacleColor
            /// @param scheme the color scheme to copy
            static void SetColorScheme(GlobalNamespace::ColorScheme* scheme)
            {
                if (!FindManager()) return;
                manager->SendMessage(setColorScheme, scheme, 1);
            }

            static void Init()
            {
                colorSO = UnityEngine::ScriptableObject::CreateInstance<GlobalNamespace::SimpleColorSO*>();
                setLeftColor = il2cpp_utils::createcsstr("SetLeftColor", il2cpp_utils::Manual);
                setRightColor = il2cpp_utils::createcsstr("SetRightColor", il2cpp_utils::Manual);
                setLeftNoteColor = il2cpp_utils::createcsstr("SetLeftNoteColor", il2cpp_utils::Manual);
                setRightNoteColor = il2cpp_utils::createcsstr("SetRightNoteColor", il2cpp_utils::Manual);
                setLeftSaberColor = il2cpp_utils::createcsstr("SetLeftSaberColor", il2cpp_utils::Manual);
                setRightSaberColor = il2cpp_utils::createcsstr("SetRightSaberColor", il2cpp_utils::Manual);
                setLeftTrailColor = il2cpp_utils::createcsstr("SetLeftTrailColor", il2cpp_utils::Manual);
                setRightTrailColor = il2cpp_utils::createcsstr("SetRightTrailColor", il2cpp_utils::Manual);
                setColorScheme = il2cpp_utils::createcsstr("SetColorScheme", il2cpp_utils::Manual);
                setObstacleColor = il2cpp_utils::createcsstr("SetObstacleColor", il2cpp_utils::Manual);
                managerName = il2cpp_utils::createcsstr("QosmeticsColorManager", il2cpp_utils::Manual);
                manager = nullptr;
            }

        private:
            static bool FindManager()
            {
                if (manager) return true;
                manager = UnityEngine::GameObject::Find(managerName);
                return (manager != nullptr);
            }

            static inline GlobalNamespace::SimpleColorSO* colorSO = nullptr;
            static inline Il2CppString* setLeftColor = nullptr;
            static inline Il2CppString* setRightColor = nullptr;
            static inline Il2CppString* setLeftNoteColor = nullptr;
            static inline Il2CppString* setRightNoteColor = nullptr;
            static inline Il2CppString* setLeftSaberColor = nullptr;
            static inline Il2CppString* setRightSaberColor = nullptr;
            static inline Il2CppString* setLeftTrailColor = nullptr;
            static inline Il2CppString* setRightTrailColor = nullptr;
            static inline Il2CppString* setColorScheme = nullptr;
            static inline Il2CppString* setObstacleColor = nullptr;

            static inline Il2CppString* managerName = nullptr;
            static inline UnityEngine::GameObject* manager = nullptr;
    };
}
#endif