#include "config.hpp"
#include "Data/QosmeticsTrail.hpp"
#include "Data/CustomTrail.hpp"
#include "GlobalNamespace/TimeHelper.hpp"
#include "UnityEngine/Mathf.hpp"
#include "GlobalNamespace/TrailElementCollection.hpp" 
#include "GlobalNamespace/TrailElement.hpp"
#include "GlobalNamespace/ColorManager.hpp"
#include "GlobalNamespace/SaberType.hpp"
#include "Utils/UnityUtils.hpp"
#include "Qosmetic/QosmeticsColorManager.hpp"

DEFINE_CLASS(Qosmetics::QosmeticsTrail);
float Qosmetics::QosmeticsTrail::trailIntensity = 1.0f;

extern config_t config;

namespace Qosmetics
{
	Qosmetics::QosmeticsTrail* QosmeticsTrail::CopyFromBase(GlobalNamespace::SaberTrail* base)
	{
		int length = (int)((float)base->samplingFrequency * base->trailDuration * 0.5f);
		int granularity = (int)((float)base->granularity * ((length > 10) ? ((float)length / 10.0f) : 1.0f));
		int colorType = 2;
		int whiteStep = (int)((float)base->samplingFrequency * base->whiteSectionMaxDuration * 0.5f);
		UnityEngine::Material* mat = base->trailRendererPrefab->meshRenderer->get_material();
		this->trailRenderer = NewTrailRenderer(mat);
		UnityEngine::Color trailColor = base->color;
		UnityEngine::Color multiplier = UnityEngine::Color::get_white();

		this->TrailSetup(length, granularity, colorType, whiteStep, mat, trailColor, multiplier);
		this->Awake();
		this->framesPassed = 0;
		this->set_enabled(true);
		return this;
	}

	void QosmeticsTrail::AddTrailAndSetup(UnityEngine::Transform* trailObject, Qosmetics::CustomTrail &trail)
	{
		QosmeticsTrail* customTrail = trailObject->get_gameObject()->GetComponent<QosmeticsTrail*>();
		if (customTrail == nullptr) customTrail = trailObject->get_gameObject()->AddComponent<QosmeticsTrail*>();
		float granularity = 60 * ((trail.get_length() > 10) ? ((float)trail.get_length() / 10.0f) : 1);
        customTrail->TrailSetup(trail.get_length(), granularity, trail.get_type(), trail.get_whiteStep(), trail.get_material(), trail.get_trailColor(), trail.get_multiplierColor());
		customTrail->Awake();
		customTrail->framesPassed = 0;
		customTrail->set_enabled(true);
	}

	void QosmeticsTrail::TrailSetup(int length, int granularity, int colorType, int whiteStep, UnityEngine::Material* trailMaterial, UnityEngine::Color trailColor, UnityEngine::Color multiplierColor)
	{
		// sets all the different values
		this->length = config.saberConfig.overrideTrailLength ? (int)config.saberConfig.trailLength : length;
		this->granularity = config.saberConfig.overrideTrailLength ? (int)(60 * (config.saberConfig.trailLength > 10.0f ? (config.saberConfig.trailLength / 10.0f) : 1.0f)): granularity;
		this->colorType = colorType;
		this->whitestep = config.saberConfig.overrideWhiteStep ? config.saberConfig.whiteStep * this->length : whiteStep;
		if (trailMaterial) this->trailMaterial = trailMaterial;
		this->trailColor = trailColor;
		this->multiplierColor = multiplierColor;
	}

    void QosmeticsTrail::Awake()
    {
		// get the material to use for the trail renderer
		if (this->trailMaterial == nullptr)
		{
			SaberLogger::GetLogger().info("trailMaterial from trail reference was nullptr, redefining the trailmaterial from local object");
			UnityEngine::MeshRenderer* renderer = this->get_gameObject()->GetComponent<UnityEngine::MeshRenderer*>();
			if (renderer != nullptr) this->trailMaterial = renderer->get_material();
		}

		// make a new trail renderer to use
        if (!this->trailRenderer) this->trailRenderer = NewTrailRenderer(this->trailMaterial);
		// create trail duration from length / sampling
        this->trailDuration = (float)this->length / (float)this->samplingFrequency;

		// make a new movementdata
		if (!this->customMovementData) this->customMovementData = GlobalNamespace::SaberMovementData::New_ctor();
		// set the movementData interface to this new movementData
		this->movementData = reinterpret_cast<GlobalNamespace::IBladeMovementData*>(this->customMovementData);

		// if either is nullptr, set it
		if (!this->topTransform)
		{
			this->topTransform = this->get_transform()->Find(il2cpp_utils::createcsstr("TrailEnd"));
		}
		if (!this->bottomTransform)
		{
			this->bottomTransform = this->get_transform()->Find(il2cpp_utils::createcsstr("TrailStart"));
		}

		// get color set
		UpdateColors();

		// set whitestep length, preferred is 0 though
		this->whiteSectionMaxDuration = (float)this->whitestep / (float)this->samplingFrequency;
    }

    void QosmeticsTrail::Update()
    {
		if (!this->topTransform || !this->bottomTransform) 
		{
			if (this->topTransform == nullptr)
			{
				this->topTransform = this->get_transform()->Find(il2cpp_utils::createcsstr("TrailEnd"));
			}
			if (this->bottomTransform == nullptr)
			{
				this->bottomTransform = this->get_transform()->Find(il2cpp_utils::createcsstr("TrailStart"));
			}
			if (!this->topTransform || !this->bottomTransform) return;
		}
		// this method just makes sure that the trail gets updated positions through it's custom movementData
        UnityEngine::Vector3 topPos = this->topTransform->get_position();
		UnityEngine::Vector3 bottomPos = this->bottomTransform->get_position();
		if (!this->customMovementData) 
		{
			this->customMovementData = GlobalNamespace::SaberMovementData::New_ctor();
			this->movementData = reinterpret_cast<GlobalNamespace::IBladeMovementData*>(this->customMovementData);
		}
        this->customMovementData->AddNewData(topPos, bottomPos, GlobalNamespace::TimeHelper::get_time());
    }

	GlobalNamespace::SaberTrailRenderer* QosmeticsTrail::NewTrailRenderer(UnityEngine::Material* material)
    {
        // make a new gameobject to house the prefab on
        UnityEngine::GameObject* newPrefab = UnityEngine::GameObject::New_ctor();

        // Trail renderer script holds reference to meshfilter and meshrenderer used to render the trial
        newPrefab->AddComponent<UnityEngine::MeshFilter*>();
        newPrefab->AddComponent<UnityEngine::MeshRenderer*>();
        GlobalNamespace::SaberTrailRenderer* trailRendererPrefab = newPrefab->AddComponent<GlobalNamespace::SaberTrailRenderer*>();

        // if the material is not nullptr set it
        if (material != nullptr) trailRendererPrefab->meshRenderer->set_material(material);      

        // give it a good name, for identification I guess
        newPrefab->set_name(il2cpp_utils::createcsstr("Trail"));

        // return the trail renderer pointer
        return trailRendererPrefab;
    }

	void QosmeticsTrail::UpdateColors()
	{
		Qosmetics::ColorManager* colorManager = UnityEngine::Object::FindObjectOfType<Qosmetics::ColorManager*>();
        if (colorManager != nullptr) // if colormanager defined, get colors from it, else just get the set color
		{
			switch (this->colorType)
			{
				case 0: // LeftSaber
					this->color = colorManager->ColorForTrailType(GlobalNamespace::SaberType::SaberA) * this->multiplierColor;
					break;
				case 1:	// RightSaber
					this->color = colorManager->ColorForTrailType(GlobalNamespace::SaberType::SaberB) * this->multiplierColor;
					break;
				default:	// Custom Color
					this->color = this->trailColor * this->multiplierColor;
					break;
			}
		}
		else this->color = this->trailColor * this->multiplierColor;
		
		// apply trailIntensity with a *= and not a = because it should not overwrite the set color.a, just modify it
		this->color.a *= trailIntensity;
	}
}