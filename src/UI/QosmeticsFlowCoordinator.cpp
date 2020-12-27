#include "UI/QosmeticsFlowCoordinator.hpp"

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

using namespace QuestUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace TMPro;
using namespace HMUI;

DEFINE_CLASS(Qosmetics::QosmeticsFlowCoordinator);

void OnOpenSubMenu(Qosmetics::QosmeticsFlowCoordinator* self, QuestUI::CustomDataType* data) {
    if (!data) return;
    qosmeticsType type = data->GetData<Qosmetics::MenuButtonClickData>().type;
    switch(type)
    {
        case saber:
            if (!self->SaberSwitcherViewController) break;
            self->SetTitle(il2cpp_utils::createcsstr("Qosmetics Sabers"), ViewController::AnimationType::In);
            self->ReplaceTopViewController(self->SaberSwitcherViewController, self, self, nullptr, ViewController::AnimationType::In, ViewController::AnimationDirection::Horizontal);
            self->ActiveViewController = self->SaberSwitcherViewController;
            break;
        case note:
            if (!self->NoteSwitcherViewController) break;
            self->SetTitle(il2cpp_utils::createcsstr("Qosmetics Bloqs"), ViewController::AnimationType::In);
            self->ReplaceTopViewController(self->NoteSwitcherViewController, self, self, nullptr, ViewController::AnimationType::In, ViewController::AnimationDirection::Horizontal);
            self->ActiveViewController = self->NoteSwitcherViewController;
            break;
        case wall:
            if (!self->WallSwitcherViewController) break;
            self->SetTitle(il2cpp_utils::createcsstr("Qosmetics Walls"), ViewController::AnimationType::In);
            self->ReplaceTopViewController(self->WallSwitcherViewController, self, self, nullptr, ViewController::AnimationType::In, ViewController::AnimationDirection::Horizontal);
            self->ActiveViewController = self->WallSwitcherViewController;
            break;
        default:
            break;
    }
}

namespace Qosmetics
{
    void QosmeticsFlowCoordinator::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if(firstActivation) 
        {
            SetTitle(il2cpp_utils::createcsstr("Qosmetics Settings"), ViewController::AnimationType::Out);
            showBackButton = true;
            for(int i = 0; i < 5; i++) 
            {
                VerticalLayoutGroup* layout = nullptr;
                GameObject* layoutGameObject = nullptr;
                TextMeshProUGUI* menuSubtitle = nullptr;
                switch((qosmeticsType)i) 
                {
                    case saber:
                        SaberSwitcherViewController = BeatSaberUI::CreateViewController<Qosmetics::SaberSwitcherViewController*>();
                        layout = BeatSaberUI::CreateVerticalLayoutGroup(SaberSwitcherViewController->get_rectTransform());
                        layout->get_rectTransform()->set_anchoredPosition(UnityEngine::Vector2(0.0f, -48.0f));

                        layoutGameObject = layout->get_gameObject();
                        layoutGameObject->GetComponent<ContentSizeFitter*>()->set_verticalFit(ContentSizeFitter::FitMode::PreferredSize);
                        layoutGameObject->AddComponent<Backgroundable*>()->ApplyBackground(il2cpp_utils::createcsstr("round-rect-panel"));
                        layout->set_padding(UnityEngine::RectOffset::New_ctor(3, 4, 2, 2));

                        menuSubtitle = BeatSaberUI::CreateText(layout->get_transform(), "Qosmetics Sabers");
                        menuSubtitle->set_alignment(TextAlignmentOptions::Center);
                        menuSubtitle->set_fontSize(4.8f);
                        break;
                    case note:
                        NoteSwitcherViewController = BeatSaberUI::CreateViewController<Qosmetics::NoteSwitcherViewController*>();
                        layout = BeatSaberUI::CreateVerticalLayoutGroup(NoteSwitcherViewController->get_rectTransform());
                        layout->get_rectTransform()->set_anchoredPosition(UnityEngine::Vector2(0.0f, -48.0f));

                        layoutGameObject = layout->get_gameObject();
                        layoutGameObject->GetComponent<ContentSizeFitter*>()->set_verticalFit(ContentSizeFitter::FitMode::PreferredSize);
                        layoutGameObject->AddComponent<Backgroundable*>()->ApplyBackground(il2cpp_utils::createcsstr("round-rect-panel"));
                        layout->set_padding(UnityEngine::RectOffset::New_ctor(3, 4, 2, 2));

                        menuSubtitle = BeatSaberUI::CreateText(layout->get_transform(), "Qosmetics Bloqs");
                        menuSubtitle->set_alignment(TextAlignmentOptions::Center);
                        menuSubtitle->set_fontSize(4.8f);
                        break;
                    case wall:
                        WallSwitcherViewController = BeatSaberUI::CreateViewController<Qosmetics::WallSwitcherViewController*>();
                        layout = BeatSaberUI::CreateVerticalLayoutGroup(WallSwitcherViewController->get_rectTransform());
                        layout->get_rectTransform()->set_anchoredPosition(UnityEngine::Vector2(0.0f, -48.0f));

                        layoutGameObject = layout->get_gameObject();
                        layoutGameObject->GetComponent<ContentSizeFitter*>()->set_verticalFit(ContentSizeFitter::FitMode::PreferredSize);
                        layoutGameObject->AddComponent<Backgroundable*>()->ApplyBackground(il2cpp_utils::createcsstr("round-rect-panel"));
                        layout->set_padding(UnityEngine::RectOffset::New_ctor(3, 4, 2, 2));

                        menuSubtitle = BeatSaberUI::CreateText(layout->get_transform(), "Qosmetics Walls");
                        menuSubtitle->set_alignment(TextAlignmentOptions::Center);
                        menuSubtitle->set_fontSize(4.8f);
                        break;
                    default:
                        break;
                }
            }
            if(!QosmeticsViewController)
            {
                QosmeticsViewController = BeatSaberUI::CreateViewController<Qosmetics::QosmeticsViewController*>();
            }
            QosmeticsViewController->add_openSubMenu(il2cpp_utils::MakeDelegate<System::Action_1<QuestUI::CustomDataType*>*>(classof(System::Action_1<QuestUI::CustomDataType*>*), this, OnOpenSubMenu));
            ProvideInitialViewControllers(QosmeticsViewController, nullptr, nullptr, nullptr, nullptr);
            ActiveViewController = QosmeticsViewController;
        }
    }

    void QosmeticsFlowCoordinator::BackButtonWasPressed(HMUI::ViewController* topViewController)
    {
        if (ActiveViewController != QosmeticsViewController)
        {
            SetTitle(il2cpp_utils::createcsstr("Qosmetics Settings"), ViewController::AnimationType::Out);
            ReplaceTopViewController(QosmeticsViewController, this, this, nullptr, ViewController::AnimationType::Out, ViewController::AnimationDirection::Horizontal);
            ActiveViewController = QosmeticsViewController;
        }
        else 
        {
            HMUI::FlowCoordinator* main = QuestUI::getModSettingsFlowCoordinator();
            if (main) main->DismissFlowCoordinator(this, HMUI::ViewController::AnimationDirection::Horizontal, nullptr, false);
        }
    }
}