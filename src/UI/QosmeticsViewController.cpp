#include "UI/QosmeticsViewController.hpp"

#include "FlowCoordinatorRegister_Internal.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "questui/shared/BeatSaberUI.hpp"

DEFINE_TYPE(Qosmetics::Core, QosmeticsViewController);

using namespace QuestUI::BeatSaberUI;

namespace Qosmetics::Core
{
    void QosmeticsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            auto& registrations = Qosmetics::Core::FlowCoordinator::GetRegistrations();

            auto horizontal = CreateHorizontalLayoutGroup(get_transform());
            if (registrations.size() == 0)
            {
                auto text = CreateText(horizontal->get_transform(), "No Qosmetics modules were installed,\nmake sure to install any of the modules for them to show up here!");
                text->set_alignment(TMPro::TextAlignmentOptions::Center);
            }
            else
            {
                for (auto* reg : registrations)
                {
                    auto btn = CreateUIButton(horizontal->get_transform(), reg->name, "SettingsButton", {0, 0}, {0, 0}, std::bind(&QosmeticsViewController::PresentFlowCoordinator, this, reg->get_flowCoordinatorType()));
                    auto sprites = reg->get_sprites();
                    SetButtonSprites(btn, sprites.first, sprites.second);
                }
            }

            flowCoordinators = Array<HMUI::FlowCoordinator*>::NewLength(registrations.size());
            for (auto& ptr : flowCoordinators)
                ptr = nullptr;
        }
    }

    void QosmeticsViewController::PresentFlowCoordinator(System::Type* type)
    {
        auto& registrations = Qosmetics::Core::FlowCoordinator::GetRegistrations();
        for (int i = 0; i < registrations.size(); i++)
        {
            auto* reg = registrations[i];
            auto* fcType = reg->get_flowCoordinatorType();
            if (fcType == type)
            {
                if (flowCoordinators[i] == nullptr)
                {
                    flowCoordinators[i] = CreateFlowCoordinator(fcType);
                }

                qosmeticsFlowCoordinator->PresentFlowCoordinator(flowCoordinators[i], nullptr, HMUI::ViewController::AnimationDirection::Horizontal, false, false);

                break;
            }
        }
    }
}