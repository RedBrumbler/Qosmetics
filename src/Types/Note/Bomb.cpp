#include "Types/Note/Bomb.hpp"
#include "QosmeticsLogger.hpp"
#include "Utils/NoteUtils.hpp"
#include "Utils/UnityUtils.hpp"
#include "Utils/MaterialUtils.hpp"

#include "UnityEngine/MeshRenderer.hpp"

DEFINE_TYPE(Qosmetics, Bomb);

using namespace UnityEngine;

#define INFO(value...) QosmeticsLogger::GetContextLogger("Bomb").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("Bomb").error(value)

static Il2CppString* MeshName = nullptr;

namespace Qosmetics
{
    void Bomb::Init(NoteManager* modelManager, ColorManager* colorManager)
    {
        this->modelManager = modelManager;
        this->colorManager = colorManager;
    }

    void Bomb::Awake()
    {
        replaced = false;
    }

    void Bomb::Replace()
    {   
        if (!MeshName) MeshName = il2cpp_utils::createcsstr("Mesh", il2cpp_utils::StringType::Manual);
        Transform* mesh = get_transform()->Find(MeshName);
        if (!mesh) return;
        NoteUtils::SetBombSize(mesh);
        if (replaced) return;

        if (!modelManager || modelManager->get_type() != ItemType::note || modelManager->get_item().get_type() != ItemType::note)
        {
            ERROR("Note model manager was nullptr or itemtype was invalid");
            replaced = true;
            return;
        }

        if (modelManager->get_item().get_config().get_hasBomb())
        {
            mesh->get_gameObject()->GetComponent<MeshRenderer*>()->set_enabled(false);

            Transform* prefab = modelManager->get_bomb();
            prefab->SetParent(mesh);
            prefab->get_gameObject()->set_name(modelManager->get_bombName());
            prefab->set_localEulerAngles(Vector3::get_zero());
            static const Vector3 ZeroPointFour = Vector3::get_one() * 0.4f;
            prefab->set_localScale(ZeroPointFour);
            prefab->set_localPosition(Vector3::get_zero());
            UnityUtils::SetLayerRecursive(prefab->get_gameObject(), 8);
            GlobalNamespace::MaterialPropertyBlockController* propertyController = GetComponent<GlobalNamespace::MaterialPropertyBlockController*>();
            NoteUtils::AddRenderersToPropertyBlockController(propertyController, prefab->get_gameObject());
            propertyController->ApplyChanges();

            if (isMirror) MaterialUtils::SetRenderQueue(prefab->get_gameObject(), 1955);
        }

        replaced = true;
    }

    void Bomb::UpdateModel()
    {
        
    }

    void Bomb::UpdateColors()
    {

    }

    void Bomb::Restore()
    {

    }
}