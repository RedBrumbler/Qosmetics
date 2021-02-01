#pragma once
#include <map>
#include <vector>
#include <string>
#include "Data/Descriptor.hpp"

namespace Qosmetics
{
    class DescriptorCache
    {
        public:

        private:
            std::map<std::string, Descriptor> saberDescriptors; 
            std::map<std::string, Descriptor> wallDescriptors; 
            std::map<std::string, Descriptor> noteDescriptors; 
            std::map<std::string, Descriptor> pointerDescriptors; 
    };
}