#include "config.hpp"
#include "include/Utils/SaberUtils.hpp"
#include "Qosmetic/QosmeticsColorManager.hpp"
#include "Data/SaberData.hpp"

extern config_t config;

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
        
        SetCustomColor(saberTransform, saberType);
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

        Array<UnityEngine::MeshFilter*>* meshFilters = saberScript->GetComponentsInChildren<UnityEngine::MeshFilter*>();
        
        if (meshFilters != nullptr)
            DisableMesh(meshFilters, customSaberData.saberConfig->get_enableFakeGlow());
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
            UnityEngine::GameObject* customGO = UnityEngine::Object::Instantiate(prefab, gameSaber);
            customGO->SetActive(false);
            customGO->set_name(il2cpp_utils::createcsstr(name));
            SetCustomColor(customGO->get_transform(), saberType);

            customGO->get_transform()->set_localScale(UnityEngine::Vector3(config.saberConfig.saberWidth, 1.0f, 1.0f));
            customGO->get_transform()->set_rotation(gameSaber->get_transform()->get_rotation());
            customGO->get_transform()->set_position(gameSaber->get_transform()->get_position());
            customGO->SetActive(true);
        }
        else
        {
            getLogger().error("Custom saber prefab was nullptr, not instantiating");
        }
    }
    
    void SaberUtils::DisableMesh(Array<UnityEngine::MeshFilter*>* meshFilters, bool enableFakeGlow)
    {
        for(int i = 0; i < meshFilters->Length(); i++)
        {
            UnityEngine::MeshFilter* filter = meshFilters->values[i];
            
            UnityEngine::GameObject* filterGO = nullptr;
            
            if(filter != nullptr)
                filterGO = filter->get_gameObject();
            

            if(filterGO != nullptr)
            {
                std::string name = to_utf8(csstrtostr(filterGO->get_name()));
                getLogger().info("found object with name %s in saber", name.c_str());
                if (enableFakeGlow && (name == "FakeGlow0" || name == "FakeGlow1")) continue;

                filterGO->SetActive(false);
            }
        }
    }

    void SaberUtils::SetCustomColor(UnityEngine::Transform* transform, GlobalNamespace::SaberType saberType)
    {
        if (transform == nullptr)
        {
            getLogger().error("Tried setting colors on nullptr transform, not setting saber colors...");
            return;
        }

        Qosmetics::ColorManager* colorManager = UnityEngine::Object::FindObjectOfType<Qosmetics::ColorManager*>();
        
        if (colorManager == nullptr)
        {
            getLogger().error("colorManager was nullptr, skipping custom saber colors...");
            return;
        }

        // make all the strings
        Il2CppString* colorString = il2cpp_utils::createcsstr("_Color");
        Il2CppString* otherColorString = il2cpp_utils::createcsstr("_OtherColor");

        UnityEngine::Color saberColor = colorManager->ColorForSaberType(saberType.value);
        UnityEngine::Color otherSaberColor = (saberType.value == 0) ? colorManager->ColorForSaberType(GlobalNamespace::SaberType::SaberB) : colorManager->ColorForSaberType(GlobalNamespace::SaberType::SaberA);
        
        auto rendererType = il2cpp_utils::GetSystemType("UnityEngine", "Renderer");
        Array<UnityEngine::Renderer*>* renderers = CRASH_UNLESS(il2cpp_utils::RunMethod<Array<UnityEngine::Renderer*>*>(transform, "GetComponentsInChildren", rendererType, true));

        typedef function_ptr_t<Array<UnityEngine::Material*>*, UnityEngine::Renderer*> GetMaterialArrayFunctionType;
        auto GetMaterialArray = *reinterpret_cast<GetMaterialArrayFunctionType>(il2cpp_functions::resolve_icall("UnityEngine.Renderer::GetMaterialArray"));

        for(int i = 0; i < renderers->Length(); i++)
        {
            Array<UnityEngine::Material*>* materials = GetMaterialArray(renderers->values[i]);
            for(int j = 0; j < materials->Length(); j++)
            {
                if(materials->values[j] == nullptr) continue;
                UnityEngine::Material* currentMaterial = materials->values[j];
                bool setColor = ShouldChangeSaberMaterialColor(materials->values[j]);

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

    void SaberUtils::SetCustomColor(std::vector<UnityEngine::Material*>& vector, GlobalNamespace::SaberType saberType)
    {
        if (vector.size() == 0) return;
        auto colorManagerType = il2cpp_utils::GetSystemType("", "ColorManager");
        Qosmetics::ColorManager* colorManager = UnityEngine::Object::FindObjectOfType<Qosmetics::ColorManager*>();
        
        if (colorManager == nullptr)
        {
            getLogger().error("colorManager was nullptr, skipping custom saber colors...");
            return;
        }

        // make all the strings
        Il2CppString* colorString = il2cpp_utils::createcsstr("_Color");
        Il2CppString* otherColorString = il2cpp_utils::createcsstr("_OtherColor");

        UnityEngine::Color saberColor = colorManager->ColorForSaberType(saberType.value);
        UnityEngine::Color otherSaberColor = (saberType.value == 0) ? colorManager->ColorForSaberType(GlobalNamespace::SaberType::SaberB) : colorManager->ColorForSaberType(GlobalNamespace::SaberType::SaberA);
        for(UnityEngine::Material* mat : vector)
        {
            bool setColor = ShouldChangeSaberMaterialColor(mat);
            if(setColor)
            {
                if (mat->HasProperty(colorString)) 
                    // if material has _Color property
                    mat->SetColor(colorString, saberColor); // set the saber color on _Color
                if (mat->HasProperty(otherColorString)) 
                    // if material has _OtherColor property
                    mat->SetColor(otherColorString, otherSaberColor); // set the other saber color on _OtherColor
            }
        }
    }

    void SaberUtils::HandleColorsDidUpdateEvent(Qosmetics::SaberData& customSaberData)
    {
        Array<GlobalNamespace::Saber*>* sabers = UnityEngine::Object::FindObjectsOfType<GlobalNamespace::Saber*>();

        for (int i = 0; i < sabers->Length(); i++)
        {
            GlobalNamespace::Saber* saber = sabers->values[i];

            UnityEngine::Transform* gameSaber = saber->get_transform();
            Il2CppString* saberName = saber->get_saberType().value == 0 ? il2cpp_utils::createcsstr("LeftSaber") : il2cpp_utils::createcsstr("RightSaber");
            UnityEngine::Transform* customSaber = gameSaber->Find(saberName);

            if (customSaber) SetCustomColor(customSaber, saber->get_saberType());
        }

        Array<QosmeticsTrail*>* trails = UnityEngine::Object::FindObjectsOfType<QosmeticsTrail*>();

        for (int i = 0; i < trails->Length(); i++)
        {
            trails->values[i]->UpdateColors();
        }
    }

    bool SaberUtils::ShouldChangeSaberMaterialColor(UnityEngine::Material* mat)
    {
        bool hasCustomColors = mat->HasProperty(UnityEngine::Shader::PropertyToID(il2cpp_utils::createcsstr("_CustomColors")));
        if (hasCustomColors) /// if there is a _CustomColors property
        {
            float customFLoat = mat->GetFloat(UnityEngine::Shader::PropertyToID(il2cpp_utils::createcsstr("_CustomColors")));
            if (customFLoat > 0.0f) return true;
        }
        else // if that property does not exist
        {
            bool hasGlow = mat->HasProperty(UnityEngine::Shader::PropertyToID(il2cpp_utils::createcsstr("_Glow")));
            if(hasGlow) // if there is a _Glow property
            {
                float glowFloat = mat->GetFloat(UnityEngine::Shader::PropertyToID(il2cpp_utils::createcsstr("_Glow")));
                if(glowFloat > 0.0f) return true; 
            }
            else // if that property does not exist
            {
                bool hasBloom = mat->HasProperty(UnityEngine::Shader::PropertyToID(il2cpp_utils::createcsstr("_Bloom")));
                if(hasBloom) /// if there is a _Bloom property
                {
                    float bloomFloat = mat->GetFloat(UnityEngine::Shader::PropertyToID(il2cpp_utils::createcsstr("_Bloom")));
                    if(bloomFloat > 0.0f) return true; 
                }
            }
        }
        return false;
    }

    void SaberUtils::AddMenuPointerSaber(UnityEngine::Transform* parent, bool isLeft, SaberData& saberData)
    {
        if (!parent) 
        {
            getLogger().info("Tried adding menu pointer to nullptr parent");
            return;
        }
        UnityEngine::GameObject* prefab = isLeft ? saberData.get_leftSaber() : saberData.get_rightSaber();
        std::string name = isLeft ? "CustomLeftPointer" : "CustomRightPointer";

        Array<UnityEngine::MeshFilter*>* meshFilters = parent->get_gameObject()->GetComponentsInChildren<UnityEngine::MeshFilter*>();

        if (prefab)
        {
            if (meshFilters != nullptr)
                DisableMesh(meshFilters, false);
            else
                getLogger().error("meshFilters were null so didn't disable");

            UnityEngine::GameObject* instantiated = UnityEngine::Object::Instantiate(prefab, parent);
            instantiated->set_name(il2cpp_utils::createcsstr(name));
            instantiated->get_transform()->set_localScale(UnityEngine::Vector3::get_one() * 0.4f);
            instantiated->get_transform()->set_localPosition(UnityEngine::Vector3(0.0f, 0.0f, -0.05f));
            instantiated->get_transform()->set_localEulerAngles(UnityEngine::Vector3::get_zero());

            SetCustomColor(instantiated->get_transform(), isLeft ? 0 : 1);
        }
    }
}
