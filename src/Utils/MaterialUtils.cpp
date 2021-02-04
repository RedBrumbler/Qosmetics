#include "Utils/MaterialUtils.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp" 

using namespace UnityEngine;

static inline function_ptr_t<void, Il2CppObject*> createFunc = nullptr;
static inline function_ptr_t<bool, Il2CppObject*, Il2CppObject*, int, Array<Il2CppString*>*> addFunc = nullptr;
static inline function_ptr_t<void, Il2CppObject*> warmupFunc = nullptr;
static inline function_ptr_t<Array<UnityEngine::Material*>*, UnityEngine::Renderer*> GetMaterialArrayFunc = nullptr;

Array<UnityEngine::Material*>* MaterialUtils::GetMaterials(Renderer* renderer)
{
    if (!renderer) return nullptr;
    if (!GetMaterialArrayFunc) GetMaterialArrayFunc = reinterpret_cast<function_ptr_t<Array<UnityEngine::Material*>*, UnityEngine::Renderer*>>(il2cpp_functions::resolve_icall("UnityEngine.Renderer::GetMaterialArray"));
    return GetMaterialArrayFunc(renderer);
}

void MaterialUtils::PrewarmAllShadersOnObject(GameObject* object)
{
    Array<UnityEngine::Renderer*>* renderers = object->GetComponentsInChildren<UnityEngine::Renderer*>(true);

    // get material method is stripped so resolve icall
    
    if (!GetMaterialArrayFunc) GetMaterialArrayFunc = reinterpret_cast<function_ptr_t<Array<UnityEngine::Material*>*, UnityEngine::Renderer*>>(il2cpp_functions::resolve_icall("UnityEngine.Renderer::GetMaterialArray"));
    //typedef function_ptr_t<Array<UnityEngine::Material*>*, UnityEngine::Renderer*> GetMaterialArrayFunctionType;
    //auto GetMaterialArray = reinterpret_cast<GetMaterialArrayFunctionType>(il2cpp_functions::resolve_icall("UnityEngine.Renderer::GetMaterialArray"));

    // all shader variant stuff is stripped so resolve icall
    if (!createFunc) createFunc = reinterpret_cast<function_ptr_t<void, Il2CppObject*>>(il2cpp_functions::resolve_icall("UnityEngine.ShaderVariantCollection::Internal_Create"));
    if (!addFunc) addFunc = reinterpret_cast<function_ptr_t<bool, Il2CppObject*, Il2CppObject*, int, Array<Il2CppString*>*>>(il2cpp_functions::resolve_icall("UnityEngine.ShaderVariantCollection::AddVariant"));
    if (!warmupFunc) warmupFunc = reinterpret_cast<function_ptr_t<void, Il2CppObject*>>(il2cpp_functions::resolve_icall("UnityEngine.ShaderVariantCollection::WarmUp"));
    Il2CppObject* obj = UnityEngine::Object::New_ctor();
    createFunc(obj);
    std::vector<Il2CppString*> temp;
    Array<Il2CppString*>* stringArr = il2cpp_utils::vectorToArray(temp);
    for (int i = 0; i < renderers->Length(); i++)
    {
        Array<UnityEngine::Material*>* materials = GetMaterialArrayFunc(renderers->values[i]);
        for (int j = 0; j < materials->Length(); j++)
        {
            addFunc(obj, materials->values[j]->get_shader(), 0, stringArr);
        }
    }
    warmupFunc(obj);
}