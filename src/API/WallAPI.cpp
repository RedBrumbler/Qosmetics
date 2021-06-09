#include "conditional-dependencies/shared/main.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "Containers/SingletonContainer.hpp"
#include "Types/Saber/SaberItem.hpp"
#include "Data/Descriptor.hpp"
#include "static-defines.hpp"

using namespace Qosmetics;

#define WALLMANAGER SingletonContainer::get_wallManager()
#define WALLCONFIG WALLMANAGER->get_item().get_config()

EXPOSE_API(GetActiveWallModel, UnityEngine::GameObject*) {
    return WALLMANAGER->GetActivePrefab();
}

EXPOSE_API(get_coreMaterials, Array<UnityEngine::Material*>*) {
    if (!WALLCONFIG.get_replaceCoreMaterial()) return nullptr;
    return WALLMANAGER->get_coreMaterialArray();
}

EXPOSE_API(get_frameMaterials, Array<UnityEngine::Material*>*) {
    if (!WALLCONFIG.get_replaceFrameMaterial()) return nullptr;
    return WALLMANAGER->get_frameMaterialArray();
}

EXPOSE_API(get_coreMesh, UnityEngine::Mesh*) {
    if (!WALLCONFIG.get_replaceCoreMesh()) return nullptr;
    return WALLMANAGER->get_coreMesh();
}

EXPOSE_API(get_frameMesh, UnityEngine::Mesh*) {
    if (!WALLCONFIG.get_replaceFrameMesh()) return nullptr;
    return WALLMANAGER->get_frameMesh();
}

EXPOSE_API(SetDefaultWall, void) {
    WALLMANAGER->SetDefault();
}

EXPOSE_API(SetActiveWall, void, const char* name) {
    WALLMANAGER->SetActiveWall(std::string(name), true);
}

EXPOSE_API(SetActiveWallFromFilePath, void, const char* path) {
    WALLMANAGER->FromFilePath(std::string(path), true);
}

EXPOSE_API(GetWallIsCustom, bool) {
    return WALLMANAGER->get_item().get_descriptor().isValid();
}

EXPOSE_API(GetWallFolder, char*) {
    char* temp = new char[WALLPATH.size() + 1];
    strcpy(temp, WALLPATH.c_str());
    return temp;
}

#undef WALLMANAGER
#undef WALLCONFIG