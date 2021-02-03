#include "Utils/SaberUtils.hpp"
#include "UnityEngine/MeshFilter.hpp"

using namespace UnityEngine;

void DisableMesh(Array<MeshFilter*>* filters, bool enableFakeGlow)
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
            filterGO->SetActive(false);
        }
    }
}

void SaberUtils::HideObjects(GameObject* object, bool enableFakeGlow)
{
    Array<MeshFilter*>* filters = object->GetComponentsInChildren<MeshFilter*>(true);
    DisableMesh(filters, enableFakeGlow);
}

