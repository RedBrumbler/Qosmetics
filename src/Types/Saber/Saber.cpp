#include "Types/Saber/Saber.hpp"
#include "Types/Saber/SaberItem.hpp"
#include "GlobalNamespace/Saber.hpp"
#include "UnityEngine/Transform.hpp"
#include "Utils/UnityUtils.hpp"
#include "QosmeticsLogger.hpp"

DEFINE_CLASS(Qosmetics::Saber);

using namespace UnityEngine;

static Il2CppString* basicSaberModelName = nullptr;

#define INFO(value...) QosmeticsLogger::GetContextLogger("Saber").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("Saber").error(value)

namespace Qosmetics
{
    void Saber::Init(SaberManager* modelManager)
    {
        this->modelManager = modelManager;
    }
    
    void Saber::Awake()
    {
        GlobalNamespace::Saber* gameSaber = GetComponent<GlobalNamespace::Saber*>();
        if (gameSaber)
            saberType = gameSaber->get_saberType();
    }

    void Saber::UpdateModel()
    {
        
    }

    void Saber::Restore()
    {

    }

    void Saber::Replace()
    {
        INFO("Replacing saber");
        if (!modelManager) return;
        GlobalNamespace::Saber* gameSaber = GetComponent<GlobalNamespace::Saber*>();

        Il2CppString* saberName = (saberType == 0) ? modelManager->get_leftSaberName() : modelManager->get_rightSaberName();
        Transform* prefab = (saberType == 0) ? modelManager->get_leftSaber() : modelManager->get_rightSaber();
        if (!prefab) return;

        if (!basicSaberModelName) basicSaberModelName = il2cpp_utils::createcsstr("BasicSaberModel(Clone)", il2cpp_utils::StringType::Manual);
        Transform* basicSaberModel = get_transform()->Find(basicSaberModelName);

        //if (basicSaberModel) UnityUtils::HideRenderersOnObject(basicSaberModel->get_gameObject());
        Transform* newSaber = get_transform()->Find(saberName);
        if (!newSaber) prefab->SetParent(get_transform());
        newSaber = get_transform()->Find(saberName);

        newSaber->get_gameObject()->set_name(saberName);
        newSaber->get_transform()->set_rotation(get_transform()->get_rotation());
        newSaber->get_transform()->set_position(get_transform()->get_position());
        INFO("Done Replacing!");
    }
}