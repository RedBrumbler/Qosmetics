#include "UI/Note/NoteDeletionElement.hpp"
#include "UI/Note/NoteSelectionElement.hpp"
#include "questui/shared/BeatSaberUI.hpp"
DEFINE_TYPE(Qosmetics::UI, NoteDeletionElement);

using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;

namespace Qosmetics::UI
{
    void NoteDeletionElement::Show(Qosmetics::UI::NoteSelectionElement* toDelete)
    {
        this->toDelete = toDelete;
        objectName->set_text(il2cpp_utils::newcsstr("<i>" + toDelete->get_descriptor().get_name() + "</i>"));
        modal->Show(true, true, nullptr);
    }

    void NoteDeletionElement::Confirm()
    {
        toDelete->Delete();
        modal->Hide(true, nullptr);
    }

    void NoteDeletionElement::Cancel()
    {
        modal->Hide(true, nullptr);
    }

    void NoteDeletionElement::Setup()
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