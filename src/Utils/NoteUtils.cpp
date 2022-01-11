#include "Config.hpp"

#include "UnityEngine/Mathf.hpp"
#include "UnityEngine/MeshFilter.hpp"
#include "UnityEngine/Shader.hpp"
#include "UnityEngine/SphereCollider.hpp"
#include "UnityEngine/Vector3.hpp"
#include "Utils/MaterialUtils.hpp"
#include "Utils/NoteUtils.hpp"
#include "Utils/PlayerSettings.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"

#include "QosmeticsLogger.hpp"

#include "GlobalNamespace/MaterialPropertyBlockController.hpp"
#include "UnityEngine/MaterialPropertyBlock.hpp"

#include <cmath>

using namespace UnityEngine;
using namespace Qosmetics;

static Il2CppString* BigCuttable = nullptr;
static Il2CppString* SmallCuttable = nullptr;
static Il2CppString* NoteArrow = nullptr;
static Il2CppString* NoteArrowGlow = nullptr;
static Il2CppString* NoteCircleGlow = nullptr;

#define INFO(value...) QosmeticsLogger::GetContextLogger("Note Utils").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("Note Utils").error(value)

static int _SlicePos = 0;
static int _CutPlane = 0;
static int _TransformOffset = 0;

namespace NoteUtils
{
    void SetColors(GameObject* object, Color color, Color otherColor, int renderQueue)
    {
        MaterialUtils::SetColors(object, color, otherColor, true, renderQueue);
    }

    void SetNoteSize(Transform* transform)
    {
        if (!transform || (!config.noteConfig.overrideNoteSize && !PlayerSettings::get_SmallNotes()))
            return;
        Vector3 size = Vector3::get_one() * (float)config.noteConfig.noteSize * (PlayerSettings::get_SmallNotes() ? 0.5f : 1.0f);
        transform->set_localScale(size);

        if (!config.noteConfig.alsoChangeHitboxes)
        {
            if (!BigCuttable)
                BigCuttable = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("BigCuttable");
            if (!SmallCuttable)
                SmallCuttable = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("SmallCuttable");

            Transform* bigCuttable = transform->Find(BigCuttable);
            Transform* smallCuttable = transform->Find(SmallCuttable);

            Vector3 othersize = Vector3::get_one() / (float)config.noteConfig.noteSize;
            if (bigCuttable)
                bigCuttable->set_localScale(othersize);
            if (smallCuttable)
                smallCuttable->set_localScale(othersize);
        }
    }

    void SetBombSize(Transform* mesh)
    {
        if (!mesh || !config.noteConfig.overrideNoteSize)
            return;
        SphereCollider* collider = mesh->get_gameObject()->GetComponent<SphereCollider*>();
        if (!collider)
            return;

        auto setRadius = reinterpret_cast<function_ptr_t<void, Il2CppObject*, float>>(il2cpp_functions::resolve_icall("UnityEngine.SphereCollider::set_radius"));
        setRadius(collider, 0.18f / config.noteConfig.noteSize);
        mesh->set_localScale(Vector3::get_one() * config.noteConfig.noteSize);
    }

    void HideBaseGameNotes(Transform* noteCube, Qosmetics::NoteConfig& config, bool hide)
    {
        if (!noteCube)
            return;

        if (config.get_disableBaseGameArrows())
        {
            // create strings cause reasons
            if (!NoteArrow)
                NoteArrow = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("NoteArrow");
            if (!NoteArrowGlow)
                NoteArrowGlow = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("NoteArrowGlow");
            if (!NoteCircleGlow)
                NoteCircleGlow = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("NoteCircleGlow");

            if (Transform* noteArrow = noteCube->Find(NoteArrow))
                noteArrow->get_gameObject()->SetActive(false);
            if (Transform* noteArrowGlow = noteCube->Find(NoteArrowGlow))
                noteArrowGlow->get_gameObject()->SetActive(false);
            if (Transform* noteCircleGlow = noteCube->Find(NoteCircleGlow))
                noteCircleGlow->get_gameObject()->SetActive(false);
        }

        MeshFilter* cubeFilter = noteCube->get_gameObject()->GetComponent<MeshFilter*>();
        // if cube renderer not found, don't try to disable it (it crashes sort of so that's not great)
        if (cubeFilter)
            cubeFilter->set_mesh(nullptr);
    }

    void SetDebrisProperties(GameObject* debris, Vector3 cutPoint, Vector3 cutNormal)
    {
        float magnitude = cutPoint.get_magnitude();

        if (magnitude > 0.04f)
        {
            cutPoint = 0.2f * cutPoint / std::sqrt(magnitude);
        }

        Vector4 slicePos(cutPoint.x, cutPoint.y, cutPoint.z, 0.0f);
        Vector4 cutPlane(cutNormal.x, cutNormal.y, cutNormal.z, 0.0f);

        ArrayW<Renderer*> renderers = debris->GetComponentsInChildren<Renderer*>();
        int rendererCount = renderers.Length();

        for (int i = 0; i < rendererCount; i++)
        {
            Renderer* renderer = renderers.get(i);
            Material* material = renderer->get_material();

            Vector3 localPosition = renderer->get_transform()->get_localPosition();
            Vector4 transformOffset(localPosition.x, localPosition.y, localPosition.z, 0.0f);

            if (!_SlicePos)
                _SlicePos = Shader::PropertyToID(il2cpp_utils::newcsstr("_SlicePos"));
            if (!_CutPlane)
                _CutPlane = Shader::PropertyToID(il2cpp_utils::newcsstr("_CutPlane"));
            if (!_TransformOffset)
                _TransformOffset = Shader::PropertyToID(il2cpp_utils::newcsstr("_TransformOffset"));

            if (material->HasProperty(_SlicePos))
                material->SetVector(_SlicePos, slicePos);
            if (material->HasProperty(_CutPlane))
                material->SetVector(_CutPlane, cutPlane);
            if (material->HasProperty(_TransformOffset))
                material->SetVector(_TransformOffset, transformOffset);
            renderer->set_sortingLayerID(9);
        }
    }

    bool anyCC(Renderer* renderer)
    {
        ArrayW<Material*> materials = renderer->GetMaterialArray();
        int materialCount = materials.Length();
        for (int i = 0; i < materialCount; i++)
        {
            Material* material = materials.get(i);
            Il2CppString* materialNameCS = material->get_name();
            std::string materialName = to_utf8(csstrtostr(materialNameCS));

            if (materialName.find("_replace") != std::string::npos && MaterialUtils::ShouldCC(materials.get(i), true))
                return true;
        }

        return false;
    }
    void AddRenderersToPropertyBlockController(GlobalNamespace::MaterialPropertyBlockController* propertyController, GameObject* obj)
    {
        if (!propertyController)
            return;
        if (!obj)
            return;

        ArrayW<Renderer*> myRenderers = obj->GetComponentsInChildren<Renderer*>(true);

        std::vector<Renderer*> newRendererVector = {};
        int rendererCount = propertyController->renderers.Length();

        for (int j = 0; j < rendererCount; j++)
        {
            newRendererVector.push_back(propertyController->renderers.get(j));
        }

        rendererCount = myRenderers.Length();
        for (int j = 0; j < rendererCount; j++)
        {
            Renderer* currentRenderer = myRenderers.get(j);
            if (anyCC(currentRenderer))
                newRendererVector.push_back(currentRenderer);
        }

        propertyController->renderers = il2cpp_utils::vectorToArray(newRendererVector);
    }
}