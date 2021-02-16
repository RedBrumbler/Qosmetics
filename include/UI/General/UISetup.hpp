#pragma once
#include "GlobalNamespace/OptionsViewController.hpp"
#include "QosmeticsFlowCoordinator.hpp"

namespace Qosmetics::UI
{
    class UISetup
    {
        public:
            enum flowCoordinatorType {
                settings,
                solo,
                party,
                campaign,
                multiHost,
                multiClient,
                invalid
            };

            static void SetupFlowCoordinatorAtSettings(GlobalNamespace::OptionsViewController* self);
            static void set_flowCoordinatorType(flowCoordinatorType type)
            {
                currentFlowCoordinatorType = type;
            }

        private:
            static inline QosmeticsFlowCoordinator* flowCoordinator = nullptr;
            static inline flowCoordinatorType currentFlowCoordinatorType = settings;
            static void OnQosmeticsMenuButtonClick(Il2CppObject* obj);
    };
}