#include "Qosmetic/QuestWall.hpp"
#include "Qosmetic/QosmeticsColorManager.hpp"
#include <thread>

ModInfo Qosmetics::QuestWall::modInfo;

std::vector<std::string> Qosmetics::QuestWall::fileNames;
std::vector<Qosmetics::WallData> Qosmetics::QuestWall::loadedWalls;

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

        // each file is a saber, so for each file
        for (int i = 0; i < fileNames.size(); i++)
        {
            // make a new saber
            WallData newWall;

            // add it to the list
            loadedWalls.emplace_back(newWall);

            // load bundle of filename
            loadedWalls[i].LoadBundle(fileDir + fileNames[i]);
        }

        return true;
    };

    void QuestWall::HealthWarning()
    {
        // for all loaded wall files, load the assets in them (if there are none loaded it won't actually do anything)
        for (auto& wall : loadedWalls)
        {
            wall.LoadAssets();
            
        }
    };

    void QuestWall::GameCore()
    {
        // if no walls loaded, skip this code
        if (loadedWalls.size() == 0)
        {
            getLogger().error("Tried using wall mod while no walls were loaded");
            return;
        }

        

        // selected wall from the loadedwalls vector, in the future this may be selectable
        selectedWall = 0; 
        Qosmetics::WallData& selected = loadedWalls[selectedWall];
        if (!selected.get_complete()) 
        {
            getLogger().error("Tried using the wall while it was not finished loading");
            return;
        }
        // clears the references to the active materials, the object may be non destructable,
        // the references can get corrupted accross scene changes so setting them nullptr to be redefined later is the best
        selected.ClearActive();
        setColors = false;
        if (selected.get_config()->get_scoreSubmissionDisabled()) bs_utils::Submission::disable(modInfo);
    };

    void QuestWall::MenuViewControllers()
    {
        if (loadedWalls.size() == 0)
        {
            getLogger().error("tried using the wall mod while no walls are loaded");
            return;
        }

        for (auto& wall : loadedWalls)
        {
            if (!wall.get_complete() && !wall.get_isLoading()) wall.LoadAssets();
        }
    }    

    
    void QuestWall::ObstacleController_Init_Pre(GlobalNamespace::ObstacleController* obstacleController)
    {
        // if no walls loaded, it's no use so just early return
        if (loadedWalls.size() == 0) return;

        WallData& selected = loadedWalls[selectedWall];
        if (!selected.get_complete()) 
        {
            getLogger().error("Tried replacing things on the wall while it was not finished loading");
            return;
        }

        WallConfig config = *selected.get_config();

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

        if (config.get_disableFakeGlow()) // if fake glow needs to be disabled
            WallUtils::DisableFakeGlow(obstacleTransform);

        if (config.get_disableFrame()) // if the frame needs to be disabled
            WallUtils::DisableFrame(obstacleTransform);
        else // if frame should stay enabled
        {
            if (config.get_replaceFrameMesh()) // if the frame mesh needs to be replaced
                WallUtils::ReplaceFrameMesh(obstacleTransform, selected.get_frameMesh());

            if (config.get_replaceFrameMaterial()) // if the frame material needs to be replaced
            {
                if (config.get_moreThan1FrameMat()) // if there is more than 1 material on the frame
                    WallUtils::ReplaceFrameSharedMaterials(obstacleTransform, selected.get_frameSharedMaterials());
                else // if there is only 1 material on the frame
                    WallUtils::ReplaceFrameMaterial(obstacleTransform, selected.get_frameMaterial());
            }
        }

        if (config.get_replaceCoreMesh()) // if the core mesh needs to be replaced
            WallUtils::ReplaceCoreMesh(obstacleTransform, selected.get_coreMesh());
        if (config.get_replaceCoreMaterial()) // if the core material needs to be replaced
        {
            if (config.get_moreThan1CoreMat()) // if there is more than 1 material on the core
                WallUtils::ReplaceCoreSharedMaterials(obstacleTransform, selected.get_coreSharedMaterials());
            else // if there is only 1 material on the frame
                WallUtils::ReplaceCoreMaterial(obstacleTransform, selected.get_coreMaterial());
        }
    }

    void QuestWall::ObstacleController_Set_Hide(GlobalNamespace::ObstacleController* obstacleController, bool value)
    {
        // if no custom walls loaded letting the game do it's own thing is best
        if (loadedWalls.size() == 0)
        {
            getLogger().error("Tried calling hide function while no walls are loaded");
            return;
        }
        
        if (!value) ObstacleController_Init_Pre(obstacleController);
    }

    void QuestWall::HandleColorsDidChangeEvent()
    {
        if (loadedWalls.size() == 0)
        {
            getLogger().error("Tried changing colors with no walls loaded");
            return;
        }

        Qosmetics::WallData& selected = loadedWalls[selectedWall];

        if (!selected.get_complete())
        {
            getLogger().error("Tried using the wall when it was not completely loaded");
            return;
        }
        WallUtils::SetObstacleColors(selected);     
    }
}