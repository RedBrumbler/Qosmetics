#include "Utils/WallUtils.hpp"

void WallUtils::SetObstacleColors(Qosmetics::WallData &customWall)
{
    auto config = *customWall.get_config();
    if (config.get_moreThan1CoreMat()) // if true, array is used
    {
        getLogger().info("Setting color on core material array!");
        auto array = customWall.get_coreSharedMaterials();

        for (int i = 0; i < array->Length(); i++) // for each material in the array
        {
            SetObstacleColor(array->values[i]);
        }
    }
    else // if only 1 material
    {
        getLogger().info("Setting color on core material");
        auto coreMat = customWall.get_coreMaterial();
        SetObstacleColor(coreMat);
    }

    if (config.get_moreThan1FrameMat()) // if true, array is used
    {
        getLogger().info("Setting color on frame material array!");
        auto array = customWall.get_frameSharedMaterials();

        for (int i = 0; i < array->Length(); i++) // for each material in the array
        {
            SetObstacleColor(array->values[i]);
        }
    }
    else // if only 1 material
    {
        getLogger().info("Setting color on frame material");
        auto frameMat = customWall.get_frameMaterial();
        SetObstacleColor(frameMat);
    }
}

void WallUtils::SetObstacleColor(UnityEngine::Material* material)
    {
        // if material happens to be a nullptr, just return
        if (material == nullptr) 
        {
            getLogger().error("Attempted to set color on nullptr material, skipping...");
            return;
        }

        // if the material doesnt even have a _Color property there is no reason to go through the trouble of determining if it should get color, it just won't work
        bool hasColorProperty = MaterialUtils::MatHasProperty(material, "_Color");
        if (!hasColorProperty) 
        {
            getLogger().info("Material did not have a \'_Color\' property, skipping...");
            return;
        }

        GlobalNamespace::ColorManager* colorManager = UnityUtils::GetLastObjectOfType<GlobalNamespace::ColorManager*>(il2cpp_utils::GetClassFromName("", "ColorManager"));
        if (colorManager == nullptr) 
        {
            getLogger().error("ColorManager was not found, skipping...");
            return;
        }

        UnityEngine::Color wallColor = colorManager->obstaclesColor->get_color();

        bool setColor = false;
        bool hasCustomColor = MaterialUtils::MatHasProperty(material, "_CustomColors");
        std::string matName = to_utf8(csstrtostr(material->get_name()));
        
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
        if (setColor)
            material->SetColor(il2cpp_utils::createcsstr("_Color"), wallColor);
    }

void WallUtils::DisableFakeGlow(UnityEngine::Transform* obstacle)
{
    UnityEngine::Transform* obstacleFakeGlow = obstacle->Find(il2cpp_utils::createcsstr("FakeGlowWrapper/ObstacleFakeGlow"));
    if (obstacleFakeGlow == nullptr)
    {
        getLogger().error("trying to find the obstacle fake glow returned nullptr, skipping fake glow disable");
        return;
    }
    UnityEngine::MeshRenderer* obstacleFakeGlowRenderer = UnityUtils::GetComponent<UnityEngine::MeshRenderer*>(obstacleFakeGlow->get_gameObject(), "MeshRenderer");
    
    HideRenderer(obstacleFakeGlowRenderer);
}

void WallUtils::DisableDefaults(UnityEngine::Transform* obstacle)
{
    UnityEngine::Transform* obstacleDepthWrite = obstacle->Find(il2cpp_utils::createcsstr("ObstacleCore/DepthWrite"));
    UnityEngine::Transform* obstacleCore = obstacle->Find(il2cpp_utils::createcsstr("ObstacleCore"));

    if (obstacleDepthWrite == nullptr)
    {
        getLogger().error("trying to find the obstacle depth write returned nullptr, skipping depth write disable");
    }
    else
    {
        UnityEngine::MeshRenderer* obstacleDepthWriteRenderer = UnityUtils::GetComponent<UnityEngine::MeshRenderer*>(obstacleDepthWrite->get_gameObject(), "MeshRenderer");
        HideRenderer(obstacleDepthWriteRenderer);
    }
    
    if (obstacleCore == nullptr)
    {
        getLogger().error("trying to find the obstacle Core returned nullptr, skipping Core disable");
    }
    else
    {
        UnityEngine::MeshRenderer* obstacleCoreRenderer = UnityUtils::GetComponent<UnityEngine::MeshRenderer*>(obstacleCore->get_gameObject(), "MeshRenderer");
        HideRenderer(obstacleCoreRenderer);
    }
}

void WallUtils::ReplaceFrameMaterial(UnityEngine::Transform* obstacle, UnityEngine::Material* material)
{
    if (material == nullptr) 
    {
        getLogger().error("Tried setting frame material with a nullptr material, skipping...");
        return;
    }

    UnityEngine::Transform* obstacleFrame = obstacle->Find(il2cpp_utils::createcsstr("ObstacleFrame"));

    if (obstacleFrame == nullptr) 
    {
        getLogger().error("Tried setting frame material on nullptr obstacleFrame, skipping...");
        return;
    }

    UnityEngine::MeshRenderer* obstacleFrameRenderer = UnityUtils::GetComponent<UnityEngine::MeshRenderer*>(obstacleFrame->get_gameObject(), "MeshRenderer");

    if (obstacleFrameRenderer == nullptr)
    {
        getLogger().error("couldn't find obstacle frame mesh renderer, skipping...");
        return;
    }

    obstacleFrameRenderer->set_material(material);
}   

void WallUtils::ReplaceFrameMesh(UnityEngine::Transform* obstacle, UnityEngine::Mesh* mesh)
{
    if (mesh == nullptr) 
    {
        getLogger().error("Tried setting frame mesh with a nullptr mesh, skipping...");
        return;
    }

    UnityEngine::Transform* obstacleFrame = obstacle->Find(il2cpp_utils::createcsstr("ObstacleFrame"));

    if (obstacleFrame == nullptr) 
    {
        getLogger().error("Tried setting frame mesh on nullptr obstacleFrame, skipping...");
        return;
    }

    UnityEngine::MeshFilter* obstacleFrameMeshFilter = UnityUtils::GetComponent<UnityEngine::MeshFilter*>(obstacleFrame->get_gameObject(), "MeshFilter");
    
    if (obstacleFrameMeshFilter == nullptr)
    {
        getLogger().error("couldn't find obstacle frame mesh filter, skipping...");
        return;
    }

    obstacleFrameMeshFilter->set_mesh(mesh);
}

void WallUtils::ReplaceCoreMaterial(UnityEngine::Transform* obstacle, UnityEngine::Material* material)
{
    if (material == nullptr) 
    {
        getLogger().error("Tried setting core material with a nullptr material, skipping...");
        return;
    }

    UnityEngine::Transform* obstacleCoreInside = obstacle->Find(il2cpp_utils::createcsstr("ObstacleCore/ObstacleCoreLWInside"));

    if (obstacleCoreInside == nullptr) 
    {
        getLogger().error("Tried setting core material on nullptr obstacleCoreInside, skipping...");
        return;
    }

    UnityEngine::MeshRenderer* obstacleCoreInsideRenderer = UnityUtils::GetComponent<UnityEngine::MeshRenderer*>(obstacleCoreInside->get_gameObject(), "MeshRenderer");

    if (obstacleCoreInsideRenderer == nullptr)
    {
        getLogger().error("couldn't find obstacle core mesh renderer, skipping...");
        return;
    }

    obstacleCoreInsideRenderer->set_material(material);
}

void WallUtils::ReplaceCoreMesh(UnityEngine::Transform* obstacle, UnityEngine::Mesh* mesh)
{
    if (mesh == nullptr) 
    {
        getLogger().error("Tried setting material with a nullptr mesh, skipping...");
        return;
    }

    UnityEngine::Transform* obstacleCoreInside = obstacle->Find(il2cpp_utils::createcsstr("ObstacleCore/ObstacleCoreLWInside"));

    if (obstacleCoreInside == nullptr) 
    {
        getLogger().error("Tried setting core mesh on nullptr obstacleCoreInside, skipping...");
        return;
    }

    UnityEngine::MeshFilter* obstacleCoreInsideMeshFilter = UnityUtils::GetComponent<UnityEngine::MeshFilter*>(obstacleCoreInside, "MeshFilter");

    if (obstacleCoreInsideMeshFilter == nullptr)
    {
        getLogger().error("couldn't find obstacle core mesh filter, skipping...");
        return;
    }

    obstacleCoreInsideMeshFilter->set_mesh(mesh);
}

void WallUtils::DisableFrame(UnityEngine::Transform* obstacle)
{
    UnityEngine::Transform* obstacleFrame = obstacle->Find(il2cpp_utils::createcsstr("ObstacleFrame"));

    if (obstacleFrame == nullptr) 
    {
        getLogger().error("Tried setting frame mesh on nullptr obstacleFrame, skipping...");
        return;
    }

    UnityEngine::MeshRenderer* obstacleFrameRenderer = UnityUtils::GetComponent<UnityEngine::MeshRenderer*>(obstacleFrame->get_gameObject(), "MeshRenderer");

    if (obstacleFrameRenderer == nullptr)
    {
        getLogger().error("couldn't find obstacle frame mesh renderer, skipping...");
        return;
    }

    HideRenderer(obstacleFrameRenderer);
}

void WallUtils::ReplaceCoreSharedMaterials(UnityEngine::Transform* obstacle, Array<UnityEngine::Material*>* materials)
{
    if (materials == nullptr) 
    {
        getLogger().error("Tried setting core material array with nullptr materials, skipping...");
        return;
    }
    UnityEngine::GameObject* obstacleCoreInside = obstacle->Find(il2cpp_utils::createcsstr("ObstacleCore/ObstacleCoreLWInside"))->get_gameObject();

    if (obstacleCoreInside == nullptr) 
    {
        getLogger().error("Tried setting core mesh on nullptr obstacleCoreInside, skipping...");
        return;
    }

    UnityEngine::MeshRenderer* obstacleCoreInsideRenderer = UnityUtils::GetComponent<UnityEngine::MeshRenderer*>(obstacleCoreInside, "MeshRenderer");

    if (obstacleCoreInsideRenderer == nullptr)
    {
        getLogger().error("couldn't find obstacle core mesh renderer, skipping...");
        return;
    }

    obstacleCoreInsideRenderer->SetMaterialArray(materials);
}

void WallUtils::ReplaceFrameSharedMaterials(UnityEngine::Transform* obstacle, Array<UnityEngine::Material*>* materials)
{
    if (materials == nullptr) 
    {
        getLogger().error("Tried setting frame material array with nullptr materials, skipping...");
        return;
    }

    UnityEngine::Transform* obstacleFrame = obstacle->Find(il2cpp_utils::createcsstr("ObstacleFrame"));

    if (obstacleFrame == nullptr) 
    {
        getLogger().error("Tried setting frame material array on nullptr obstacleFrame, skipping...");
        return;
    }

    UnityEngine::MeshRenderer* obstacleFrameRenderer = UnityUtils::GetComponent<UnityEngine::MeshRenderer*>(obstacleFrame->get_gameObject(), "MeshRenderer");
    
    if (obstacleFrameRenderer == nullptr)
    {
        getLogger().error("couldn't find obstacle frame mesh renderer, skipping...");
        return;
    }

    obstacleFrameRenderer->SetMaterialArray(materials);
}

