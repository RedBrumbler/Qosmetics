#include "Types/Trail/QosmeticsTrail.hpp"
#include "Config.hpp"
#include "GlobalNamespace/IBladeMovementData.hpp"
#include "GlobalNamespace/TimeHelper.hpp"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/Vector3.hpp"

#include "QosmeticsLogger.hpp"
#include "Utils/ChromaUtils.hpp"
#include "Utils/TrailUtils.hpp"

#include "chroma/shared/SaberAPI.hpp"

extern config_t config;

DEFINE_TYPE(Qosmetics, QosmeticsTrail);

using namespace UnityEngine;

static Il2CppString* topTransformName = nullptr;
static Il2CppString* bottomTransformName = nullptr;
static Il2CppString* customBottomTransformName = nullptr;

#define INFO(value...) QosmeticsLogger::GetContextLogger("Trail").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("Trail").error(value)

float Qosmetics::QosmeticsTrail::trailIntensity = 1.0f;

namespace Qosmetics
{
    void QosmeticsTrail::ctor()
    {
        INVOKE_CTOR();
        auto origCtor = il2cpp_utils::FindMethod(classof(GlobalNamespace::SaberTrail*), ".ctor");
        if (origCtor)
        {
            INFO("Running original ctor");
            il2cpp_utils::RunMethod(this, origCtor);
        }

        topTransform = nullptr;
        bottomTransform = nullptr;
        customBottomTransform = nullptr;
        trailMaterial = nullptr;
        length = 14;
        whitestep = 0;
        colorType = 2;
        customMovementData = nullptr;
        colorManager = nullptr;
        customInited = false;
        trailConfig = nullptr;
    }

    void QosmeticsTrail::dtor()
    {
        Object::Destroy(trailRenderer);
    }

    void QosmeticsTrail::Awake()
    {
        if (!trailMaterial)
        {
            MeshRenderer* renderer = get_gameObject()->GetComponent<MeshRenderer*>();
            if (renderer)
                trailMaterial = renderer->get_sharedMaterial();
        }

        if (!this->customMovementData)
            this->customMovementData = GlobalNamespace::SaberMovementData::New_ctor();
        this->movementData = reinterpret_cast<GlobalNamespace::IBladeMovementData*>(this->customMovementData);

        if (!this->trailRenderer)
            this->trailRenderer = this->trailRendererPrefab = NewTrailRenderer();

        this->trailDuration = (float)this->length / (float)this->samplingFrequency;
        this->whiteSectionMaxDuration = (float)this->whitestep / (float)this->samplingFrequency;

        if (!topTransformName)
            topTransformName = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("TrailEnd");
        if (!bottomTransformName)
            bottomTransformName = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("TrailStart");
        if (!customBottomTransformName)
            customBottomTransformName = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("CustomTrailStart");

        this->topTransform = get_transform()->Find(topTransformName);

        if (!this->topTransform)
        {
            this->topTransform = GameObject::New_ctor()->get_transform();
            this->topTransform->SetParent(get_transform());
            this->topTransform->set_localPosition({0.0f, 0.0f, 1.0f});
            this->topTransform->get_gameObject()->set_name(il2cpp_utils::newcsstr("TrailEnd"));
            this->topTransform = get_transform()->Find(topTransformName);
        }

        this->bottomTransform = get_transform()->Find(bottomTransformName);

        if (!this->bottomTransform)
        {
            this->bottomTransform = GameObject::New_ctor()->get_transform();
            this->bottomTransform->SetParent(get_transform());
            this->bottomTransform->set_localPosition({0.0f, 0.0f, 0.0f});
            this->bottomTransform->get_gameObject()->set_name(bottomTransformName);
            this->bottomTransform = get_transform()->Find(bottomTransformName);
        }

        this->customBottomTransform = get_transform()->Find(customBottomTransformName);

        if (!this->customBottomTransform)
        {
            this->customBottomTransform = GameObject::New_ctor()->get_transform();
            this->customBottomTransform->SetParent(get_transform());
            this->customBottomTransform->set_name(customBottomTransformName);
            this->customBottomTransform = get_transform()->Find(customBottomTransformName);
        }

        if (config.saberConfig.overrideTrailWidth && this->customBottomTransform && this->bottomTransform && this->topTransform)
        {
            UnityEngine::Vector3 newPos = UnityEngine::Vector3::Lerp(this->topTransform->get_localPosition(), this->bottomTransform->get_localPosition(), config.saberConfig.trailWidth);
            this->customBottomTransform->set_localPosition(newPos);
        }
    }

    void QosmeticsTrail::InitTrail(int length, int colorType, int whiteStep, UnityEngine::Material* material, UnityEngine::Color trailColor, UnityEngine::Color multiplierColor, bool ignoreOverrides = false)
    {
        customInited = false;
        int overriddenLength = (int)config.saberConfig.trailLength;
        this->length = (config.saberConfig.overrideTrailLength && !ignoreOverrides) ? overriddenLength : length;

        this->granularity = (int)(60.0f * ((this->length > 10.0f) ? this->length / 10.0f : 1.0f));
        this->colorType = (config.saberConfig.whiteTrail && !ignoreOverrides) ? 2 : colorType;

        int overriddenWhitestep = (int)config.saberConfig.whiteStep * this->length;
        this->whitestep = (config.saberConfig.overrideWhiteStep && !ignoreOverrides) ? overriddenWhitestep : whiteStep;

        this->trailMaterial = material;
        this->trailColor = (config.saberConfig.whiteTrail && !ignoreOverrides) ? UnityEngine::Color::get_white() : trailColor;
        this->multiplierColor = multiplierColor;

        if (!this->trailMaterial)
        {
            MeshRenderer* renderer = get_gameObject()->GetComponent<MeshRenderer*>();
            if (renderer)
                this->trailMaterial = renderer->get_sharedMaterial();
        }

        this->trailRenderer = this->trailRendererPrefab = NewTrailRenderer();

        if (!topTransformName)
            topTransformName = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("TrailEnd");
        if (!bottomTransformName)
            bottomTransformName = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("TrailStart");
        if (!customBottomTransformName)
            customBottomTransformName = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("CustomTrailStart");

        this->topTransform = get_transform()->Find(topTransformName);
        this->bottomTransform = get_transform()->Find(bottomTransformName);
        this->customBottomTransform = get_transform()->Find(customBottomTransformName);
        if (!customBottomTransform)
        {
            this->customBottomTransform = UnityEngine::Object::Instantiate(this->bottomTransform, get_transform());
            this->customBottomTransform->set_name(customBottomTransformName);
            this->customBottomTransform = get_transform()->Find(customBottomTransformName);
        }

        UpdateTrail();
        customInited = true;
    }

    void QosmeticsTrail::InitFromDefault(UnityEngine::Transform* objToCopy)
    {
        if (!objToCopy)
            return;
        GlobalNamespace::SaberTrail* orig = objToCopy->GetComponent<GlobalNamespace::SaberTrail*>();
        if (!orig)
            return;

        int length = (int)((float)orig->samplingFrequency * orig->trailDuration * 0.5f);
        int colorType = 2;
        int whiteStep = (int)((float)orig->samplingFrequency * orig->whiteSectionMaxDuration * 0.5f);
        UnityEngine::Material* mat = orig->trailRendererPrefab->meshRenderer->get_material();
        get_gameObject()->AddComponent<MeshRenderer*>()->set_material(mat);
        UnityEngine::Color trailColor = orig->color;
        UnityEngine::Color multiplier = UnityEngine::Color::get_white();

        InitTrail(length,
                  colorType,
                  whiteStep,
                  nullptr,
                  trailColor,
                  multiplier);
    }

    void QosmeticsTrail::Update()
    {
        if (!customInited)
            return;
        if (!this->topTransform || !this->bottomTransform || !this->customBottomTransform)
            return;
        if (!this->customMovementData)
            return;

        if (!trailRenderer->meshRenderer->get_enabled())
            return;

        UnityEngine::Vector3 topPos = this->topTransform->get_position();
        UnityEngine::Vector3 bottomPos = (config.saberConfig.overrideTrailWidth && this->customBottomTransform) ? this->customBottomTransform->get_position() : this->bottomTransform->get_position();

        this->customMovementData->AddNewData(topPos, bottomPos, GlobalNamespace::TimeHelper::get_time());
    }

    void QosmeticsTrail::UpdateTrail()
    {
        if (this->customMovementData)
            this->customMovementData->Finalize();
        this->customMovementData = GlobalNamespace::SaberMovementData::New_ctor();
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
        if (!this->trailRenderer || !this->trailRenderer->meshRenderer->get_sharedMaterial())
        {
            MeshRenderer* renderer = this->get_gameObject()->GetComponent<MeshRenderer*>();
            if (!this->trailMaterial && renderer)
            {
                this->trailMaterial = renderer->get_sharedMaterial();
                this->trailRenderer = this->trailRendererPrefab = NewTrailRenderer();
            }
        }

        UpdateColors();
    }

    void QosmeticsTrail::UpdateColors()
    {
        using SaberAPI = Chroma::SaberAPI;

        if (colorType == 0 || colorType == 1)
        {
            std::optional<UnityEngine::Color> saberColor = SaberAPI::getGlobalSaberColorSafe(colorType);
            if (saberColor != std::nullopt)
            {
                this->color = (*saberColor) * this->multiplierColor;
            }
            else
            {
                this->color = colorManager->ColorForTrailType(colorType) * this->multiplierColor;
            }
        }
        else
        {
            this->color = this->trailColor * this->multiplierColor;
        }

        this->color.a *= trailIntensity;
    }

    void QosmeticsTrail::UpdateChromaColors(int colorType, GlobalNamespace::SaberModelController* modelController, Sombrero::FastColor color)
    {
        if (modelController->Equals(this->attachedSaberModelController))
            UpdateColors();
    }

    void QosmeticsTrail::SetTrailConfig(TrailConfig* config)
    {
        if (!config)
            return;
        trailConfig = config;
        Reset();
    }

    void QosmeticsTrail::SetColorManager(ColorManager* colorManager)
    {
        this->colorManager = colorManager;
        std::function<void()> callback = std::bind(&QosmeticsTrail::UpdateColors, this);
        //this->colorManager->RegisterCallback(callback, callbackType::trail);
        //ChromaUtils::registerSaberCallback({+[this](int, GlobalNamespace::SaberModelController* modelController, Color) -> void { if(modelController->Equals(this->attachedSaberModelController)) UpdateColors(); }, this});
        ChromaUtils::registerSaberCallback({&QosmeticsTrail::UpdateChromaColors, this});
        UpdateColors();
    }

    void QosmeticsTrail::Reset()
    {
        if (this->trailConfig)
        {
            InitTrail(trailConfig->get_length(),
                      (int)trailConfig->get_colorType(),
                      trailConfig->get_whiteStep(),
                      trailMaterial,
                      trailConfig->get_trailColor(),
                      trailConfig->get_multiplierColor());
        }
        else
        {
            InitTrail(length,
                      colorType,
                      whitestep,
                      trailMaterial,
                      trailColor,
                      multiplierColor);
        }
    }

    GlobalNamespace::SaberTrailRenderer* QosmeticsTrail::NewTrailRenderer()
    {
        if (!trailMaterial)
            return nullptr;
        if (trailRenderer)
            Object::Destroy(trailRenderer->get_gameObject());
        GlobalNamespace::SaberTrailRenderer* newRenderer = TrailUtils::NewTrailRenderer(trailMaterial);
        Vector3 topPos = topTransform ? topTransform->get_position() : Vector3(0.0f, 0.0f, 1.0f);
        Vector3 bottomPos = (config.saberConfig.overrideTrailWidth && customBottomTransform) ? customBottomTransform->get_position() : (bottomTransform ? bottomTransform->get_position() : Vector3::get_zero());

        float trailWidth = (bottomPos - topPos).get_magnitude(); //GetTrailWidth(customMovementData->get_lastAddedData());
        INFO("width: %.2f, duration: %.2f, granularity: %d, whiteSectionMaxDuration: %.2f", trailWidth, trailDuration, granularity, whiteSectionMaxDuration);
        newRenderer->Init(trailWidth, trailDuration, granularity, whiteSectionMaxDuration);
        return newRenderer;
    }
}