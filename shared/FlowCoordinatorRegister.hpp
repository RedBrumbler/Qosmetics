#pragma once

#include "UnityEngine/Sprite.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"
#include <utility>

namespace Qosmetics::Core::FlowCoordinator
{
    struct Registration
    {
    public:
        Registration(std::string_view name) : name(name){};

        std::string name;
        virtual System::Type* get_flowCoordinatorType() const = 0;

        /// @brief a method to get the sprites for the button, the first should be the inactive one, the second should be the active one
        virtual std::pair<UnityEngine::Sprite*, UnityEngine::Sprite*> get_sprites() const = 0;
    };

    void Register(const Registration* registration);
}

#ifndef QOSMETICS_FLOWCOORDINATOR_REGISTER
/// This exists to make registering a flowcoordinator to the list of supported buttons in the Qosmetics menu easier.
/// the code block after this register should provide the 2 sprites required for a button
/// example usage:
/// QOSMETICS_FLOWCOORDINATOR_REGISTER(Saber, Qosmetics::Saber::FlowCoordinator)
/// {
///     return std::make_pair(QuestUI::BeatSaberUI::Base64ToSprite(saber_inactive), QuestUI::BeatSaberUI::Base64ToSprite(saber_active));
/// }
#define QOSMETICS_FLOWCOORDINATOR_REGISTER(name_, typename)                                                                              \
    struct flowcoordinator_registration_##name_ : public Qosmetics::Core::FlowCoordinator::Registration                                  \
    {                                                                                                                                    \
        flowcoordinator_registration_##name_() : Qosmetics::Core::FlowCoordinator::Registration(#name_)                                  \
        {                                                                                                                                \
            Qosmetics::Core::FlowCoordinator::Register(this);                                                                            \
        }                                                                                                                                \
        System::Type* get_flowCoordinatorType() const override { return csTypeOf(typename); }                                            \
        std::pair<UnityEngine::Sprite*, UnityEngine::Sprite*> get_sprites();                                                             \
    };                                                                                                                                   \
    static flowcoordinator_registration_##name_ flowcoordinator_registration_##name_ _Instance = flowcoordinator_registration_##name_(); \
    std::pair<UnityEngine::Sprite*, UnityEngine::Sprite*> get_sprites() const
#endif
