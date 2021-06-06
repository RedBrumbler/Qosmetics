#include "Config.hpp"
#include "QosmeticsLogger.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"

#include "Types/Note/Note.hpp"
#include "Types/Note/Bomb.hpp"
#include "Types/Note/Debris.hpp"

#include "Containers/SingletonContainer.hpp"

#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Vector4.hpp"
#include "UnityEngine/Quaternion.hpp"

#include "GlobalNamespace/ICubeNoteMirrorable.hpp"
#include "GlobalNamespace/MirroredBombNoteController.hpp"
#include "GlobalNamespace/MirroredCubeNoteController.hpp"
#include "GlobalNamespace/MirroredNoteController_1.hpp"

#include "GlobalNamespace/BombNoteController.hpp"
#include "GlobalNamespace/TutorialNoteController.hpp"
#include "GlobalNamespace/GameNoteController.hpp"
#include "GlobalNamespace/GameNoteController_GameNoteType.hpp"
#include "GlobalNamespace/NoteData.hpp"
#include "GlobalNamespace/NoteDebris.hpp"
#include "GlobalNamespace/BeatmapObjectType.hpp"

#include "Utils/UnityUtils.hpp"
#include "Utils/PlayerSettings.hpp"

#define INFO(value...) QosmeticsLogger::GetLogger().info(value);
#define ERROR(value...) QosmeticsLogger::GetLogger().error(value);

using namespace Qosmetics;
using namespace UnityEngine;

MAKE_HOOK_OFFSETLESS(GameNoteController_Init, void, GlobalNamespace::GameNoteController* self, GlobalNamespace::NoteData* noteData, float worldRotation, Vector3 moveStartPos, Vector3 moveEndPos, Vector3 jumpEndPos, float moveDuration, float jumpDuration, float jumpGravity, GlobalNamespace::GameNoteController_GameNoteType gameNoteType, float cutDirectionAngleOffset, float cutAngleTolerance, float uniformScale)
{
    GameNoteController_Init(self, noteData, worldRotation, moveStartPos, moveEndPos, jumpEndPos, moveDuration, jumpDuration, jumpGravity, gameNoteType, cutDirectionAngleOffset, cutAngleTolerance, uniformScale);
    
    if (!PlayerSettings::get_SpawnNotes()) return;

    Qosmetics::Note* note = self->get_gameObject()->GetComponent<Qosmetics::Note*>();

    if (!note)
    {
        note = self->get_gameObject()->AddComponent<Qosmetics::Note*>();
        note->Init(SingletonContainer::get_noteManager(), SingletonContainer::get_colorManager());
    }

    note->gameNoteController = self;
    note->Replace();
}

MAKE_HOOK_OFFSETLESS(TutorialNoteController_Init, void, GlobalNamespace::TutorialNoteController* self, GlobalNamespace::NoteData* noteData, float worldRotation, UnityEngine::Vector3 moveStartPos, UnityEngine::Vector3 moveEndPos, UnityEngine::Vector3 jumpEndPos, float moveDuration, float jumpDuration, float jumpGravity, float cutDirectionAngleOffset, float cutAngleTolerance, float uniformScale)
{
    TutorialNoteController_Init(self, noteData, worldRotation, moveStartPos, moveEndPos, jumpEndPos, moveDuration, jumpDuration, jumpGravity, cutDirectionAngleOffset, cutAngleTolerance, uniformScale);

    Qosmetics::Note* note = self->get_gameObject()->GetComponent<Qosmetics::Note*>();

    if (!note)
    {
        note = self->get_gameObject()->AddComponent<Qosmetics::Note*>();
        note->Init(SingletonContainer::get_noteManager(), SingletonContainer::get_colorManager());
    }

    note->gameNoteController = self;
    note->Replace();
}

MAKE_HOOK_OFFSETLESS(BombNoteController_Init, void, GlobalNamespace::BombNoteController* self, GlobalNamespace::NoteData* noteData, float worldRotation, Vector3 moveStartPos, Vector3 moveEndPos, Vector3 jumpEndPos, float moveDuration, float jumpDuration, float jumpGravity)
{
    BombNoteController_Init(self, noteData, worldRotation, moveStartPos, moveEndPos, jumpEndPos, moveDuration, jumpDuration, jumpGravity);
    
    if (!PlayerSettings::get_SpawnNotes()) return;

    Qosmetics::Bomb* bomb = self->get_gameObject()->GetComponent<Qosmetics::Bomb*>();

    if (!bomb)
    {
        bomb = self->get_gameObject()->AddComponent<Qosmetics::Bomb*>();
        bomb->Init(SingletonContainer::get_noteManager(), SingletonContainer::get_colorManager());
    }

    bomb->Replace();
}

static Il2CppString* MeshName = nullptr;

MAKE_HOOK_OFFSETLESS(MirroredBombNoteController_Mirror, void, GlobalNamespace::MirroredBombNoteController* self, GlobalNamespace::NoteController* noteController)
{
    MirroredBombNoteController_Mirror(self, noteController);
    NoteItem& item = SingletonContainer::get_noteManager()->get_item();
    if (config.noteConfig.disableReflections || (item.get_type() != ItemType::invalid && item.get_config().get_hasBomb() && !config.noteConfig.forceDefaultBombs))
    {
        self->set_enabled(false);
        return;
    }

    if (!PlayerSettings::get_SpawnNotes()) return;

    // only bombs have a child Mesh, this can be used to identify them
    if (!MeshName) MeshName = il2cpp_utils::createcsstr("Mesh", il2cpp_utils::StringType::Manual);
    if (!self->get_transform()->Find(MeshName)) return;

    Qosmetics::Bomb* bomb = self->get_gameObject()->GetComponent<Qosmetics::Bomb*>();

    if (!bomb)
    {
        bomb = self->get_gameObject()->AddComponent<Qosmetics::Bomb*>();
        bomb->Init(SingletonContainer::get_noteManager(), SingletonContainer::get_colorManager());
        bomb->isMirror = true;
    }

    bomb->Replace();
}

MAKE_HOOK_OFFSETLESS(MirroredCubeNoteController_Mirror, void, GlobalNamespace::MirroredCubeNoteController* self, GlobalNamespace::ICubeNoteMirrorable* noteController)
{
    MirroredCubeNoteController_Mirror(self, noteController);
    if (config.noteConfig.disableReflections || SingletonContainer::get_noteManager()->get_item().get_type() != ItemType::invalid)
    {
        self->set_enabled(false);
        return;
    }

    if (!PlayerSettings::get_SpawnNotes()) return;

    Qosmetics::Note* note = self->get_gameObject()->GetComponent<Qosmetics::Note*>();

    if (!note)
    {
        note = self->get_gameObject()->AddComponent<Qosmetics::Note*>();
        note->Init(SingletonContainer::get_noteManager(), SingletonContainer::get_colorManager());
        note->isMirror = true;
    }

    note->gameNoteController = reinterpret_cast<GlobalNamespace::NoteController*>(noteController);
    note->Replace();
}

MAKE_HOOK_OFFSETLESS(NoteDebris_Init, void, GlobalNamespace::NoteDebris* self, GlobalNamespace::ColorType colorType, Vector3 notePos, Quaternion noteRot, Vector3 noteMoveVec, Vector3 noteScale, Vector3 positionOffset, Quaternion rotationOffset, Vector3 cutPoint, Vector3 cutNormal, Vector3 force, Vector3 torque, float lifeTime)
{
    NoteDebris_Init(self, colorType, notePos, noteRot, noteMoveVec, noteScale, positionOffset, rotationOffset, cutPoint, cutNormal, force, torque, lifeTime);
    if (!PlayerSettings::get_SpawnDebris()) return;
    if (!PlayerSettings::get_SpawnNotes()) return;

    Qosmetics::Debris* debris = self->get_gameObject()->GetComponent<Qosmetics::Debris*>();

    if (!debris)
    {
        debris = self->get_gameObject()->AddComponent<Qosmetics::Debris*>();
        debris->Init(SingletonContainer::get_noteManager(), SingletonContainer::get_colorManager());
    }
    
    debris->SetData(colorType, notePos + cutPoint, notePos + cutNormal);
    debris->Replace();
}

void installNoteHooks(LoggerContextObject& logger)
{
    INSTALL_HOOK_OFFSETLESS(logger, GameNoteController_Init, il2cpp_utils::FindMethodUnsafe("", "GameNoteController", "Init", 12));
    INSTALL_HOOK_OFFSETLESS(logger, TutorialNoteController_Init, il2cpp_utils::FindMethodUnsafe("", "TutorialNoteController", "Init", 11));
    INSTALL_HOOK_OFFSETLESS(logger, MirroredCubeNoteController_Mirror, il2cpp_utils::FindMethodUnsafe("", "MirroredCubeNoteController", "Mirror", 1));
    INSTALL_HOOK_OFFSETLESS(logger, BombNoteController_Init, il2cpp_utils::FindMethodUnsafe("", "BombNoteController", "Init", 8));
    INSTALL_HOOK_OFFSETLESS(logger, MirroredBombNoteController_Mirror, il2cpp_utils::FindMethodUnsafe("", "MirroredBombNoteController", "Mirror", 1));
    INSTALL_HOOK_OFFSETLESS(logger, NoteDebris_Init, il2cpp_utils::FindMethodUnsafe("", "NoteDebris", "Init", 12));
}