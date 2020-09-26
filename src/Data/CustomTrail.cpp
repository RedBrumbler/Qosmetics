#include "Data/CustomTrail.hpp"

namespace Qosmetics
{
    void CustomTrail::FindMaterial(UnityEngine::Transform* saber)
    {
        // get the trail object from the given transform, which is done by getting the GO at the objectpath
        UnityEngine::Transform* trailObject = saber->Find(il2cpp_utils::createcsstr(get_objectPath()));
        
        // if not found we can't take a material off, so return
        if (trailObject == nullptr) 
        {
            getLogger().error("Trail object was not found, skipping FindMaterial...");
            return;
        }

        // find the renderer on the trail object to get the material from
        UnityEngine::MeshRenderer* renderer = UnityUtils::GetComponent<UnityEngine::MeshRenderer*>(trailObject->get_gameObject(), "UnityEngine", "Renderer");

        // if the renderer is not found then we can't get a material so just return
        if (renderer == nullptr)
        {
            getLogger().error("trail object renderer was not found, skipping FindMaterial...");
            return;
        }       
        // set the locally stored material pointer
        set_material(renderer->get_sharedMaterial());
    }

    UnityEngine::Color CustomTrail::ColorMultiply(UnityEngine::Color first, UnityEngine::Color second)
    {
        UnityEngine::Color result;

        result.r = first.r * second.r;
        result.g = first.g * second.g;
        result.b = first.b * second.b;
        result.a = first.a * second.a;

        return result;
    }    

    void CustomTrail::set_trailInfo(Xft::XWeaponTrail* weaponTrail, Qosmetics::CustomTrail &trail)
    {
        UnityEngine::Transform* trailTransform = weaponTrail->get_transform();

        UnityEngine::Transform* begin = trailTransform->get_transform()->Find(il2cpp_utils::createcsstr("TrailStart"));
        UnityEngine::Transform* end = trailTransform->get_transform()->Find(il2cpp_utils::createcsstr("TrailEnd"));
                
        // if either begin or end are nullptr, it's no use setting more info on the trail so just return
        if (begin == nullptr || end == nullptr)
        {
            getLogger().error("Could not find top and bottom for the trail, skipping rest of setInfo..");
            return;
        }

        // setting the begin and end transforms for the trail (located inside the trail gameObject)
        weaponTrail->pointStart = begin;
        weaponTrail->pointEnd = end;

        // set the length of the trail from the trail config
        weaponTrail->maxFrame = trail.get_length();
        weaponTrail->granularity *= (trail.get_length() > 10) ? ((float)trail.get_length() / 10.0f) : 1;
        // set the whitestep of the trail from the trail config
        weaponTrail->whiteSteps = trail.get_whiteStep();

        // see if the material on the trail is nullptr, it should not be but if it is, find the material again
        if (trail.get_material() == nullptr)
        {
            // gets leftsaber or rightsaber transform
            UnityEngine::Transform* saberTransform = trailTransform;
            std::string parentName = "";
            do
            {
                saberTransform = saberTransform->get_parent();
                parentName = to_utf8(csstrtostr(saberTransform->get_gameObject()->get_name()));
            }while(!(parentName == "LeftSaber" || parentName == "RightSaber"));

            // findmaterial runs on the leftsaber or rightsaber transform and thus it's needed
            trail.FindMaterial(saberTransform);
        }

        // make a new prefab from scratch that gets the material set ahead of time just to be sure
        weaponTrail->trailRendererPrefab = trail.NewTrailRenderer();

        if (weaponTrail->trailRendererPrefab == nullptr)
        {
            getLogger().error("Failed to make a new trail renderer, skipping rest of set trail");
            return;
        }

        // get matswitcher and if not null delete it
        GlobalNamespace::ConditionalMaterialSwitcher* matSwitcher = UnityUtils::GetComponent<GlobalNamespace::ConditionalMaterialSwitcher*>(weaponTrail->trailRendererPrefab->get_gameObject(), "", "ConditionalMaterialSwitcher");
        if (matSwitcher != nullptr) UnityEngine::Object::Destroy(matSwitcher);
                
        // calling start so that the trailrenderer gets instanced.
        //weaponTrail->Start();

        // make a color instance to use later on, also get color manager to get the right colors for the sabers
        UnityEngine::Color newColor = UnityEngine::Color::get_white();
        GlobalNamespace::ColorManager* colorManager = UnityUtils::GetLastObjectOfType<GlobalNamespace::ColorManager*>(il2cpp_utils::GetClassFromName("", "ColorManager"));
                
        if (colorManager != nullptr && (int)trail.get_type() == 0)
            // if type is left saber (0) and the color manager is defined, get the color from A saber, and multiply by multiplier color
            newColor = ColorMultiply(colorManager->ColorForSaberType(GlobalNamespace::SaberType::SaberA), trail.get_multiplierColor());
        else if (colorManager != nullptr && (int)trail.get_type() == 1)
            // else if type is right saber (1) and the color manager is defined, get the color from B saber, and multiply by multiplier color
            newColor = ColorMultiply(colorManager->ColorForSaberType(GlobalNamespace::SaberType::SaberB), trail.get_multiplierColor());
        else 
            // last resort must mean the color type is custom color (2), or the color manager was not defined. then just take the configured color that's set on the trail
            newColor = trail.get_color();
        
        // apply trail intensity player setting
        newColor.a *= trailIntensity;

        // setting color
        weaponTrail->set_color(newColor);
        getLogger().info("end of trail set info");
    }

    GlobalNamespace::XWeaponTrailRenderer* CustomTrail::NewTrailRenderer()
    {
        // make a new gameobject to house the prefab on
        UnityEngine::GameObject* newPrefab = *il2cpp_utils::RunMethod<UnityEngine::GameObject*>("UnityEngine", "Object", "Instantiate", UnityEngine::GameObject::New_ctor());
        
        if (newPrefab == nullptr)
        {
            getLogger().error("Failed to instantiate a new gameObject for trail renderer");
            return nullptr;
        }

        // Trail renderer script holds reference to meshfilter and meshrenderer used to render the trial
        CRASH_UNLESS(il2cpp_utils::RunMethod<UnityEngine::MeshFilter*>(newPrefab, "AddComponent", UnityUtils::TypeFromString("MeshFilter")));
        CRASH_UNLESS(il2cpp_utils::RunMethod<UnityEngine::MeshRenderer*>(newPrefab, "AddComponent", UnityUtils::TypeFromString("MeshRenderer")));
        GlobalNamespace::XWeaponTrailRenderer* trailRendererPrefab = CRASH_UNLESS(il2cpp_utils::RunMethod<GlobalNamespace::XWeaponTrailRenderer*>(newPrefab, "AddComponent", UnityUtils::TypeFromString("", "XWeaponTrailRenderer")));

        // if the material is defined, set it immediately since this is not a static method :)
        if (this->trailMaterial != nullptr) trailRendererPrefab->meshRenderer->set_sharedMaterial(this->trailMaterial);

        // give it a good name, for identification I guess
        newPrefab->set_name(il2cpp_utils::createcsstr("Trail"));

        // return the trail renderer pointer
        return trailRendererPrefab;
    }

    GlobalNamespace::XWeaponTrailRenderer* CustomTrail::NewTrailRenderer(UnityEngine::Material* material)
    {
        // make a new gameobject to house the prefab on
        UnityEngine::GameObject* newPrefab = *il2cpp_utils::RunMethod<UnityEngine::GameObject*>("UnityEngine", "Object", "Instantiate", UnityEngine::GameObject::New_ctor());
        
        if (newPrefab == nullptr)
        {
            getLogger().error("Failed to instantiate a new gameObject for trail renderer");
            return nullptr;
        }

        // Trail renderer script holds reference to meshfilter and meshrenderer used to render the trial
        CRASH_UNLESS(il2cpp_utils::RunMethod<UnityEngine::MeshFilter*>(newPrefab, "AddComponent", UnityUtils::TypeFromString("MeshFilter")));
        CRASH_UNLESS(il2cpp_utils::RunMethod<UnityEngine::MeshRenderer*>(newPrefab, "AddComponent", UnityUtils::TypeFromString("MeshRenderer")));
        GlobalNamespace::XWeaponTrailRenderer* trailRendererPrefab = CRASH_UNLESS(il2cpp_utils::RunMethod<GlobalNamespace::XWeaponTrailRenderer*>(newPrefab, "AddComponent", UnityUtils::TypeFromString("", "XWeaponTrailRenderer")));

        // if the material is not nullptr set it
        if (material != nullptr) trailRendererPrefab->meshRenderer->set_sharedMaterial(material);      

        // give it a good name, for identification I guess
        newPrefab->set_name(il2cpp_utils::createcsstr("Trail"));

        // return the trail renderer pointer
        return trailRendererPrefab;
    }
}
