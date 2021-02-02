#include "TypeRegisterer.hpp"

#include "Data/ModelManager.hpp"
#include "Saber/SaberManager.hpp"
#include "Note/NoteManager.hpp"
#include "Wall/WallManager.hpp"
#include "custom-types/shared/register.hpp"

using namespace Qosmetics;

void RegisterTypes()
{
    custom_types::Register::RegisterType<ModelManager>();
    custom_types::Register::RegisterTypes<SaberManager, NoteManager, WallManager>();
}