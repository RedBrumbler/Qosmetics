#include "RedecorationRegister.hpp"
#include "RedecorationRegister_Internal.hpp"
#include <unordered_map>
#include <vector>

namespace Qosmetics::Core::Redecoration
{
    std::unordered_map<std::string, std::vector<const Registration*>> registrationsByContract = {};

    void Register(const Registration* registration)
    {
        auto vecItr = registrationsByContract.find(registration->get_contract());
        if (vecItr == registrationsByContract.end())
        {
            registrationsByContract.insert(std::make_pair(registration->get_contract(), std::vector<const Registration*>{registration}));
        }
        else
        {
            auto& vec = vecItr->second;
            vec.push_back(registration);
            std::sort(vec.begin(), vec.end(), [](auto* lhs, auto* rhs) -> bool
                      { return lhs->operator<(*rhs); });
        }
    }

    bool Registration::operator<(const Registration& rhs) const
    {
        return priority < rhs.priority;
    }

    OptRegVectRef GetRegistrations(std::string prefabName)
    {
        auto vecItr = registrationsByContract.find(prefabName);
        if (vecItr == registrationsByContract.end())
        {
            return std::nullopt;
        }
        return vecItr->second;
    }
}