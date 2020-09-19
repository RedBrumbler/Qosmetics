#include "Qosmetic/QuestNote.hpp"

ModInfo Qosmetics::QuestNote::modInfo;

std::vector<std::string> Qosmetics::QuestNote::fileNames;
std::vector<Qosmetics::NoteData> Qosmetics::QuestNote::loadedNotes;

namespace Qosmetics
{   
    void QuestNote::makeFolder()
    {
        if (!direxists(fileDir.c_str()))
        {
            int makePath = mkpath(fileDir.data(), 0700);
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

        // each file is a saber, so for each file
        for (int i = 0; i < fileNames.size(); i++)
        {
            // make a new saber
            NoteData newNote;

            // add it to the list
            loadedNotes.emplace_back(newNote);

            // load bundle of filename
            loadedNotes[i].LoadBundle(fileDir + fileNames[i]);
        }

        return true;
    };

    void QuestNote::HealthWarning()
    {
        // for all loaded note files, load the assets in them (if there are none loaded it won't actually do anything)
        for (auto& note : loadedNotes)
        {
            note.LoadAssets();
        }
    };

    void QuestNote::MenuViewControllers()
    {
        if (loadedNotes.size() == 0)
        {
            getLogger().error("Tried using bloq mod while no bloqs were loaded");
            return;
        }

        for (auto& note : loadedNotes)
        {
            if (!note.get_complete() && !note.get_isLoading()) note.LoadAssets();
        }
    }

    void QuestNote::GameCore()
    {
        // if no notes loaded, skip this code
        if (loadedNotes.size() == 0)
        {
            getLogger().error("Tried using bloq mod while no bloqs were loaded");
            return;
        }

        // selected note from the loadednotes vector, in the future this may be selectable
        selectedNote = 0; 
        Qosmetics::NoteData& selected = loadedNotes[selectedNote];

        NoteUtils::clearMaterialList();

        if (!selected.get_complete())
        {
            getLogger().error("Tried using the bloq while it was not finished loading");
            return;
        }
        setColors = false;
    };

    void QuestNote::NoteController_Init_Post(GlobalNamespace::NoteController* noteController)
    {
        if (loadedNotes.size() == 0) return;
        auto& selected = loadedNotes[selectedNote];
        
        if (!selected.get_complete())
        {
            getLogger().error("Attempted to use bloq that was not finished loading");
            return;
        }
        
        if (!setColors) // if colors have not been set yet, set them
        {
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

    void QuestNote::NoteDebris_Init_Post(GlobalNamespace::NoteDebris* noteDebris, GlobalNamespace::NoteType noteType, UnityEngine::Transform* initTransform, UnityEngine::Vector3 cutPoint, UnityEngine::Vector3 cutNormal)
    {
        if (loadedNotes.size() == 0) return;
        if (disableDebris) return;
        auto& selected = loadedNotes[selectedNote];
        if (!selected.get_config()->get_hasDebris() || !selected.get_complete()) return;
        NoteUtils::ReplaceDebris(noteDebris, noteType, initTransform, cutPoint, cutNormal, loadedNotes[selectedNote]);
    }

    void QuestNote::BombController_Init_Post(GlobalNamespace::BombNoteController* noteController)
    {
        if (loadedNotes.size() == 0) return;
        auto& selected = loadedNotes[selectedNote];
        if (!selected.get_config()->get_hasBomb() || !selected.get_complete()) return;
        NoteUtils::ReplaceBomb(noteController, loadedNotes[selectedNote]);
    }
}