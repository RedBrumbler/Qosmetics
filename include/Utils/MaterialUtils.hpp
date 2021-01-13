#pragma once

#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp" 
#include "beatsaber-hook/shared/utils/typedefs.h"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/Shader.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/Resources.hpp"
#include "Logging/GenericLogger.hpp"

class MaterialUtils
{
    public:
        static const Logger& getLogger()
        { 
            return Qosmetics::GenericLogger::GetLogger();
        }

        /// @brief returns if the material has the property of propertyName
        /// @return Bool
        static bool MatHasProperty(UnityEngine::Material* material, std::string propertyName)
        {
            return material->HasProperty(il2cpp_utils::createcsstr(propertyName));
        };

        /// @brief returns if the material has the property of propertyName
        /// @return Bool
        static bool MatHasProperty(UnityEngine::Material* material, int propertyID)
        {
            return material->HasProperty(propertyID);
        };

        /// @brief Sets the material on the given renderer to the given material
        static void SetMaterial(UnityEngine::Renderer* renderer, UnityEngine::Material* material)
        {
            renderer->set_material(material);
        };

        /// @brief Finds the material on the given MeshRenderer
        /// @return Material 
        static UnityEngine::Material* GetMaterial(UnityEngine::Renderer* renderer)
        {
            return renderer->get_material();
        };

        /// @brief Converts the string propertyName to its equivalent shader ID integer
        /// @return int shaderID
        static int PropertyNameToShaderID(std::string propertyName)
        {
            return UnityEngine::Shader::PropertyToID(il2cpp_utils::createcsstr(propertyName));
        };

        /// @brief  Gets the material float of the given propertyName
        /// @return float
        static float GetMaterialFloat(UnityEngine::Material* material, std::string propertyName)
        {
            return material->GetFloat(PropertyNameToShaderID(propertyName));
        };

        /// @brief Gets the material Color of name propertyName, defaults to _Color
        /// @return Color
        static UnityEngine::Color GetMaterialColor(UnityEngine::Material* material, std::string propertyName = "_Color")
        {
            return material->GetColor(PropertyNameToShaderID(propertyName));
        };

        /// @brief Sets the material Color of name propertyName, defaults to _Color
        static void SetMaterialColor(UnityEngine::Material* material, UnityEngine::Color color, std::string propertyName)
        {
            material->SetColor(PropertyNameToShaderID(propertyName), color);
        };

        static void ReplaceAllMaterialsForGameObjectChildren(UnityEngine::GameObject* gameObject, UnityEngine::Material* material, std::vector<UnityEngine::Renderer*>& renderers, std::string materialToReplaceName = "")
        {
            Array<UnityEngine::Renderer*>* objrenderers = gameObject->GetComponentsInChildren<UnityEngine::Renderer*>(true);

            for (int i = 0; i < objrenderers->Length(); i++)
            {
                if (!objrenderers->values[i]) continue;
                if (ReplaceAllMaterialsForRenderer(objrenderers->values[i], material, materialToReplaceName)) renderers.push_back(objrenderers->values[i]);
            }
        };

        static bool ReplaceAllMaterialsForRenderer(UnityEngine::Renderer* renderer, UnityEngine::Material* material, std::string materialToReplaceName = "")
        {
            if (renderer == nullptr)
            {
                getLogger().error("nullptr renderer was used, returning");
            }
            // putting the type directly in the functionPtr is not gonna work, so we have to typedef it
            typedef function_ptr_t<Array<UnityEngine::Material*>*, UnityEngine::Renderer*> GetMaterialArrayFunctionType;
            auto GetMaterialArray = *reinterpret_cast<GetMaterialArrayFunctionType>(il2cpp_functions::resolve_icall("UnityEngine.Renderer::GetMaterialArray"));
            
            Array<UnityEngine::Material*>* materialsCopy = GetMaterialArray != nullptr ? GetMaterialArray(renderer) : renderer->get_sharedMaterials(); // by using this conditional at least the game doesn't crash if it fails
            if (materialsCopy == nullptr) 
            {
                getLogger().error("Could not find material array");
                return false;
            }
            //Array<UnityEngine::Material*>* materialsCopy = renderer->get_sharedMaterials(); // not actually a copy, wish it was tho as it would make this a whole lot easier
            bool materialsDidChange = false;
            bool hasCC = false;
            for (int i = 0; i < materialsCopy->Length(); i++) // for each material
            {
                if (materialsCopy->values[i] == nullptr)
                {
                    getLogger().error("material reference was nullptr");
                    continue;
                } 

                std::string matName = to_utf8(csstrtostr(materialsCopy->values[i]->get_name())); // name of the material on the gameobject
                matName = toLowerCase(matName);

                if (matName.find("_done") != std::string::npos) 
                {
                    getLogger().error("Material %s was already replaced as indicated by \'_done\'", matName.c_str());
                    continue; // if materialname has _done that means it was already replaced once, thus no need to replace it again
                }

                // ex. matname = GameNote_replace_doCC and materialToReplaceName = GameNote_replace, it will replace it
                if ((matName.find(materialToReplaceName) != std::string::npos) || materialToReplaceName == "")
                {
                    getLogger().info("Replacing material %s with %s", matName.c_str(), materialToReplaceName.c_str());
                    UnityEngine::Color oldColor = materialsCopy->values[i]->GetColor(il2cpp_utils::createcsstr("_Color"));
                    
                    matName += "_done";

                    if (!shouldCC(materialsCopy->values[i])) matName += "_noCC"; // if the material should not have CC, add that to the name
                    else hasCC = true;
                    materialsCopy->values[i] = (UnityEngine::Material*)UnityEngine::Object::Instantiate((UnityEngine::Object*)material); // make new material, else we mess up the materials in the menu for no reason
                    materialsCopy->values[i]->set_name(il2cpp_utils::createcsstr(matName));
                    materialsCopy->values[i]->SetColor(il2cpp_utils::createcsstr("_Color"), oldColor);
                    materialsDidChange = true;
                }
            }

            if (materialsDidChange)
            {
                renderer->SetMaterialArray(materialsCopy);
            }
            if (hasCC) return true;
            return false;
        }

        /// @brief finds wether or not this material should have CC applied, excluding the special shader _replace and _noCC parts
        static bool shouldCC(UnityEngine::Material* material)
        {
            if (material == nullptr)
            {
                getLogger().error("Tried determining CC on nullptr material, returning false");
                return false;
            }
            bool setColor = false;
            bool hasCustomColor = MaterialUtils::MatHasProperty(material, "_CustomColors");
        
            if (hasCustomColor)
            {
                getLogger().info("Material had \'_CustomColors\' property, checking value...");
                float customColor = MaterialUtils::GetMaterialFloat(material, "_CustomColors");
                if (customColor > 0.0f) setColor = true;
            }
            else
            {
                bool hasGlow = MaterialUtils::MatHasProperty(material, "_Glow");

                if (hasGlow)
                {
                    getLogger().info("Material had \'_Glow property, checking value...\'");
                    float customColor = MaterialUtils::GetMaterialFloat(material, "_Glow");
                    if (customColor > 0.0f) setColor = true;
                }
                else
                {
                    bool hasBloom = MaterialUtils::MatHasProperty(material, "_Bloom");

                    if (hasBloom)
                    {
                        getLogger().info("Material had \'_Bloom property, checking value...\'");
                        float customColor = MaterialUtils::GetMaterialFloat(material, "_Bloom");
                        if (customColor > 0.0f) setColor = true;
                    }              
                }
            }
            return setColor;
        }

        static void PreWarmAllShadersOnObj(UnityEngine::GameObject* gameObject)
        {
            Array<UnityEngine::Renderer*>* renderers = gameObject->GetComponentsInChildren<UnityEngine::Renderer*>(true);

            typedef function_ptr_t<Array<UnityEngine::Material*>*, UnityEngine::Renderer*> GetMaterialArrayFunctionType;
            auto GetMaterialArray = *reinterpret_cast<GetMaterialArrayFunctionType>(il2cpp_functions::resolve_icall("UnityEngine.Renderer::GetMaterialArray"));

            auto createFunc = reinterpret_cast<function_ptr_t<void, Il2CppObject*>>(il2cpp_functions::resolve_icall("UnityEngine.ShaderVariantCollection::Internal_Create")); // or something similar
            auto addFunc = reinterpret_cast<function_ptr_t<bool, Il2CppObject*, Il2CppObject*, int, Array<Il2CppString*>*>>(il2cpp_functions::resolve_icall("UnityEngine.ShaderVariantCollection::AddVariant")); // or something similar
            auto warmupFunc = reinterpret_cast<function_ptr_t<void, Il2CppObject*>>(il2cpp_functions::resolve_icall("UnityEngine.ShaderVariantCollection::WarmUp")); // or something similar

            Il2CppObject* obj = UnityEngine::Object::New_ctor();
            createFunc(obj);
            std::vector<Il2CppString*> temp;
            Array<Il2CppString*>* stringArr = il2cpp_utils::vectorToArray(temp);

            for (int i = 0; i < renderers->Length(); i++)
            {
                Array<UnityEngine::Material*>* materials = GetMaterialArray(renderers->values[i]);
                for (int j = 0; j < materials->Length(); j++)
                {
                    addFunc(obj, materials->values[j]->get_shader(), 0, stringArr);
                }
            }
            
            warmupFunc(obj);
        }

    private:
        static std::string toLowerCase(std::string in)
        {
            std::string out = "";

            for (char letter : in) out += tolower(letter);

            return out;
        }
};