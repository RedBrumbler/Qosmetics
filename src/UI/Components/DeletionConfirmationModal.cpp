#include "UI/Components/DeletionConfirmationModal.hpp"
#include "UI/Components/QosmeticObjectTableCell.hpp"
#include "questui/shared/BeatSaberUI.hpp"

#include "System/Action.hpp"

DEFINE_TYPE(Qosmetics::Core, DeletionConfirmationModal);

using namespace QuestUI::BeatSaberUI;
using namespace UnityEngine;

#define MakeDelegate(DelegateType, varName) (il2cpp_utils::MakeDelegate<DelegateType>(classof(DelegateType), varName))

namespace Qosmetics::Core
{
    DeletionConfirmationModal* DeletionConfirmationModal::Create(UnityEngine::Transform* parent)
    {
        auto baseModal = CreateModal(parent, Vector2(80, 60), nullptr, true);
        auto go = baseModal->get_gameObject();
        auto presentPanelAnimations = baseModal->dyn__presentPanelAnimations();
        auto dismissPanelAnimation = baseModal->dyn__dismissPanelAnimation();
        auto container = baseModal->dyn__container();

        Object::DestroyImmediate(baseModal);
        auto modal = go->AddComponent<DeletionConfirmationModal*>();
        modal->dyn__presentPanelAnimations() = presentPanelAnimations;
        modal->dyn__dismissPanelAnimation() = dismissPanelAnimation;
        modal->dyn__container() = container;

        std::function<void()> onBlockerClicked = std::bind(&DeletionConfirmationModal::Dismiss, modal);
        modal->add_blockerClickedEvent(MakeDelegate(System::Action*, onBlockerClicked));
        return modal;
    }

    void DeletionConfirmationModal::Show(QosmeticObjectTableCell* cellToDelete)
    {
        currentCell = cellToDelete;
        objectName->set_text(il2cpp_utils::newcsstr(currentCell->descriptor.get_name()));
        this->HMUI::ModalView::Show(true, true, nullptr);
    }

    void DeletionConfirmationModal::Dismiss()
    {
        /// TODO: any other things that need to be done ?
        Hide(true, nullptr);
    }

    void DeletionConfirmationModal::Confirm()
    {
        currentCell->Delete();
    }
}