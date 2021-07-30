#include "Config.hpp"
#include "Types/Trail/TrailHelper.hpp"
#include "Utils/ChromaUtils.hpp"
#include "chroma/shared/SaberAPI.hpp"

#include "QosmeticsLogger.hpp"

#define INFO(value...) QosmeticsLogger::GetContextLogger("TrailHelper").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("TrailHelper").error(value)

DEFINE_TYPE(Qosmetics, TrailHelper);

using namespace UnityEngine;

namespace Qosmetics
{
    void TrailHelper::Init(Qosmetics::ColorManager* colorManager)
    {
        this->colorManager = colorManager;
        trailInstance = GetComponent<AltTrail*>();

        ChromaUtils::registerSaberCallback({&TrailHelper::UpdateChromaColors, this});
    }

    void TrailHelper::TrailSetup()
    {
        INFO("instance ptr: %p, trail Config ptr: %p", trailInstance, trailConfig);

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
        initData.TrailLength = config.saberConfig.overrideTrailLength ? config.saberConfig.trailLength : trailConfig->get_length();;
        initData.Whitestep = config.saberConfig.overrideWhiteStep ? config.saberConfig.whiteStep : trailConfig->get_whiteStep();
        initData.TrailColor = {1.0f, 1.0f, 1.0f, 1.0f};
        initData.Granularity = (int)(60.0f * ((initData.TrailLength > 10.0f) ? initData.TrailLength / 10.0f : 1.0f));;

        trailInstance->Setup(initData, bottomTransform, topTransform, GetComponent<Renderer*>()->get_material(), false);
        INFO("Trail is Setup");
    }

    void TrailHelper::SetTrailActive(bool active)
    {
        // if state differs, apply it
        if (trailInstance->get_enabled() ^ active) trailInstance->set_enabled(active);
    }

    void TrailHelper::UpdateColors()
    {
        Color color;
        int colorType = trailConfig->get_colorType();
        
        // colortype can only be 0, 1 or 2, meaning 0b0, 0b1 or 0b10, if ^0b10 makes it false then it's 0 or 1
        if (colorType ^ 0b10)
        {
            std::optional<UnityEngine::Color> saberColor = Chroma::SaberAPI::getGlobalSaberColorSafe(colorType);
            if (saberColor)
            {
                color = (*saberColor) * trailConfig->get_multiplierColor();
            }
            else
            {
	    	    color = colorManager->ColorForTrailType(colorType) * trailConfig->get_multiplierColor();
            }
        }
        else
        {
	    	color = trailConfig->get_color();
        }
        
        trailInstance->SetColor(color);
    }

    void TrailHelper::UpdateChromaColors(int, GlobalNamespace::SaberModelController*, UnityEngine::Color)
    {
        UpdateColors();
    }

    void TrailHelper::set_trailConfig(Qosmetics::TrailConfig* trailConfig)
    {
        this->trailConfig = trailConfig;
    }
}