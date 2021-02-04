#include "TypeRegisterer.hpp"

#include "Trail/QosmeticsTrail.hpp"
#include "Types/Qosmetic/ModelManager.hpp"
#include "Types/Saber/SaberManager.hpp"
#include "Types/Note/NoteManager.hpp"
#include "Types/Wall/WallManager.hpp"
#include "Types/Pointer/PointerManager.hpp"

#include "Types/Saber/Saber.hpp"
#include "Types/Pointer/Pointer.hpp"

#include "custom-types/shared/register.hpp"

using namespace Qosmetics;

void RegisterTypes()
{
    custom_types::Register::RegisterType<ModelManager>();
    custom_types::Register::RegisterTypes<SaberManager, NoteManager, WallManager, PointerManager>();
    custom_types::Register::RegisterTypes<Qosmetic, QosmeticsTrail>();
    custom_types::Register::RegisterTypes<Saber, Pointer>();
}