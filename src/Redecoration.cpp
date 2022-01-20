#include "Redecoration.hpp"
#include "RedecorationRegister_Internal.hpp"

namespace Qosmetics::Core::Redecoration
{
    void Redecorate(Il2CppObject*& object, std::string_view name, System::Type* objectType, System::Type* containerType)
    {
        auto optvecref = GetRegistrations(name.data());
        if (optvecref == std::nullopt) return;
        auto& vec = optvecref->get();

        for (auto* reg : vec)
        {
            if (reg->get_prefabType() != objectType || reg->get_containerType() != containerType) continue;

            object = reg->Redecorate(object);
        }
    }
}