#include "UI/General/UISetup.hpp"

#include "UnityEngine/Transform.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/GameObject.hpp"

#include "TMPro/TextMeshProUGUI.hpp"
#include "Polyglot/LocalizedTextMeshProUGUI.hpp"

#include "questui/shared/QuestUI.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "UnityEngine/UI/HorizontalLayoutGroup.hpp"
#include "UnityEngine/Events/UnityAction.hpp"

#include "UI/General/QosmeticsFlowCoordinator.hpp"

#include "GlobalNamespace/MainFlowCoordinator.hpp"
#include "GlobalNamespace/SoloFreePlayFlowCoordinator.hpp"
#include "GlobalNamespace/PartyFreePlayFlowCoordinator.hpp"
#include "GlobalNamespace/CampaignFlowCoordinator.hpp"
#include "GlobalNamespace/HostGameServerLobbyFlowCoordinator.hpp"
#include "GlobalNamespace/ClientGameServerLobbyFlowCoordinator.hpp"

#include "HMUI/ViewController_AnimationType.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"

#include "static-defines.hpp"

#include "QosmeticsLogger.hpp"
#include "Containers/SingletonContainer.hpp"

using namespace HMUI;
using namespace UnityEngine;
using namespace UnityEngine::Events;
using namespace UnityEngine::UI;
using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;
using namespace Qosmetics;
using namespace Qosmetics::UI;
using namespace TMPro;

#define INFO(value...) QosmeticsLogger::GetContextLogger("UISetup").info(value);
#define ERROR(value...) QosmeticsLogger::GetContextLogger("UISetup").error(value);

extern void swapButtonSprites(Button* button, std::string normalName, std::string selectedName);

namespace Qosmetics::UI
{
    void UISetup::SetupFlowCoordinatorAtGameplay(GlobalNamespace::GameplaySetupViewController* self)
    {
        INFO("GameplaySetupViewController");
        QuestUI::BeatSaberUI::ClearCache();
        UnityEngine::UI::HorizontalLayoutGroup* layout = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(self->get_transform());
        UnityEngine::RectTransform* rectTransform = layout->get_gameObject()->GetComponent<UnityEngine::RectTransform*>();

        rectTransform->set_anchoredPosition(UnityEngine::Vector2(52.5f, 25.6f));
        rectTransform->set_localScale(UnityEngine::Vector3::get_one() * 0.2f);
        INFO("GameplaySetupViewController making button");

        UnityEngine::UI::Button* button = QuestUI::BeatSaberUI::CreateUIButton(rectTransform, "", "SettingsButton", [&](){
            OnQosmeticsMenuButtonClick(nullptr);
        });

        swapButtonSprites(button, string_format("%s%s", UIPATH.c_str(), "Icons/MenuIcon.png"), string_format("%s%s", UIPATH.c_str(), "Icons/MenuIconSelected.png"));
        INFO("endof GameplaySetupViewController");
    }

    void UISetup::SetupFlowCoordinatorAtSettings(GlobalNamespace::OptionsViewController* self)
    {
        bool questUIExists = QuestUI::GetModsCount() > 0;
        Button* baseButton = self->settingsButton;
        Button* button = Object::Instantiate(baseButton);
        button->set_name(il2cpp_utils::createcsstr("Qosmetics Settings"));

        UnityEngine::Transform* wrapper = self->get_transform()->Find(il2cpp_utils::createcsstr("Wrapper"));
        button->get_transform()->SetParent(wrapper, false);

        if (questUIExists)
        {
            HorizontalLayoutGroup* layout = CreateHorizontalLayoutGroup(self->get_transform());
            
            Transform* layoutTransform = layout->get_transform();
            self->editAvatarButton->get_transform()->SetParent(layoutTransform);
            self->playerOptionsButton->get_transform()->SetParent(layoutTransform);
            self->settingsButton->get_transform()->SetParent(layoutTransform);

            layoutTransform->SetAsFirstSibling();
            layout->set_spacing(-64.0f);
            layout->get_gameObject()->GetComponent<UnityEngine::RectTransform*>()->set_anchoredPosition(UnityEngine::Vector2(0.0f, -7.5f));

            HorizontalLayoutGroup* oldLayout = wrapper->get_gameObject()->GetComponent<HorizontalLayoutGroup*>();
            oldLayout->get_gameObject()->GetComponent<RectTransform*>()->set_anchoredPosition(UnityEngine::Vector2(0.0f, 0.0f));
            button->get_transform()->SetAsLastSibling();
        }
        else button->get_transform()->SetAsFirstSibling();

        button->get_onClick()->AddListener(il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(classof(UnityEngine::Events::UnityAction*), (Il2CppObject*)nullptr, OnQosmeticsMenuButtonClick));

        std::string mainPath = UIPATH;
        swapButtonSprites(button, string_format("%s%s", mainPath.c_str(), "Icons/MenuIcon.png"), string_format("%s%s", mainPath.c_str(), "Icons/MenuIconSelected.png"));

        UnityEngine::Object::Destroy(button->GetComponentInChildren<Polyglot::LocalizedTextMeshProUGUI*>());

        button->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->SetText(il2cpp_utils::createcsstr("Qosmetics Settings"));
    }
    
    void UISetup::OnQosmeticsMenuButtonClick(Il2CppObject* obj)
    {        
        flowCoordinator = SingletonContainer::get_qosmeticsFlowCoordinator();

        if (!flowCoordinator) 
        {
            ERROR("Could not find an instance of QosmeticsFlowCoordinator, returning early");
            return;
        }
        
        HMUI::FlowCoordinator* currentCoordinator = nullptr;
        switch (currentFlowCoordinatorType)
        {
            case settings:
                currentCoordinator = reinterpret_cast<HMUI::FlowCoordinator*>(UnityEngine::Object::FindObjectOfType<GlobalNamespace::MainFlowCoordinator*>());
                break;
            case solo:
                currentCoordinator = reinterpret_cast<HMUI::FlowCoordinator*>(UnityEngine::Object::FindObjectOfType<GlobalNamespace::SoloFreePlayFlowCoordinator*>());
                break;
            case party:
                currentCoordinator = reinterpret_cast<HMUI::FlowCoordinator*>(UnityEngine::Object::FindObjectOfType<GlobalNamespace::PartyFreePlayFlowCoordinator*>());
                break;
            case campaign:
                currentCoordinator = reinterpret_cast<HMUI::FlowCoordinator*>(UnityEngine::Object::FindObjectOfType<GlobalNamespace::CampaignFlowCoordinator*>());
                break;
            case multiHost:
                currentCoordinator = reinterpret_cast<HMUI::FlowCoordinator*>(UnityEngine::Object::FindObjectOfType<GlobalNamespace::HostGameServerLobbyFlowCoordinator*>());
                break;
            case multiClient:
                currentCoordinator = reinterpret_cast<HMUI::FlowCoordinator*>(UnityEngine::Object::FindObjectOfType<GlobalNamespace::ClientGameServerLobbyFlowCoordinator*>());
                break;
            default:
                break;
        }

        if (currentCoordinator) currentCoordinator->PresentFlowCoordinator(flowCoordinator, nullptr, HMUI::ViewController::AnimationDirection::Horizontal, false, false);
    }
}