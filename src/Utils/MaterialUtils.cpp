#include "Utils/MaterialUtils.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Shader.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"

#include "QosmeticsLogger.hpp"

using namespace UnityEngine;

#define INFO(value...) QosmeticsLogger::GetContextLogger("Material Utils").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("Material Utils").error(value)

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
    if (!renderer)
        return nullptr;
    if (!GetMaterialArrayFunc)
        GetMaterialArrayFunc = reinterpret_cast<function_ptr_t<Array<UnityEngine::Material*>*, UnityEngine::Renderer*>>(il2cpp_functions::resolve_icall("UnityEngine.Renderer::GetMaterialArray"));
    return GetMaterialArrayFunc(renderer);
}

void MaterialUtils::PrewarmAllShadersOnObject(GameObject* object)
{
    if (!object)
        return;
    ArrayW<UnityEngine::Renderer*> renderers = object->GetComponentsInChildren<UnityEngine::Renderer*>(true);

    // get material method is stripped so resolve icall

    if (!GetMaterialArrayFunc)
        GetMaterialArrayFunc = reinterpret_cast<function_ptr_t<Array<UnityEngine::Material*>*, UnityEngine::Renderer*>>(il2cpp_functions::resolve_icall("UnityEngine.Renderer::GetMaterialArray"));
    //typedef function_ptr_t<Array<UnityEngine::Material*>*, UnityEngine::Renderer*> GetMaterialArrayFunctionType;
    //auto GetMaterialArray = reinterpret_cast<GetMaterialArrayFunctionType>(il2cpp_functions::resolve_icall("UnityEngine.Renderer::GetMaterialArray"));

    // all shader variant stuff is stripped so resolve icall
    if (!createFunc)
        createFunc = reinterpret_cast<function_ptr_t<void, Il2CppObject*>>(il2cpp_functions::resolve_icall("UnityEngine.ShaderVariantCollection::Internal_Create"));
    if (!addFunc)
        addFunc = reinterpret_cast<function_ptr_t<bool, Il2CppObject*, Il2CppObject*, int, Array<Il2CppString*>*>>(il2cpp_functions::resolve_icall("UnityEngine.ShaderVariantCollection::AddVariant"));
    if (!warmupFunc)
        warmupFunc = reinterpret_cast<function_ptr_t<void, Il2CppObject*>>(il2cpp_functions::resolve_icall("UnityEngine.ShaderVariantCollection::WarmUp"));
    Il2CppObject* obj = UnityEngine::Object::New_ctor();
    createFunc(obj);
    std::vector<Il2CppString*> temp;
    Array<Il2CppString*>* stringArr = il2cpp_utils::vectorToArray(temp);
    for (int i = 0; i < renderers.Length(); i++)
    {
        Array<UnityEngine::Material*>* materials = GetMaterialArrayFunc(renderers.get(i));
        for (int j = 0; j < materials->Length(); j++)
        {
            addFunc(obj, materials->values[j]->get_shader(), 0, stringArr);
        }
    }
    warmupFunc(obj);
}

bool MaterialUtils::ShouldCC(UnityEngine::Material* mat, bool checkName)
{
    if (!CustomColorID)
        CustomColorID = Shader::PropertyToID(il2cpp_utils::newcsstr("_CustomColors"));
    if (!GlowID)
        GlowID = Shader::PropertyToID(il2cpp_utils::newcsstr("_Glow"));
    if (!BloomID)
        BloomID = Shader::PropertyToID(il2cpp_utils::newcsstr("_Bloom"));

    // ew ugly else if ladder, but there is no other way of doing it
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
        if (name.find("_replace") != std::string::npos && name.find("_noCC") == std::string::npos)
            return true;
        return false;
    }

    return false;
}

void MaterialUtils::SetRenderQueue(GameObject* object, int renderQueue)
{
    if (!object)
        return;
    ArrayW<Renderer*> renderers = object->GetComponentsInChildren<Renderer*>(true);
    if (!renderers)
        return;
    int rendererLength = renderers.Length();
    for (int i = 0; i < rendererLength; i++)
    {
        Renderer* currentRenderer = renderers.get(i);
        if (!currentRenderer)
            continue;
        Array<Material*>* materials = MaterialUtils::GetMaterials(currentRenderer);
        int materialLength = materials->Length();
        for (int j = 0; j < materialLength; j++)
        {
            Material* currentMaterial = materials->values[j];
            currentMaterial->set_renderQueue(renderQueue);
        }
    }
}

void MaterialUtils::SetColors(GameObject* object, Color color, Color otherColor, bool checkName, int renderQueue)
{
    if (!object)
        return;
    if (!ColorID)
        ColorID = Shader::PropertyToID(il2cpp_utils::newcsstr("_Color"));
    if (!OtherColorID)
        OtherColorID = Shader::PropertyToID(il2cpp_utils::newcsstr("_OtherColor"));

    // get all renderers on the object
    ArrayW<Renderer*> renderers = object->GetComponentsInChildren<Renderer*>(true);
    if (!renderers)
        return;
    int rendererLength = renderers.Length();
    for (int i = 0; i < rendererLength; i++)
    {
        Renderer* currentRenderer = renderers.get(i);
        if (!currentRenderer)
            continue;
        Array<Material*>* materials = MaterialUtils::GetMaterials(currentRenderer);
        int materialLength = materials->Length();
        for (int j = 0; j < materialLength; j++)
        {
            Material* currentMaterial = materials->values[j];
            // if renderqueue is given, set it
            if (renderQueue)
                currentMaterial->set_renderQueue(renderQueue);
            if (!currentMaterial || !ShouldCC(currentMaterial, checkName))
                continue;
            if (currentMaterial->HasProperty(ColorID))
                currentMaterial->SetColor(ColorID, color);
            if (currentMaterial->HasProperty(OtherColorID))
                currentMaterial->SetColor(OtherColorID, otherColor);
        }
    }
}

void MaterialUtils::SetColors(UnityEngine::GameObject* object, UnityEngine::Color color, bool checkName, int renderQueue)
{
    if (!object)
        return;
    if (!ColorID)
        ColorID = Shader::PropertyToID(il2cpp_utils::newcsstr("_Color"));

    // get all renderers on the object
    ArrayW<Renderer*> renderers = object->GetComponentsInChildren<Renderer*>(true);
    if (!renderers)
        return;
    int rendererLength = renderers.Length();
    for (int i = 0; i < rendererLength; i++)
    {
        Renderer* currentRenderer = renderers.get(i);
        if (!currentRenderer)
            continue;
        Array<Material*>* materials = MaterialUtils::GetMaterials(currentRenderer);
        int materialLength = materials->Length();
        for (int j = 0; j < materialLength; j++)
        {
            Material* currentMaterial = materials->values[j];
            // if renderqueue is given, set it
            if (renderQueue)
                currentMaterial->set_renderQueue(renderQueue);
            if (!currentMaterial || !ShouldCC(currentMaterial, checkName))
                continue;
            if (currentMaterial->HasProperty(ColorID))
                currentMaterial->SetColor(ColorID, color);
        }
    }
}

std::string toLowerCase(std::string in)
{
    std::string out = "";
    for (char letter : in)
        out += tolower(letter);
    return out;
}

void MaterialUtils::ReplaceMaterialsForGameObject(GameObject* object)
{
    ArrayW<Material*> allMaterials = Resources::FindObjectsOfTypeAll<Material*>();

    int matCount = allMaterials.Length();
    INFO("Found %d materials", matCount);
    for (int i = 0; i < matCount; i++)
    {
        Material* material = allMaterials.get(i);
        if (!material)
            continue;
        Il2CppString* materialNameCS = material->get_name();
        if (!materialNameCS)
            continue;
        std::string materialName = to_utf8(csstrtostr(materialNameCS));
        if (materialName == "")
            continue;
        // names with _replace are custom materials, they can be skipped
        if (materialName.find("_replace") != std::string::npos)
            continue;

        ReplaceMaterialForGameObjectChildren(object, material, toLowerCase(materialName));
    }
}

void MaterialUtils::ReplaceMaterialForGameObjectChildren(GameObject* gameObject, Material* material, std::string materialToReplaceName)
{
    ArrayW<Renderer*> renderers = gameObject->GetComponentsInChildren<Renderer*>(true);

    int rendererCount = renderers.Length();

    for (int i = 0; i < rendererCount; i++)
    {
        ReplaceMaterialForRenderer(renderers.get(i), material, materialToReplaceName);
    }
}

void MaterialUtils::ReplaceMaterialForRenderer(Renderer* renderer, Material* replacingMaterial, std::string materialToReplaceName)
{
    ArrayW<Material*> materials = renderer->GetMaterialArray();
    std::vector<Material*> materialsCopy = {};
    bool materialsDidChange = false;

    int materialLength = materials.Length();
    for (int i = 0; i < materialLength; i++)
    {
        Material* material = materials.get(i);
        Il2CppString* materialNameCS = material->get_name();
        std::string materialName = to_utf8(csstrtostr(materialNameCS));
        materialName = toLowerCase(materialName);

        // if not a replace material, skip
        if (materialName.find("_replace") == std::string::npos)
        {
            materialsCopy.push_back(material);
            continue;
        }
        // if material already replaced, skip
        if (materialName.find("_done") != std::string::npos)
        {
            materialsCopy.push_back(material);
            continue;
        }
        // if not the current material name there, skip
        if (materialName.find(materialToReplaceName) == std::string::npos)
        {
            materialsCopy.push_back(material);
            continue;
        }

        INFO("Replacing %s with %s", materialName.c_str(), materialToReplaceName.c_str());

        // now we should be good to update this stuff
        materialName += "_done";
        if (!ShouldCC(material) && materialName.find("_noCC") == std::string::npos)
            materialName += "_noCC";
        // now we have the new material name, as well as being sure that this material is the one that needs to be replaced;
        Color oldColor = material->get_color();

        Material* newMat = (Material*)Object::Instantiate((UnityEngine::Object*)replacingMaterial);
        newMat->set_color(oldColor);
        newMat->set_name(il2cpp_utils::newcsstr(materialName));
        materialsCopy.push_back(newMat);

        materialsDidChange = true;
    }

    if (materialsDidChange)
    {
        renderer->SetMaterialArray(il2cpp_utils::vectorToArray(materialsCopy));
    }
}
