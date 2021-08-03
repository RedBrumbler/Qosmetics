#pragma once

#include "custom-types/shared/macros.hpp"

#include "HMUI/ModalView.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "TMPro/TextMeshProUGUI.hpp"

namespace Qosmetics::UI {
    class WallSelectionElement;
}
DECLARE_CLASS_CODEGEN(Qosmetics::UI, WallDeletionElement, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD(HMUI::ModalView*, modal);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, objectName);
    public:
        Qosmetics::UI::WallSelectionElement* toDelete;
        void Show(Qosmetics::UI::WallSelectionElement* toDelete);
        void Confirm();
        void Cancel();
        void Setup();
)