#include "Utils/TrailUtils.hpp"
#include "Data/QosmeticsTrail.hpp"

namespace Qosmetics
{
    void TrailUtils::ResetTrails()
    {
        Array<GlobalNamespace::SaberTrail*>* trails = UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::SaberTrail*>();
        for(int i = 0; i < trails->Length(); i++)
        {
            CRASH_UNLESS(il2cpp_utils::SetFieldValue(trails->values[i], "_trailWidth", 0.0f)); 
        }
    }

    void TrailUtils::MoveTrail(UnityEngine::Transform* basicSaberModel, UnityEngine::Transform* customSaber)
    {
        if (customSaber == nullptr) 
        {
            getLogger().error("customSaber was null, not moving trail...");
            return;
        }
        if (basicSaberModel == nullptr)
        {
            getLogger().error("basicSaberModel was null, not moving trail...");
            return;
        }

        getLogger().info("Attempting to move base game trail into the custom saber per request of michaelzoller");
        GlobalNamespace::SaberTrail* trailComponent = basicSaberModel->get_gameObject()->GetComponent<GlobalNamespace::SaberTrail*>();

        UnityEngine::Transform* trailStart = UnityEngine::Object::Instantiate(UnityEngine::GameObject::New_ctor(), customSaber)->get_transform();
        trailStart->get_gameObject()->set_name(il2cpp_utils::createcsstr("TrailStart"));
        trailStart->set_localPosition(UnityEngine::Vector3::get_zero());

        UnityEngine::Transform* trailEnd = UnityEngine::Object::Instantiate(UnityEngine::GameObject::New_ctor(), customSaber)->get_transform();
        trailEnd->get_gameObject()->set_name(il2cpp_utils::createcsstr("TrailEnd"));
        trailEnd->set_localPosition(UnityEngine::Vector3(0.0f, 0.0f, 1.0f));

        if (trailComponent == nullptr) 
        {
            getLogger().error("Tried moving trail but trailcomponent was nullptr, skipping...");
            return;
        }
        else
        {
            getLogger().info("Found trail Component!");
        }
                
        // add new trail script into which stuff will be moved
        UnityEngine::GameObject* saberGO = customSaber->get_gameObject();
        if (saberGO == nullptr) return;
        QosmeticsTrail* newTrail = saberGO->AddComponent<QosmeticsTrail*>()->CopyFromBase(trailComponent);

        if (newTrail == nullptr)
        {
            getLogger().error("Adding new trail component failed, skipping move trail...");
            return;
        }
        
        // relocate children from basicsabermodel into LeftSaber.Leftsaber (or right)
        std::vector<UnityEngine::Transform*> children;
        int childCount = basicSaberModel->get_childCount();
        for (int i = 0; i < childCount; i++)
        {
            UnityEngine::Transform* child = basicSaberModel->GetChild(i);
            children.push_back(child);
        }
        
        for (UnityEngine::Transform* child : children)
        {
            if (child != nullptr)
            {
                std::string name = to_utf8(csstrtostr(child->get_gameObject()->get_name()));
                //getLogger().info("%s pointer address: %p", name.c_str(), child);
                child->SetParent(customSaber);
            } 
            else getLogger().error("A child was nullptr, not setting parent...");
        }
        
        // yeet original trail
        RemoveTrail(basicSaberModel);
    }

    void TrailUtils::AddTrail(Qosmetics::CustomTrail &trail, UnityEngine::Transform* customSaber)
    {
        if (customSaber == nullptr)
        {
            getLogger().error("Custom Saber nullptr, not adding trail");
            return;
        }
        // find the trail object
        UnityEngine::Transform* trailTransform = customSaber->Find(il2cpp_utils::createcsstr(trail.get_objectPath()));
        
        // add weapon trail component to it
        QosmeticsTrail::AddTrailAndSetup(trailTransform, trail);
    }

    void TrailUtils::RemoveTrail(UnityEngine::Transform* basicSaberModel)
    {
        getLogger().info("RemoveTrail");
        if (basicSaberModel == nullptr)
        {
            getLogger().error("basicSaberModel was null, not removing trail");
            return;
        }

        GlobalNamespace::SaberTrail* trailComponent = basicSaberModel->get_gameObject()->GetComponent<GlobalNamespace::SaberTrail*>();
        
        if (trailComponent == nullptr)
        {
            getLogger().error("Finding weapontrail failed, skipping remove trail...");
            return;
        }
        else
        {
            getLogger().info("Removing original trail");
        }
        
        // disabled component
        if (trailComponent->trailRenderer == nullptr)
        {
            getLogger().error("trailRenderer was nullptr, couldn't disable it");
            return;
        }
        trailComponent->set_enabled(false);
        //trailComponent->trailRenderer->trailDuration = 0.0f;
        //trailComponent->trailRenderer->whiteSectionMaxDuration = 0.0f;
    }

}