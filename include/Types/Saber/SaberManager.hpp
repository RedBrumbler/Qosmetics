#pragma once
#include "custom-types/shared/macros.hpp"
#include "Types/Saber/SaberItem.hpp"
#include "QosmeticsLogger.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, SaberManager, Il2CppObject, 
        
        DECLARE_INSTANCE_METHOD(UnityEngine::GameObject*, GetActivePrefab);
        DECLARE_INSTANCE_METHOD(void, SetActiveModel, Il2CppString* csname);
        DECLARE_INSTANCE_METHOD(void, SetDefault);

        // gets the actual original prefab
        //DECLARE_OVERRIDE_METHOD(UnityEngine::GameObject*, GetActivePrefab, il2cpp_utils::FindMethodUnsafe("Qosmetics", "ModelManager", "GetActivePrefab", 0));
        //DECLARE_OVERRIDE_METHOD(void, SetActiveModel, il2cpp_utils::FindMethodUnsafe("Qosmetics", "ModelManager", "SetActiveModel", 1), Il2CppString* csname);
        //DECLARE_OVERRIDE_METHOD(void, SetDefault, il2cpp_utils::FindMethodUnsafe("Qosmetics", "ModelManager", "SetDefault", 0));
    
        DECLARE_STATIC_METHOD(Il2CppString*, get_basicSaberModelName);
        DECLARE_STATIC_METHOD(Il2CppString*, get_leftSaberName);
        DECLARE_STATIC_METHOD(Il2CppString*, get_rightSaberName);
        
        // gets a copy of either saber, not the original
        DECLARE_INSTANCE_METHOD(UnityEngine::Transform*, get_leftSaber);
        DECLARE_INSTANCE_METHOD(UnityEngine::Transform*, get_rightSaber);

        DECLARE_CTOR(ctor);

    public:
        void internalSetActiveModel(std::string name, bool load = false);
        void SetActiveSaber(std::string name, bool load = false);
        void FromFilePath(std::string path, bool load = false);
        ItemType get_type();
        SaberItem& get_item();
        
    protected:
        SaberItem* activeItem = nullptr;
)