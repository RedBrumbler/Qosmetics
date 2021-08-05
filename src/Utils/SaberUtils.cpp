#include "Utils/SaberUtils.hpp"
#include "UnityEngine/MeshFilter.hpp"
#include "Utils/MaterialUtils.hpp"
#include "Utils/UnityUtils.hpp"
#include "Config.hpp"

#include "Containers/SingletonContainer.hpp"
#include "Types/Saber/Saber.hpp"
#include "Types/Saber/SaberManager.hpp"
#include "Types/Trail/TrailHelper.hpp"
#include "Types/Trail/AltTrail.hpp"
#include "Types/Colors/ColorComponent.hpp"
#include "GlobalNamespace/SaberTrail.hpp"
#include "GlobalNamespace/SaberTrailRenderer.hpp"

#include "QosmeticsLogger.hpp"

#define INFO(value...) QosmeticsLogger::GetContextLogger("TrailHelper").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("TrailHelper").error(value)




using namespace UnityEngine;

extern config_t config;

static void DisableMesh(Array<MeshFilter*>* filters, bool enableFakeGlow, bool doHide)
{
    if (!filters) return;

    for(int i = 0; i < filters->Length(); i++)
    {
        MeshFilter* filter = filters->values[i];
        
        GameObject* filterGO = nullptr;
        
        if(filter)
            filterGO = filter->get_gameObject();
        
        if(filterGO)
        {
            std::string name = to_utf8(csstrtostr(filterGO->get_name()));
            if (enableFakeGlow && (name == "FakeGlow0" || name == "FakeGlow1")) continue;
            filterGO->SetActive(!doHide);
        }
    }
}

namespace SaberUtils
{
    void HideObjects(GameObject* object, bool enableFakeGlow, bool doHide)
    {
        Array<MeshFilter*>* filters = object->GetComponentsInChildren<MeshFilter*>(true);
        DisableMesh(filters, enableFakeGlow, doHide);
    }

    void SetColors(GameObject* object, Color color, Color otherColor)
    {
        MaterialUtils::SetColors(object, color, otherColor);
    }

    void SetSaberSize(Transform* saber)
    {
        if (!saber) return;
        saber->set_localScale(Vector3(config.saberConfig.saberWidth, config.saberConfig.saberWidth, 1.0f));
    }

    UnityEngine::Transform* MakeDummySaber(int saberType)
    {
        using namespace Qosmetics;

        if (saberType != 0 && saberType != 1) return nullptr;

        Qosmetics::SaberManager* modelManager = Qosmetics::SingletonContainer::get_saberManager();
        if (!modelManager || modelManager->get_type() != Qosmetics::ItemType::saber)
        {
            ERROR("Saber Model Manager was not found, or item was not saber (default)");
            return nullptr;
        }

        UnityEngine::Transform* customSaber = saberType == 0 ? modelManager->get_leftSaber() : modelManager->get_rightSaber();
        if (!customSaber) return nullptr;
        
        Il2CppString* saberName = saberType == 0 ? modelManager->get_leftSaberName() : modelManager->get_rightSaberName();
        customSaber->get_gameObject()->set_name(saberName);

        Qosmetics::SaberItem& item = modelManager->get_item();
        Qosmetics::SaberConfig& itemConfig = item.get_config();

        auto colorManager = Qosmetics::SingletonContainer::get_colorManager();

        // if custom trail is selected, and custom trails on saber
        if (config.saberConfig.trailType == TrailType::custom && itemConfig.get_hasCustomTrails())
        {
            std::vector<TrailConfig>& trails = (saberType == 0) ? itemConfig.get_leftTrails() : itemConfig.get_rightTrails();
            
            INFO("Putting custom trails on custom saber");
            for (auto& trail : trails)
            {
                Il2CppString* trailPath = trail.get_name();
                Transform* trailObj = customSaber->Find(trailPath);
                if (!trailObj) continue;
                
                auto helper = UnityUtils::GetAddComponent<Qosmetics::TrailHelper*>(trailObj->get_gameObject());
                helper->set_trailConfig(trail);
                helper->Init(colorManager, nullptr);
                helper->TrailSetup();
            }
        }
        // if trail type was not none
        else if (config.saberConfig.trailType != TrailType::none)
        {
            // get original saber for copying over orig trail stuff
            Array<Qosmetics::Saber*>* sabers = Resources::FindObjectsOfTypeAll<Qosmetics::Saber*>();
            Qosmetics::Saber* saber = nullptr;
    
            int length = sabers->Length();
    
            for (int i = 0; i < length; i++)
            {
                auto currentSaber = sabers->values[i];
                if (currentSaber->saberType.value == saberType) 
                {
                    saber = currentSaber;
                    break;
                }
            }

            // if we found an original to copy
            if (saber)
            {
                auto origHelper = saber->GetComponent<Qosmetics::TrailHelper*>();
                Transform* basicSaberModel = saber->get_transform()->Find(modelManager->get_basicSaberModelName());
                if (origHelper)
                {
                    GlobalNamespace::SaberTrail* orig = basicSaberModel->GetComponent<GlobalNamespace::SaberTrail*>();
		            UnityEngine::Material* mat = orig->trailRendererPrefab->meshRenderer->get_material();

                    // setup material reference
                    customSaber->get_gameObject()->AddComponent<MeshRenderer*>()->set_material(mat);

                    TrailConfig trail(origHelper->colorType, origHelper->color, origHelper->multiplier, origHelper->length, origHelper->whiteStep * origHelper->length);
                    auto helper = UnityUtils::GetAddComponent<Qosmetics::TrailHelper*>(customSaber->get_gameObject());
                    helper->set_trailConfig(trail);
                    helper->Init(colorManager, nullptr);
                    helper->TrailSetup();
                }
            }
        }   

        auto colorComponent = customSaber->get_gameObject()->AddComponent<Qosmetics::ColorComponent*>();
        colorComponent->Init(colorManager, 0, saberType);
        colorComponent->UpdateSaberColors();

        return customSaber;
    }
}