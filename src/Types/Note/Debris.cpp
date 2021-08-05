#include "Types/Note/Debris.hpp"
#include "Utils/NoteUtils.hpp"
#include "Utils/UnityUtils.hpp"
#include "QosmeticsLogger.hpp"
#include "chroma/shared/NoteAPI.hpp"

#include "UnityEngine/MeshRenderer.hpp"

#include "UnityEngine/Transform.hpp"
#include "UnityEngine/GameObject.hpp"

DEFINE_TYPE(Qosmetics, Debris);

using namespace UnityEngine;

static Il2CppString* NoteDebrisMeshName = nullptr;

#define INFO(value...) QosmeticsLogger::GetContextLogger("Debris").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("Debris").error(value)

namespace Qosmetics
{
    void Debris::Awake()
    {
        replaced[0] = false;
        replaced[1] = false;
    }

    void Debris::Replace()
    {
        if (!NoteDebrisMeshName) NoteDebrisMeshName = il2cpp_utils::createcsstr("NoteDebrisMesh", il2cpp_utils::StringType::Manual);
        Transform* mesh = get_transform()->Find(NoteDebrisMeshName);
        if (!mesh) return;
        NoteUtils::SetNoteSize(mesh);

        if (!modelManager || modelManager->get_type() != ItemType::note)
        {
            ERROR("Note model manager was nullptr or itemtype was invalid");
            replaced[colorType] = true;
            return;
        }

        if (replaced[colorType]) 
        {
            UpdateModel();
            return;
        }

        if (modelManager->get_item().get_config().get_hasDebris())
        {
            mesh->get_gameObject()->GetComponent<MeshRenderer*>()->set_enabled(false);

            Transform* prefab = nullptr;
            Il2CppString* name = nullptr;
            switch (colorType)
            {
                case 0:
                    name = modelManager->get_leftDebrisName();
                    prefab = modelManager->get_leftDebris();
                    break;
                case 1:
                    name = modelManager->get_rightDebrisName();
                    prefab = modelManager->get_rightDebris();
                    break;
                default:
                    break;
            }

            prefab->SetParent(mesh);
            prefab->get_gameObject()->set_name(name);
            prefab->set_localEulerAngles(Vector3::get_zero());
            static const Vector3 ZeroPointFour = Vector3::get_one() * 0.4f;
            prefab->set_localScale(ZeroPointFour);
            prefab->set_localPosition(Vector3::get_zero());
            UnityUtils::SetLayerRecursive(prefab->get_gameObject(), 9);

            NoteUtils::AddRenderersToPropertyBlockController(GetComponent<GlobalNamespace::MaterialPropertyBlockController*>(), prefab->get_gameObject());
            
            replaced[colorType] = true;
            UpdateModel();
        }
        else
        {
            replaced[colorType] = true;
        }
    }

    void Debris::UpdateModel()
    {
        if (!modelManager->get_item().get_config().get_hasDebris()) return;
        Transform* mesh = get_transform()->Find(NoteDebrisMeshName);
        if (!mesh) return;
        for (int i = 0; i < 2; i++)
        {
            if (!replaced[colorType]) continue;
            Il2CppString* name = nullptr;
            switch(i)
            {
                case 0:
                    name = modelManager->get_leftDebrisName();
                    break;
                case 1:
                    name = modelManager->get_rightDebrisName();
                    break;
                default:
                    break;
            }

            Transform* transform = mesh->Find(name);
            if (transform)
            {
                GameObject* obj = transform->get_gameObject();
                obj->SetActive(i == colorType);
                if (i == colorType)
                {
                    NoteUtils::SetDebrisProperties(obj, cutPoint, cutNormal);
                }
            }
        }

        UpdateColors();
    }

    void Debris::UpdateColors()
    {
        auto optionalThisColor = Chroma::NoteAPI::getNoteColorSafe(colorType);
        auto optionalOtherColor = Chroma::NoteAPI::getNoteColorSafe(1 - colorType);

        Color thisColor = optionalThisColor ? *optionalThisColor : colorManager->ColorForNoteType(colorType);
        Color otherColor = optionalOtherColor ? *optionalOtherColor : colorManager->ColorForNoteType(1 - colorType);
    
        Il2CppString* name = nullptr;
        switch(colorType)
        {
            case 0:
                name = modelManager->get_leftDebrisName();
                break;
            case 1:
                name = modelManager->get_rightDebrisName();
                break;
            default:
                break;
        }

        Transform* mesh = get_transform()->Find(NoteDebrisMeshName);
        Transform* transform = mesh->Find(name);
        if (transform)
        {
            GameObject* obj = transform->get_gameObject();
            NoteUtils::SetColors(obj, thisColor, otherColor);
            GetComponent<GlobalNamespace::MaterialPropertyBlockController*>()->ApplyChanges();
        }
    }

    void Debris::Restore()
    {

    }

    void Debris::Init(NoteManager* modelManager, ColorManager* colorManager)
    {
        this->modelManager = modelManager;
        this->colorManager = colorManager;
    }

    void Debris::SetData(int colorType, Vector3 cutPoint, Vector3 cutNormal)
    {
        this->colorType = colorType;
        this->cutPoint = cutPoint;
        this->cutNormal = cutNormal;
    }
}