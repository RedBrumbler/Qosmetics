#include "Installers/GameInstaller.hpp"

#include "Types/Saber/SaberManager.hpp"
#include "Types/Note/NoteManager.hpp"
#include "Types/Wall/WallManager.hpp"
#include "Types/Colors/ColorManager.hpp"
#include "QosmeticsLogger.hpp"

DEFINE_CLASS(Qosmetics::GameInstaller);

#define INFO(value...) QosmeticsLogger::GetContextLogger("GameInstaller").info(value);
#define ERROR(value...) QosmeticsLogger::GetContextLogger("GameInstaller").error(value);

namespace Qosmetics
{
    void GameInstaller::ctor() {};

    void GameInstaller::InstallBindings()
    {
        INFO("Installing Game Bindings");
        container->Bind<SaberManager*>()
            ->AsSingle()
            ->NonLazy();
        container->Bind<NoteManager*>()
            ->AsSingle()
            ->NonLazy();
        container->Bind<WallManager*>()
            ->AsSingle()
            ->NonLazy();
        container->Bind<ColorManager*>()
            ->AsSingle()
            ->NonLazy();
    }
}