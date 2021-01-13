#include "config.hpp"
#include "Qosmetic/QuestNote.hpp"
#include "Qosmetic/QosmeticsColorManager.hpp"
#include <thread>
#include "Data/QosmeticsDescriptorCache.hpp"

extern config_t config;

namespace Qosmetics
{   
    void QuestNote::makeFolder()
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

    bool QuestNote::ShaderWarmup()
    {
        if (!direxists(fileDir)) 
        {
            getLogger().info("Bloq Directory did not exist, creating directory at %s", fileDir.c_str());
            makeFolder();
        }
        else getLogger().info("Bloq Directory Exists");

        // get all file names in the bloq directory
        FileUtils::getFileNamesInDir("qbloq", fileDir, fileNames);

        // if no files were found, just return
        if (fileNames.size() == 0) 
        {
            getLogger().error("No Bloq files were found");
            return false;
        }

        // each file is a note, so for each file
        for (int i = 0; i < fileNames.size(); i++)
        {
            Descriptor* descriptor = DescriptorCache::GetDescriptor(fileNames[i], note);
            if (!descriptor->valid) 
            {
                getLogger().info("Note Descriptor was invalid, making a new one");
                descriptor = new Descriptor("", "", "", fileDir + fileNames[i], note, nullptr);
            }
            
            // make a new saber
            NoteData* newNote = new NoteData(descriptor);

            noteMap[descriptor] = newNote;
            //noteMap[descriptor]->LoadBundle();

            DescriptorCache::AddToNoteCache(descriptor);
        }
        Descriptor* descriptor = DescriptorCache::GetDescriptor(config.lastActiveNote, note);
        SetActiveNote(descriptor);
        SelectionDefinitive();
        
        //if (activeNote) activeNote->LoadBundle();
        return true;
    };

    void QuestNote::HealthWarning()
    {
        // for all loaded note files, load the assets in them (if there are none loaded it won't actually do anything)
        if (activeNote && !activeNote->get_complete() && !activeNote->get_isLoading()) activeNote->LoadAssets();
        /*
        for (auto& pair : noteMap)
        {
            pair.second->LoadAssets();
        }
        */
    };

    void QuestNote::MenuViewControllers()
    {
        if (activeNote && !activeNote->get_complete() && !activeNote->get_isLoading()) activeNote->LoadAssets();
        /*
        for (auto& pair : noteMap)
        {
            if (!pair.second->get_complete() && !pair.second->get_isLoading()) pair.second->LoadAssets();
        }
        */
    }

    void QuestNote::GameCore()
    {
        // if no notes loaded, skip this code
        if (!activeNote) return;

        Qosmetics::NoteData& selected = *activeNote;
        selected.ClearActive();
        NoteUtils::clearMaterialList();

        if (!selected.get_complete())
        {
            getLogger().error("Tried using the bloq while it was not finished loading");
            return;
        }

        //auto action = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(classof(UnityEngine::Events::UnityAction*), this, []{});
        selected.ClearMatVectors();
        selected.FindPrefab();
        setColors = false;
    };

    void QuestNote::NoteController_Init_Post(GlobalNamespace::NoteController* noteController)
    {
        NoteUtils::SetNoteSize(noteController->get_transform()->Find(il2cpp_utils::createcsstr("NoteCube")));
        if (!activeNote) return;
        NoteData& selected = *activeNote;
        
        if (!selected.get_complete())
        {
            getLogger().error("Attempted to use bloq that was not finished loading");
            return;
        }

        if (!setColors) // if colors have not been set yet, set them
        {
            ColorManager::Init();
            if (selected.get_config()->get_hasDebris()) // if there is debris, set the color
            {
                NoteUtils::SetColor(selected.get_leftDebris()->get_transform(), true);
                NoteUtils::SetColor(selected.get_rightDebris()->get_transform(), false);
            }
            NoteUtils::SetColor(selected.get_leftArrow()->get_transform(), true);
            NoteUtils::SetColor(selected.get_rightArrow()->get_transform(), false);
            NoteUtils::SetColor(selected.get_leftDot()->get_transform(), true);
            NoteUtils::SetColor(selected.get_rightDot()->get_transform(), false);
            setColors = true;
            getLogger().info("Finished setting colors on objects");
        }
        NoteUtils::ReplaceNote(noteController, selected);
    }

    void QuestNote::NoteDebris_Init_Post(GlobalNamespace::NoteDebris* noteDebris, GlobalNamespace::BeatmapSaveData::NoteType noteType, UnityEngine::Transform* initTransform, UnityEngine::Vector3 cutPoint, UnityEngine::Vector3 cutNormal)
    {
        NoteUtils::SetNoteSize(noteDebris->get_transform());
        if (!activeNote) return;
        if (disableDebris) return;
        NoteData &selected = *activeNote;

        if (!selected.get_complete())
        {
            getLogger().error("Attempted to use bloq that was not finished loading");
            return;
        }
        if (!selected.get_config()->get_hasDebris() || !selected.get_complete()) return;
        NoteUtils::ReplaceDebris(noteDebris, noteType, initTransform, cutPoint, cutNormal, *activeNote);
    }

    void QuestNote::HandleColorsDidChangeEvent()
    {
        if (!activeNote) return;
        NoteData &selected = *activeNote;
        if (!selected.get_complete())
        {
            getLogger().error("Attempted to use bloq that was not finished loading");
            return;
        }
        NoteUtils::HandleColorsDidChangeEvent(selected);
    }

    void QuestNote::BombController_Init_Post(GlobalNamespace::BombNoteController* noteController)
    {
        NoteUtils::SetBombSize(noteController->get_transform());
        if (!activeNote) return;
        NoteData &selected = *activeNote;
        if (!selected.get_complete())
        {
            getLogger().error("Attempted to use bloq that was not finished loading");
            return;
        }

        if (!selected.get_config()->get_hasBomb() || !selected.get_complete())
        {
            if (!selected.get_complete()) getLogger().error("Attempted to use bomb that wasn't loaded");
            if (!selected.get_config()->get_hasBomb()) getLogger().error("Attempted to use bomb that doesn't exist");
            return;
        } 
        NoteUtils::ReplaceBomb(noteController, selected);
    }

    void QuestNote::OnActiveNoteSet(bool ifLoadAlsoAssets)
    {
        if (!activeNote) 
        {
            config.lastActiveNote = "";
            unsetenv("qbloqsenabled");
            getLogger().info("activeNote was nullptr, clearing last active note");
            return;
        }
        setenv("qbloqsenabled", "1", 1);
        config.lastActiveNote = activeNote->get_descriptor()->get_fileName(); 
        getLogger().info("Last active note is now %s, should be %s", config.lastActiveNote.c_str(), activeNote->get_descriptor()->get_filePath().c_str());
        // if not already loaded, and not loading right now, load the bundle and also assets in one go if requested
        if (!activeNote->get_complete() && !activeNote->get_isLoading()) activeNote->LoadBundle(ifLoadAlsoAssets); 
    }

    void QuestNote::ModifierScoreDisableCheck(GlobalNamespace::GameplayModifiers* modifiers)
    {
        getLogger().info("Checking game modifiers in order to disable scores if needed...");
        if((modifiers->get_ghostNotes() || modifiers->get_disappearingArrows()) && activeNote)
        {
            getLogger().info("Ghost notes or disappearing arrows active!");
            bs_utils::Submission::disable(modInfo);
        }
        else if (config.noteConfig.alsoChangeHitboxes)
        {
            getLogger().info("Hitboxes Getting changed, disabling score submission!");
            bs_utils::Submission::disable(modInfo);
        }
        else
        {
            getLogger().info("No reason to disable Submission from Notes!");
            bs_utils::Submission::enable(modInfo);
        }
    }
}