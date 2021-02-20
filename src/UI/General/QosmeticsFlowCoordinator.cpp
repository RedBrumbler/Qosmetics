#include "UI/General/QosmeticsFlowCoordinator.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "HMUI/ViewController_AnimationType.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/ButtonStaticAnimations.hpp"

#include "Utils/UIUtils.hpp"

#include "QosmeticsLogger.hpp"

using namespace HMUI;
using namespace UnityEngine;
using namespace UnityEngine::Events;
using namespace UnityEngine::UI;
using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;
using namespace Qosmetics;
using namespace Qosmetics::UI;
using namespace TMPro;

DEFINE_CLASS(Qosmetics::UI::QosmeticsFlowCoordinator);

#define INFO(value...) QosmeticsLogger::GetContextLogger("Qosmetics Flow Coordinator").info(value);
#define ERROR(value...) QosmeticsLogger::GetContextLogger("Qosmetics Flow Coordinator").error(value);

static Il2CppString* qosmeticsSettings = nullptr;
static Il2CppString* saberTitle = nullptr;
static Il2CppString* noteTitle = nullptr;
static Il2CppString* wallTitle = nullptr;

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

namespace Qosmetics::UI
{
    void QosmeticsFlowCoordinator::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            qosmeticsSettings = il2cpp_utils::createcsstr("Qosmetics Settings", il2cpp_utils::StringType::Manual);
            saberTitle = il2cpp_utils::createcsstr("Qosmetics Sabers", il2cpp_utils::StringType::Manual);
            noteTitle = il2cpp_utils::createcsstr("Qosmetics Bloqs", il2cpp_utils::StringType::Manual);
            wallTitle = il2cpp_utils::createcsstr("Qosmetics Walls", il2cpp_utils::StringType::Manual);

            SetTitle(qosmeticsSettings, ViewController::AnimationType::Out);
            
            showBackButton = true;

            // saber stuff
            // preview is made first since the other 2 viewcontrollers need a reference to the view controller in order to update the preview after config changes
            saberPreviewViewController = CreateViewController<SaberPreviewViewController*>();
            saberPreviewViewController->Init(saberManager, colorManager);
            saberSwitcherViewController = CreateViewController<SaberSwitcherViewController*>();
            
            saberSwitcherViewController->Init(saberManager, saberPreviewViewController);
            VerticalLayoutGroup* layout = CreateVerticalLayoutGroup(saberSwitcherViewController->get_rectTransform());
            SetupSubTitle(layout, "Qosmetics Sabers");

            saberSettingsViewController = CreateViewController<SaberSettingsViewController*>();

            // note stuff
            noteSwitcherViewController = CreateViewController<NoteSwitcherViewController*>();
            layout = CreateVerticalLayoutGroup(noteSwitcherViewController->get_rectTransform());
            SetupSubTitle(layout, "Qosmetics Bloqs");

            noteSettingsViewController = CreateViewController<NoteSettingsViewController*>();
            notePreviewViewController = CreateViewController<NotePreviewViewController*>();

            // wall stuff
            wallSwitcherViewController = CreateViewController<WallSwitcherViewController*>();
            layout = CreateVerticalLayoutGroup(wallSwitcherViewController->get_rectTransform());
            SetupSubTitle(layout, "Qosmetics Walls");

            wallSettingsViewController = CreateViewController<WallSettingsViewController*>();
            wallPreviewViewController = CreateViewController<WallPreviewViewController*>();

            patronViewController = CreateViewController<PatronViewController*>();
            floorLogoViewController = CreateViewController<FloorLogoViewController*>();

            qosmeticsViewController = CreateViewController<QosmeticsViewController*>();

            std::function<void(ItemType)> func = std::bind(&QosmeticsFlowCoordinator::SubMenuButtonWasPressed, this, std::placeholders::_1);
            qosmeticsViewController->set_selectCallback(func);

            ProvideInitialViewControllers(qosmeticsViewController, nullptr, patronViewController, floorLogoViewController, nullptr);
        }

        TitleViewController* titleView = Object::FindObjectOfType<TitleViewController*>();
        UIUtils::SetTitleColor(titleView, Color(0.3f, 0.15f, 0.6f, 1.0f));
    }

    void QosmeticsFlowCoordinator::SubMenuButtonWasPressed(ItemType type)
    {
        TitleViewController* titleView = Object::FindObjectOfType<TitleViewController*>();

        switch (type)
        {
            case saber:
                SetTitle(saberTitle, ViewController::AnimationType::In);
                ReplaceTopViewController(saberSwitcherViewController, this, this, nullptr, ViewController::AnimationType::In, ViewController::AnimationDirection::Horizontal);
                SetLeftScreenViewController(saberSettingsViewController, ViewController::AnimationType::In);
                SetRightScreenViewController(saberPreviewViewController, ViewController::AnimationType::In);
                UIUtils::SetTitleColor(titleView, Color::get_red());
                break;
            case note:
                SetTitle(noteTitle, ViewController::AnimationType::In);
                ReplaceTopViewController(noteSwitcherViewController, this, this, nullptr, ViewController::AnimationType::In, ViewController::AnimationDirection::Horizontal);
                SetLeftScreenViewController(noteSettingsViewController, ViewController::AnimationType::In);
                SetRightScreenViewController(notePreviewViewController, ViewController::AnimationType::In);
                UIUtils::SetTitleColor(titleView, Color::get_blue());
                break;
            case wall:
                SetTitle(wallTitle, ViewController::AnimationType::In);
                ReplaceTopViewController(wallSwitcherViewController, this, this, nullptr, ViewController::AnimationType::In, ViewController::AnimationDirection::Horizontal);
                SetLeftScreenViewController(wallSettingsViewController, ViewController::AnimationType::In);
                SetRightScreenViewController(wallPreviewViewController, ViewController::AnimationType::In);
                UIUtils::SetTitleColor(titleView, Color::get_magenta());
                break;
            default:
                break;
        }
    }

    void QosmeticsFlowCoordinator::BackButtonWasPressed(HMUI::ViewController* topViewController)
    {
        TitleViewController* titleView = Object::FindObjectOfType<TitleViewController*>();
        if (topViewController != qosmeticsViewController)
        {
            SetTitle(qosmeticsSettings, ViewController::AnimationType::Out);
            ReplaceTopViewController(qosmeticsViewController, this, this, nullptr, ViewController::AnimationType::Out, ViewController::AnimationDirection::Horizontal);
            SetLeftScreenViewController(nullptr, ViewController::AnimationType::Out);
            SetRightScreenViewController(patronViewController, ViewController::AnimationType::Out);
            UIUtils::SetTitleColor(titleView, Color(0.3f, 0.15f, 0.6f, 1.0f));
        }
        else 
        {
            this->parentFlowCoordinator->DismissFlowCoordinator(this, ViewController::AnimationDirection::Horizontal, nullptr, false);
            UIUtils::SetTitleColor(titleView, Color(0.0f, 0.75f, 1.0f, 1.0f), true);
        }
    }

    void QosmeticsFlowCoordinator::Init(Qosmetics::SaberManager* saberManager, Qosmetics::NoteManager* noteManager, Qosmetics::WallManager* wallManager, Qosmetics::ColorManager* colorManager)
    {
        this->saberManager = saberManager;
        this->noteManager = noteManager;
        this->wallManager = wallManager;
        this->colorManager = colorManager;
    }
}