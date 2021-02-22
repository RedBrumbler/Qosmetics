#pragma once

#include "custom-types/shared/macros.hpp"
#include "zenjeqt/shared/Installer.hpp"
#include "UnityEngine/GameObject.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, UIInstaller, Zenjeqt::Installer,
    DECLARE_INSTANCE_FIELD_DEFAULT(UnityEngine::GameObject*, viewControllerBase, nullptr);
    DECLARE_CTOR(ctor);
    DECLARE_OVERRIDE_METHOD(void, InstallBindings, il2cpp_utils::FindMethodUnsafe("Zenjeqt", "Installer", "InstallBindings", 0));

    REGISTER_FUNCTION(UIInstaller,
        REGISTER_FIELD(viewControllerBase);
        REGISTER_METHOD(ctor);
        REGISTER_METHOD(InstallBindings);
    )
)