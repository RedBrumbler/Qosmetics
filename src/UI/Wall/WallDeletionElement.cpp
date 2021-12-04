#include "UI/Wall/WallDeletionElement.hpp"
#include "UI/Wall/WallSelectionElement.hpp"
#include "questui/shared/BeatSaberUI.hpp"
DEFINE_TYPE(Qosmetics::UI, WallDeletionElement);

using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;

namespace Qosmetics::UI
{
    void WallDeletionElement::Show(Qosmetics::UI::WallSelectionElement* toDelete)
    {
        this->toDelete = toDelete;
        objectName->set_text(il2cpp_utils::newcsstr("<i>" + toDelete->get_descriptor().get_name() + "</i>"));
        modal->Show(true, true, nullptr);
    }

    void WallDeletionElement::Confirm()
    {
        toDelete->Delete();
        modal->Hide(true, nullptr);
    }

    void WallDeletionElement::Cancel()
    {
        modal->Hide(true, nullptr);
    }

    void WallDeletionElement::Setup()
    {
        modal = GetComponent<HMUI::ModalView*>();

        //UnityEngine::GameObject* container = CreateScrollableModalContainer(modal);
        auto wrapper = CreateHorizontalLayoutGroup(get_transform());
        auto container = CreateVerticalLayoutGroup(wrapper->get_transform());
        container->set_childAlignment(UnityEngine::TextAnchor::MiddleCenter);
        CreateText(container->get_transform(), "Are you sure you want to delete")->set_alignment(TMPro::TextAlignmentOptions::Center);

        objectName = CreateText(container->get_transform(), "PLACEHOLDER");
        objectName->set_alignment(TMPro::TextAlignmentOptions::Center);

        auto horizon = CreateHorizontalLayoutGroup(container->get_transform());

        CreateUIButton(horizon->get_transform(), "<color=#ff0000>confirm</color>", [this]() -> void
                       { this->Confirm(); });

        CreateUIButton(horizon->get_transform(), "<color=#00ff00>cancel</color>", [this]() -> void
                       { this->Cancel(); });
    }
}