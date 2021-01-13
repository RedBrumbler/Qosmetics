#include "UI/QosmeticsSongSelectFlowCoordinator.hpp"

#include "UI/MenuButtonClickData.hpp"
#include "questui/shared/CustomTypes/Data/CustomDataType.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"
#include "UnityEngine/Object.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/UI/ContentSizeFitter.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/ViewController_AnimationType.hpp"
#include "System/Action_1.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"

#include "GlobalNamespace/SoloFreePlayFlowCoordinator.hpp"

using namespace QuestUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace TMPro;
using namespace HMUI;

DEFINE_CLASS(Qosmetics::QosmeticsSongSelectFlowCoordinator);

namespace Qosmetics
{
    void QosmeticsSongSelectFlowCoordinator::BackButtonWasPressed(HMUI::ViewController* topViewController)
    {
        if (ActiveViewController != QosmeticsViewController)
        {
            SetTitle(il2cpp_utils::createcsstr("Qosmetics Settings"), ViewController::AnimationType::Out);
            ReplaceTopViewController(QosmeticsViewController, this, this, nullptr, ViewController::AnimationType::Out, ViewController::AnimationDirection::Horizontal);
            SetLeftScreenViewController(nullptr, ViewController::AnimationType::Out);
            SetRightScreenViewController(nullptr, ViewController::AnimationType::Out);
            ActiveViewController = QosmeticsViewController;
        }
        else 
        {
            if (previousFlowCoordinator) previousFlowCoordinator->DismissFlowCoordinator(this, ViewController::AnimationDirection::Horizontal, nullptr, false);
        }
    }
}
