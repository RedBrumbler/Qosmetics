#pragma once

#include "custom-types/shared/macros.hpp"
#include "Types/Note/NoteManager.hpp"
#include "Types/Colors/ColorManager.hpp"
#include "UnityEngine/MonoBehaviour.hpp"

#include "GlobalNamespace/NoteController.hpp"
#include "GlobalNamespace/NoteData.hpp"
#include <map>
#include "UnityEngine/Transform.hpp"

enum CustomNoteType {
    LeftArrow,
    LeftDot,
    RightArrow,
    RightDot
};

DECLARE_CLASS_CODEGEN(Qosmetics, Note, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD_DEFAULT(GlobalNamespace::NoteController*, gameNoteController, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::NoteManager*, modelManager, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::ColorManager*, colorManager, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(bool, isMirror, false);
    
    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, OnEnable);
    DECLARE_INSTANCE_METHOD(void, Replace);
    DECLARE_INSTANCE_METHOD(void, UpdateModel);
    DECLARE_INSTANCE_METHOD(void, UpdateColors);
    DECLARE_INSTANCE_METHOD(void, Restore);
    DECLARE_INSTANCE_METHOD(void, Init, NoteManager* modelManager, ColorManager* colorManager);
    DECLARE_INSTANCE_METHOD(UnityEngine::Transform*, GetCubeTransform);

    public:
        bool replacedTypes[4];
        static CustomNoteType GetNoteType(GlobalNamespace::NoteController* noteController);
        static CustomNoteType GetNoteType(GlobalNamespace::NoteData* noteData);
        bool get_replaced(CustomNoteType noteType);
        void set_replaced(CustomNoteType noteType);
)