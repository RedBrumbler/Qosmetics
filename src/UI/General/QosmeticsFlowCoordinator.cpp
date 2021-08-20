#include "UI/General/QosmeticsFlowCoordinator.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "HMUI/ViewController_AnimationType.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/ButtonStaticAnimations.hpp"

#include "Utils/UIUtils.hpp"
#include "Utils/TextUtils.hpp"
#include "Utils/DateUtils.hpp"

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

DEFINE_TYPE(Qosmetics::UI, QosmeticsFlowCoordinator);

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
            std::string qosmeticsSettingsCPP = DateUtils::get_isMonth(6) ? TextUtils::rainbowify("Qosmetics Settings") : "Qosmetics Settings";
            std::string saberTitleCPP = DateUtils::get_isMonth(6) ? TextUtils::rainbowify("Qosmetics Sabers") : "Qosmetics Sabers";
            std::string noteTitleCPP = DateUtils::get_isMonth(6) ? TextUtils::rainbowify("Qosmetics Bloqs") : "Qosmetics Bloqs";
            std::string wallTitleCPP = DateUtils::get_isMonth(6) ? TextUtils::rainbowify("Qosmetics Walls") : "Qosmetics Walls";
            
            if (!qosmeticsSettings) qosmeticsSettings = il2cpp_utils::createcsstr(qosmeticsSettingsCPP, il2cpp_utils::StringType::Manual);
            if (!saberTitle) saberTitle = il2cpp_utils::createcsstr(saberTitleCPP, il2cpp_utils::StringType::Manual);
            if (!noteTitle) noteTitle = il2cpp_utils::createcsstr(noteTitleCPP, il2cpp_utils::StringType::Manual);
            if (!wallTitle) wallTitle = il2cpp_utils::createcsstr(wallTitleCPP, il2cpp_utils::StringType::Manual);

            SetTitle(qosmeticsSettings, ViewController::AnimationType::Out);
            showBackButton = true;

            std::function<void(ItemType)> func = std::bind(&QosmeticsFlowCoordinator::SubMenuButtonWasPressed, this, std::placeholders::_1);
            qosmeticsViewController->set_selectCallback(func);

            ProvideInitialViewControllers(qosmeticsViewController, userProfileViewController, patronViewController, floorLogoViewController, nullptr);
        }
        
        patronViewController->get_gameObject()->SetActive(true);
        //SetRightScreenViewController(patronViewController, ViewController::AnimationType::Out);
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
            SetLeftScreenViewController(userProfileViewController, ViewController::AnimationType::Out);
            SetRightScreenViewController(patronViewController, ViewController::AnimationType::Out);
            UIUtils::SetTitleColor(titleView, Color(0.3f, 0.15f, 0.6f, 1.0f));
        }
        else 
        {
            patronViewController->get_gameObject()->SetActive(false);
            SetRightScreenViewController(nullptr, ViewController::AnimationType::Out);
            this->parentFlowCoordinator->DismissFlowCoordinator(this, ViewController::AnimationDirection::Horizontal, nullptr, false);
            UIUtils::SetTitleColor(titleView, Color(0.0f, 0.75f, 1.0f, 1.0f), true);
        }
    }

    void QosmeticsFlowCoordinator::Init(SaberSwitcherViewController* saberSwitcherViewController,
                                        SaberSettingsViewController* saberSettingsViewController,
                                        SaberPreviewViewController* saberPreviewViewController,
                                        NoteSwitcherViewController* noteSwitcherViewController,
                                        NoteSettingsViewController* noteSettingsViewController,
                                        NotePreviewViewController* notePreviewViewController,
                                        WallSwitcherViewController* wallSwitcherViewController,
                                        WallSettingsViewController* wallSettingsViewController,
                                        WallPreviewViewController* wallPreviewViewController,
                                        PatronViewController* patronViewController,
                                        QosmeticsViewController* qosmeticsViewController,
                                        FloorLogoViewController* floorLogoViewController,
                                        UserProfileViewController* userProfileViewController)
    {
        this->saberSwitcherViewController = saberSwitcherViewController;
        this->saberSettingsViewController = saberSettingsViewController;
        this->saberPreviewViewController = saberPreviewViewController;
        
        this->noteSwitcherViewController = noteSwitcherViewController;
        this->noteSettingsViewController = noteSettingsViewController;
        this->notePreviewViewController = notePreviewViewController;

        this->wallSwitcherViewController = wallSwitcherViewController;
        this->wallSettingsViewController = wallSettingsViewController;
        this->wallPreviewViewController = wallPreviewViewController;

        this->patronViewController = patronViewController;
        this->qosmeticsViewController = qosmeticsViewController;
        this->floorLogoViewController = floorLogoViewController;
        this->userProfileViewController = userProfileViewController;
    }
}