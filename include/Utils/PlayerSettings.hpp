#pragma once

#include "GlobalNamespace/GameplayModifiers.hpp"
#include "GlobalNamespace/PlayerSpecificSettings.hpp"

namespace PlayerSettings
{
    bool get_SmallNotes();
    bool get_SpawnNotes();
    bool get_SpawnDebris();
    void CheckForIllegalModifiers(GlobalNamespace::GameplayModifiers* modifiers);
    void CheckReducedDebris(GlobalNamespace::PlayerSpecificSettings* playerSettings);
}