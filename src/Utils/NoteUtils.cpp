#include "Utils/NoteUtils.hpp"

namespace Qosmetics
{
    void NoteUtils::ReplaceNote(GlobalNamespace::NoteController* noteController, Qosmetics::NoteData &customNoteData)
    {
        GlobalNamespace::NoteData* noteData = noteController->get_noteData();
        UnityEngine::Transform* noteTransform = noteController->get_transform();

        ReplaceNoteMaterials(noteTransform, customNoteData);

        UnityEngine::Transform* noteCube = noteTransform->Find(il2cpp_utils::createcsstr("NoteCube"));

        // check if the notecube was even founds
        if (noteCube != nullptr)
        {
            DisableBaseGameNotes(noteCube, customNoteData.get_config()->get_disableBaseGameArrows());
        }
        else // if not found, it might be a tutorial note, check for that
        {
            noteCube = noteTransform->Find(il2cpp_utils::createcsstr("Cube"));

            if (noteCube == nullptr) return; // we are not logging this error, because bombs also call this function and then we'd constantly get error logs which, no...
            getLogger().info("Found tutorial note!");
            DisableBaseGameTutorialNotes(noteCube, customNoteData.get_config()->get_disableBaseGameArrows());
            
        }

        DisableOldNote(noteCube, noteData); // disable any other note type except the wanted one, if it's found enable it
        if (!FindOldNote(noteCube, noteData)) /// if no old note found, add a new one
        {
            getLogger().info("didn't find an old note, adding new one");
            AddNote(noteCube, noteController, customNoteData);
        }
    }

    void NoteUtils::DisableBaseGameNotes(UnityEngine::Transform* noteCube, bool disableArrows)
    {
        if (noteCube == nullptr) 
        {
            getLogger().error("tried disabling base game notes on nullptr notecube transform");
            return;
        }

        if(disableArrows)
        {
            UnityEngine::Transform* noteArrow = noteCube->Find(il2cpp_utils::createcsstr("NoteArrow"));
            UnityEngine::Transform* noteArrowGlow = noteCube->Find(il2cpp_utils::createcsstr("NoteArrowGlow"));
            UnityEngine::Transform* noteCircleGlow = noteCube->Find(il2cpp_utils::createcsstr("NoteCircleGlow"));

            if (noteCircleGlow != nullptr) noteCircleGlow->get_gameObject()->SetActive(false);
            else getLogger().error("Couldn't find circleGlow");
            if (noteArrow != nullptr) noteArrow->get_gameObject()->SetActive(false);
            else getLogger().error("Couldn't find arrow");
            if (noteArrowGlow != nullptr) noteArrowGlow->get_gameObject()->SetActive(false);
            else getLogger().error("Couldn't find arrowGlow");
        }

        UnityEngine::MeshFilter* cubeFilter = UnityUtils::GetComponent<UnityEngine::MeshFilter*>(noteCube, "MeshFilter");
        // if cube renderer not found, don't try to disable it (it crashes sort of so that's not great)
        if (cubeFilter != nullptr) cubeFilter->set_mesh(nullptr);
        else getLogger().error("Cube Filter for note was not found");
    }

    void NoteUtils::DisableBaseGameTutorialNotes(UnityEngine::Transform* noteCube, bool disableArrows)
    {
        if (noteCube == nullptr) 
        {
            getLogger().error("tried disabling base game notes on nullptr notecube transform");
            return;
        }

        if(disableArrows)
        {
            UnityEngine::Transform* noteArrow = noteCube->Find(il2cpp_utils::createcsstr("Arrow"));
            UnityEngine::Transform* noteArrowGlow = noteCube->Find(il2cpp_utils::createcsstr("ArrowGlow"));
            UnityEngine::Transform* noteCircleGlow = noteCube->Find(il2cpp_utils::createcsstr("Circle"));

            if (noteCircleGlow != nullptr) noteCircleGlow->get_gameObject()->SetActive(false);
            if (noteArrow != nullptr) noteArrow->get_gameObject()->SetActive(false);
            if (noteArrowGlow != nullptr) noteArrowGlow->get_gameObject()->SetActive(false);
        }

        UnityEngine::MeshRenderer* cubeRenderer = UnityUtils::GetComponent<UnityEngine::MeshRenderer*>(noteCube, "MeshRenderer");
        // if cube renderer not found, don't try to disable it (it crashes sort of so that's not great)
        if (cubeRenderer != nullptr) cubeRenderer->set_enabled(false);
    }

    void NoteUtils::AddNote(UnityEngine::Transform* note, GlobalNamespace::NoteController* noteController, Qosmetics::NoteData &customNoteData)
    {
        if (note == nullptr) 
        {
            getLogger().error("Note transform was nullptr, skipping add note...");
            return;
        }
        else if (noteController == nullptr)
        {
            getLogger().error("Note controller was nullptr, skipping add note...");
            return;
        }
        
        GlobalNamespace::NoteData* noteData = (GlobalNamespace::NoteData*)noteController->get_noteData();

        if (noteData == nullptr)
        {
            getLogger().error("NoteData was nullptr, skipping add note...");
            return;
        }

        bool isLeft = false, isBomb = false, isGhost = false, isDot = false;
        FindNoteType(noteData, isLeft, isDot, isGhost, isBomb);

        if (isBomb) return;

        UnityEngine::GameObject* prefab = nullptr;
        std::string name = "";
        if (isLeft) // left note
        {
            if (!isDot) // left arrow note
            {
                prefab = customNoteData.get_leftArrow();
                name = "customLeftArrow";
            }
            else // left dot note
            {
                prefab = customNoteData.get_leftDot();
                name = "customLeftDot";
            }
        }
        else // right note
        {
            if (!isDot) // right arrow note
            {
                prefab = customNoteData.get_rightArrow();
                name = "customRightArrow";
            }
            else // right dot note
            {
                prefab = customNoteData.get_rightDot();
                name = "customRightDot";
            }
        }
        if (prefab != nullptr)
        {
            getLogger().info("Spawning custom note");
            UnityEngine::GameObject* instantiatedNote = (UnityEngine::GameObject*)UnityEngine::Object::Instantiate((UnityEngine::Object*)prefab);

            instantiatedNote->get_transform()->SetParent(note);
            instantiatedNote->get_transform()->set_localPosition(UnityEngine::Vector3::get_zero());
            instantiatedNote->get_transform()->set_localEulerAngles(UnityEngine::Vector3::get_zero());
            instantiatedNote->get_transform()->set_localScale(UnityEngine::Vector3::get_one() * 0.4);

            instantiatedNote->set_name(il2cpp_utils::createcsstr(name));
            instantiatedNote->SetActive(true);
        }
        else
        {
            getLogger().error("Spawning note was not succesfull due to prefab being nullptr");
        }
    }

    bool NoteUtils::FindOldDebris(UnityEngine::Transform* note, GlobalNamespace::BeatmapSaveData::NoteType type)
    {
        if (note == nullptr) 
        {
            getLogger().error("note was nullptr, skipping findOldDebris with default false...");
            return false;
        }
        UnityEngine::Transform* oldDebris = nullptr;
        if (type.value == 0) // left debris
        {
            oldDebris = note->Find(il2cpp_utils::createcsstr("customLeftDebris"));
        }
        else if (type.value == 1) // right debris
        {
            oldDebris = note->Find(il2cpp_utils::createcsstr("customRightDebris"));
        }
        return oldDebris != nullptr;
    }

    void NoteUtils::DisableBaseGameDebris(UnityEngine::Transform* noteDebrisMesh)
    {
        if (noteDebrisMesh == nullptr) return;
        UnityEngine::MeshRenderer* debrisRenderer = UnityUtils::GetComponent<UnityEngine::MeshRenderer*>(noteDebrisMesh->get_gameObject(), "MeshRenderer");
        if (debrisRenderer != nullptr) debrisRenderer->set_enabled(false);
    }

    void NoteUtils::AddDebris(UnityEngine::Transform* noteDebrisMesh, GlobalNamespace::BeatmapSaveData::NoteType noteType, Qosmetics::NoteData &customNoteData)
    {
        if (noteDebrisMesh == nullptr) return;
        UnityEngine::GameObject* prefab = nullptr;
        std::string name = "";
        if (noteType.value == 0) // left note
        {
            prefab = customNoteData.get_leftDebris();
            name = "customLeftDebris";
        }
        else if (noteType.value == 1) // right note
        {
            prefab = customNoteData.get_rightDebris();
            name = "customRightDebris";
        }

        if (prefab != nullptr)
        {
            UnityEngine::GameObject* instantiatedDebris = (UnityEngine::GameObject*)UnityEngine::Object::Instantiate((UnityEngine::Object*)prefab);

            instantiatedDebris->get_transform()->SetParent(noteDebrisMesh);
            instantiatedDebris->get_transform()->set_localPosition(UnityEngine::Vector3::get_zero());
            instantiatedDebris->get_transform()->set_localScale(UnityEngine::Vector3::get_one() * 0.4f);
            instantiatedDebris->get_transform()->set_localEulerAngles(UnityEngine::Vector3::get_zero());

            instantiatedDebris->set_name(il2cpp_utils::createcsstr(name));
        }
    }

    void NoteUtils::DisableOldDebris(UnityEngine::Transform* noteDebrisMesh, GlobalNamespace::BeatmapSaveData::NoteType noteType)
    {
        UnityEngine::Transform* thisDebris = nullptr;
        UnityEngine::Transform* otherDebris = nullptr;

        if (noteType.value == 0) // left note
        {
            thisDebris = noteDebrisMesh->Find(il2cpp_utils::createcsstr("customLeftDebris"));
            otherDebris = noteDebrisMesh->Find(il2cpp_utils::createcsstr("customRightDebris"));
        }
        else if (noteType.value == 1) // right note
        {
            thisDebris = noteDebrisMesh->Find(il2cpp_utils::createcsstr("customRightDebris"));
            otherDebris = noteDebrisMesh->Find(il2cpp_utils::createcsstr("customLeftDebris"));
        }

        if (thisDebris != nullptr) thisDebris->get_gameObject()->SetActive(true);
        if (otherDebris != nullptr) otherDebris->get_gameObject()->SetActive(false);
    }

    void NoteUtils::SetDebrisVectorData(UnityEngine::Transform* base, UnityEngine::Transform* initTransform, UnityEngine::Vector3 cutPoint, UnityEngine::Vector3 cutNormal)
    {
        UnityEngine::Vector3 vector = initTransform->InverseTransformPoint(cutPoint);
        UnityEngine::Vector3 vector2 = initTransform->InverseTransformVector(cutNormal);

        float magnitude = vector.get_magnitude();

        if (magnitude > 0.2 * 0.2)
        {
            vector = 0.2f * vector / UnityEngine::Mathf::Sqrt(magnitude);
        }

        UnityEngine::Vector4 slicePos;

        slicePos.x = vector.x;
        slicePos.y = vector.y;
        slicePos.z = vector.z;

        UnityEngine::Vector4 cutPlane;

        cutPlane.x = vector2.x;
        cutPlane.y = vector2.y;
        cutPlane.z = vector2.z;

        Array<UnityEngine::Renderer*>* renderers =  base->GetComponentsInChildren<UnityEngine::Renderer*>();
        int rendererCount = renderers->Length();

        for (int i = 0; i < rendererCount; i++)
        {
            UnityEngine::Material* material = renderers->values[i]->get_material();

            UnityEngine::Vector4 transformOffset;

            transformOffset.x = renderers->values[i]->get_gameObject()->get_transform()->get_localPosition().x;
            transformOffset.y = renderers->values[i]->get_gameObject()->get_transform()->get_localPosition().y;
            transformOffset.z = renderers->values[i]->get_gameObject()->get_transform()->get_localPosition().z;


            if (material->HasProperty(MaterialUtils::PropertyNameToShaderID("_SlicePos"))) material->SetVector(il2cpp_utils::createcsstr("_SlicePos"), slicePos);
            if (material->HasProperty(MaterialUtils::PropertyNameToShaderID("_CutPlane"))) material->SetVector(il2cpp_utils::createcsstr("_CutPlane"), cutPlane);
            if (material->HasProperty(MaterialUtils::PropertyNameToShaderID("_TransformOffset"))) material->SetVector(il2cpp_utils::createcsstr("_TransformOffset"), transformOffset);
            renderers->values[i]->set_sortingLayerID(9);
        }
    }

    void NoteUtils::ReplaceDebris(GlobalNamespace::NoteDebris* noteDebris, GlobalNamespace::BeatmapSaveData::NoteType noteType, UnityEngine::Transform* initTransform, UnityEngine::Vector3 cutPoint, UnityEngine::Vector3 cutNormal, Qosmetics::NoteData &customNoteData)
    {
        UnityEngine::Transform* note = noteDebris->get_transform();

        ReplaceDebrisMaterials(note, customNoteData);

        UnityEngine::Transform* NoteDebrisMesh = note->Find(il2cpp_utils::createcsstr("NoteDebrisMesh"));
        DisableBaseGameDebris(NoteDebrisMesh);
        DisableOldDebris(NoteDebrisMesh, noteType); /// disable any old debris of the other type that's in this object
        if (!FindOldDebris(NoteDebrisMesh, noteType))
        {
            AddDebris(NoteDebrisMesh, noteType, customNoteData);
        }

        SetDebrisVectorData(NoteDebrisMesh, initTransform, cutPoint, cutNormal);
    }

    void NoteUtils::ReplaceBomb(GlobalNamespace::BombNoteController* noteController, Qosmetics::NoteData &customNoteData)
    {
        // the bomb method is much more straight forward so no need to make seperate methods for finding old bombs and such
        UnityEngine::Transform* bomb = noteController->get_transform();

        ReplaceBombMaterials(bomb, customNoteData);

        UnityEngine::Transform* mesh = bomb->Find(il2cpp_utils::createcsstr("Mesh"));
        if (mesh == nullptr) return;

        UnityEngine::MeshRenderer* bombRenderer = UnityUtils::GetComponent<UnityEngine::MeshRenderer*>(mesh, "MeshRenderer");
        bombRenderer->set_enabled(false);

        UnityEngine::Transform* oldBomb = mesh->Find(il2cpp_utils::createcsstr("customBomb"));
        
        if (oldBomb == nullptr)
        {
            UnityEngine::GameObject* instantiatedBomb = (UnityEngine::GameObject*)UnityEngine::Object::Instantiate((UnityEngine::Object*)customNoteData.get_bomb());

            instantiatedBomb->get_transform()->SetParent(mesh->get_transform());
            instantiatedBomb->get_transform()->set_localPosition(UnityEngine::Vector3::get_zero());
            instantiatedBomb->get_transform()->set_localEulerAngles(UnityEngine::Vector3::get_zero());
            instantiatedBomb->get_transform()->set_localScale(UnityEngine::Vector3::get_one() * 0.4);

            instantiatedBomb->set_name(il2cpp_utils::createcsstr("customBomb"));
        }
        else
        {
            oldBomb->get_gameObject()->SetActive(true);
        }
    }

    bool NoteUtils::FindOldNote(UnityEngine::Transform* note, GlobalNamespace::NoteData* noteData)
    {
        if (note == nullptr) return false;
        bool isLeft = false, isBomb = false, isGhost = false, isDot = false;
        FindNoteType(noteData, isLeft, isDot, isGhost, isBomb);

        if (isGhost || isBomb) return false;
        UnityEngine::Transform* oldNote = nullptr;
        // note is the NoteCube in the note prefab
        if (isLeft)
        {
            if (!isDot)
            {   // left arrow note
                oldNote = note->Find(il2cpp_utils::createcsstr("customLeftArrow"));
            }
            else
            {   // left dot note
                oldNote = note->Find(il2cpp_utils::createcsstr("customLeftDot"));
            }
        }
        else
        {
            if (!isDot)
            {   // right arrow note
                oldNote = note->Find(il2cpp_utils::createcsstr("customRightArrow"));
            }
            else
            {   // right dot note
                oldNote = note->Find(il2cpp_utils::createcsstr("customRightDot"));
            }
        }
        // if oldnote is nullptr it was not found thus this returns false, if it was found then this will return true as oldnote would not be a nullptr
        return oldNote != nullptr;
    }

    void NoteUtils::DisableOldNote(UnityEngine::Transform* note, GlobalNamespace::NoteData* noteData)
    {
        if (note == nullptr)
        {
            getLogger().error("Tried disabling old custom notes on nullptr note");
            return;
        }
        bool isLeft = false, isBomb = false, isGhost = false, isDot = false;
        FindNoteType(noteData, isLeft, isDot, isGhost, isBomb);
        if (isGhost || isBomb) return;

        std::vector<UnityEngine::Transform*> toDisable; // vector to store all stuff in that will need to be disabled
        if (isLeft)
        {
            if (!isDot)
            {   // left arrow note should stay
                UnityEngine::Transform* leftDot = note->Find(il2cpp_utils::createcsstr("customLeftDot"));
                UnityEngine::Transform* rightArrow = note->Find(il2cpp_utils::createcsstr("customRightArrow"));
                UnityEngine::Transform* rightDot = note->Find(il2cpp_utils::createcsstr("customRightDot"));

                if (leftDot != nullptr) toDisable.push_back(leftDot);
                if (rightArrow != nullptr) toDisable.push_back(rightArrow);
                if (rightDot != nullptr) toDisable.push_back(rightDot);

                UnityEngine::Transform* leftArrow = note->Find(il2cpp_utils::createcsstr("customLeftArrow"));

                if (leftArrow != nullptr) leftArrow->get_gameObject()->SetActive(true);
            }
            else
            {   // left dot note should stay
                UnityEngine::Transform* leftArrow = note->Find(il2cpp_utils::createcsstr("customLeftArrow"));
                UnityEngine::Transform* rightArrow = note->Find(il2cpp_utils::createcsstr("customRightArrow"));
                UnityEngine::Transform* rightDot = note->Find(il2cpp_utils::createcsstr("customRightDot"));

                if (leftArrow != nullptr) toDisable.push_back(leftArrow);
                if (rightArrow != nullptr) toDisable.push_back(rightArrow);
                if (rightDot != nullptr) toDisable.push_back(rightDot);

                UnityEngine::Transform* leftDot = note->Find(il2cpp_utils::createcsstr("customLeftDot"));

                if (leftDot != nullptr) leftDot->get_gameObject()->SetActive(true);
            }
        }
        else // if Right note
        {
            if (!isDot)
            {   // right arrow note should stay
                UnityEngine::Transform* leftDot = note->Find(il2cpp_utils::createcsstr("customLeftDot"));
                UnityEngine::Transform* leftArrow = note->Find(il2cpp_utils::createcsstr("customLeftArrow"));
                UnityEngine::Transform* rightDot = note->Find(il2cpp_utils::createcsstr("customRightDot"));

                if (leftDot != nullptr) toDisable.push_back(leftDot);
                if (leftArrow != nullptr) toDisable.push_back(leftArrow);
                if (rightDot != nullptr) toDisable.push_back(rightDot);

                UnityEngine::Transform* rightArrow = note->Find(il2cpp_utils::createcsstr("customRightArrow"));

                if (rightArrow != nullptr) rightArrow->get_gameObject()->SetActive(true);
            }
            else
            {   // right dot note should stay
                UnityEngine::Transform* leftDot = note->Find(il2cpp_utils::createcsstr("customLeftDot"));
                UnityEngine::Transform* rightArrow = note->Find(il2cpp_utils::createcsstr("customRightArrow"));
                UnityEngine::Transform* leftArrow = note->Find(il2cpp_utils::createcsstr("customLeftArrow"));

                if (leftDot != nullptr) toDisable.push_back(leftDot);
                if (rightArrow != nullptr) toDisable.push_back(rightArrow);
                if (leftArrow != nullptr) toDisable.push_back(leftArrow);

                UnityEngine::Transform* rightDot = note->Find(il2cpp_utils::createcsstr("customRightDot"));

                if (rightDot != nullptr) rightDot->get_gameObject()->SetActive(true);
            }
        }

        // disable all object added to the array
        for (auto ob : toDisable)
        {
            ob->get_gameObject()->SetActive(false);
        }
    }

    void NoteUtils::SetColor(UnityEngine::Transform* object, bool isLeft)
    {
        getLogger().info("Attempting to set colors on custom Bloqs");
        GlobalNamespace::ColorManager* colorManager = UnityUtils::GetLastObjectOfType<GlobalNamespace::ColorManager*>(il2cpp_utils::GetClassFromName("", "ColorManager"));

        if (colorManager == nullptr)
        {   
            getLogger().error("ColorManager was nullptr, skipping setting colors...");
            return;
        }
        
        UnityEngine::Color thisColor = isLeft ? colorManager->ColorForSaberType(0) : colorManager->ColorForSaberType(1);
        UnityEngine::Color otherColor = isLeft ? colorManager->ColorForSaberType(1) : colorManager->ColorForSaberType(0);

        Array<UnityEngine::Renderer*>* renderers = object->GetComponentsInChildren<UnityEngine::Renderer*>();
        if (renderers == nullptr) 
        {
            getLogger().error("Found array of renderers was nullptr, skipping setting colors...");
            return;
        }
        int rendererCount = renderers->Length();
        typedef function_ptr_t<Array<UnityEngine::Material*>*, UnityEngine::Renderer*> GetMaterialArrayFunctionType;
        auto GetMaterialArray = *reinterpret_cast<GetMaterialArrayFunctionType>(il2cpp_functions::resolve_icall("UnityEngine.Renderer::GetMaterialArray"));

        for (int i = 0; i < rendererCount; i++)
        {
            UnityEngine::Renderer* current = renderers->values[i];
            if (current == nullptr) continue;
            Array<UnityEngine::Material*>* materials = GetMaterialArray(current);

            for (int j = 0; j < materials->Length(); j++)
            {
                UnityEngine::Material* material = materials->values[j];
                bool setColor = false;
                bool hasCustomColor = material->HasProperty(il2cpp_utils::createcsstr("_CustomColors"));
                std::string matName = to_utf8(csstrtostr(material->get_name()));

                if (hasCustomColor)
                {
                    float customColor = material->GetFloat(UnityEngine::Shader::PropertyToID(il2cpp_utils::createcsstr("_CustomColors")));
                    if (customColor > 0.0f) setColor = true;
                }
                else
                {
                    bool hasGlow = material->HasProperty(il2cpp_utils::createcsstr("_Glow"));

                    if (hasGlow)
                    {
                        float customColor = material->GetFloat(UnityEngine::Shader::PropertyToID(il2cpp_utils::createcsstr("_Glow")));
                        if (customColor > 0.0f) setColor = true;
                    }
                    else
                    {
                        bool hasBloom = material->HasProperty(il2cpp_utils::createcsstr("_Bloom"));

                        if (hasBloom)
                        {
                            float customColor = material->GetFloat(UnityEngine::Shader::PropertyToID(il2cpp_utils::createcsstr("_Bloom")));
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
                {
                    if (material->HasProperty(il2cpp_utils::createcsstr("_Color"))) material->SetColor(il2cpp_utils::createcsstr("_Color"), thisColor);
                    if (material->HasProperty(il2cpp_utils::createcsstr("_OtherColor"))) material->SetColor(il2cpp_utils::createcsstr("_OtherColor"), otherColor);
                }
            }
        }
        
    }

    void NoteUtils::ReplaceBombMaterials(UnityEngine::Transform* bomb, Qosmetics::NoteData &customNoteData)
    {
        AddBombMaterials(bomb);
        if (!customNoteData.get_config()->get_hasBomb() || customNoteData.get_replacedBombMaterials()) return;
        customNoteData.set_replacedBombMaterials(true);
        int i = 0;
        for (auto currentMaterial : materialList)
        {
            std::string materialName = to_utf8(csstrtostr(currentMaterial->get_name()));
            if ((materialName.find("_replace") != std::string::npos) || (materialName.find("_done") != std::string::npos) || materialName == "") // if material has _replace in it, it obviously is a material that came in with the bundles, and should not be used to replace
            {
                getLogger().info("Skipped replace material for %d: %s", i, materialName.c_str());
                continue;
            }
            materialName += "_replace";
            materialName = toLowerCase(materialName);
            MaterialUtils::ReplaceAllMaterialsForGameObjectChildren(customNoteData.get_bomb(), currentMaterial, materialName);
            
            i++;
        }
    }

    void NoteUtils::ReplaceDebrisMaterials(UnityEngine::Transform* debris, Qosmetics::NoteData &customNoteData)
    {
        AddDebrisMaterials(debris);

        if (!customNoteData.get_config()->get_hasDebris() || customNoteData.get_replacedDebrisMaterials()) return;
        customNoteData.set_replacedDebrisMaterials(true);
        int i = 0;
        for (auto currentMaterial : materialList)
        {
            std::string materialName = to_utf8(csstrtostr(currentMaterial->get_name()));
            if ((materialName.find("_replace") != std::string::npos) || (materialName.find("_done") != std::string::npos) || materialName == "") // if material has _replace in it, it obviously is a material that came in with the bundles, and should not be used to replace
            {
                getLogger().info("Skipped replace material for %d: %s", i, materialName.c_str());
                continue;
            }
            materialName += "_replace";
            materialName = toLowerCase(materialName);
            MaterialUtils::ReplaceAllMaterialsForGameObjectChildren(customNoteData.get_leftDebris(), currentMaterial, materialName);
            MaterialUtils::ReplaceAllMaterialsForGameObjectChildren(customNoteData.get_rightDebris(), currentMaterial, materialName);
            
            i++;
        }
    }

    void NoteUtils::ReplaceNoteMaterials(UnityEngine::Transform* note, Qosmetics::NoteData &customNoteData)
    {
        MakeMaterialList(note);
        
        if (customNoteData.get_replacedMaterials()) return;
        customNoteData.set_replacedMaterials(true);
        int i = 0;
        for (auto currentMaterial : materialList)
        {
            std::string materialName = to_utf8(csstrtostr(currentMaterial->get_name()));
            if ((materialName.find("_replace") != std::string::npos) || (materialName.find("_done") != std::string::npos) || materialName == "") // if material has _replace in it, it obviously is a material that came in with the bundles, and should not be used to replace
            {
                getLogger().info("Skipped replace material for %d: %s", i, materialName.c_str());
                continue;
            }
            materialName += "_replace";
            materialName = toLowerCase(materialName);
            MaterialUtils::ReplaceAllMaterialsForGameObjectChildren(customNoteData.get_notePrefab(), currentMaterial, materialName);
            
            i++;
        }
    }

    void NoteUtils::AddDebrisMaterials(UnityEngine::Transform* debrisRoot)
    {
        if (definingDebris || debrisDefined) return;
        definingDebris = true;

        if (debrisRoot == nullptr)
        {
            getLogger().error("tried defining material list from nullptr debris");
            definingDebris = false;
            return;
        }

        Array<UnityEngine::Renderer*>* renderers = debrisRoot->GetComponentsInChildren<UnityEngine::Renderer*>(true);
        if (renderers == nullptr)
        {
            getLogger().error("tried defining material list with nullptr renderer array");
            definingDebris = false;
            return;
        }

        typedef function_ptr_t<Array<UnityEngine::Material*>*, UnityEngine::Renderer*> GetMaterialArrayFunctionType;
        auto GetMaterialArray = *reinterpret_cast<GetMaterialArrayFunctionType>(il2cpp_functions::resolve_icall("UnityEngine.Renderer::GetMaterialArray"));

        if (GetMaterialArray == nullptr) 
        {
            getLogger().error("GetMaterialArray function pointer was nullptr, returning");
            definingDebris = false;
            return;
        }

        for (int i = 0; i < renderers->Length(); i++) // for each renderer
        {
            if (renderers->values[i] == nullptr) continue;
            Array<UnityEngine::Material*>* materials = GetMaterialArray(renderers->values[i]); // for each material on renderer
            if (materials == nullptr) continue;
            for (int j = 0; j < materials->Length(); j++)
            {
                if (materials->values[j] == nullptr) continue;
                materialList.push_back(materials->values[j]);
            }
            GlobalNamespace::ConditionalMaterialSwitcher* switcher = UnityUtils::GetComponent<GlobalNamespace::ConditionalMaterialSwitcher*>(renderers->values[i]->get_gameObject(), "", "ConditionalMaterialSwitcher");
            if (switcher != nullptr)
            {
                materialList.push_back(switcher->material0);
                materialList.push_back(switcher->material1);
            }
        }

        debrisDefined = true;
        definingDebris = false;
    }

    void NoteUtils::AddBombMaterials(UnityEngine::Transform* bombRoot)
    {
        if (definingBomb || bombDefined) return;
        definingBomb = true;

        if (bombRoot == nullptr)
        {
            getLogger().error("tried defining material list from nullptr bomb");
            definingBomb = false;
            return;
        }

        Array<UnityEngine::Renderer*>* renderers = bombRoot->GetComponentsInChildren<UnityEngine::Renderer*>(true);
        if (renderers == nullptr)
        {
            getLogger().error("tried defining material list with nullptr renderer array");
            definingBomb = false;
            return;
        }

        typedef function_ptr_t<Array<UnityEngine::Material*>*, UnityEngine::Renderer*> GetMaterialArrayFunctionType;
        auto GetMaterialArray = *reinterpret_cast<GetMaterialArrayFunctionType>(il2cpp_functions::resolve_icall("UnityEngine.Renderer::GetMaterialArray"));

        if (GetMaterialArray == nullptr) 
        {
            getLogger().error("GetMaterialArray function pointer was nullptr, returning");
            definingBomb = false;
            return;
        }

        for (int i = 0; i < renderers->Length(); i++) // for each renderer
        {
            if (renderers->values[i] == nullptr) continue;
            Array<UnityEngine::Material*>* materials = GetMaterialArray(renderers->values[i]); // for each material on renderer
            if (materials == nullptr) continue;
            for (int j = 0; j < materials->Length(); j++)
            {
                if (materials->values[j] == nullptr) continue;
                materialList.push_back(materials->values[j]);
            }

            GlobalNamespace::ConditionalMaterialSwitcher* switcher = UnityUtils::GetComponent<GlobalNamespace::ConditionalMaterialSwitcher*>(renderers->values[i]->get_gameObject(), "", "ConditionalMaterialSwitcher");
            if (switcher != nullptr)
            {
                materialList.push_back(switcher->material0);
                materialList.push_back(switcher->material1);
            }
        }

        bombDefined = true;
        definingBomb = false;
    }
    void NoteUtils::MakeMaterialList(UnityEngine::Transform* noteRoot)
    {
        if (definingList || listDefined) return;
        definingList = true;

        if (noteRoot == nullptr)
        {
            getLogger().error("tried defining material list from nullptr note");
            definingList = false;
            return;
        }

        Array<UnityEngine::Renderer*>* renderers = noteRoot->GetComponentsInChildren<UnityEngine::Renderer*>(true);
        if (renderers == nullptr)
        {
            getLogger().error("tried defining material list with nullptr renderer array");
            definingList = false;
            return;
        }

        typedef function_ptr_t<Array<UnityEngine::Material*>*, UnityEngine::Renderer*> GetMaterialArrayFunctionType;
        auto GetMaterialArray = *reinterpret_cast<GetMaterialArrayFunctionType>(il2cpp_functions::resolve_icall("UnityEngine.Renderer::GetMaterialArray"));

        if (GetMaterialArray == nullptr) 
        {
            getLogger().error("GetMaterialArray function pointer was nullptr, returning");
            definingList = false;
            return;
        }

        for (int i = 0; i < renderers->Length(); i++) // for each renderer
        {
            if (renderers->values[i] == nullptr) continue;
            Array<UnityEngine::Material*>* materials = GetMaterialArray(renderers->values[i]); // for each material on renderer
            if (materials == nullptr) continue;
            for (int j = 0; j < materials->Length(); j++)
            {
                if (materials->values[j] == nullptr) continue;
                materialList.push_back(materials->values[j]);
            }

            // add the HD version from the materialswitcher to the list as well
            GlobalNamespace::ConditionalMaterialSwitcher* switcher = UnityUtils::GetComponent<GlobalNamespace::ConditionalMaterialSwitcher*>(renderers->values[i]->get_gameObject(), "", "ConditionalMaterialSwitcher");
            if (switcher != nullptr)
            {
                materialList.push_back(switcher->material0);
                materialList.push_back(switcher->material1);
            }
        }
        getLogger().info("note materials list should have been made now");
        listDefined = true;
        definingList = false;
    }
}