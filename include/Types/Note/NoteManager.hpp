#pragma once
#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/Transform.hpp"
#include "Types/Note/NoteItem.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, NoteManager, Il2CppObject,     
    DECLARE_INSTANCE_METHOD(UnityEngine::GameObject*, GetActivePrefab);
    DECLARE_INSTANCE_METHOD(void, SetActiveModel, Il2CppString* csname);
    DECLARE_INSTANCE_METHOD(void, SetDefault);

    DECLARE_INSTANCE_METHOD(UnityEngine::Transform*, get_leftArrow);    
    DECLARE_INSTANCE_METHOD(UnityEngine::Transform*, get_rightArrow);    
    DECLARE_INSTANCE_METHOD(UnityEngine::Transform*, get_leftDot);
    DECLARE_INSTANCE_METHOD(UnityEngine::Transform*, get_rightDot);
    DECLARE_INSTANCE_METHOD(UnityEngine::Transform*, get_leftDebris);
    DECLARE_INSTANCE_METHOD(UnityEngine::Transform*, get_rightDebris);
    DECLARE_INSTANCE_METHOD(UnityEngine::Transform*, get_bomb);
    
    DECLARE_STATIC_METHOD(Il2CppString*, get_leftArrowName);    
    DECLARE_STATIC_METHOD(Il2CppString*, get_rightArrowName);    
    DECLARE_STATIC_METHOD(Il2CppString*, get_leftDotName);
    DECLARE_STATIC_METHOD(Il2CppString*, get_rightDotName);
    DECLARE_STATIC_METHOD(Il2CppString*, get_leftDebrisName);
    DECLARE_STATIC_METHOD(Il2CppString*, get_rightDebrisName);
    DECLARE_STATIC_METHOD(Il2CppString*, get_bombName);
    DECLARE_CTOR(ctor);
    
    public:
        void internalSetActiveModel(std::string name, bool load = false);
        void SetActiveNote(std::string name, bool load);
        void FromFilePath(std::string path, bool load);
        ItemType get_type();
        NoteItem& get_item();
        
    protected:
        NoteItem* activeItem;
)