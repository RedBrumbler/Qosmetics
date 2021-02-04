#include "Types/Saber/Saber.hpp"
#include "Types/Saber/SaberItem.hpp"
#include "GlobalNamespace/Saber.hpp"
#include "UnityEngine/Transform.hpp"
#include "Utils/UnityUtils.hpp"
#include "Utils/SaberUtils.hpp"
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
        if (!modelManager || modelManager->get_type() != ItemType::saber)
        {
            ERROR("Saber model manager was nullptr or itemtype was invalid");
            return;
        }
        
        Il2CppString* saberName = (saberType == 0) ? modelManager->get_leftSaberName() : modelManager->get_rightSaberName();
        GameObject* prefab = (saberType == 0) ? modelManager->get_leftSaber() : modelManager->get_rightSaber();
        if (!prefab) 
        {
            ERROR("Replacing model was nullptr!");
            return;
        }
        prefab->set_name(saberName);
        if (!basicSaberModelName) basicSaberModelName = il2cpp_utils::createcsstr("BasicSaberModel(Clone)", il2cpp_utils::StringType::Manual);
        Transform* basicSaberModel = get_transform()->Find(basicSaberModelName);
        if (basicSaberModel) SaberUtils::HideObjects(basicSaberModel->get_gameObject(), modelManager->get_item().get_config().get_enableFakeGlow());
        //if (basicSaberModel) UnityUtils::HideRenderersOnObject(basicSaberModel->get_gameObject());
        Transform* newSaber = get_transform()->Find(saberName);
        if (!newSaber) prefab->get_transform()->SetParent(get_transform());
        newSaber = get_transform()->Find(saberName);

        newSaber->get_gameObject()->set_name(saberName);
        newSaber->get_transform()->set_rotation(get_transform()->get_rotation());
        newSaber->get_transform()->set_position(get_transform()->get_position());
        INFO("Done Replacing!");
    }
}