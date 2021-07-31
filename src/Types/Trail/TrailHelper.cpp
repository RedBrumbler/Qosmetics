#include "Config.hpp"
#include "Types/Trail/TrailHelper.hpp"
#include "Utils/ChromaUtils.hpp"
#include "chroma/shared/SaberAPI.hpp"

#include "QosmeticsLogger.hpp"

#define INFO(value...) QosmeticsLogger::GetContextLogger("TrailHelper").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("TrailHelper").error(value)

#define LOGINT(val) INFO("%s: %d", #val, val)
DEFINE_TYPE(Qosmetics, TrailHelper);

using namespace UnityEngine;

namespace Qosmetics
{
    void TrailHelper::GetOrAddTrail(bool remake)
    {
        // if not set, get it
        if (!trailInstance)
            trailInstance = GetComponent<AltTrail*>();

        // if still not set, it didn't exist, add it
        if (!trailInstance)
            trailInstance = get_gameObject()->AddComponent<AltTrail*>();
        // if set, check if inited, if so reset by deleting and adding new
        else if (trailInstance->get_inited() && remake)
        {
            Object::DestroyImmediate(trailInstance);
            trailInstance = get_gameObject()->AddComponent<AltTrail*>();
        }
    }

    void TrailHelper::Init(Qosmetics::ColorManager* colorManager, GlobalNamespace::SaberModelController* parentModelController)
    {
        this->colorManager = colorManager;
        this->parentModelController = parentModelController;
        ChromaUtils::registerSaberCallback({&TrailHelper::UpdateChromaColors, this});
        GetOrAddTrail(false);
    }

    void TrailHelper::TrailSetup()
    {
        GetOrAddTrail(true);

        static Il2CppString* topTransformName = il2cpp_utils::createcsstr("TrailEnd", il2cpp_utils::StringType::Manual);
        static Il2CppString* customTransformName = il2cpp_utils::createcsstr("CustomTrailStart", il2cpp_utils::StringType::Manual);
        static Il2CppString* bottomTransformName = il2cpp_utils::createcsstr("TrailStart", il2cpp_utils::StringType::Manual);
        
        Transform* topTransform = get_transform()->Find(topTransformName);
        Transform* bottomTransform = get_transform()->Find(bottomTransformName);

        if (config.saberConfig.overrideTrailWidth)
        {
            Transform* customBottomTransform = get_transform()->Find(customTransformName);

            if (!customBottomTransform)
            {
                customBottomTransform = GameObject::New_ctor()->get_transform();
                customBottomTransform->SetParent(get_transform());
		        customBottomTransform->set_name(customTransformName);
            }

			UnityEngine::Vector3 newPos = UnityEngine::Vector3::Lerp(topTransform->get_localPosition(), bottomTransform->get_localPosition(), config.saberConfig.trailWidth);
			customBottomTransform->set_localPosition(newPos);
            bottomTransform = customBottomTransform;
        }

        TrailInitData initData;
        // override set trail length?
        initData.TrailLength = config.saberConfig.overrideTrailLength ? config.saberConfig.trailLength : trailConfig->get_length();
        initData.TrailLength = initData.TrailLength >= 4 ? initData.TrailLength : 4;

        LOGINT(initData.TrailLength);
        // override set trail whitestep?
        initData.Whitestep = config.saberConfig.overrideWhiteStep ? config.saberConfig.whiteStep : trailConfig->get_whiteStep();
        initData.TrailColor = {1.0f, 1.0f, 1.0f, 1.0f};
        
        // calculate granularity
        initData.Granularity = (int)(60.0f * ((initData.TrailLength > 10) ? (float)initData.TrailLength / 10.0f : 1.0f));;
        LOGINT(initData.Granularity);

        trailInstance->Setup(initData, bottomTransform, topTransform, GetComponent<Renderer*>()->get_material(), false);
        INFO("Trail is Setup");
        UpdateColors(); 
    }

    void TrailHelper::SetTrailActive(bool active)
    {
        GetOrAddTrail(false);
        // if state differs, apply it
        if (trailInstance->get_enabled() ^ active) trailInstance->set_enabled(active);
    }

    void TrailHelper::UpdateColors()
    {
        if (!trailConfig) 
        {
            ERROR("Updating colors with nullptr trailconfig, returning!");
            return;
        }

        GetOrAddTrail(false);
        trailInstance->SetColor(GetColor(trailConfig->get_colorType()));
    }

    Color TrailHelper::GetColor(int colorType)
    {
        if (config.saberConfig.whiteTrail)
        {
            return {1.0f, 1.0f, 1.0f, 1.0f};
        }
        Color color;

        // colortype can only be 0, 1 or 2, meaning 0b0, 0b1 or 0b10, if ^0b10 makes it false then it's 2, else 0 ro 1
        if (colorType ^ 0b10)
        {
            std::optional<UnityEngine::Color> saberColor = Chroma::SaberAPI::getGlobalSaberColorSafe(colorType);
            if (saberColor)
            {
                color = (*saberColor) * trailConfig->get_multiplierColor();
            }
            else if (colorManager)
            {
	    	    color = colorManager->ColorForTrailType(colorType) * trailConfig->get_multiplierColor();
            }
            else
            {
	    	    color = trailConfig->get_color();
            }
        }
        else
        {
	    	color = trailConfig->get_color();
        }
        return color;
    }

    void TrailHelper::UpdateChromaColors(int saberType, GlobalNamespace::SaberModelController* saberModelController, UnityEngine::Color color)
    {
        if (saberModelController->Equals(parentModelController)) UpdateColors();
    }

    void TrailHelper::set_trailConfig(Qosmetics::TrailConfig* trailConfig)
    {
        this->trailConfig = trailConfig;
    }
}