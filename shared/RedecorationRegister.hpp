#pragma once

#include "beatsaber-hook/shared/utils/typedefs.h"

namespace Qosmetics::Core::Redecoration
{
    struct Registration
    {
        /// @brief Create a new registration
        /// @param contract the prefab contract (field name w/o underscore prefix!)
        /// @param priority the priority of this redecoration
        /// @param chain whether to continue chaining after this redecoration, following priority
        Registration(std::string contract, int priority = 0, bool chain = true) : contract(contract), priority(priority), chain(chain){};

        /// @brief the method that will be called when this registration is used
        /// @param prefab the object to redecorate
        /// @return the redecorated object (should be the same object)
        virtual Il2CppObject* Redecorate(Il2CppObject* prefab) const = 0;

        virtual void Register() const = 0;

        bool get_chain() const { return chain; }
        int get_priority() const { return priority; }
        std::string get_contract() const { return contract; }
        virtual System::Type* get_prefabType() const = 0;
        virtual System::Type* get_containerType() const = 0;

        bool operator<(const Registration& rhs) const;

    private:
        bool chain;
        int priority;
        std::string contract;
    };

    /// @brief register a registration to redecorate something
    /// @param registration the registration to use
    void Register(const Registration* registration);
}

#ifndef REDECORATION_REGISTRATION
/*
macro to be used in a similiar way to a hook, this makes creating registrations far easier than having to manually manage everythinbg, and ensures registrations are used as intended

example usage:
REDECORATION_REGISTRATION(saberModelControllerPrefab, 10, true, GlobalNamespace::SaberModelController*, GlobalNamespace::SaberModelContainer*)
{
    // adding a Qosmetics trailhelper to the prefab for some reason (this is a very simple example and probably wouldn't properly work)
    saberModelControllerPrefab->AddComponent<Qosmetics::TrailHelper*>();
    return saberModelControllerPrefab;
}
*/
#define REDECORATION_REGISTRATION(contract_, priority_, chain_, prefabType_, containerType_)                                                                                \
    struct redecoration_registration_##contract_##priority_ : public Qosmetics::Core::Redecoration::Registration                                                            \
    {                                                                                                                                                                       \
        redecoration_registration_##contract_##priority_() : Registration(#contract_, nullptr, nullptr priority_, chain_)                                                   \
        {                                                                                                                                                                   \
            Qosmetics::Core::Redecoration::Register(this);                                                                                                                  \
        }                                                                                                                                                                   \
        prefabType_ Redecorate(prefabType_ contract_) const;                                                                                                                \
        Il2CppObject* Redecorate(Il2CppObject* contract_) const override                                                                                                    \
        {                                                                                                                                                                   \
            return redecoration_registration_##contract_##priority_::Redecorate(reinterpret_cast<prefabType_>(contract_));                                                  \
        }                                                                                                                                                                   \
        System::Type* get_prefabType() const override { return csTypeOf(prefabType_); }                                                                                     \
        System::Type* get_containerType() const override { return csTypeOf(containerType_); }                                                                               \
    };                                                                                                                                                                      \
    static redecoration_registration_##contract_##priority_ redecoration_registration_##contract_##priority_ Instance = redecoration_registration_##contract_##priority_(); \
    prefabType_ redecoration_registration_##contract_##priority_::Redecorate(prefabType_) const
#endif