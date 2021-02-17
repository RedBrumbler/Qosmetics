#pragma once
#include "GlobalNamespace/OptionsViewController.hpp"
#include "QosmeticsFlowCoordinator.hpp"
#include "Types/Saber/SaberManager.hpp"
#include "Types/Note/NoteManager.hpp"
#include "Types/Wall/WallManager.hpp"

#include "Types/Colors/ColorManager.hpp"

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
            static void Init(Qosmetics::SaberManager* saberManager, Qosmetics::NoteManager* noteManager, Qosmetics::WallManager* wallManager, Qosmetics::ColorManager* colorManager)
            {
                UISetup::saberManager = saberManager;
                UISetup::noteManager = noteManager;
                UISetup::wallManager = wallManager;
                UISetup::colorManager = colorManager;
            }

            static void set_flowCoordinatorType(flowCoordinatorType type)
            {
                currentFlowCoordinatorType = type;
            }

        private:
            static inline Qosmetics::SaberManager* saberManager = nullptr;
            static inline Qosmetics::NoteManager* noteManager = nullptr;
            static inline Qosmetics::WallManager* wallManager = nullptr;
            static inline Qosmetics::ColorManager* colorManager = nullptr;
            static inline QosmeticsFlowCoordinator* flowCoordinator = nullptr;
            static inline flowCoordinatorType currentFlowCoordinatorType = settings;
            static void OnQosmeticsMenuButtonClick(Il2CppObject* obj);
    };
}