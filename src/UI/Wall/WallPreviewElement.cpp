#include "Config.hpp"
#include "UI/Wall/WallPreviewElement.hpp"

DEFINE_TYPE(Qosmetics::UI, WallPreviewElement);

#include "HMUI/Touchable.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/ExternalComponents.hpp"
#include "questui/shared/CustomTypes/Data/CustomDataType.hpp"

#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/Events/UnityAction_1.hpp"
#include "UnityEngine/WaitUntil.hpp"
#include "System/Collections/IEnumerator.hpp"
#include "System/Func_1.hpp"

#include "Utils/WallUtils.hpp"


using namespace HMUI;
using namespace UnityEngine;
using namespace UnityEngine::Events;
using namespace UnityEngine::UI;
using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;
using namespace Qosmetics;
using namespace Qosmetics::UI;
using namespace TMPro;

#include "QosmeticsLogger.hpp"
#define INFO(value...) QosmeticsLogger::GetContextLogger("Wall Preview Element").info(value);
#define ERROR(value...) QosmeticsLogger::GetContextLogger("Wall Preview Element").error(value);

namespace Qosmetics::UI
{
    void WallPreviewElement::Init(WallManager* modelManager, ColorManager* colorManager)
    {
        this->modelManager = modelManager;
        this->colorManager = colorManager;

        get_transform()->set_localScale(Vector3::get_one() * 50.0f);
        get_transform()->set_localPosition(UnityEngine::Vector3(-30.0f, 0.0f, -75.0f));
        get_transform()->set_localEulerAngles(UnityEngine::Vector3(0.0f, 330.0f, 0.0f));
    }

    void WallPreviewElement::UpdatePreview(bool reinstantiate)
    {
        INFO("Updating Preview Element");
        if (reinstantiate || !prefab)
        {
            if (!modelManager) 
            {
                ERROR("No ModelManager");
                return;
            }
            
            GameObject* prefabToInstantiate = modelManager->GetActivePrefab();
            if (!prefabToInstantiate)
            {
                ERROR("To Instantiate nullptr");
                return;
            }
            prefab = Instantiate(prefabToInstantiate, get_transform());
            prefab->SetActive(true);
        }
        
        Transform* prefabTransform = prefab->get_transform();

        prefabTransform->set_localPosition(UnityEngine::Vector3(0.0f, 0.0f, 0.0f));
        prefabTransform->set_localEulerAngles(UnityEngine::Vector3(0.0f, 100.0f, 0.0f));
        prefabTransform->set_localScale(UnityEngine::Vector3(1.5f, 1.0f, 0.5f));

        int childCount = prefabTransform->get_childCount();
        Color color = colorManager->ColorForObstacle();
        WallUtils::SetColors(prefab, color);
        
        for (int i = 0; i < childCount; i++)
        {
            Transform* child = prefabTransform->GetChild(i);

            std::string name = to_utf8(csstrtostr(child->get_gameObject()->get_name()));
            if (name == "Core")
            {
                child->get_gameObject()->SetActive(!config.wallConfig.forceCoreOff);
            }
            else if (name == "Frame")
            {
                child->get_gameObject()->SetActive(!config.wallConfig.forceFrameOff);
                WallUtils::SetSizeParams(child->get_gameObject(), child->get_lossyScale());
            }
            else
            {
                INFO("Destroying %s", name.c_str());
                Object::Destroy(child);
            }
        }
    }

    void WallPreviewElement::ClearPreview()
    {
        if (prefab) Object::Destroy(prefab);
        prefab = nullptr;
    }
}