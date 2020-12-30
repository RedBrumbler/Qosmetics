#include "config.hpp"
#include "Qosmetic/QuestWall.hpp"
#include "Qosmetic/QosmeticsColorManager.hpp"
#include <thread>
#include "Data/QosmeticsDescriptorCache.hpp"

extern config_t config;

namespace Qosmetics
{
    void QuestWall::makeFolder()
    {
        if (!direxists(fileDir.c_str()))
        {
            int makePath = mkpath(fileDir.data());
            if (makePath == -1)
            {
                getLogger().debug("Failed to make path!");
            }
        }
    }

    bool QuestWall::ShaderWarmup()
    {
        if (!direxists(fileDir)) 
        {
            getLogger().info("Wall Directory did not exist, creating directory at %s", fileDir.c_str());
            makeFolder();
        }
        else getLogger().info("Wall Directory Exists");
        // get all file names in the wall directory
        FileUtils::getFileNamesInDir("qwall", fileDir, fileNames);

        // if no files were found, just return
        if (fileNames.size() == 0) return false;

        // each file is a wall, so for each file
        for (int i = 0; i < fileNames.size(); i++)
        {
            Descriptor* descriptor = DescriptorCache::GetDescriptor(fileNames[i], wall);
            if (!descriptor->valid) 
            {
                getLogger().info("Wall Descriptor was invalid, making a new one");
                descriptor = new Descriptor("", "", "", fileDir + fileNames[i], wall, nullptr);
            }
            
            // make a new wall
            WallData* newWall = new WallData(descriptor);

            wallMap[descriptor] = newWall;
            //wallMap[descriptor]->LoadBundle();

            DescriptorCache::AddToWallCache(descriptor);
        }
        Descriptor* descriptor = DescriptorCache::GetDescriptor(config.lastActiveWall, wall);
        SetActiveWall(descriptor);
        //if (activeWall) activeWall->LoadBundle();
        return true;
    };

    void QuestWall::HealthWarning()
    {
        if (activeWall && !activeWall->get_complete() && !activeWall->get_isLoading()) activeWall->LoadAssets();
        // for all loaded wall files, load the assets in them (if there are none loaded it won't actually do anything)
        /*
        for (auto& pair : wallMap)
        {
            pair.second->LoadAssets();
        }
        */
    };

    void QuestWall::GameCore()
    {
        CheckScoreDisabling();
        if (!activeWall) return;

        // selected wall from the loadedwalls vector, in the future this may be selectable

        Qosmetics::WallData& selected = *activeWall;

        if (!selected.get_complete()) 
        {
            getLogger().error("Tried using the wall while it was not finished loading");
            return;
        }
        // clears the references to the active materials, the object may be non destructable,
        // the references can get corrupted accross scene changes so setting them nullptr to be redefined later is the best
        selected.FindPrefab();
        selected.ClearActive();
        setColors = false;
        
    }

    void QuestWall::MenuViewControllers()
    {
        if (activeWall && !activeWall->get_complete() && !activeWall->get_isLoading()) activeWall->LoadAssets();
        /*
        for (auto& pair : wallMap)
        {
            if (!pair.second->get_complete() && !pair.second->get_isLoading()) pair.second->LoadAssets();
        }
        */
    }    

    
    void QuestWall::ObstacleController_Init_Pre(GlobalNamespace::ObstacleController* obstacleController)
    {
        // if no walls loaded, it's no use so just early return
        if (config.wallConfig.forceFakeGlowOff) WallUtils::DisableFakeGlow(obstacleController->get_transform());
        if (config.wallConfig.forceCoreOff)
        {
            WallUtils::DisableDefaults(obstacleController->get_transform());
            WallUtils::HideRenderer(obstacleController->get_transform()->Find(il2cpp_utils::createcsstr("ObstacleCore/ObstacleCoreLWInside"))->GetComponent<UnityEngine::MeshRenderer*>());
        }
        if (config.wallConfig.forceFrameOff)
        {
            WallUtils::DisableFrame(obstacleController->get_transform());
        }
        if (!activeWall) return;

        WallData& selected = *activeWall;
        if (!selected.get_complete()) 
        {
            getLogger().error("Tried replacing things on the wall while it was not finished loading");
            return;
        }

        WallConfig wallConfig = *selected.get_config();

        UnityEngine::GameObject* obstacleGO = obstacleController->get_gameObject();
        UnityEngine::Transform* obstacleTransform = obstacleGO->get_transform();
        
        // stuff that needs to be disabled by default
        WallUtils::DisableDefaults(obstacleTransform);

        // get the colors set if not already
        if (!setColors)
        {
            getLogger().info("Setting wall colors");
            ColorManager::Init();
            
            WallUtils::SetObstacleColors(selected);
            setColors = true;
        }

        if (wallConfig.get_disableFakeGlow() || config.wallConfig.forceFakeGlowOff) // if fake glow needs to be disabled
            WallUtils::DisableFakeGlow(obstacleTransform);

        if (wallConfig.get_disableFrame()) // if the frame needs to be disabled
            WallUtils::DisableFrame(obstacleTransform);
        else // if frame should stay enabled
        {
            if (wallConfig.get_replaceFrameMesh()) // if the frame mesh needs to be replaced
                WallUtils::ReplaceFrameMesh(obstacleTransform, selected.get_frameMesh());

            if (wallConfig.get_replaceFrameMaterial()) // if the frame material needs to be replaced
            {
                if (wallConfig.get_moreThan1FrameMat()) // if there is more than 1 material on the frame
                    WallUtils::ReplaceFrameSharedMaterials(obstacleTransform, selected.get_frameSharedMaterials());
                else // if there is only 1 material on the frame
                    WallUtils::ReplaceFrameMaterial(obstacleTransform, selected.get_frameMaterial());
            }
        }

        if (wallConfig.get_replaceCoreMesh()) // if the core mesh needs to be replaced
            WallUtils::ReplaceCoreMesh(obstacleTransform, selected.get_coreMesh());
        if (wallConfig.get_replaceCoreMaterial()) // if the core material needs to be replaced
        {
            if (wallConfig.get_moreThan1CoreMat()) // if there is more than 1 material on the core
                WallUtils::ReplaceCoreSharedMaterials(obstacleTransform, selected.get_coreSharedMaterials());
            else // if there is only 1 material on the frame
                WallUtils::ReplaceCoreMaterial(obstacleTransform, selected.get_coreMaterial());
        }
    }

    void QuestWall::ObstacleController_Set_Hide(GlobalNamespace::ObstacleController* obstacleController, bool value)
    {
        // if no custom walls loaded letting the game do it's own thing is best
        if (!activeWall) return;
        if (!value) ObstacleController_Init_Pre(obstacleController);
    }

    void QuestWall::HandleColorsDidChangeEvent()
    {
        if (!activeWall) return;

        Qosmetics::WallData& selected = *activeWall;

        if (!selected.get_complete())
        {
            getLogger().error("Tried using the wall when it was not completely loaded");
            return;
        }

        WallUtils::SetObstacleColors(selected);     
    }

    void QuestWall::OnActiveWallSet(bool ifLoadAlsoAssets)
    {
        if (!activeWall) 
        {
            config.lastActiveWall = "";
            unsetenv("qwallsenabled");
            getLogger().info("activeWall was nullptr, clearing last active wall");
            return;
        }
        setenv("qwallsenabled", "1", 1);
        config.lastActiveWall = activeWall->get_descriptor()->get_fileName();

        // if not already loaded, and not loading right now, load the bundle and also assets in one go if requested
        if (!activeWall->get_complete() && !activeWall->get_isLoading()) activeWall->LoadBundle(ifLoadAlsoAssets); 
    }

    void QuestWall::CheckScoreDisabling()
    {
        if (activeWall && activeWall->get_config()->get_scoreSubmissionDisabled()) 
        {
            getLogger().info("Custom wall disabled scores");
            bs_utils::Submission::disable(modInfo);
        }
        else if (config.wallConfig.forceCoreOff) 
        {
            getLogger().info("Force Core off disabled scores");
            bs_utils::Submission::disable(modInfo);
        }
        else 
        {
            getLogger().info("Walls had no reason to disable scores");
            bs_utils::Submission::enable(modInfo);
        }
    }

}