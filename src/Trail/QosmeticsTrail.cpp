#include "Trail/QosmeticsTrail.hpp"
#include "GlobalNamespace/IBladeMovementData.hpp"
#include "GlobalNamespace/TimeHelper.hpp"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Renderer.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/Vector3.hpp"

#include "Config.hpp"
#include "Utils/TrailUtils.hpp"
#include "QosmeticsLogger.hpp"

extern config_t config;

DEFINE_CLASS(Qosmetics::QosmeticsTrail);

using namespace UnityEngine;

static Il2CppString* topTransformName = nullptr;
static Il2CppString* bottomTransformName = nullptr;
static Il2CppString* customBottomTransformName = nullptr;

#define INFO(value...) QosmeticsLogger::GetContextLogger("Trail").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("Trail").error(value)

float Qosmetics::QosmeticsTrail::trailIntensity = 1.0f;

namespace Qosmetics
{
    void QosmeticsTrail::Awake()
    {
        if (!trailMaterial)
        {
            ERROR("Trail material was nullptr, defining from renderer!");
            Renderer* renderer = GetComponent<Renderer*>();
            if (renderer) trailMaterial = renderer->get_material();
        }

        if (!this->trailRenderer) this->trailRenderer = NewTrailRenderer();

        this->trailDuration = (float)this->length / (float)this->samplingFrequency;
        this->whiteSectionMaxDuration = (float)this->whitestep / (float)this->samplingFrequency;

        if (!this->customMovementData) this->customMovementData = GlobalNamespace::SaberMovementData::New_ctor();
        this->movementData = reinterpret_cast<GlobalNamespace::IBladeMovementData*>(this->customMovementData);

        if (!topTransformName) topTransformName = il2cpp_utils::createcsstr("TrailEnd", il2cpp_utils::StringType::Manual);
        if (!bottomTransformName) bottomTransformName = il2cpp_utils::createcsstr("TrailStart", il2cpp_utils::StringType::Manual);
        if (!customBottomTransformName) customBottomTransformName = il2cpp_utils::createcsstr("CustomTrailStart", il2cpp_utils::StringType::Manual);

        if (!this->topTransform) this->topTransform = get_transform()->Find(topTransformName);
        if (!this->bottomTransform) this->bottomTransform = get_transform()->Find(bottomTransformName);
        if (!this->customBottomTransform) 
        {
            this->customBottomTransform = get_transform()->Find(customBottomTransformName);
            if (!this->customBottomTransform)
            {
                this->customBottomTransform = UnityEngine::Object::Instantiate(this->bottomTransform, get_transform());
			    this->customBottomTransform->set_name(customBottomTransformName);
            }
        }

        if (config.saberConfig.overrideTrailWidth && this->customBottomTransform)
		{
			UnityEngine::Vector3 newPos = UnityEngine::Vector3::Lerp(this->topTransform->get_localPosition(), this->bottomTransform->get_localPosition(), config.saberConfig.trailWidth);
			this->customBottomTransform->set_localPosition(newPos);
		}
    }

    void QosmeticsTrail::InitTrail(int length, int colorType, int whiteStep, UnityEngine::Material* material, UnityEngine::Color trailColor, UnityEngine::Color multiplierColor, bool ignoreOverrides = false)
    {
        INFO("Initing trail");
        customInited = false;
        int overriddenLength = (int)config.saberConfig.trailLength;
        this->length = (config.saberConfig.overrideTrailLength && !ignoreOverrides) ? overriddenLength : length;
        
        this->granularity = (int)(60 * (this->length > 10.0f)) ? this->length / 10.0f : 1.0f;
		this->colorType = (config.saberConfig.whiteTrail && !ignoreOverrides) ? 2 : colorType;
        
        int overriddenWhitestep = (int)config.saberConfig.whiteStep * this->length;
		this->whitestep = (config.saberConfig.overrideWhiteStep && !ignoreOverrides) ? overriddenWhitestep : whiteStep;
		
        this->trailMaterial = (material) ? material : nullptr;
		this->trailColor = (config.saberConfig.whiteTrail && !ignoreOverrides) ? UnityEngine::Color::get_white() : trailColor;
		this->multiplierColor = multiplierColor;

        if (!this->trailMaterial) 
        {
            Renderer* renderer = GetComponent<Renderer*>();
            if (renderer) this->trailMaterial = renderer->get_material();
        }

        this->trailRenderer = NewTrailRenderer();

        if (!topTransformName) topTransformName = il2cpp_utils::createcsstr("TrailEnd", il2cpp_utils::StringType::Manual);
        if (!bottomTransformName) bottomTransformName = il2cpp_utils::createcsstr("TrailStart", il2cpp_utils::StringType::Manual);
        if (!customBottomTransformName) customBottomTransformName = il2cpp_utils::createcsstr("CustomTrailStart", il2cpp_utils::StringType::Manual);

        this->topTransform = get_transform()->Find(topTransformName);
        this->bottomTransform = get_transform()->Find(bottomTransformName);
        this->customBottomTransform = get_transform()->Find(customBottomTransformName);
        if (!customBottomTransform)
        {
            this->customBottomTransform = UnityEngine::Object::Instantiate(this->bottomTransform, get_transform());
			this->customBottomTransform->set_name(customBottomTransformName);
        }

        UpdateTrail();

        Awake();
        framesPassed = 0;
        customInited = true;
        set_enabled(true);
        INFO("Trail Inited!");
    }

    void QosmeticsTrail::InitFromDefault(UnityEngine::Transform* objToCopy)
    {
        if (!objToCopy) return;
        GlobalNamespace::SaberTrail* orig = objToCopy->GetComponent<GlobalNamespace::SaberTrail*>();
        if (!orig) return;
        INFO("Creating trail from original trail");
        #warning implement default trail load
    }

    void QosmeticsTrail::Update()
    {
        if (!customInited) return;
        if (!this->topTransform || !this->bottomTransform) return;
        UnityEngine::Vector3 topPos = this->topTransform->get_position();
		UnityEngine::Vector3 bottomPos = (config.saberConfig.overrideTrailWidth && this->customBottomTransform) ? this->customBottomTransform->get_position() : this->bottomTransform->get_position();

        this->customMovementData->AddNewData(topPos, bottomPos, GlobalNamespace::TimeHelper::get_time());
    }

    void QosmeticsTrail::UpdateTrail()
    {
        if (!this->customMovementData) this->customMovementData = GlobalNamespace::SaberMovementData::New_ctor();
        this->movementData = reinterpret_cast<GlobalNamespace::IBladeMovementData*>(this->customMovementData);

        this->trailDuration = (float)this->length / (float)this->samplingFrequency;
        this->whiteSectionMaxDuration = (float)this->whitestep / (float)this->samplingFrequency;
        
        // change trail width if enabled
        if (config.saberConfig.overrideTrailWidth && this->customBottomTransform)
		{
			Vector3 newPos = Vector3::Lerp(this->topTransform->get_localPosition(), this->bottomTransform->get_localPosition(), config.saberConfig.trailWidth);
			this->customBottomTransform->set_localPosition(newPos);
		}
        // if there is any issue with these objects, they will be remade
        if (!this->trailRenderer || !this->trailRenderer->meshRenderer->get_material())
		{
			Renderer* renderer = this->GetComponent<Renderer*>();
			if (!this->trailMaterial && renderer)
			{
				this->trailMaterial = renderer->get_material();
				this->trailRenderer = NewTrailRenderer();
			}
		}

        UpdateColors();
    }

    void QosmeticsTrail::UpdateColors()
    {
        INFO("Updating Trail Colors");
        /*
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
        */

       switch (this->colorType)
		{
			case 0: // LeftSaber
				this->color = Color::get_red() * this->multiplierColor;
				break;
			case 1:	// RightSaber
				this->color = Color::get_blue() * this->multiplierColor;
				break;
			default:	// Custom Color
				this->color = this->trailColor * this->multiplierColor;
				break;
		}

        this->color.a *= trailIntensity;
    }

    void QosmeticsTrail::SetTrailConfig(TrailConfig* config)
    {
        if (!config) return;
        trailConfig = config;
        InitTrail(   trailConfig->get_length(),
                (int)trailConfig->get_colorType(),
                trailConfig->get_whiteStep(),
                nullptr,
                trailConfig->get_trailColor(),
                trailConfig->get_multiplierColor(),
                true
            );
    }

    void QosmeticsTrail::Reset()
    {
        InitTrail(   trailConfig->get_length(),
                (int)trailConfig->get_colorType(),
                trailConfig->get_whiteStep(),
                nullptr,
                trailConfig->get_trailColor(),
                trailConfig->get_multiplierColor(),
                true
            );
    }

    GlobalNamespace::SaberTrailRenderer* QosmeticsTrail::NewTrailRenderer()
    {
        return TrailUtils::NewTrailRenderer(trailMaterial);
    }
}