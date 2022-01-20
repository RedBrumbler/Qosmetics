#pragma once

#include "HMUI/ModalView.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "custom-types/shared/macros.hpp"

namespace Qosmetics::Core
{
    class QosmeticObjectTableCell;
}

DECLARE_CLASS_CODEGEN(Qosmetics::Core, DeletionConfirmationModal, HMUI::ModalView,
                      DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, objectName);
                      static DeletionConfirmationModal * Create(UnityEngine::Transform * parent);
                      void Show(QosmeticObjectTableCell* cellToDelete);
                      void Dismiss();
                      void Confirm();

                      QosmeticObjectTableCell * currentCell;

)