#pragma once

#include "beatsaber-hook/shared/utils/typedefs.h"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/Color.hpp"
#include "extern/beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "extern/beatsaber-hook/shared/utils/utils.h"

#if defined __has_include && __has_include("sombrero/shared/ColorUtils.hpp")
#include "sombrero/shared/ColorUtils.hpp"
#define SOMBRERO
#else
#include "UnityEngine/Color.hpp"
#endif

namespace Qosmetics
{
    class AltTrail : public UnityEngine::MonoBehaviour
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

            #ifdef SOMBRERO
            Sombrero::FastColor MyColor;
            #else
            UnityEngine::Color MyColor;
            #endif

        private:
            uint8_t privateMembers[sizeof(int) * 6 + sizeof(void*) * 3 + sizeof(std::vector<void*>) + sizeof(float) + sizeof(bool)];
    };
}
DEFINE_IL2CPP_ARG_TYPE(Qosmetics::AltTrail*, "Qosmetics", "AltTrail");