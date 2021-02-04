#pragma once
#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "Types/Wall/WallItem.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, WallManager, UnityEngine::MonoBehaviour,     
    DECLARE_METHOD(UnityEngine::GameObject*, GetActivePrefab);
    DECLARE_METHOD(void, SetActiveModel, Il2CppString* csname);
    DECLARE_METHOD(void, SetDefault);
    
    DECLARE_METHOD(UnityEngine::Material*, get_coreMaterial);
    DECLARE_METHOD(UnityEngine::Material*, get_frameMaterial);
    DECLARE_METHOD(UnityEngine::Material*, get_coreMaterialArray);
    DECLARE_METHOD(UnityEngine::Material*, get_frameMaterialArray);
    DECLARE_METHOD(UnityEngine::Material*, get_coreSharedMaterials);
    DECLARE_METHOD(UnityEngine::Material*, get_frameSharedMaterials);
    public:
        void internalSetActiveModel(std::string name, bool load = false);
        void SetActiveWall(std::string name);
        ItemType get_type();
        WallItem& get_item();
        
    protected:
        WallItem* activeItem;
    REGISTER_FUNCTION(WallManager,
        REGISTER_METHOD(GetACtivePrefab);        
        REGISTER_METHOD(SetActiveModel);
        REGISTER_METHOD(SetDefault);     

        REGISTER_METHOD(get_coreMaterial);
        REGISTER_METHOD(get_frameMaterial);
        REGISTER_METHOD(get_coreMaterialArray);
        REGISTER_METHOD(get_frameMaterialArray);
        REGISTER_METHOD(get_coreSharedMaterials);
        REGISTER_METHOD(get_frameSharedMaterials);
   
    )
)