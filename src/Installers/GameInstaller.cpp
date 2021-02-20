#include "Installers/GameInstaller.hpp"

#include "Types/Saber/SaberManager.hpp"
#include "Types/Note/NoteManager.hpp"
#include "Types/Wall/WallManager.hpp"
#include "Types/Colors/ColorManager.hpp"

DEFINE_CLASS(Qosmetics::GameInstaller);

namespace Qosmetics
{
    void GameInstaller::ctor() {};

    void GameInstaller::InstallBindings()
    {
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