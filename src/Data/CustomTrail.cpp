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
        if (renderer == nullptr)
        {
            getLogger().error("trail object renderer was not found, skipping FindMaterial...");
            return;
        }       

        // Create an object of type System.Object. This is just to ensure something exists
        // Assumes that the object created is obj
        /*
        auto createFunc = reinterpret_cast<function_ptr_t<void, Il2CppObject*>>(il2cpp_functions::resolve_icall("UnityEngine.ShaderVariantCollection::Internal_Create")); // or something similar
        auto addFunc = reinterpret_cast<function_ptr_t<bool, Il2CppObject*, Il2CppObject*, int, Array<Il2CppString*>*>>(il2cpp_functions::resolve_icall("UnityEngine.ShaderVariantCollection::AddVariant")); // or something similar
        auto warmupFunc = reinterpret_cast<function_ptr_t<void, Il2CppObject*>>(il2cpp_functions::resolve_icall("UnityEngine.ShaderVariantCollection::WarmUp")); // or something similar
        
        Il2CppObject* obj = UnityEngine::Object::New_ctor();
        createFunc(obj);
        std::vector<Il2CppString*> temp;
        Array<Il2CppString*>* stringArr = il2cpp_utils::vectorToArray(temp);

        addFunc(obj, renderer->get_material()->get_shader(), 0, stringArr);
        warmupFunc(obj);
        */
        // set the locally stored material pointer
        set_material(renderer->get_sharedMaterial());
    }
}
