#include "Utils/WallUtils.hpp"
#include "Utils/MaterialUtils.hpp"
#include "UnityEngine/Shader.hpp"

#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
static Il2CppString* ObstacleCore = nullptr;
static Il2CppString* DepthWrite = nullptr;
static Il2CppString* FakeGlowWrapperObstacleFakeGlow = nullptr;
static Il2CppString* ObstacleFrame = nullptr;
static Il2CppString* ObstacleCoreLWInside = nullptr;

static int _SizeParams = 0;
static int _EdgeSize = 0;

using namespace UnityEngine;

namespace WallUtils
{
    void SetSizeParams(GameObject* object, Vector3 localScale, float edgeSize)
    {
        if (!_SizeParams) _SizeParams = Shader::PropertyToID(il2cpp_utils::createcsstr("_SizeParams"));
        if (!_EdgeSize) _EdgeSize = Shader::PropertyToID(il2cpp_utils::createcsstr("_EdgeSize"));

        Array<Renderer*>* renderers = object->GetComponentsInChildren<Renderer*>(true);
        localScale = localScale * 0.5f;
        Vector4 sizeParams = UnityEngine::Vector4(localScale.x, localScale.y, localScale.z, 0.05f);

        int rendererCount = renderers->Length();
        for (int i = 0; i < rendererCount; i++)
        {
            Renderer* renderer = renderers->values[i];

            Array<Material*>* materials = MaterialUtils::GetMaterials(renderer);
            int materialCount = materials->Length();
            for (int j = 0; j < materialCount; j++)
            {
                Material* material = materials->values[j];

                if (material->HasProperty(_SizeParams)) material->SetVector(_SizeParams, sizeParams);
                if (edgeSize && material->HasProperty(_EdgeSize)) material->SetFloat(_EdgeSize, edgeSize);
            }
        }
    }

    WallReferences GatherReferences(UnityEngine::Transform* obstacle)
    {
        if (!ObstacleCore) ObstacleCore = il2cpp_utils::createcsstr("ObstacleCore", il2cpp_utils::StringType::Manual);
        if (!DepthWrite) DepthWrite = il2cpp_utils::createcsstr("DepthWrite", il2cpp_utils::StringType::Manual);

        UnityEngine::Transform* obstacleCore = obstacle ? obstacle->Find(ObstacleCore) : nullptr;

        if (!FakeGlowWrapperObstacleFakeGlow) FakeGlowWrapperObstacleFakeGlow = il2cpp_utils::createcsstr("FakeGlowWrapper/ObstacleFakeGlow", il2cpp_utils::StringType::Manual);
        UnityEngine::Transform* obstacleFakeGlow = obstacle ? obstacle->Find(FakeGlowWrapperObstacleFakeGlow) : nullptr;

        Renderer* fakeGlowRenderer = obstacleFakeGlow ? obstacleFakeGlow->get_gameObject()->GetComponent<Renderer*>() : nullptr;
        
        if (!ObstacleFrame) ObstacleFrame = il2cpp_utils::createcsstr("ObstacleFrame", il2cpp_utils::StringType::Manual);
        UnityEngine::Transform* obstacleFrame = obstacle ? obstacle->Find(ObstacleFrame) : nullptr;
        
        Renderer* frameRenderer = obstacleFrame ? obstacleFrame->get_gameObject()->GetComponent<Renderer*>() : nullptr;
        MeshFilter* frameFilter = obstacleFrame ? obstacleFrame->get_gameObject()->GetComponent<MeshFilter*>() : nullptr;

        //if (!ObstacleCoreLWInside) ObstacleCoreLWInside = il2cpp_utils::createcsstr("ObstacleCoreLWInside", il2cpp_utils::StringType::Manual);
        //UnityEngine::Transform* obstacleCoreInside = obstacleCore->Find(ObstacleCoreLWInside);

        Renderer* coreRenderer = obstacleCore ? obstacleCore->get_gameObject()->GetComponent<Renderer*>() : nullptr;//obstacleCoreInside->get_gameObject()->GetComponent<Renderer*>();
        MeshFilter* coreFilter = obstacleCore ? obstacleCore->get_gameObject()->GetComponent<MeshFilter*>() : nullptr;//obstacleCoreInside->get_gameObject()->GetComponent<MeshFilter*>();

        return {coreRenderer, frameRenderer, fakeGlowRenderer, coreFilter, frameFilter};
    }

    WallReferences PrepareAndGatherReferences(UnityEngine::Transform* obstacle)
    {
        if (!ObstacleCore) ObstacleCore = il2cpp_utils::createcsstr("ObstacleCore", il2cpp_utils::StringType::Manual);
        if (!DepthWrite) DepthWrite = il2cpp_utils::createcsstr("DepthWrite", il2cpp_utils::StringType::Manual);

        UnityEngine::Transform* obstacleCore = obstacle->Find(ObstacleCore);
        UnityEngine::Transform* obstacleDepthWrite = obstacleCore ? obstacleCore->Find(DepthWrite) : nullptr;

        //obstacleCore->get_gameObject()->GetComponent<Renderer*>()->set_enabled(false);
        if (obstacleDepthWrite) obstacleDepthWrite->get_gameObject()->GetComponent<Renderer*>()->set_enabled(false);

        return GatherReferences(obstacle);
    }

    void SetColors(UnityEngine::GameObject* obstacle, UnityEngine::Color color, int renderQueue)
    {
        MaterialUtils::SetColors(obstacle, color, false, renderQueue);
    }
}