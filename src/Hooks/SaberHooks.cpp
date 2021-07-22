#include "QosmeticsLogger.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"

#include "Types/Saber/Saber.hpp"
#include "Types/Pointer/Pointer.hpp"
#include "Types/Trail/QosmeticsTrail.hpp"

#include "Utils/UnityUtils.hpp"

#include "UnityEngine/MeshFilter.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/Transform.hpp"

#include "GlobalNamespace/ConditionalMaterialSwitcher.hpp"
#include "GlobalNamespace/SaberModelContainer.hpp"
#include "GlobalNamespace/SaberModelController.hpp"
#include "GlobalNamespace/Saber.hpp"
#include "GlobalNamespace/SaberTrailRenderer.hpp"
#include "GlobalNamespace/VRController.hpp"
#include "GlobalNamespace/GamePause.hpp"

#include <map>

#define INFO(value...) QosmeticsLogger::GetLogger().info(value);
#define ERROR(value...) QosmeticsLogger::GetLogger().error(value);

using namespace Qosmetics;
using namespace UnityEngine;

#include "Containers/SingletonContainer.hpp"

extern bool atLeastMenu;

// fix for trail renderers not getting these set on time
MAKE_HOOK_MATCH(SaberTrailRenderer_OnEnable, &GlobalNamespace::SaberTrailRenderer::OnEnable, void, GlobalNamespace::SaberTrailRenderer* self)
{
    if (!self->meshRenderer)self->meshRenderer = self->get_gameObject()->GetComponent<MeshRenderer*>();
    if (!self->meshFilter) self->meshFilter = self->get_gameObject()->GetComponent<MeshFilter*>();
    SaberTrailRenderer_OnEnable(self);
}

// creates saber to use for left or right things which is poggers
MAKE_HOOK_MATCH(SaberModelContainer_Start, &GlobalNamespace::SaberModelContainer::Start, void, GlobalNamespace::SaberModelContainer* self)
{
    SaberModelContainer_Start(self);

    INFO("SaberModelContainer");
    // if for some fucken reason the saber is nulltpr dont try to use it
    if (!self->saber) 
    {
        ERROR("Saber Was nullptr");
        return;
    }

    if (atLeastMenu)
    {
        INFO("Replacing...");
        Qosmetics::Saber* customSaber = UnityUtils::GetAddComponent<Qosmetics::Saber*>(self->saber->get_gameObject());
        customSaber->Init(SingletonContainer::get_saberManager(), SingletonContainer::get_colorManager());
    }
}

MAKE_HOOK_MATCH(ConditionalMaterialSwitcher_Awake, &GlobalNamespace::ConditionalMaterialSwitcher::Awake, void, GlobalNamespace::ConditionalMaterialSwitcher* self)
{
    // basically QuestTrailOverlap is forced with this
    Transform* trailTransform = self->get_transform();
    std::string thisName = to_utf8(csstrtostr(trailTransform->get_gameObject()->get_name()));
    if (thisName == "Trail(Clone)") return;
    ConditionalMaterialSwitcher_Awake(self);
}

/// unused
MAKE_HOOK_FIND(VRController_ctor, classof(GlobalNamespace::VRController*), ".ctor", void, GlobalNamespace::VRController* self)
{
    VRController_ctor(self);
    INFO("VRController ctor: %d", self->get_node().value);
    
    // left or right hand controller
    if (self->get_node().value == 4 || self->get_node().value == 5)
    {
        INFO("Adding Pointer Component");
        Qosmetics::Pointer* pointer = self->get_gameObject()->AddComponent<Qosmetics::Pointer*>();
        pointer->Init(SingletonContainer::get_saberManager(), SingletonContainer::get_colorManager());
        pointer->vrController = self;
    
        if (atLeastMenu)
        {
            pointer->Replace();
        }
    }
}

MAKE_HOOK_MATCH(GamePause_Pause, &GlobalNamespace::GamePause::Pause, void, GlobalNamespace::GamePause* self)
{
    GamePause_Pause(self);
    return;
    Pointer::AddToAll();

    Array<Qosmetics::Pointer*>* pointers = Resources::FindObjectsOfTypeAll<Qosmetics::Pointer*>();

    int pointerLength = pointers->Length();
    for (int i = 0; i < pointerLength; i++)
    {
        pointers->values[i]->OnApplicationFocus(true);
    }
}

MAKE_HOOK_MATCH(GamePause_Resume, &GlobalNamespace::GamePause::Resume, void, GlobalNamespace::GamePause* self)
{
    GamePause_Resume(self);
    return;
    Array<Qosmetics::Pointer*>* pointers = Resources::FindObjectsOfTypeAll<Qosmetics::Pointer*>();

    int pointerLength = pointers->Length();
    for (int i = 0; i < pointerLength; i++)
    {
        pointers->values[i]->OnApplicationFocus(false);
    }
}

void installSaberHooks(LoggerContextObject& logger)
{
    INSTALL_HOOK(logger, ConditionalMaterialSwitcher_Awake); 
    INSTALL_HOOK(logger, SaberModelContainer_Start);
    INSTALL_HOOK(logger, SaberTrailRenderer_OnEnable);
    INSTALL_HOOK(logger, GamePause_Pause);
    INSTALL_HOOK(logger, GamePause_Resume);
    //INSTALL_HOOK(logger, VRController_ctor);
}