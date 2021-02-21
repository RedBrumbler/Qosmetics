#pragma once

#include "custom-types/shared/macros.hpp"
#include "zenjeqt/shared/Installer.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, GameInstaller, Zenjeqt::Installer,
    DECLARE_CTOR(ctor);
    DECLARE_OVERRIDE_METHOD(void, InstallBindings, il2cpp_utils::FindMethodUnsafe("Zenjeqt", "Installer", "InstallBindings", 0));

    REGISTER_FUNCTION(GameInstaller,
        REGISTER_METHOD(ctor);
        REGISTER_METHOD(InstallBindings);
    )
)