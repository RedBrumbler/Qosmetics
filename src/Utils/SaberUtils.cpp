#include "include/Utils/SaberUtils.hpp"

namespace Qosmetics
{
    void SaberUtils::ReplaceSaber(GlobalNamespace::Saber* gameSaber, UnityEngine::GameObject* customSaber)
    {
        getLogger().info("ReplaceSaber");
        UnityEngine::GameObject* gameSaberGO = gameSaber->get_gameObject();
        UnityEngine::Transform* customSaberTransform = customSaber->get_transform();
        
        GlobalNamespace::SaberType saberType = gameSaber->get_saberType();
        
        Il2CppString* saberName = il2cpp_utils::createcsstr((saberType.value == 0 ? "LeftSaber" : "RightSaber"));
        
        UnityEngine::Transform* saberTransform = customSaberTransform->Find(saberName);
        UnityEngine::Transform* gameSaberTransform = gameSaberGO->get_transform();
        
        UnityEngine::Vector3 gameSaberPos = gameSaberTransform->get_position();
        UnityEngine::Quaternion gameSaberRot = gameSaberTransform->get_rotation();

        auto meshFilterType = il2cpp_utils::GetSystemType("UnityEngine", "MeshFilter");
        
        Array<UnityEngine::MeshFilter*>* meshFilters = *il2cpp_utils::RunMethod<Array<UnityEngine::MeshFilter*>*>(gameSaberTransform, "GetComponentsInChildren", meshFilterType, false);
        /*if (meshFilters != nullptr) // if meshfilters is not nullptr, disable them
            //DisableMesh(meshFilters);
        else // if they are nullptr, log it and move on
            getLogger().error("meshfilter array was nullptr, not disabling original saber");
        */
        saberTransform->set_parent(gameSaberTransform);
        saberTransform->set_position(gameSaberPos);
        saberTransform->set_rotation(gameSaberRot);
        
        setCustomColor(saberTransform, saberType);
    }

    void SaberUtils::AddSaber(GlobalNamespace::Saber* saberScript, Qosmetics::SaberData& customSaberData)
    {
        getLogger().info("AddSaber");
        if (saberScript == nullptr)
        {
            getLogger().error("given saberscript was nullptr, not adding saber");
            return;
        }

        UnityEngine::Transform* gameSaber = saberScript->get_transform();

        UnityEngine::Transform* basicSaberModel = gameSaber->Find(il2cpp_utils::createcsstr("BasicSaberModel(Clone)"));
        
        if (basicSaberModel == nullptr)
        {
            getLogger().error("Couldn't find basicsabermodel in AddSaber");
            return;
        }

        Array<UnityEngine::MeshFilter*>* meshFilters = basicSaberModel->GetComponentsInChildren<UnityEngine::MeshFilter*>();
        
        if (meshFilters != nullptr)
            DisableMesh(meshFilters, customSaberData);
        else
            getLogger().error("meshFilters were null so didn't disable");
        // actually start adding the sabers
        GlobalNamespace::SaberType saberType = saberScript->get_saberType();

        UnityEngine::GameObject* prefab = nullptr;
        std::string name = "";

        if (saberType.value == 0) // LeftSaber
        {
            prefab = customSaberData.get_leftSaber();
            name = "LeftSaber";
        }
        else // RightSaber
        {
            prefab = customSaberData.get_rightSaber();
            name = "RightSaber";
        }

        if (prefab != nullptr)
        {
            UnityEngine::GameObject* customGO = (UnityEngine::GameObject*)UnityEngine::Object::Instantiate((UnityEngine::Object*)prefab);
            customGO->set_name(il2cpp_utils::createcsstr(name));
            setCustomColor(customGO->get_transform(), saberType);

            customGO->get_transform()->SetParent(saberScript->get_transform());
            customGO->get_transform()->set_localScale(UnityEngine::Vector3::get_one());
            customGO->get_transform()->set_eulerAngles(UnityEngine::Vector3::get_zero());
            customGO->get_transform()->set_localPosition(UnityEngine::Vector3::get_zero());
        }
        else
        {
            getLogger().error("Custom saber prefab was nullptr, not instantiating");
        }
    }
    
    void SaberUtils::DisableMesh(Array<UnityEngine::MeshFilter*>* meshFilters, Qosmetics::SaberData& customSaberData)
    {
        bool enableFakeGlow = customSaberData.saberConfig->get_enableFakeGlow();
        for(int i = 0; i < meshFilters->Length(); i++)
        {
            UnityEngine::MeshFilter* filter = meshFilters->values[i];
            
            UnityEngine::GameObject* filterGO = nullptr;
            
            if(filter != nullptr)
                filterGO = filter->get_gameObject();
            

            if(filterGO != nullptr)
            {
                std::string name = to_utf8(csstrtostr(filterGO->get_name()));
                if (enableFakeGlow && (name == "FakeGlow0" || name == "FakeGlow1")) continue;

                filterGO->SetActive(false);
            }
        }
    }

    void SaberUtils::setCustomColor(UnityEngine::Transform* transform, GlobalNamespace::SaberType saberType)
    {
        if (transform == nullptr)
        {
            getLogger().error("Tried setting colors on nullptr transform, not setting saber colors...");
            return;
        }

        auto colorManagerType = il2cpp_utils::GetSystemType("", "ColorManager");
        GlobalNamespace::ColorManager* colorManager = UnityUtils::GetLastObjectOfType<GlobalNamespace::ColorManager*>(il2cpp_utils::GetClassFromName("", "ColorManager"));
        
        if (colorManager == nullptr)
        {
            getLogger().error("colorManager was nullptr, skipping custom saber colors...");
            return;
        }

        // make all the strings
        Il2CppString* glowString = il2cpp_utils::createcsstr("_Glow");
        Il2CppString* bloomString = il2cpp_utils::createcsstr("_Bloom");
        Il2CppString* customString = il2cpp_utils::createcsstr("_CustomColors");
        Il2CppString* colorString = il2cpp_utils::createcsstr("_Color");
        Il2CppString* otherColorString = il2cpp_utils::createcsstr("_OtherColor");

        UnityEngine::Color saberColor = colorManager->ColorForSaberType(saberType);
        UnityEngine::Color otherSaberColor = (saberType.value == 0) ? colorManager->ColorForSaberType(GlobalNamespace::SaberType::SaberB) : colorManager->ColorForSaberType(GlobalNamespace::SaberType::SaberA);
        
        auto rendererType = il2cpp_utils::GetSystemType("UnityEngine", "Renderer");
        Array<UnityEngine::Renderer*>* renderers = CRASH_UNLESS(il2cpp_utils::RunMethod<Array<UnityEngine::Renderer*>*>(transform, "GetComponentsInChildren", rendererType, false));
        
        int glowID = UnityEngine::Shader::PropertyToID(glowString);
        int bloomID = UnityEngine::Shader::PropertyToID(bloomString);
        int customID = UnityEngine::Shader::PropertyToID(customString);

        for(int i = 0; i < renderers->Length(); i++)
        {
            Array<UnityEngine::Material*>* sharedMaterials = renderers->values[i]->get_sharedMaterials();
            for(int j = 0; j < sharedMaterials->Length(); j++)
            {
                if(sharedMaterials->values[j] == nullptr) continue;
                UnityEngine::Material* currentMaterial = sharedMaterials->values[j];
                std::string matName = to_utf8(csstrtostr(currentMaterial->get_name()));
                bool hasCustomColors = currentMaterial->HasProperty(customID);
                bool setColor = false;

                if (hasCustomColors) /// if there is a _CustomColors property
                {
                    float customFLoat = currentMaterial->GetFloat(customID);
                    if (customFLoat > 0.0f) setColor = true;
                }
                else // if that property does not exist
                {
                    bool hasGlow = currentMaterial->HasProperty(glowID);
                    if(hasGlow) // if there is a _Glow property
                    {
                        float glowFloat = currentMaterial->GetFloat(glowID);
                        if(glowFloat > 0.0f) setColor = true; 
                    }
                    else // if that property does not exist
                    {
                        bool hasBloom = currentMaterial->HasProperty(bloomID);
                        if(hasBloom) /// if there is a _Bloom property
                        {
                            float bloomFloat = currentMaterial->GetFloat(bloomID);
                            if(bloomFloat > 0.0f) setColor = true; 
                        }
                        else // if that property does not exist
                        {
                            bool hasReplaceName = (matName.find("_replace") != std::string::npos); // if material has _replace in the name
                            if (hasReplaceName)
                            {
                                if (matName.find("_noCC") == std::string::npos) // if the mat does not have "_noCC" in its name
                                    setColor = true;
                            }
                        }
                        
                    }
                }

                if(setColor)
                {
                    if (currentMaterial->HasProperty(colorString)) 
                        // if material has _Color property
                        currentMaterial->SetColor(colorString, saberColor); // set the saber color on _Color
                    if (currentMaterial->HasProperty(otherColorString)) 
                        // if material has _OtherColor property
                        currentMaterial->SetColor(otherColorString, otherSaberColor); // set the other saber color on _OtherColor
                }
            }
        }
    }
}
