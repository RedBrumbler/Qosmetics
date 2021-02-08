#include "Types/Saber/Saber.hpp"
#include "Types/Saber/SaberItem.hpp"
#include "Types/Trail/QosmeticsTrail.hpp"

#include "GlobalNamespace/Saber.hpp"
#include "UnityEngine/Transform.hpp"
#include "Utils/UnityUtils.hpp"
#include "Utils/SaberUtils.hpp"
#include "Utils/TrailUtils.hpp"
#include "QosmeticsLogger.hpp"
#include "Config.hpp"

DEFINE_CLASS(Qosmetics::Saber);

using namespace UnityEngine;

#define INFO(value...) QosmeticsLogger::GetContextLogger("Saber").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("Saber").error(value)

extern config_t config;

namespace Qosmetics
{
    void Saber::Init(SaberManager* modelManager, ColorManager* colorManager)
    {
        this->modelManager = modelManager;
        this->colorManager = colorManager;
        std::function<void()> callback = std::bind( &Saber::UpdateColors, this );
        this->colorManager->RegisterCallback(callback, callbackType::saber);
        replaced = false;
    }
    
    void Saber::Awake()
    {
        GlobalNamespace::Saber* gameSaber = GetComponent<GlobalNamespace::Saber*>();
        if (gameSaber)
            saberType = gameSaber->get_saberType();
    }

    void Saber::UpdateModel()
    {
        if (modelManager && modelManager->get_type() != ItemType::saber) // if type is not saber, we must be using a default saber
        {
            Transform* basicSaberModel = get_transform()->Find(modelManager->get_basicSaberModelName());
            SaberUtils::SetSaberSize(basicSaberModel);
        }
        else if (modelManager)// we actually ARE using a custom saber
        {
            Il2CppString* saberName = (saberType == 0) ? modelManager->get_leftSaberName() : modelManager->get_rightSaberName();
            Transform* customSaber = get_transform()->Find(saberName);
            SaberUtils::SetSaberSize(customSaber);
        }

        // whether or not it's custom, reset the trails, config and stuff is automatically handled by the trails themselves
        Array<QosmeticsTrail*>* trails = GetComponentsInChildren<QosmeticsTrail*>();
        if (trails)
        {
            for (int i = 0; i < trails->Length(); i++)
            {
                QosmeticsTrail* trail = trails->values[i];
                if (trail) trail->Reset();
            }
        }
    }

    void Saber::Restore()
    {
        if (!replaced) return;
        Transform* basicSaberModel = get_transform()->Find(modelManager->get_basicSaberModelName());
        if (basicSaberModel) SaberUtils::HideObjects(basicSaberModel->get_gameObject(), false, false);
        Il2CppString* saberName = (saberType == 0) ? modelManager->get_leftSaberName() : modelManager->get_rightSaberName();
        Transform* saber = get_transform()->Find(saberName);
        if (saber) Object::Destroy(saber);
        replaced = false;
    }

    void Saber::Replace()
    {
        INFO("Replacing saber");

        if (!modelManager || modelManager->get_type() != ItemType::saber)
        {
            ERROR("Saber model manager was nullptr or itemtype was invalid");
            SetupTrails();
            UpdateModel();
            return;
        }
        
        Il2CppString* saberName = (saberType == 0) ? modelManager->get_leftSaberName() : modelManager->get_rightSaberName();
        GameObject* prefab = (saberType == 0) ? modelManager->get_leftSaber() : modelManager->get_rightSaber();
        
        prefab->set_name(saberName);
        if (!prefab) 
        {
            ERROR("Replacing model was nullptr!");
            return;
        }

        Transform* basicSaberModel = get_transform()->Find(modelManager->get_basicSaberModelName());
        if (basicSaberModel) SaberUtils::HideObjects(basicSaberModel->get_gameObject(), modelManager->get_item().get_config().get_enableFakeGlow());
        Transform* newSaber = get_transform()->Find(saberName);
        if (!newSaber) 
        {
            prefab->get_transform()->SetParent(get_transform());
        }
        else Object::Destroy(prefab);
        newSaber = get_transform()->Find(saberName);

        newSaber->get_gameObject()->set_name(saberName);
        newSaber->get_transform()->set_rotation(get_transform()->get_rotation());
        newSaber->get_transform()->set_position(get_transform()->get_position());

        SetupTrails();
        UpdateModel();
        INFO("Done Replacing!");
    }

    void Saber::SetupTrails()
    {
        Transform* basicSaberModel = get_transform()->Find(modelManager->get_basicSaberModelName());
        if (modelManager && modelManager->get_type() == ItemType::saber)
        {
            SaberItem& item = modelManager->get_item();
            SaberConfig& itemConfig = item.get_config();

            std::vector<TrailConfig>& trails = (saberType == 0) ? itemConfig.get_leftTrails() : itemConfig.get_rightTrails();
            Il2CppString* saberName = (saberType == 0) ? modelManager->get_leftSaberName() : modelManager->get_rightSaberName();
            Transform* customSaber = get_transform()->Find(saberName);
            if (trails.size() > 0 && config.saberConfig.trailType == TrailType::custom && customSaber)
            {
                for (auto& trail : trails)
                {
                    Il2CppString* trailPath = trail.get_name();
                    Transform* trailObj = customSaber->Find(trailPath);
                    if (!trailObj) continue;
                    QosmeticsTrail* trailComponent = UnityUtils::GetAddComponent<Qosmetics::QosmeticsTrail*>(trailObj->get_gameObject());
                    trailComponent->SetColorManager(colorManager);
                    trailComponent->SetTrailConfig(&trail);
                }
            }
            else if (config.saberConfig.trailType != TrailType::none && customSaber && basicSaberModel)// there were no trails, or base game was configured
            {
                QosmeticsTrail* trailComponent = UnityUtils::GetAddComponent<Qosmetics::QosmeticsTrail*>(customSaber->get_gameObject());
                trailComponent->SetColorManager(colorManager);
                trailComponent->InitFromDefault(basicSaberModel);
            }
        }
        else if (modelManager && config.saberConfig.trailType != TrailType::none)
        {
            QosmeticsTrail* trailComponent = UnityUtils::GetAddComponent<Qosmetics::QosmeticsTrail*>(basicSaberModel->get_gameObject());
            trailComponent->SetColorManager(colorManager);
            trailComponent->InitFromDefault(basicSaberModel);
        }
        TrailUtils::RemoveTrail(basicSaberModel);
    }

    void Saber::UpdateColors()
    {
        if (!colorManager) return;
        Color thisColor = colorManager->ColorForSaberType(saberType.value);
        Color otherColor = colorManager->ColorForSaberType(1 - saberType.value);
        Il2CppString* saberName = (saberType == 0) ? modelManager->get_leftSaberName() : modelManager->get_rightSaberName();

        SaberUtils::SetColors(get_transform()->Find(saberName)->get_gameObject(), thisColor, otherColor);
    }
}