#include "UI/Saber/SaberPreviewElement.hpp"

DEFINE_CLASS(Qosmetics::UI::SaberPreviewElement);

#include "HMUI/Touchable.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/ExternalComponents.hpp"
#include "questui/shared/CustomTypes/Data/CustomDataType.hpp"

#include "UnityEngine/Transform.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/Events/UnityAction_1.hpp"
#include "UnityEngine/WaitUntil.hpp"
#include "System/Collections/IEnumerator.hpp"
#include "System/Func_1.hpp"

using namespace HMUI;
using namespace UnityEngine;
using namespace UnityEngine::Events;
using namespace UnityEngine::UI;
using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;
using namespace Qosmetics;
using namespace Qosmetics::UI;
using namespace TMPro;

namespace Qosmetics::UI
{
    void SaberPreviewElement::Init(SaberManager* modelManager, ColorManager* colorManager)
    {
        this->modelManager = modelManager;
        this->colorManager = colorManager;
    }

    void SaberPreviewElement::UpdatePreview()
    {
        if (!modelManager) return;
        GameObject* prefab = Instantiate(modelManager->GetActivePrefab(), get_transform());
    }

    void SaberPreviewElement::ClearPreview()
    {
        Object::Destroy(get_transform()->GetChild(0));
    }
}