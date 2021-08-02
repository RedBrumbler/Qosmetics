#include "conditional-dependencies/shared/main.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "Containers/SingletonContainer.hpp"
#include "Types/Saber/SaberItem.hpp"
#include "Data/Descriptor.hpp"
#include "static-defines.hpp"

using namespace Qosmetics;

#define NOTEMANAGER SingletonContainer::get_noteManager()
#define NOTECONFIG NOTEMANAGER->get_item().get_config()

EXPOSE_API(GetActiveNoteModel, UnityEngine::GameObject*) {
    return NOTEMANAGER->GetActivePrefab();
}

EXPOSE_API(get_note, UnityEngine::Transform*, int type) {
    switch (type)
    {
        case 0: // left arrow
            return NOTEMANAGER->get_leftArrow();
        case 1: // left dot
            return NOTEMANAGER->get_leftDot();
        case 2: // right arrow
            return NOTEMANAGER->get_rightArrow();
        case 3: // right dot
            return NOTEMANAGER->get_rightDot();
        default:
            return nullptr;
    }
}

EXPOSE_API(get_debris, UnityEngine::Transform*, int type) {
    if (!NOTECONFIG.get_hasDebris()) return nullptr;
    switch (type)
    {
        case 0: // left arrow
            return NOTEMANAGER->get_leftDebris();
        case 1: // left dot
            return NOTEMANAGER->get_rightDebris();
        default:
            return nullptr;
    }
}

EXPOSE_API(get_bomb, UnityEngine::Transform*) {
    if (!NOTECONFIG.get_hasBomb()) return nullptr;
    return NOTEMANAGER->get_bomb();
}

EXPOSE_API(SetDefaultNote, void) {
    NOTEMANAGER->SetDefault();
}

EXPOSE_API(SetActiveNote, void, const char* name) {
    NOTEMANAGER->SetActiveNote(std::string(name), true);
}

EXPOSE_API(SetActiveNoteFromFilePath, void, const char* path) {
    NOTEMANAGER->FromFilePath(std::string(path), true);
}

EXPOSE_API(GetNoteIsCustom, bool) {
    return NOTEMANAGER->get_item().get_descriptor().isValid();
}

EXPOSE_API(GetNoteFolder, void, std::string& result) {
    result = NOTEPATH;
}
#undef NOTEMANAGER
#undef NOTECONFIG