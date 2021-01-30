#pragma once
#include "UI/QosmeticsViewController.hpp"
#include "Data/Descriptor.hpp"
namespace Qosmetics
{
    typedef struct MenuButtonClickData {
        QosmeticsViewController* viewController;
        qosmeticsType type;
    } MenuButtonClickData;
}