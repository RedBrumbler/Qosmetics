#include "FlowCoordinatorRegister.hpp"
#include "FlowCoordinatorRegister_Internal.hpp"

namespace Qosmetics::Core::FlowCoordinator
{
    std::vector<const Registration*> registrations;
    void Register(const Registration* registration)
    {
        registrations.push_back(registration);
    }

    const std::vector<const Registration*>& GetRegistrations()
    {
        return registrations;
    }
}