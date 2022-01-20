#include "UI/GameplaySetupView.hpp"
#include "FlowCoordinatorRegister_Internal.hpp"
#include "UI/QosmeticsFlowCoordinator.hpp"
#include "UI/QosmeticsViewController.hpp"
#include "questui/shared/ArrayUtil.hpp"
#include "questui/shared/BeatSaberUI.hpp"

#include "GlobalNamespace/OptionsViewController.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "UnityEngine/Resources.hpp"

DEFINE_TYPE(Qosmetics::Core, GameplaySetupView);

using namespace UnityEngine;
using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;

namespace Qosmetics::Core
{
    void GameplaySetupView::FastForwardQosmeticsFlowCoordinator(System::Type* type)
    {
        auto fc = GetMainFlowCoordinator()->YoungestChildFlowCoordinatorOrSelf();

        auto qfc = ArrayUtil::First(Resources::FindObjectsOfTypeAll<Qosmetics::Core::QosmeticsFlowCoordinator*>());
        if (!qfc)
            qfc = CreateFlowCoordinator<Qosmetics::Core::QosmeticsFlowCoordinator*>();

        fc->PresentFlowCoordinator(qfc, nullptr, HMUI::ViewController::AnimationDirection::Horizontal, true, false);
        reinterpret_cast<Qosmetics::Core::QosmeticsViewController*>(qfc->qosmeticsViewController)->PresentFlowCoordinator(type);
    }

    void GameplaySetupView::DidActivate(bool firstActivation)
    {
        if (firstActivation)
        {
            auto vertical = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(get_transform());
            vertical->set_childAlignment(UnityEngine::TextAnchor::UpperCenter);
            vertical->get_rectTransform()->set_anchoredPosition({0, 20});
            auto horizontal = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(vertical->get_transform());
            horizontal->set_childAlignment(UnityEngine::TextAnchor::UpperCenter);

            auto& registrations = Qosmetics::Core::FlowCoordinator::GetRegistrations();

            int count = registrations.size();
            if (count <= 0)
            {
                auto text = CreateText(horizontal->get_transform(), "No Qosmetics modules were installed,\nmake sure to install any of the modules for them to show up here!");
                text->set_alignment(TMPro::TextAlignmentOptions::Center);
            }
            else
            {
                int rows = (count / 3) + 1;
                float buttonSize = 25.0f / rows;
                float spacing = 4.0f / rows;
                horizontal->set_spacing(spacing);
                vertical->set_spacing(spacing);

                int counter = 0;
                horizontal->template GetComponent<UnityEngine::UI::LayoutElement*>()->set_preferredWidth(buttonSize * 3 + spacing * 2);

                for (auto* reg : registrations)
                {
                    if (counter >= 3)
                    {
                        horizontal = CreateHorizontalLayoutGroup(vertical->get_transform());
                        horizontal->set_childAlignment(UnityEngine::TextAnchor::UpperCenter);
                        horizontal->set_spacing(spacing);
                        horizontal->template GetComponent<UnityEngine::UI::LayoutElement*>()->set_preferredWidth(buttonSize * 3 + spacing * 2);
                    }

                    auto btn = CreateUIButton(horizontal->get_transform(), reg->name, "SettingsButton", Vector2(0, 0), Vector2(buttonSize, buttonSize), std::bind(&GameplaySetupView::FastForwardQosmeticsFlowCoordinator, this, reg->get_flowCoordinatorType()));
                    auto sprites = reg->get_sprites();
                    SetButtonSprites(btn, sprites.first, sprites.second);
                    reinterpret_cast<UnityEngine::RectTransform*>(btn->get_transform()->GetChild(0))->set_sizeDelta({buttonSize, buttonSize});

                    counter++;
                }
            }
        }
    }
}