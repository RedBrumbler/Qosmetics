#include "Utils/MaterialUtils.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp" 
#include "UnityEngine/Shader.hpp"

using namespace UnityEngine;

static inline function_ptr_t<void, Il2CppObject*> createFunc = nullptr;
static inline function_ptr_t<bool, Il2CppObject*, Il2CppObject*, int, Array<Il2CppString*>*> addFunc = nullptr;
static inline function_ptr_t<void, Il2CppObject*> warmupFunc = nullptr;
static inline function_ptr_t<Array<UnityEngine::Material*>*, UnityEngine::Renderer*> GetMaterialArrayFunc = nullptr;

static int CustomColorID = 0;
static int GlowID = 0;
static int BloomID = 0;
static int ColorID = 0;
static int OtherColorID = 0;

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

bool MaterialUtils::ShouldCC(UnityEngine::Material* mat, bool checkName)
{
    if (!CustomColorID) Shader::PropertyToID(il2cpp_utils::createcsstr("_CustomColors"));
    if (!GlowID) Shader::PropertyToID(il2cpp_utils::createcsstr("_Glow"));
    if (!BloomID) Shader::PropertyToID(il2cpp_utils::createcsstr("_Bloom"));

    if (mat->HasProperty(CustomColorID))
    {
        return mat->GetFloat(CustomColorID) > 0;
    }
    else if (mat->HasProperty(GlowID))
    {
        return mat->GetFloat(GlowID) > 0;
    }
    else if (mat->HasProperty(BloomID))
    {
        return mat->GetFloat(BloomID) > 0;
    }   
    else if (checkName)
    {
        std::string name = to_utf8(csstrtostr(mat->get_name()));
        if (name.find("_noCC") == std::string::npos) return true;
        return false;
    }
    return false;
}

void MaterialUtils::SetColors(GameObject* object, Color color, Color otherColor, bool checkName)
{
    if (!ColorID) Shader::PropertyToID(il2cpp_utils::createcsstr("_Color"));
    if (!OtherColorID) Shader::PropertyToID(il2cpp_utils::createcsstr("_OtherColor"));

    // get all renderers on the object
    Array<Renderer*>* renderers = object->GetComponentsInChildren<Renderer*>();
    if (!renderers) return;
    int rendererLength = renderers->Length();
    for (int i = 0; i < rendererLength; i++)
    {
        Renderer* currentRenderer = renderers->values[i];
        if (!currentRenderer) continue;
        Array<Material*>* materials = MaterialUtils::GetMaterials(currentRenderer);
        int materialLength = materials->Length();
        for (int j = 0; j < materialLength; j++)
        {
            Material* currentMaterial = materials->values[j];
            if (!currentMaterial || !ShouldCC(currentMaterial, checkName)) continue;
            if (currentMaterial->HasProperty(ColorID)) currentMaterial->SetColor(ColorID, color);
            if (currentMaterial->HasProperty(OtherColorID)) currentMaterial->SetColor(OtherColorID, otherColor);
        }
    }
}