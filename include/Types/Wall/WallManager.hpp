#pragma once
#include "custom-types/shared/macros.hpp"
#include "Types/Wall/WallItem.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/MeshFilter.hpp"
#include "UnityEngine/Mesh.hpp"
#include "UnityEngine/Material.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, WallManager, Il2CppObject,     
    DECLARE_METHOD(UnityEngine::GameObject*, GetActivePrefab);
    DECLARE_METHOD(void, SetActiveModel, Il2CppString* csname);
    DECLARE_METHOD(void, SetDefault);

    DECLARE_METHOD(static Il2CppString*, get_coreName);
    DECLARE_METHOD(static Il2CppString*, get_frameName);

    DECLARE_METHOD(UnityEngine::Material*, get_coreMaterial);
    DECLARE_METHOD(UnityEngine::Material*, get_frameMaterial);
    DECLARE_METHOD(Array<UnityEngine::Material*>*, get_coreMaterialArray);
    DECLARE_METHOD(Array<UnityEngine::Material*>*, get_frameMaterialArray);
    DECLARE_METHOD(Array<UnityEngine::Material*>*, get_coreSharedMaterials);
    DECLARE_METHOD(Array<UnityEngine::Material*>*, get_frameSharedMaterials);
    DECLARE_METHOD(UnityEngine::Mesh*, get_coreMesh);
    DECLARE_METHOD(UnityEngine::Mesh*, get_frameMesh);

    DECLARE_CTOR(ctor);
    public:
        void internalSetActiveModel(std::string name, bool load = false);
        void SetActiveWall(std::string name, bool load = false);
        void FromFilePath(std::string path, bool load = false);
        ItemType get_type();
        WallItem& get_item();
        
    protected:
        WallItem* activeItem;
        UnityEngine::Transform* get_core();
        UnityEngine::Transform* get_frame();
        UnityEngine::MeshFilter* get_coreFilter();
        UnityEngine::MeshFilter* get_frameFilter();
        UnityEngine::MeshRenderer* get_coreRenderer();
        UnityEngine::MeshRenderer* get_frameRenderer();
    
    REGISTER_FUNCTION(
        REGISTER_METHOD(GetActivePrefab);        
        REGISTER_METHOD(SetActiveModel);
        REGISTER_METHOD(SetDefault);     
        REGISTER_METHOD(ctor);     

        REGISTER_METHOD(get_coreName);
        REGISTER_METHOD(get_frameName);
        

        REGISTER_METHOD(get_coreMaterial);
        REGISTER_METHOD(get_frameMaterial);
        REGISTER_METHOD(get_coreMaterialArray);
        REGISTER_METHOD(get_frameMaterialArray);
        REGISTER_METHOD(get_coreSharedMaterials);
        REGISTER_METHOD(get_frameSharedMaterials);
        REGISTER_METHOD(get_coreMesh);
        REGISTER_METHOD(get_frameMesh);

    )
)