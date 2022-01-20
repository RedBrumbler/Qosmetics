#pragma once
#include "RedecorationRegister.hpp"
#include <vector>

namespace Qosmetics::Core::Redecoration
{
    using OptRegVectRef = std::optional<std::reference_wrapper<std::vector<const Registration*>>>;
    OptRegVectRef GetRegistrations(std::string prefabName);
}