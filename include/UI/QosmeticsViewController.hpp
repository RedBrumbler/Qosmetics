#pragma once

#include "HMUI/ViewController.hpp"
#include "UI/QosmeticsFlowCoordinator.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "custom-types/shared/macros.hpp"

#define DECLARE_OVERRIDE_METHOD_MATCH(retval, name, mptr, ...) \
    DECLARE_OVERRIDE_METHOD(retval, name, il2cpp_utils::il2cpp_type_check::MetadataGetter<mptr>::get(), __VA_ARGS__)

DECLARE_CLASS_CODEGEN(Qosmetics::Core, QosmeticsViewController, HMUI::ViewController,
                      DECLARE_OVERRIDE_METHOD_MATCH(void, DidActivate, &HMUI::ViewController::DidActivate, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
                      DECLARE_INSTANCE_METHOD(void, PresentFlowCoordinator, System::Type* type);

                      DECLARE_INSTANCE_FIELD(Qosmetics::Core::QosmeticsFlowCoordinator*, qosmeticsFlowCoordinator);
                      DECLARE_INSTANCE_FIELD(ArrayW<HMUI::FlowCoordinator*>, flowCoordinators);

)