#include "Utils/SaberUtils.hpp"
#include "UnityEngine/MeshFilter.hpp"
#include "Utils/MaterialUtils.hpp"
#include "Config.hpp"

using namespace UnityEngine;

extern config_t config;

void DisableMesh(Array<MeshFilter*>* filters, bool enableFakeGlow, bool doHide)
{
    if (!filters) return;

    for(int i = 0; i < filters->Length(); i++)
    {
        MeshFilter* filter = filters->values[i];
        
        GameObject* filterGO = nullptr;
        
        if(filter)
            filterGO = filter->get_gameObject();
        
        if(filterGO)
        {
            std::string name = to_utf8(csstrtostr(filterGO->get_name()));
            if (enableFakeGlow && (name == "FakeGlow0" || name == "FakeGlow1")) continue;
            filterGO->SetActive(!doHide);
        }
    }
}

void SaberUtils::HideObjects(GameObject* object, bool enableFakeGlow, bool doHide)
{
    Array<MeshFilter*>* filters = object->GetComponentsInChildren<MeshFilter*>(true);
    DisableMesh(filters, enableFakeGlow, doHide);
}

void SaberUtils::SetColors(GameObject* object, Color color, Color otherColor)
{
    MaterialUtils::SetColors(object, color, otherColor);
}

void SaberUtils::SetSaberSize(Transform* saber)
{
    if (!saber) return;
    saber->set_localScale(Vector3(config.saberConfig.saberWidth, config.saberConfig.saberWidth, 1.0f));
}
