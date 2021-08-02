#pragma once

#include "custom-types/shared/macros.hpp"

#include "HMUI/ModalView.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "TMPro/TextMeshProUGUI.hpp"

namespace Qosmetics::UI {
    class SaberSelectionElement;
}
DECLARE_CLASS_CODEGEN(Qosmetics::UI, SaberDeletionElement, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD(HMUI::ModalView*, modal);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, objectName);
    public:
        Qosmetics::UI::SaberSelectionElement* toDelete;
        void Show(Qosmetics::UI::SaberSelectionElement* toDelete);
        void Confirm();
        void Cancel();
        void Setup();
)