#include "config.hpp"
#include "Qosmetic/QuestSaber.hpp"
#include "Qosmetic/QosmeticsColorManager.hpp"
#include <thread>
#include "Data/QosmeticsDescriptorCache.hpp"

extern config_t config;

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
        if (!direxists(SABERPATH)) 
        {
            getLogger().info("Saber Directory did not exist, creating directory at %s", SABERPATH.c_str());
            makeFolder(SABERPATH);
        }
        else getLogger().info("Saber Directory Exists");

        // get all file names in the saber directory, if none found check the legacy saber directory for sabers (for compatibility with old sabers)
        bool foundFilesNewDir = FileUtils::getFileNamesInDir(SABEREXT, SABERPATH, fileNames);
        for (auto fileName : fileNames)
        {
            filePaths.push_back(SABERPATH + fileName);
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
            Descriptor* descriptor = DescriptorCache::GetDescriptor(FileUtils::GetFileName(filePaths[i]), saber);
            if (!descriptor->valid) 
            {
                getLogger().info("Saber Descriptor was invalid, making a new one");
                descriptor = new Descriptor("", "", "", filePaths[i], saber, nullptr);
            }
            
            // make a new saber
            SaberData* newSaber = new SaberData(descriptor);

            saberMap[descriptor] = newSaber;
            //saberMap[descriptor]->LoadBundle();

            DescriptorCache::AddToSaberCache(descriptor);
        }
        Descriptor* descriptor = DescriptorCache::GetDescriptor(config.lastActiveSaber, saber);
        SetActiveSaber(descriptor);
        SelectionDefinitive();

        //if (activeSaber) activeSaber->LoadBundle(true);
        return true;
    };

    void QuestSaber::HealthWarning()
    {
        if (activeSaber && !activeSaber->get_complete() && !activeSaber->get_isLoading()) activeSaber->LoadAssets();
        /*
        for (auto& pair : saberMap)
        {
            pair.second->LoadAssets();
        }
        */
    };

    
    void QuestSaber::MenuViewControllers()
    {
        if (activeSaber && !activeSaber->get_complete() && !activeSaber->get_isLoading()) activeSaber->LoadAssets();
        /*
        for (auto& pair : saberMap)
        {
            if (!pair.second->get_complete() && !pair.second->get_isLoading()) 
            {
                pair.second->LoadAssets();
            }
        }  
        */
    }

    void QuestSaber::GameCore()
    {
        // if no sabers loaded, skip this code
        if (!activeSaber)
        {
            getLogger().error("Tried using saber mod while no sabers were loaded");
            return;
        }
        
        // selected saber from the loadedsabers vector, in the future this may be selectable
        activeSaber->FindPrefab();
        activeSaber->ClearActive();//loadedSabers[selectedSaber].ClearActive();
        activeSaber->ClearMatVectors();//loadedSabers[selectedSaber].ClearMatVectors();
    };
    
    void QuestSaber::SaberStart(GlobalNamespace::Saber* instance)
    {
        SaberUtils::SetSaberSize(instance->get_transform()->Find(il2cpp_utils::createcsstr("BasicSaberModel(Clone)")));
        if (!activeSaber) 
        {
            UnityEngine::Transform* basicSaberModel = instance->get_transform()->Find(il2cpp_utils::createcsstr("BasicSaberModel(Clone)"));
            if (config.saberConfig.trailType == TrailType::none)
            {
                TrailUtils::RemoveTrail(basicSaberModel);
            }
            else TrailUtils::MoveTrail(basicSaberModel, basicSaberModel); 
            
            return;
        }
        SaberData& selected = *activeSaber;
        if (!selected.get_complete())
        {
            getLogger().error("Tried using the saber while it was not finished loading");
            return;
        }
        ColorManager::Init();
        // replace the saber
        SaberUtils::AddSaber(instance, selected);
        Qosmetics::SaberConfig saberConfig = *selected.saberConfig;

        // get the transform in order to find the other neccesary transforms lower in the hierarchy
        UnityEngine::Transform* saberTransform = instance->get_transform();

        UnityEngine::Transform* basicSaberModel = saberTransform->Find(il2cpp_utils::createcsstr("BasicSaberModel(Clone)"));
        UnityEngine::Transform* customSaber = saberTransform->Find(il2cpp_utils::createcsstr(instance->get_saberType().value == 0 ? "LeftSaber" : "RightSaber"));
        
        // if both transforms are found and the saber doesn't have custom trails, move the trail inside the leftsaber/rightsaber object per request of MichaelZoller
        if (basicSaberModel != nullptr && customSaber != nullptr && (!saberConfig.get_hasCustomTrails() || config.saberConfig.trailType == baseGame))
        {
            TrailUtils::MoveTrail(basicSaberModel, customSaber);
        }
        // if the saber has custom trails, log the fact that it did and that that is why the trail was not moved (it will be disabled later on)
        else if (saberConfig.get_hasCustomTrails())
        {
            getLogger().info("Saber had custom trails, not moving trail");
        }
        else
        {
            // last resort, let the logs show which of the 2 was null
            getLogger().error("basicsabermodel null: %d, customSaber null: %d", basicSaberModel == nullptr, customSaber == nullptr);
        }

        if (basicSaberModel != nullptr && customSaber != nullptr && saberConfig.get_hasCustomTrails() && config.saberConfig.trailType == custom)
        {
            switch (instance->get_saberType().value)
            {
                case 0: // LeftSaber
                    for (auto &trail : *saberConfig.get_leftTrails())
                    {
                        TrailUtils::AddTrail(trail, customSaber);
                    }
                    break;
                case 1: // RightSaber
                    for (auto &trail : *saberConfig.get_rightTrails())
                    {
                        TrailUtils::AddTrail(trail, customSaber);
                    }
                    break;
            }
            TrailUtils::RemoveTrail(basicSaberModel);
        }
        
        if (basicSaberModel && config.saberConfig.trailType == none) 
             TrailUtils::RemoveTrail(basicSaberModel);

        /*
        if (saberConfig.get_hasCustomSaberParticles() && customSaber != nullptr && false) // disabled permanently atm
        {
            // TODO: probably not anymore because this will be doable with an eventsystem which will not be implemented for some time
            
            // This code currently doesn't work, so by && with false it just never executes
            GlobalNamespace::ObstacleSaberSparkleEffectManager* sparkleManager = UnityUtils::GetFirstObjectOfType<GlobalNamespace::ObstacleSaberSparkleEffectManager*>(il2cpp_utils::GetClassFromName("", "ObstacleSaberSparkleEffectManager"));
            if (sparkleManager != nullptr)
            {
                sparkleManager->Start();
                GlobalNamespace::ObstacleSaberSparkleEffect* effect = sparkleManager->effects->values[instance->get_saberType().value];
                if (effect != nullptr)
                {
                    effect->Awake();
                    UnityEngine::Transform* wallParticles = customSaber->Find(il2cpp_utils::createcsstr("SaberParticles"));
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
            }
        }

        if (saberConfig.get_hasCustomSliceParticles() && customSaber != nullptr)
        {
            // TODO
        }
        */
        getLogger().info("End of saber start");
    };

    void QuestSaber::HandleColorsDidChangeEvent()
    {
        if (!activeSaber) return;

        SaberData& selected = *activeSaber;
        if (!selected.get_complete())
        {
            getLogger().error("Tried using the saber while it was not finished loading");
            return;
        }
        SaberUtils::HandleColorsDidUpdateEvent(selected);
    }

    void QuestSaber::ReplaceMenuPointers(UnityEngine::Transform* controller, UnityEngine::XR::XRNode node)
    {
        if (!controller)
        {
            getLogger().error("controller transform was nullptr, not replacing menu pointers");
            return;
        }
        if (!activeSaber) return;
        SaberData& selected = *activeSaber;

        if (!selected.get_complete())
        {
            getLogger().error("Tried using saber that was not finished loading, returning");
            return;
        }

        bool isLeft = node.value == 4; // left == 4, right == 5
        std::string menuHandle = "MenuHandle";
        std::string name = isLeft ? "MenuHandle/CustomLeftPointer" : "MenuHandle/CustomRightPointer";

        if (UnityEngine::Transform* oldPointer = controller->Find(il2cpp_utils::createcsstr(name)))
        {
            SaberUtils::SetCustomColor(oldPointer, isLeft ? 0 : 1);
        }
        else
        {
            selected.FindPrefab();
            SaberUtils::AddMenuPointerSaber(controller->Find(il2cpp_utils::createcsstr(menuHandle)), isLeft, selected);
        }

        UpdateMenuPointers(controller, node);
    }

    void QuestSaber::UpdateMenuPointers(UnityEngine::Transform* controller, UnityEngine::XR::XRNode node)
    {
        if (!controller) return;
        bool isLeft = node == 4;

        std::string name = isLeft ? "MenuHandle/CustomLeftPointer" : "MenuHandle/CustomRightPointer";
        if (UnityEngine::Transform* oldPointer = controller->Find(il2cpp_utils::createcsstr(name)))
        {
            SaberUtils::SetCustomColor(oldPointer, isLeft ? 0 : 1);
            oldPointer->set_localScale(UnityEngine::Vector3(config.saberConfig.saberWidth, config.saberConfig.saberWidth, 1.0f) * config.saberConfig.menuPointerSize);
            oldPointer->set_localPosition(UnityEngine::Vector3(0.0f, 0.0f, -0.2f * (1.0f - config.saberConfig.menuPointerSize)));

            Array<Qosmetics::QosmeticsTrail*>* trails = oldPointer->get_gameObject()->GetComponentsInChildren<Qosmetics::QosmeticsTrail*>(true);
            if (config.saberConfig.trailType != TrailType::custom && trails)
            {
                for (int i = 0; i < trails->Length(); i++)
                {
                    Qosmetics::QosmeticsTrail* trail = trails->values[i];
                    if (!trail) continue;
                    trail->set_enabled(false);
                }
            }
            else if (config.saberConfig.trailType == TrailType::custom)
            {
                for (int i = 0; i < trails->Length(); i++)
                {
                    Qosmetics::QosmeticsTrail* trail = trails->values[i];
                    if (!trail) continue;
                    trail->TrailSetup(trail->length, trail->granularity, trail->colorType, trail->whitestep, trail->trailMaterial, trail->trailColor, trail->multiplierColor);
                    trail->Awake();
                    trail->framesPassed = 0;
                    trail->set_enabled(true);
                }
            }
        }
    }

    void QuestSaber::OnActiveSaberSet(bool ifLoadAlsoAssets)
    {
        if (!activeSaber) 
        {
            config.lastActiveSaber = "";
            unsetenv("qsabersenabled");
            getLogger().info("activeSaber was nullptr, clearing last active saber");
            return;
        }
        setenv("qsabersenabled", "1", 1);
        config.lastActiveSaber = activeSaber->saberDescriptor->get_fileName();

        // if not already loaded, and not loading right now, load the bundle and also assets in one go if requested
        if (!activeSaber->get_complete() && !activeSaber->get_isLoading()) activeSaber->LoadBundle(ifLoadAlsoAssets);
        //if (previousActiveSaber && previousActiveSaber != activeSaber && previousActiveSaber->get_complete()) previousActiveSaber->UnloadBundle();
    }
}

