#include "Data/QosmeticsTrail.hpp"
#include "Data/CustomTrail.hpp"
#include "GlobalNamespace/TimeHelper.hpp"
#include "UnityEngine/Mathf.hpp"
#include "GlobalNamespace/TrailElementCollection.hpp" 
#include "GlobalNamespace/TrailElement.hpp"
#include "GlobalNamespace/ColorManager.hpp"
#include "GlobalNamespace/SaberType.hpp"
#include "Utils/UnityUtils.hpp"

DEFINE_CLASS(Qosmetics::QosmeticsTrail);
float Qosmetics::QosmeticsTrail::trailIntensity = 1.0f;

namespace Qosmetics
{
	void QosmeticsTrail::AddTrailAndSetup(UnityEngine::Transform* trailObject, Qosmetics::CustomTrail &trail)
	{
		QosmeticsTrail* customTrail = trailObject->get_gameObject()->GetComponent<QosmeticsTrail*>();
		if (customTrail == nullptr) customTrail = trailObject->get_gameObject()->AddComponent<QosmeticsTrail*>();
		float granularity = 60 * ((trail.get_length() > 10) ? ((float)trail.get_length() / 10.0f) : 1);
        customTrail->TrailSetup(trail.get_length(), granularity, trail.get_type(), trail.get_whiteStep(), trail.get_material(), trail.get_trailColor(), trail.get_multiplierColor());
	}

	void QosmeticsTrail::TrailSetup(int length, int granularity, int colorType, int whiteStep, UnityEngine::Material* trailMaterial, UnityEngine::Color trailColor, UnityEngine::Color multiplierColor)
	{
		// sets all the different values
		this->length = length;
		this->granularity = granularity;
		this->colorType = colorType;
		this->whitestep = whiteStep;
		this->trailMaterial = trailMaterial;
		this->trailColor = trailColor;
		this->multiplierColor = multiplierColor;
	}

    void QosmeticsTrail::Awake()
    {
		// get the material to use for the trail renderer
		if (this->trailMaterial == nullptr) this->trailMaterial = this->get_gameObject()->GetComponent<UnityEngine::MeshRenderer*>()->get_material();

		// make a new trail renderer to use
        this->trailRenderer = NewTrailRenderer(this->trailMaterial);
		// create trail duration from length / sampling
        this->trailDuration = (float)this->length / (float)this->samplingFrequency;

		// make a new movementdata
		this->customMovementData = GlobalNamespace::SaberMovementData::New_ctor();
		// set the movementData interface to this new movementData
		this->movementData = reinterpret_cast<GlobalNamespace::IBladeMovementData*>(this->customMovementData);

		// if either is nullptr, set it
		if (this->topTransform == nullptr)
		{
			this->topTransform = this->get_transform()->Find(il2cpp_utils::createcsstr("TrailEnd"));
		}
		if (this->bottomTransform == nullptr)
		{
			this->bottomTransform = this->get_transform()->Find(il2cpp_utils::createcsstr("TrailStart"));
		}

		// get color set
		GlobalNamespace::ColorManager* colorManager = UnityUtils::GetLastObjectOfType<GlobalNamespace::ColorManager*>(il2cpp_utils::GetClassFromName("", "ColorManager"));
        if (colorManager != nullptr) // if colormanager defined, get colors from it, else just get the set color
		{
			switch (this->colorType)
			{
				case 0: // LeftSaber
					this->color = colorManager->ColorForSaberType(GlobalNamespace::SaberType::SaberA) * this->multiplierColor;
					break;
				case 1:	// RightSaber
					this->color = colorManager->ColorForSaberType(GlobalNamespace::SaberType::SaberB) * this->multiplierColor;
					break;
				case 2:	// Custom Color
					this->color = this->trailColor * this->multiplierColor;
					break;
			}
		}
		else this->color = this->trailColor * this->multiplierColor;
		
		// apply trailIntensity with a *= and not a = because it should not overwrite the set color.a, just modify it
		this->color.a *= trailIntensity;

		// set whitestep length, preferred is 0 though
		this->whiteSectionMaxDuration = (float)this->whitestep / (float)this->samplingFrequency;
    }

    void QosmeticsTrail::Update()
    {
		
		if (!this->topTransform || !this->bottomTransform) 
			this->set_enabled(false); // if either transform not defined (should be after awake) disable the trail

		// this method just makes sure that the trail gets updated positions through it's custom movementData
        UnityEngine::Vector3 topPos = this->topTransform->get_position();
		UnityEngine::Vector3 bottomPos = this->bottomTransform->get_position();
        this->customMovementData->AddNewData(topPos, bottomPos, GlobalNamespace::TimeHelper::get_time());
    }

	GlobalNamespace::SaberTrailRenderer* QosmeticsTrail::NewTrailRenderer(UnityEngine::Material* material)
    {
        // make a new gameobject to house the prefab on
        UnityEngine::GameObject* newPrefab = *il2cpp_utils::RunMethod<UnityEngine::GameObject*>("UnityEngine", "Object", "Instantiate", UnityEngine::GameObject::New_ctor());

        // Trail renderer script holds reference to meshfilter and meshrenderer used to render the trial
        CRASH_UNLESS(il2cpp_utils::RunMethod<UnityEngine::MeshFilter*>(newPrefab, "AddComponent", UnityUtils::TypeFromString("MeshFilter")));
        CRASH_UNLESS(il2cpp_utils::RunMethod<UnityEngine::MeshRenderer*>(newPrefab, "AddComponent", UnityUtils::TypeFromString("MeshRenderer")));
        GlobalNamespace::SaberTrailRenderer* trailRendererPrefab = CRASH_UNLESS(il2cpp_utils::RunMethod<GlobalNamespace::SaberTrailRenderer*>(newPrefab, "AddComponent", UnityUtils::TypeFromString("", "SaberTrailRenderer")));

        // if the material is not nullptr set it
        if (material != nullptr) trailRendererPrefab->meshRenderer->set_sharedMaterial(material);      

        // give it a good name, for identification I guess
        newPrefab->set_name(il2cpp_utils::createcsstr("Trail"));

        // return the trail renderer pointer
        return trailRendererPrefab;
    }
}