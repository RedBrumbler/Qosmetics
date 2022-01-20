#pragma once

#include <string>
#include <vector>

namespace Qosmetics::Core
{
    struct Patrons
    {
        static Patrons Parse(std::string json);

        bool any();
        std::vector<std::string> enthusiastic = {};
        std::vector<std::string> amazing = {};
        std::vector<std::string> legendary = {};
        std::vector<std::string> paypal = {};
    };
}