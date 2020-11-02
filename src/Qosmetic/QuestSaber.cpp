#include "Qosmetic/QuestSaber.hpp"

ModInfo Qosmetics::QuestSaber::modInfo;

std::vector<std::string> Qosmetics::QuestSaber::fileNames;
std::vector<std::string> Qosmetics::QuestSaber::legacyFileNames;
std::vector<std::string> Qosmetics::QuestSaber::filePaths;
std::vector<Qosmetics::SaberData> Qosmetics::QuestSaber::loadedSabers;

namespace Qosmetics
{
    void QuestSaber::makeFolder(std::string directory)
    {
        if (!direxists(directory.c_str()))
        {
            int makePath = mkpath(directory.data());
            if (makePath == -1)
            {
                getLogger().debug("Failed to make path %s", directory.c_str());
            }
        }
    }

    bool QuestSaber::ShaderWarmup()
    {
        if (!direxists(fileDir)) 
        {
            getLogger().info("Saber Directory did not exist, creating directory at %s", fileDir.c_str());
            makeFolder(fileDir);
        }
        else getLogger().info("Saber Directory Exists");

        // get all file names in the saber directory, if none found check the legacy saber directory for sabers (for compatibility with old sabers)
        bool foundFilesNewDir = FileUtils::getFileNamesInDir("qsaber", fileDir, fileNames);
        for (auto fileName : fileNames)
        {
            filePaths.push_back(fileDir + fileName);
        }

        if (!foundFilesNewDir) 
        {
            if (!direxists(legacyFileDir))
            {
                getLogger().info("Saber Directory did not exist, creating directory at %s", legacyFileDir.c_str());
                makeFolder(legacyFileDir);
            }
            else getLogger().info("Legacy Saber Directory Exists");
            
            FileUtils::getFileNamesInDir("qsaber", legacyFileDir, legacyFileNames);
            for (auto fileName : legacyFileNames)
            {
                filePaths.push_back(legacyFileDir + fileName);
            }
        }
        // if no files were found, just return
        if (filePaths.size() == 0)
        {
            getLogger().error("no filepaths were found, returning false...");
            return false;
        } 

        // each file is a saber, so for each file
        for (int i = 0; i < filePaths.size(); i++)
        {
            // make a new saber
            SaberData newSaber;

            // add it to the list
            loadedSabers.emplace_back(newSaber);

            // load bundle of filename
            loadedSabers[i].LoadBundle(filePaths[i]);
        }

        return true;
    };

    void QuestSaber::HealthWarning()
    {
        // for all loaded saber files, load the assets in them (if there are none loaded it won't actually do anything)
        for (auto& saber : loadedSabers)
        {
            saber.LoadAssets();
        }
    };

    
    void QuestSaber::MenuViewControllers()
    {
        if (loadedSabers.size() == 0)
        {
            getLogger().error("Tried using saber mod while no sabers were loaded");
            return;
        }

        for (auto &saber : loadedSabers)
        {
            if (!saber.get_complete() && !saber.get_isLoading()) 
            {
                saber.LoadAssets();
            }
        }
    }

    void QuestSaber::GameCore()
    {
        // if no sabers loaded, skip this code
        if (loadedSabers.size() == 0)
        {
            getLogger().error("Tried using saber mod while no sabers were loaded");
            return;
        }
        
        // selected saber from the loadedsabers vector, in the future this may be selectable
        selectedSaber = 0;

        loadedSabers[selectedSaber].ClearActive();
    };
    
    void QuestSaber::SaberStart(GlobalNamespace::Saber* instance)
    {
        SaberData& selected = loadedSabers[selectedSaber];
        if (instance->get_saberType().value == 0) return;
        if (!selected.get_complete())
        {
            getLogger().error("Tried using the saber while it was not finished loading");
            return;
        }

        // replace the saber
        SaberUtils::AddSaber(instance, selected);
        Qosmetics::SaberConfig config = *selected.saberConfig;

        // get the transform in order to find the other neccesary transforms lower in the hierarchy
        UnityEngine::Transform* saberTransform = instance->get_transform();

        UnityEngine::Transform* basicSaberModel = saberTransform->Find(il2cpp_utils::createcsstr("BasicSaberModel(Clone)"));
        UnityEngine::Transform* customSaber = saberTransform->Find(il2cpp_utils::createcsstr(instance->get_saberType().value == 0 ? "LeftSaber" : "RightSaber"));
        
        // if both transforms are found and the saber doesn't have custom trails, move the trail inside the leftsaber/rightsaber object per request of MichaelZoller
        if (basicSaberModel != nullptr && customSaber != nullptr && !config.get_hasCustomTrails()) 
        {
            TrailUtils::MoveTrail(basicSaberModel, customSaber);
            
        }
        // if the saber has custom trails, log the fact that it did and that that is why the trail was not moved (it will be disabled later on)
        else if (config.get_hasCustomTrails())
        {
            getLogger().info("Saber had custom trails, not moving trail");
        }
        else
        {
            // last resort, let the logs show which of the 2 was null
            getLogger().error("basicsabermodel null: %d, customSaber null: %d", basicSaberModel == nullptr, customSaber == nullptr);
        }

        if (basicSaberModel != nullptr && customSaber != nullptr && config.get_hasCustomTrails())
        {
            for (auto &trail : *config.get_leftTrails())
            {
                TrailUtils::AddTrail(trail, customSaber);
            }
            for (auto &trail : *config.get_rightTrails())
            {
                TrailUtils::AddTrail(trail, customSaber);
            }
            TrailUtils::RemoveTrail(basicSaberModel);
        }
             
    
        if (config.get_hasCustomWallParticles() && customSaber != nullptr && false) // disabled permanently atm
        {
            // TODO
            /*
            // This code currently doesn't work, so by && with false it just never executes
            GlobalNamespace::ObstacleSaberSparkleEffectManager* sparkleManager = UnityUtils::GetFirstObjectOfType<GlobalNamespace::ObstacleSaberSparkleEffectManager*>(il2cpp_utils::GetClassFromName("", "ObstacleSaberSparkleEffectManager"));
            if (sparkleManager != nullptr)
            {
                sparkleManager->Start();
                GlobalNamespace::ObstacleSaberSparkleEffect* effect = sparkleManager->effects->values[instance->get_saberType().value];
                if (effect != nullptr)
                {
                    effect->Awake();
                    UnityEngine::Transform* wallParticles = customSaber->Find(il2cpp_utils::createcsstr("WallParticles"));
                    if (wallParticles != nullptr)
                    {
                        UnityEngine::ParticleSystem* ps = UnityUtils::GetComponent<UnityEngine::ParticleSystem*>(wallParticles->get_gameObject(), "ParticleSystem");
                        if (ps != nullptr) 
                        {
                            effect->sparkleParticleSystem = ps;
                            effect->sparkleParticleSystemEmmisionModule = effect->sparkleParticleSystem->get_emission();
                        }
                    }
                }
            }*/
        }

        if (config.get_hasCustomSliceParticles() && customSaber != nullptr)
        {
            // TODO
        }

        getLogger().info("End of saber start");
    };
}

