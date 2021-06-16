#include "Types/Note/Note.hpp"
#include "chroma/shared/NoteAPI.hpp"

#include "Utils/UnityUtils.hpp"
#include "Utils/NoteUtils.hpp"
#include "QosmeticsLogger.hpp"

#include "GlobalNamespace/ColorNoteVisuals.hpp"
#include "GlobalNamespace/MaterialPropertyBlockController.hpp"
#include "UnityEngine/MaterialPropertyBlock.hpp"

DEFINE_TYPE(Qosmetics::Note);

using namespace UnityEngine;

#define INFO(value...) QosmeticsLogger::GetContextLogger("Note").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("Note").error(value)

static Il2CppString* noteCube = nullptr;
static Il2CppString* cube = nullptr;

namespace Qosmetics
{
    void Note::Init(NoteManager* modelManager, ColorManager* colorManager)
    {
        if (!noteCube) noteCube = il2cpp_utils::createcsstr("NoteCube", il2cpp_utils::StringType::Manual);
        if (!cube) cube = il2cpp_utils::createcsstr("Cube", il2cpp_utils::StringType::Manual);

        this->modelManager = modelManager;
        this->colorManager = colorManager;

        std::function<void()> callback = std::bind(&Note::UpdateColors, this );
        this->colorManager->RegisterCallback(callback, callbackType::note);
    }

    void Note::OnEnable()
    {
        
    }
    
    void Note::Awake()
    {
        for (int i = 0; i < 4; i++)
        {
            replacedTypes[i] = false;
        }
    }

    void Note::Replace()
    {
        CustomNoteType noteType = GetNoteType(gameNoteController);
        if (!modelManager || modelManager->get_type() != ItemType::note || modelManager->get_item().get_type() != ItemType::note)
        {
            // dont do the size setting again
            if (get_replaced(noteType)) return;
            Transform* noteCubeTransform = get_transform()->Find(noteCube);
            // if not found return, to prevent crashes
            if (!noteCubeTransform) return;
            // set note size for cool funny haha small notes
            NoteUtils::SetNoteSize(noteCubeTransform);
            return;
        }
        // now we are in "always custom" space, this means this code only runs on custom notes

        // if already replaced, don't replace again
        if (get_replaced(noteType)) 
        {
            UpdateModel();
            return;
        }

        Transform* noteCubeTransform = GetCubeTransform();

        // set note size for cool funny haha small notes
        NoteUtils::SetNoteSize(noteCubeTransform);
        // hide the base game notes
        NoteUtils::HideBaseGameNotes(noteCubeTransform, modelManager->get_item().get_config());
        
        Transform* prefab = nullptr;
        Il2CppString* name = nullptr;
        // specific data depending on which note this is
        switch (noteType)
        {
            case CustomNoteType::LeftArrow:
                name = modelManager->get_leftArrowName();
                prefab = modelManager->get_leftArrow();
                break;
            case CustomNoteType::LeftDot:
                name = modelManager->get_leftDotName();
                prefab = modelManager->get_leftDot();
                break;
            case CustomNoteType::RightArrow:
                name = modelManager->get_rightArrowName();
                prefab = modelManager->get_rightArrow();
                break;
            case CustomNoteType::RightDot:
                name = modelManager->get_rightDotName();
                prefab = modelManager->get_rightDot();
                break;
            default:
                break;
        }

        if (prefab)
        {
            // all notes need to be inited the same way
            UnityUtils::SetLayerRecursive(prefab->get_gameObject(), 8);
            prefab->SetParent(noteCubeTransform);
            prefab->set_localEulerAngles(Vector3::get_zero());
            static const Vector3 ZeroPointFour = Vector3::get_one() * 0.4f;
            prefab->set_localScale(ZeroPointFour);
            prefab->set_localPosition(Vector3::get_zero());
            prefab->get_gameObject()->set_name(name);
            set_replaced(noteType);
            
            GlobalNamespace::MaterialPropertyBlockController* propertyController = noteCubeTransform->get_gameObject()->GetComponent<GlobalNamespace::MaterialPropertyBlockController*>();
            NoteUtils::AddRenderersToPropertyBlockController(propertyController, prefab->get_gameObject());
        }
        UpdateModel();
    }

    void Note::UpdateModel()
    {
        if (!modelManager || modelManager->get_type() != ItemType::note) return;
        CustomNoteType noteType = GetNoteType(gameNoteController);
        Transform* noteCubeTransform = GetCubeTransform();
        if (noteCubeTransform)
        {
            for (int i = 0; i < 4; i++)
            {
                if (!replacedTypes[i]) continue;
                Il2CppString* name = nullptr;
                switch (i)
                {
                    case 0:
                        name = modelManager->get_leftArrowName();
                        break;
                    case 1:
                        name = modelManager->get_leftDotName();
                        break;
                    case 2:
                        name = modelManager->get_rightArrowName();
                        break;
                    case 3:
                        name = modelManager->get_rightDotName();
                        break;
                    default:
                        break;
                }
                if (!name) continue;

                Transform* customNote = noteCubeTransform->Find(name);
                if (!customNote) continue;
                customNote->get_gameObject()->SetActive((int)noteType == i);
            }
        }
        
        NoteUtils::SetNoteSize(noteCubeTransform);
        UpdateColors();
    }

    void Note::UpdateColors()
    {
        CustomNoteType noteType = GetNoteType(gameNoteController);
        if (!get_replaced(noteType)) return;

        int colorType = gameNoteController->get_noteData()->get_colorType();
        auto optionalThisColor = Chroma::NoteAPI::getNoteControllerColorSafe(gameNoteController, colorType);
        Color thisColor;
        Color otherColor;
        if (optionalThisColor) // chroma
        {
            thisColor = *optionalThisColor;
            static float h;
            static float s;
            static float v;
            Color::RGBToHSV(thisColor, h, s, v);
            otherColor = Color::HSVToRGB(h + 0.5f, s, v);
        }
        else // not chroma
        {
            thisColor = colorManager->ColorForNoteType(colorType);
            otherColor = colorManager->ColorForNoteType(1 - colorType);
        }

        Il2CppString* name = nullptr;
        switch (noteType)
        {
            case CustomNoteType::LeftArrow:
                name = modelManager->get_leftArrowName();
                break;
            case CustomNoteType::LeftDot:
                name = modelManager->get_leftDotName();
                break;
            case CustomNoteType::RightArrow:
                name = modelManager->get_rightArrowName();
                break;
            case CustomNoteType::RightDot:
                name = modelManager->get_rightDotName();
                break;
            default:
                break;
        }

        if (name)
        {
            Transform* noteCubeTransform = GetCubeTransform();
            if (!noteCubeTransform) return;
            Transform* theNote = noteCubeTransform->Find(name);
            if (!theNote) return;
            NoteUtils::SetColors(theNote->get_gameObject(), thisColor, otherColor, isMirror ? 1955 : 0);
            noteCubeTransform->get_gameObject()->GetComponent<GlobalNamespace::MaterialPropertyBlockController*>()->ApplyChanges();
        }
    }

    void Note::Restore()
    {

    }

    CustomNoteType Note::GetNoteType(GlobalNamespace::NoteController* noteController)
    {
        return GetNoteType(noteController->get_noteData());
    }

    CustomNoteType Note::GetNoteType(GlobalNamespace::NoteData* noteData)
    {
        int colorType = noteData->get_colorType().value << 1;
        int cutType = noteData->get_cutDirection().value >> 3;

        return (CustomNoteType)(colorType + cutType);
    }

    bool Note::get_replaced(CustomNoteType noteType)
    {
        return replacedTypes[(int)noteType];
    }

    void Note::set_replaced(CustomNoteType noteType)
    {
        replacedTypes[(int)noteType] = true;
    }

    Transform* Note::GetCubeTransform()
    {
        Transform* noteCubeTransform = get_transform()->Find(noteCube);
        // if not found, might be a tutorial note
        if (!noteCubeTransform) 
        {
            noteCubeTransform = get_transform()->Find(cube);
        }

        return noteCubeTransform;
    }
}