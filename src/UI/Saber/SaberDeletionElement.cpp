#include "UI/Saber/SaberDeletionElement.hpp"
#include "UI/Saber/SaberSelectionElement.hpp"
#include "questui/shared/BeatSaberUI.hpp"

DEFINE_TYPE(Qosmetics::UI, SaberDeletionElement);

using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;

namespace Qosmetics::UI
{
    void SaberDeletionElement::Show(Qosmetics::UI::SaberSelectionElement* toDelete)
    {
        this->toDelete = toDelete;
        objectName->set_text(il2cpp_utils::createcsstr("<i>" + toDelete->get_descriptor().get_name() + "</i>"));
        modal->Show(true, true, nullptr);
    }

    void SaberDeletionElement::Confirm()
    {
        toDelete->Delete();
        modal->Hide(true, nullptr);
    }

    void SaberDeletionElement::Cancel()
    {
        modal->Hide(true, nullptr);
    }

    void SaberDeletionElement::Setup()
    {
        modal = GetComponent<HMUI::ModalView*>();

        CreateText(get_transform(), "Are you sure you want to delete");

        objectName = CreateText(get_transform(), "PLACEHOLDER");

        auto horizon = CreateHorizontalLayoutGroup(get_transform());

        CreateUIButton(horizon->get_transform(), "<color=#ff0000>confirm</color>", [this]() -> void {
            this->Confirm();
        });

        CreateUIButton(horizon->get_transform(), "<color=#00ff00>cancel</color>", [this]() -> void {
            this->Cancel();
        });
    }
}