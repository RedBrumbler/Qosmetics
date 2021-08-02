#include "Config.hpp"
#include "Types/Saber/Saber.hpp"
#include "Types/Saber/SaberItem.hpp"
#include "Types/Trail/QosmeticsTrail.hpp"
#include "Types/Trail/AltTrail.hpp"
#include "Types/Trail/TrailHelper.hpp"

#include "UnityEngine/MeshRenderer.hpp"
#include "GlobalNamespace/Saber.hpp"
#include "UnityEngine/Transform.hpp"
#include "Utils/UnityUtils.hpp"
#include "Utils/SaberUtils.hpp"
#include "Utils/TrailUtils.hpp"
#include "Utils/ChromaUtils.hpp"
#include "QosmeticsLogger.hpp"

#include "chroma/shared/SaberAPI.hpp"
#include "beatsaber-hook/shared/utils/typedefs-wrappers.hpp"


DEFINE_TYPE(Qosmetics, Saber);

using namespace UnityEngine;

#define INFO(value...) QosmeticsLogger::GetContextLogger("Saber").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("Saber").error(value)

#define LOG_COLOR(text, color) \
    INFO("color %s: %.2f, %.2f, %.2f", text, color.r, color.g, color.b)

extern config_t config;

void StaticUpdateColors(Qosmetics::Saber* self, int, GlobalNamespace::SaberModelController*, UnityEngine::Color)
{
    self->UpdateColors();
}

namespace Qosmetics
{
    void Saber::Init(SaberManager* modelManager, ColorManager* colorManager)
    {
        this->modelManager = modelManager;
        this->colorManager = colorManager;
        std::function<void()> callback = std::bind( &Saber::UpdateColors, this );
        this->colorManager->RegisterCallback(callback, callbackType::saber);
        
        GlobalNamespace::Saber* gameSaber = GetComponent<GlobalNamespace::Saber*>();
        if (gameSaber)
            saberType = gameSaber->get_saberType();

        modelController = GetComponentInChildren<GlobalNamespace::SaberModelController*>(true);
        if (modelManager->get_item().get_type() != ItemType::invalid)
        {
            ChromaUtils::setSaberColoredByChroma(modelController, false);
            ChromaUtils::registerSaberCallback({&Saber::UpdateChromaColors, this});
        }
        
        replaced = false;
        Replace();
    }
    
    void Saber::OnDestroy()
    {
        ChromaUtils::setSaberColoredByChroma(modelController, true);
    }

    void Saber::Awake()
    {
        modelController = GetComponentInChildren<GlobalNamespace::SaberModelController*>(true);
    }

    void Saber::UpdateModel(bool firstUpdate)
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
            UpdateColors();
        }

        if (!firstUpdate)
        {
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
    }

    void Saber::Restore()
    {
        if (!replaced) return;
        Transform* basicSaberModel = get_transform()->Find(modelManager->get_basicSaberModelName());
        if (basicSaberModel) SaberUtils::HideObjects(basicSaberModel->get_gameObject(), false, false);
        Il2CppString* saberName = (saberType == 0) ? modelManager->get_leftSaberName() : modelManager->get_rightSaberName();
        Transform* saber = get_transform()->Find(saberName);
        if (saber) saber->get_gameObject()->SetActive(false);
        replaced = false;
    }

    void Saber::Replace()
    {
        if (!modelManager || modelManager->get_type() != ItemType::saber)
        {
            ERROR("Saber model manager was nullptr or itemtype was invalid");
            SetupTrails();
            UpdateModel();
            return;
        }
        
        Il2CppString* saberName = (saberType == 0) ? modelManager->get_leftSaberName() : modelManager->get_rightSaberName();
        Transform* prefab = get_transform()->Find(saberName);
        if (!prefab)
        {
            Transform* prefab = (saberType == 0) ? modelManager->get_leftSaber() : modelManager->get_rightSaber();        
            if (!prefab) 
            {
                ERROR("Replacing model was nullptr!");
                return;
            }
            prefab->SetParent(get_transform());

            prefab->get_gameObject()->set_name(saberName);
            prefab->set_rotation(get_transform()->get_rotation());
            prefab->set_position(get_transform()->get_position());
        }
        else prefab->get_gameObject()->SetActive(true);

        Transform* basicSaberModel = get_transform()->Find(modelManager->get_basicSaberModelName());
        if (basicSaberModel) SaberUtils::HideObjects(basicSaberModel->get_gameObject(), modelManager->get_item().get_config().get_enableFakeGlow());
        
        SetupTrails();
        UpdateModel(true);
    }

    void Saber::SetupTrails()
    {
        if (!modelManager) return;
        Transform* basicSaberModel = get_transform()->Find(modelManager->get_basicSaberModelName());
        if (modelManager->get_type() == ItemType::saber)
        {
            SaberItem& item = modelManager->get_item();
            SaberConfig& itemConfig = item.get_config();

            std::vector<TrailConfig>& trails = (saberType == 0) ? itemConfig.get_leftTrails() : itemConfig.get_rightTrails();
            Il2CppString* saberName = (saberType == 0) ? modelManager->get_leftSaberName() : modelManager->get_rightSaberName();
            Transform* customSaber = get_transform()->Find(saberName);
            if (trails.size() > 0 && config.saberConfig.trailType == TrailType::custom && customSaber)
            {
                INFO("Putting custom trails on custom saber");
                for (auto& trail : trails)
                {
                    Il2CppString* trailPath = trail.get_name();
                    Transform* trailObj = customSaber->Find(trailPath);
                    if (!trailObj) continue;
                    
                    auto helper = UnityUtils::GetAddComponent<Qosmetics::TrailHelper*>(trailObj->get_gameObject());
                    helper->set_trailConfig(trail);
                    helper->Init(colorManager, modelController);
                    helper->TrailSetup();
                }
            }
            else if (config.saberConfig.trailType != TrailType::none && customSaber && basicSaberModel)// there were no trails, or base game was configured
            {
                INFO("Putting base game trails on custom saber");
                GlobalNamespace::SaberTrail* orig = basicSaberModel->GetComponent<GlobalNamespace::SaberTrail*>();
		        UnityEngine::Material* mat = orig->trailRendererPrefab->meshRenderer->get_material();
                
                customSaber->get_gameObject()->AddComponent<MeshRenderer*>()->set_material(mat);
                auto helper = UnityUtils::GetAddComponent<Qosmetics::TrailHelper*>(customSaber->get_gameObject());
                static Color white = {1.0f, 1.0f, 1.0f, 1.0f};
                TrailConfig trail(saberType.value, orig->color, white, 20, 4);

                helper->set_trailConfig(trail);
                helper->Init(colorManager, modelController);
                helper->TrailSetup();
                
                /*
                QosmeticsTrail* trailComponent = UnityUtils::GetAddComponent<Qosmetics::QosmeticsTrail*>(customSaber->get_gameObject());
                trailComponent->attachedSaberModelController = modelController;
                trailComponent->SetColorManager(colorManager);
                trailComponent->InitFromDefault(basicSaberModel);
                */
            }
        }
        else if (config.saberConfig.trailType != TrailType::none && basicSaberModel)
        {
            INFO("Changing base game trails on base game saber");
            GlobalNamespace::SaberTrail* orig = basicSaberModel->GetComponent<GlobalNamespace::SaberTrail*>();
		    UnityEngine::Material* mat = orig->trailRendererPrefab->meshRenderer->get_material();
            
            basicSaberModel->get_gameObject()->AddComponent<MeshRenderer*>()->set_material(mat);
            auto helper = UnityUtils::GetAddComponent<Qosmetics::TrailHelper*>(basicSaberModel->get_gameObject());
            static Color white = {1.0f, 1.0f, 1.0f, 1.0f};
            TrailConfig trail(saberType.value, orig->color, white, 20, 4);

            helper->set_trailConfig(trail);
            helper->Init(colorManager, modelController);
            helper->TrailSetup();
            /*
            QosmeticsTrail* trailComponent = UnityUtils::GetAddComponent<Qosmetics::QosmeticsTrail*>(basicSaberModel->get_gameObject());
            trailComponent->attachedSaberModelController = modelController;
            trailComponent->SetColorManager(colorManager);
            trailComponent->InitFromDefault(basicSaberModel);
            */
        }
        TrailUtils::RemoveTrail(basicSaberModel);
    }

    void Saber::UpdateColors()
    {
        if (!colorManager) return;

        auto sabersColorOptional = Chroma::SaberAPI::getGlobalSabersColorSafe();
        
        Color thisColor;
        Color otherColor;
        
        switch (saberType.value)
        {   
            // left saber
            case 0:
                thisColor = (sabersColorOptional.first != std::nullopt) ? *(sabersColorOptional.first) : colorManager->ColorForSaberType(saberType.value);
                otherColor = (sabersColorOptional.second != std::nullopt) ? *(sabersColorOptional.second) : colorManager->ColorForSaberType(1 - saberType.value);
                break;
            // right saber
            case 1:
                thisColor = (sabersColorOptional.second != std::nullopt) ? *(sabersColorOptional.second) : colorManager->ColorForSaberType(saberType.value);
                otherColor = (sabersColorOptional.first != std::nullopt) ? *(sabersColorOptional.first) : colorManager->ColorForSaberType(1 - saberType.value);
                break;
        }

        INFO(sabersColorOptional.first ? "using chroma color" : "using normal color");

        Il2CppString* saberName = (saberType.value == 0) ? modelManager->get_leftSaberName() : modelManager->get_rightSaberName();

        SaberUtils::SetColors(get_transform()->Find(saberName)->get_gameObject(), thisColor, otherColor);
    }

    void Saber::UpdateChromaColors(int saberTypePassed, GlobalNamespace::SaberModelController* modelController, UnityEngine::Color)
    {
        if(modelController->Equals(this->modelController)) UpdateColors(); 
    }

}