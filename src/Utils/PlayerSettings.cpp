#include "Utils/PlayerSettings.hpp"

namespace PlayerSettings
{
    bool ghostNotes = false;
    bool disappearingArrows = false;
    bool reduceDebris = false;
    bool smallNotes = false;
    
    bool get_SmallNotes()
    {
        return smallNotes;
    }

    bool get_SpawnNotes()
    {
        return !(ghostNotes || disappearingArrows);
    }

    bool get_SpawnDebris()
    {
        return !reduceDebris;
    }

    void CheckForIllegalModifiers(GlobalNamespace::GameplayModifiers* modifiers)
    {
        ghostNotes = modifiers->get_ghostNotes();
        disappearingArrows = modifiers->get_disappearingArrows();
        smallNotes = modifiers->get_smallCubes();
    }

    void CheckReducedDebris(GlobalNamespace::PlayerSpecificSettings* playerSettings)
    {
        reduceDebris = playerSettings->get_reduceDebris();
    }
}