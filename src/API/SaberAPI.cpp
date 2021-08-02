#include "conditional-dependencies/shared/main.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "Containers/SingletonContainer.hpp"
#include "Types/Saber/SaberItem.hpp"
#include "Data/Descriptor.hpp"
#include "static-defines.hpp"
#include "Types/Trail/TrailHelper.hpp"

#include "Utils/SaberUtils.hpp"

#include "QosmeticsLogger.hpp"

#define INFO(value...) QosmeticsLogger::GetContextLogger("SaberAPI").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("SaberAPI").error(value)

using namespace Qosmetics;

#define SABERMANAGER SingletonContainer::get_saberManager()
#define CONFIG SABERMANAGER->get_item().get_config()

EXPOSE_API(GetActiveSaberModel, UnityEngine::GameObject*) {
    return SABERMANAGER->GetActivePrefab();
}

EXPOSE_API(GetDummySaber, UnityEngine::Transform*, int type) {
    return SaberUtils::MakeDummySaber(type);
}

EXPOSE_API(get_saber, UnityEngine::Transform*, int type) {
    switch (type)
    {
        case 0:
            return SABERMANAGER->get_leftSaber();
        case 1:
            return SABERMANAGER->get_rightSaber();
        default:
            return nullptr;
    }
}

EXPOSE_API(SetDefaultSaber, void) {
    SABERMANAGER->SetDefault();
}

EXPOSE_API(SetActiveSaber, void, const char* name) {
    SABERMANAGER->SetActiveSaber(std::string(name), true);
}

EXPOSE_API(SetActiveSaberFromFilePath, void, const char* path) {
    SABERMANAGER->FromFilePath(std::string(path), true);
}

EXPOSE_API(GetSaberIsCustom, bool) {
    return SABERMANAGER->get_item().get_descriptor().isValid();
}

EXPOSE_API(GetSaberFolder, char*) {
    char* temp = new char[SABERPATH.size() + 1];
    strcpy(temp, SABERPATH.c_str());
    return temp;
}

EXPOSE_API(SetTrailActive, void, Il2CppObject* trailHelper, bool active) {
    ((Qosmetics::TrailHelper*)trailHelper)->SetTrailActive(active);
}

#undef SABERMANAGER
#undef CONFIG