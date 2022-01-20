#include "Redecoration.hpp"
#include "hooks.hpp"
#include "logging.hpp"

#include "GlobalNamespace/BeatmapObjectsInstaller.hpp"
#include "GlobalNamespace/BombNoteController.hpp"
#include "GlobalNamespace/GameNoteController.hpp"
#include "GlobalNamespace/ObstacleController.hpp"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Object.hpp"
#include "UnityEngine/Transform.hpp"

#define MAKE_CLONE_AND_PARENT(identifier)                                                                                           \
    static UnityEngine::GameObject* parent_clone_##identifier = nullptr;                                                            \
    if (parent_clone_##identifier)                                                                                                  \
    {                                                                                                                               \
        UnityEngine::Object::Destroy(parent_clone_##identifier);                                                                    \
    }                                                                                                                               \
    static auto parent_name_##identifier = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("parent_clone_" #identifier); \
    parent_clone_##identifier = UnityEngine::GameObject::New_ctor(parent_name_##identifier);                                        \
    parent_clone_##identifier->SetActive(false);                                                                                    \
    UnityEngine::Object::DontDestroyOnLoad(parent_clone_##identifier);                                                              \
    UnityEngine::Object::Instantiate(identifier, parent_clone_##identifier->get_transform());                                       \
    auto type_##identifier = il2cpp_utils::GetSystemType(il2cpp_utils::ExtractType(identifier->klass));                             \
    auto clone_##identifier = parent_clone_##identifier->GetComponentsInChildren(type_##identifier)[0];                             \
    reinterpret_cast<UnityEngine::Component*&>(self->dyn__##identifier()) = clone_##identifier;                                     \
    self->dyn__##identifier()->get_gameObject()->SetActive(true)

#define DESTROY_CLONE_PARENT(identifier) \
    UnityEngine::Object::Destroy(parent_clone_##identifier);

#define REDECORATE(identifier) \
    Qosmetics::Core::Redecoration::Redecorate(reinterpret_cast<Il2CppObject*&>(self->dyn__##identifier()), #identifier, type_##identifier, self_type)

MAKE_AUTO_HOOK_ORIG_MATCH(BeatmapObjectsInstaller_InstallBindings, &GlobalNamespace::BeatmapObjectsInstaller::InstallBindings, void, GlobalNamespace::BeatmapObjectsInstaller* self)
{
    auto normalBasicNotePrefab = self->dyn__normalBasicNotePrefab();
    auto proModeNotePrefab = self->dyn__proModeNotePrefab();
    auto bombNotePrefab = self->dyn__bombNotePrefab();
    auto obstaclePrefab = self->dyn__obstaclePrefab();

    MAKE_CLONE_AND_PARENT(normalBasicNotePrefab);
    MAKE_CLONE_AND_PARENT(proModeNotePrefab);
    MAKE_CLONE_AND_PARENT(bombNotePrefab);
    MAKE_CLONE_AND_PARENT(obstaclePrefab);

    static auto self_type = il2cpp_utils::GetSystemType(il2cpp_utils::ExtractType(self->klass));
    REDECORATE(normalBasicNotePrefab);
    REDECORATE(proModeNotePrefab);
    REDECORATE(bombNotePrefab);
    REDECORATE(obstaclePrefab);

    BeatmapObjectsInstaller_InstallBindings(self);

    self->dyn__normalBasicNotePrefab() = normalBasicNotePrefab;
    self->dyn__proModeNotePrefab() = proModeNotePrefab;
    self->dyn__bombNotePrefab() = bombNotePrefab;
    self->dyn__obstaclePrefab() = obstaclePrefab;
}
