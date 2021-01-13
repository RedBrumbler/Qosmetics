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
            self->SetLeftScreenViewController(self->SaberSettingsViewController, ViewController::AnimationType::In);
            self->SetRightScreenViewController(self->SaberPreviewViewController, ViewController::AnimationType::In);
            self->ActiveViewController = self->SaberSwitcherViewController;
            break;
        case note:
            if (!self->NoteSwitcherViewController) break;
            self->SetTitle(il2cpp_utils::createcsstr("Qosmetics Bloqs"), ViewController::AnimationType::In);
            self->ReplaceTopViewController(self->NoteSwitcherViewController, self, self, nullptr, ViewController::AnimationType::In, ViewController::AnimationDirection::Horizontal);
            self->SetLeftScreenViewController(self->NoteSettingsViewController, ViewController::AnimationType::In);
            self->SetRightScreenViewController(self->NotePreviewViewController, ViewController::AnimationType::In);
            self->ActiveViewController = self->NoteSwitcherViewController;
            break;
        case wall:
            if (!self->WallSwitcherViewController) break;
            self->SetTitle(il2cpp_utils::createcsstr("Qosmetics Walls"), ViewController::AnimationType::In);
            self->ReplaceTopViewController(self->WallSwitcherViewController, self, self, nullptr, ViewController::AnimationType::In, ViewController::AnimationDirection::Horizontal);
            self->SetLeftScreenViewController(self->WallSettingsViewController, ViewController::AnimationType::In);
            self->SetRightScreenViewController(self->WallPreviewViewController, ViewController::AnimationType::In);
            self->ActiveViewController = self->WallSwitcherViewController;
            break;
        default:
            break;
    }
}

VerticalLayoutGroup* SetupSubTitle(VerticalLayoutGroup* layout, std::string subTitle = "")
{
    layout->get_rectTransform()->set_anchoredPosition(UnityEngine::Vector2(0.0f, -48.0f));

    GameObject* layoutGameObject = layout->get_gameObject();
    layoutGameObject->GetComponent<ContentSizeFitter*>()->set_verticalFit(ContentSizeFitter::FitMode::PreferredSize);
    layoutGameObject->AddComponent<Backgroundable*>()->ApplyBackground(il2cpp_utils::createcsstr("round-rect-panel"));
    layout->set_padding(UnityEngine::RectOffset::New_ctor(3, 4, 2, 2));

    if (subTitle != "")
    {
        TextMeshProUGUI* menuSubtitle = BeatSaberUI::CreateText(layout->get_transform(), subTitle);
        menuSubtitle->set_alignment(TextAlignmentOptions::Center);
        menuSubtitle->set_fontSize(4.8f);
    }
    return layout;
}

void FindViewControllers(Qosmetics::QosmeticsFlowCoordinator* self)
{
    // doesn't actually work at all
    self->SaberSwitcherViewController = Object::FindObjectOfType<Qosmetics::SaberSwitcherViewController*>();
    self->SaberSettingsViewController = Object::FindObjectOfType<Qosmetics::SaberSettingsViewController*>();
    self->SaberPreviewViewController = Object::FindObjectOfType<Qosmetics::SaberPreviewViewController*>();

    self->NoteSwitcherViewController = Object::FindObjectOfType<Qosmetics::NoteSwitcherViewController*>();
    self->NoteSettingsViewController = Object::FindObjectOfType<Qosmetics::NoteSettingsViewController*>();
    self->NotePreviewViewController = Object::FindObjectOfType<Qosmetics::NotePreviewViewController*>();

    self->WallSwitcherViewController = Object::FindObjectOfType<Qosmetics::WallSwitcherViewController*>();
    self->WallSettingsViewController = Object::FindObjectOfType<Qosmetics::WallSettingsViewController*>();
    self->WallPreviewViewController = Object::FindObjectOfType<Qosmetics::WallPreviewViewController*>();

    self->QosmeticsViewController = Object::FindObjectOfType<Qosmetics::QosmeticsViewController*>();
}

namespace Qosmetics
{
    void QosmeticsFlowCoordinator::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if(firstActivation) 
        {
            SetTitle(il2cpp_utils::createcsstr("Qosmetics Settings"), ViewController::AnimationType::Out);
            showBackButton = true;

            { // Saber Setup
                SaberSwitcherViewController = BeatSaberUI::CreateViewController<Qosmetics::SaberSwitcherViewController*>();
                SetupSubTitle(BeatSaberUI::CreateVerticalLayoutGroup(SaberSwitcherViewController->get_rectTransform()), "Qosmetics Sabers");

                SaberSettingsViewController = BeatSaberUI::CreateViewController<Qosmetics::SaberSettingsViewController*>();
                SaberPreviewViewController = BeatSaberUI::CreateViewController<Qosmetics::SaberPreviewViewController*>();
                //SetupSubTitle(BeatSaberUI::CreateVerticalLayoutGroup(SaberSwitcherViewController->get_rectTransform()));
            }

            { // Note Setup
                NoteSwitcherViewController = BeatSaberUI::CreateViewController<Qosmetics::NoteSwitcherViewController*>();
                SetupSubTitle(BeatSaberUI::CreateVerticalLayoutGroup(NoteSwitcherViewController->get_rectTransform()), "Qosmetics Bloqs");

                NoteSettingsViewController = BeatSaberUI::CreateViewController<Qosmetics::NoteSettingsViewController*>();
                NotePreviewViewController = BeatSaberUI::CreateViewController<Qosmetics::NotePreviewViewController*>();
                //SetupSubTitle(BeatSaberUI::CreateVerticalLayoutGroup(SaberSwitcherViewController->get_rectTransform()));
            }
            
            { // Wall Setup
                WallSwitcherViewController = BeatSaberUI::CreateViewController<Qosmetics::WallSwitcherViewController*>();
                SetupSubTitle(BeatSaberUI::CreateVerticalLayoutGroup(WallSwitcherViewController->get_rectTransform()), "Qosmetics Walls");

                WallSettingsViewController = BeatSaberUI::CreateViewController<Qosmetics::WallSettingsViewController*>();
                WallPreviewViewController = BeatSaberUI::CreateViewController<Qosmetics::WallPreviewViewController*>();
                //SetupSubTitle(BeatSaberUI::CreateVerticalLayoutGroup(SaberSwitcherViewController->get_rectTransform()));
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
            SetLeftScreenViewController(nullptr, ViewController::AnimationType::Out);
            SetRightScreenViewController(nullptr, ViewController::AnimationType::Out);
            ActiveViewController = QosmeticsViewController;
        }
        else 
        {
            BeatSaberUI::getMainFlowCoordinator()->DismissFlowCoordinator(this, ViewController::AnimationDirection::Horizontal, nullptr, false);
        }
    }
}