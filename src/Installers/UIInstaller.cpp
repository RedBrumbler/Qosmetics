#include "Installers/UIInstaller.hpp"

#include "QosmeticsLogger.hpp"
#include "Containers/FlowCoordinatorContainer.hpp"
DEFINE_CLASS(Qosmetics::UIInstaller);

#define INFO(value...) QosmeticsLogger::GetContextLogger("UIInstaller").info(value);
#define ERROR(value...) QosmeticsLogger::GetContextLogger("UIInstaller").error(value);

namespace Qosmetics
{
    void UIInstaller::ctor() {};

    void UIInstaller::InstallBindings()
    {
        INFO("Installing UI Bindings");
        container->Bind<FlowCoordinatorContainer*>()
            ->AsSingle()
            ->NonLazy();
    }
}