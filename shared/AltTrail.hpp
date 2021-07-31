#pragma once

#include "extern/beatsaber-hook/shared/utils/typedefs.h"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/Color.hpp"
#include "extern/beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "extern/beatsaber-hook/shared/utils/utils.h"

#define CACHED_METHOD_CALL_VOID(name) \
    void name() \
    { \
        static auto* ___internal__method = ::il2cpp_utils::FindMethod(this, #name, std::vector<Il2CppClass*>{}, ::std::vector<const Il2CppType*>{})); \
        auto ___instance_arg = this; \
        ::il2cpp_utils::RunMethodThrow<void, false>(___instance_arg, ___internal__method);\
    }

namespace Qosmetics
{
    class AltTrail : UnityEngine::MonoBehaviour
    {
        public:
            Il2CppString* SortingLayerName;
            UnityEngine::Transform* PointStart;
            UnityEngine::Transform* PointEnd;
            UnityEngine::Material* MyMaterial;

            Il2CppObject* vertexPool;

            int SortingOrder;
            int TrailLength;
            int Granularity;
            float WhiteStep;
            UnityEngine::Color MyColor;

            void Reset(bool addNewElemsToSnap)
            {
                auto* ptr = il2cpp_utils::FindMethodUnsafe("Qosmetics", "AltTrail", "Reset", 1);
                if (ptr) il2cpp_utils::RunMethod(this, ptr, addNewElemsToSnap);
            }

        private:
            uint8_t privateMembers[sizeof(int) * 6 + sizeof(void*) * 3 + sizeof(std::vector<void*>) + sizeof(float) + sizeof(bool)];
    }
}

#undef CACHED_METHOD_CALL

DEFINE_IL2CPP_ARG_TYPE(Qosmetics::AltTrail*, "Qosmetics", "AltTrail");