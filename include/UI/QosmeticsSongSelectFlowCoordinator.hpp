#pragma once
#include "HMUI/ViewController.hpp"
#include "HMUI/NavigationController.hpp"
#include "HMUI/FlowCoordinator.hpp"

#include "custom-types/shared/macros.hpp"

#include "UI/QosmeticsFlowCoordinator.hpp"
DECLARE_CLASS_CODEGEN(Qosmetics, QosmeticsSongSelectFlowCoordinator, Qosmetics::QosmeticsFlowCoordinator,
    DECLARE_INSTANCE_FIELD_DEFAULT(HMUI::FlowCoordinator*, previousFlowCoordinator, nullptr);
    DECLARE_OVERRIDE_METHOD(void, BackButtonWasPressed, il2cpp_utils::FindMethodUnsafe("Qosmetics", "QosmeticsFlowCoordinator", "BackButtonWasPressed", 1), HMUI::ViewController* topViewController);

    REGISTER_FUNCTION(QosmeticsSongSelectFlowCoordinator,
        REGISTER_METHOD(BackButtonWasPressed);
        REGISTER_FIELD(previousFlowCoordinator);
    )
)