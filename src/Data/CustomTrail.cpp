#include "Data/CustomTrail.hpp"
#include "UnityEngine/MeshFilter.hpp"
#include "UnityEngine/Mesh.hpp"
#include "UnityEngine/Shader.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"

namespace Qosmetics
{
    void CustomTrail::FindMaterial(UnityEngine::Transform* saber)
    {
        // get the trail object from the given transform, which is done by getting the GO at the objectpath
        UnityEngine::Transform* trailObject = saber->Find(il2cpp_utils::createcsstr(get_objectPath()));
        
        // if not found we can't take a material off, so return
        if (trailObject == nullptr) 
        {
            getLogger().error("Trail object was not found, skipping FindMaterial...");
            return;
        }

        // find the renderer on the trail object to get the material from
        UnityEngine::MeshRenderer* renderer = UnityUtils::GetComponent<UnityEngine::MeshRenderer*>(trailObject->get_gameObject(), "UnityEngine", "Renderer");

        // if the renderer is not found then we can't get a material so just return
        if (!renderer)
        {
            getLogger().error("trail object renderer was not found, skipping FindMaterial...");
            return;
        }       

        // set the locally stored material pointer
        set_material(renderer->get_sharedMaterial());
    }
}
