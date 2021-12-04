#include "Types/Trail/TrailHelper.hpp"
#include "Config.hpp"
#include "Utils/ChromaUtils.hpp"
#include "chroma/shared/SaberAPI.hpp"

#include "QosmeticsLogger.hpp"

#define INFO(value...) QosmeticsLogger::GetContextLogger("TrailHelper").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("TrailHelper").error(value)

DEFINE_TYPE(Qosmetics, TrailHelper);

using namespace UnityEngine;

namespace Qosmetics
{
    void TrailHelper::ctor()
    {
        trailInstance = nullptr;
    }

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
        LOGPTR(this);
        this->colorManager = colorManager;
        this->parentModelController = parentModelController;
        // if not set, no use in registering callback
        if (parentModelController)
            ChromaUtils::registerSaberCallback({&TrailHelper::UpdateChromaColors, this});
        GetOrAddTrail(false);
    }

    void TrailHelper::TrailSetup()
    {
        GetOrAddTrail(true);

        static Il2CppString* topTransformName = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("TrailEnd");
        static Il2CppString* customTransformName = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("CustomTrailStart");
        static Il2CppString* bottomTransformName = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("TrailStart");

        Transform* topTransform = get_transform()->Find(topTransformName);

        if (!topTransform)
        {
            topTransform = GameObject::New_ctor()->get_transform();
            topTransform->SetParent(get_transform());
            topTransform->set_localPosition({0.0f, 0.0f, 1.0f});
            topTransform->get_gameObject()->set_name(topTransformName);
        }

        Transform* bottomTransform = get_transform()->Find(bottomTransformName);

        if (!bottomTransform)
        {
            bottomTransform = GameObject::New_ctor()->get_transform();
            bottomTransform->SetParent(get_transform());
            bottomTransform->set_localPosition({0.0f, 0.0f, 0.0f});
            bottomTransform->get_gameObject()->set_name(bottomTransformName);
        }

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
        initData.TrailLength = config.saberConfig.overrideTrailLength ? config.saberConfig.trailLength : length;
        initData.TrailLength = initData.TrailLength >= 4 ? initData.TrailLength : 4;

        LOGINT(initData.TrailLength);
        // override set trail whitestep?
        initData.Whitestep = config.saberConfig.overrideWhiteStep ? config.saberConfig.whiteStep : whiteStep;
        initData.TrailColor = {1.0f, 1.0f, 1.0f, 1.0f};

        // calculate granularity
        initData.Granularity = (int)(60.0f * ((initData.TrailLength > 10) ? (float)initData.TrailLength / 10.0f : 1.0f));
        ;
        LOGINT(initData.Granularity);
        LOGPTR(trailInstance);
        trailInstance->Setup(initData, bottomTransform, topTransform, GetComponent<Renderer*>()->get_material(), true);
        INFO("Trail is Setup");
        UpdateColors();
    }

    void TrailHelper::SetTrailActive(bool active)
    {
        GetOrAddTrail(false);
        // if state differs, apply it
        if (trailInstance->get_enabled() ^ active)
            trailInstance->set_enabled(active);
    }

    void TrailHelper::SetColors(const Sombrero::FastColor& leftColor, const Sombrero::FastColor& rightColor)
    {
        if (colorType ^ 0b10)
        {
            GetOrAddTrail(false);
            trailInstance->SetColor(colorType == 0 ? leftColor : rightColor);
        }
        // if not left or right color type, just do nothing
    }

    void TrailHelper::UpdateColors()
    {
        GetOrAddTrail(false);
        trailInstance->SetColor(GetColor(colorType));
    }

    Sombrero::FastColor TrailHelper::GetColor(int colorType)
    {
        if (config.saberConfig.whiteTrail)
        {
            return {1.0f, 1.0f, 1.0f, 1.0f};
        }

        Sombrero::FastColor color = this->color;
        // colortype can only be 0, 1 or 2, meaning 0b0, 0b1 or 0b10, if ^0b10 makes it false then it's 2, else 0 ro 1
        if (colorType ^ 0b10)
        {
            std::optional<UnityEngine::Color> saberColor = Chroma::SaberAPI::getGlobalSaberColorSafe(colorType);
            if (saberColor)
            {
                color = (*saberColor) * multiplier;
            }
            else if (colorManager)
            {
                color = colorManager->ColorForTrailType(colorType) * multiplier;
            }
        }
        return color;
    }

    void TrailHelper::UpdateChromaColors(int saberType, GlobalNamespace::SaberModelController* saberModelController, Sombrero::FastColor color)
    {
        if (saberModelController->Equals(parentModelController))
            UpdateColors();
    }

    void TrailHelper::set_trailConfig(Qosmetics::TrailConfig& trailConfig)
    {
        colorType = trailConfig.get_colorType();
        color = trailConfig.get_color();
        multiplier = trailConfig.get_multiplierColor();
        length = trailConfig.get_length();
        whiteStep = trailConfig.get_whiteStep();
    }
}