#include "Utils/TrailUtils.hpp"

namespace Qosmetics
{
    void TrailUtils::ResetTrails()
    {
        Array<Xft::XWeaponTrail*>* trails = UnityEngine::Resources::FindObjectsOfTypeAll<Xft::XWeaponTrail*>();
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

        /*basicSaberModel->SetParent(customSaber);
        basicSaberModel = customSaber->Find(il2cpp_utils::createcsstr("BasicSaberModel(Clone)"));

        return;*/

        getLogger().info("Attempting to move base game trail into the custom saber per request of michaelzoller");
        Xft::XWeaponTrail* trailComponent = UnityUtils::GetComponent<Xft::XWeaponTrail*>(basicSaberModel->get_gameObject(), "Xft", "XWeaponTrail");

        if (trailComponent == nullptr) 
        {
            getLogger().error("Tried moving trail but trailcomponent was nullptr, skipping...");
            return;
        }
                
        // add new trail script into which stuff will be moved
        UnityEngine::GameObject* saberGO = customSaber->get_gameObject();
        if (saberGO == nullptr) return;
        Xft::XWeaponTrail* newTrail = *il2cpp_utils::RunMethod<Xft::XWeaponTrail*>(saberGO, "AddComponent", UnityUtils::TypeFromString("Xft", "XWeaponTrail"));

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

        for (auto child : children)
        {
            if (child != nullptr) child->SetParent(customSaber);
            else getLogger().error("A child was nullptr, not setting parent...");
        }

        // copy over properties
        UnityEngine::Color trailColor = trailComponent->get_color();
        UnityEngine::Transform* start = trailComponent->pointStart;
        UnityEngine::Transform* end = trailComponent-> pointEnd;
        auto* trailPrefab = trailComponent->trailRendererPrefab;

        if (start == nullptr)
        {
            getLogger().error("start was nullptr, returning...");
            return;
        }
        else if (end == nullptr)
        {
            getLogger().error("start was nullptr, returning...");
            return;
        }
        else if (trailPrefab == nullptr)
        {
            getLogger().error("start was nullptr, returning...");
            return;
        }

        newTrail->set_color(trailColor);
        newTrail->pointStart = start;
        newTrail->pointEnd = end;
        newTrail->trailRendererPrefab = trailPrefab;
        // yeet original trail
        RemoveTrail(basicSaberModel);
    }

    void TrailUtils::AddTrail(Qosmetics::CustomTrail &trail, UnityEngine::Transform* customSaber)
    {
        // find the trail object
        UnityEngine::GameObject* trailGO = customSaber->Find(il2cpp_utils::createcsstr(trail.get_objectPath()))->get_gameObject();
        
        // add weapon trail component to it
        Xft::XWeaponTrail* newTrail = *il2cpp_utils::RunMethod<Xft::XWeaponTrail*>(trailGO, "AddComponent", UnityUtils::TypeFromString("Xft", "XWeaponTrail"));
        
        if (newTrail == nullptr)
        {
            getLogger().error("Adding new trail component failed, skipping add trail...");
            return;
        }

        // set the trail info on that sucker
        Qosmetics::CustomTrail::set_trailInfo(newTrail, trail);
    }

    void TrailUtils::RemoveTrail(UnityEngine::Transform* basicSaberModel)
    {
        if (basicSaberModel == nullptr)
        {
            getLogger().error("basicSaberModel was null, not removing trail");
        }
        Xft::XWeaponTrail* trailComponent = UnityUtils::GetComponent<Xft::XWeaponTrail*>(basicSaberModel->get_gameObject(), "Xft", "XWeaponTrail");
        
        if (trailComponent == nullptr)
        {
            getLogger().error("Finding weapontrail failed, skipping remove trail...");
            return;
        }

        // disabled in this case is just trail width to 0
        trailComponent->trailWidth = 0.0;
    }

}