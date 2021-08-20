#pragma once
#include "custom-types/shared/macros.hpp"
#include "Types/Wall/WallItem.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/MeshFilter.hpp"
#include "UnityEngine/Mesh.hpp"
#include "UnityEngine/Material.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, WallManager, Il2CppObject,     
    DECLARE_INSTANCE_METHOD(UnityEngine::GameObject*, GetActivePrefab);
    DECLARE_INSTANCE_METHOD(void, SetActiveModel, Il2CppString* csname);
    DECLARE_INSTANCE_METHOD(void, SetDefault);

    DECLARE_STATIC_METHOD(Il2CppString*, get_coreName);
    DECLARE_STATIC_METHOD(Il2CppString*, get_frameName);

    DECLARE_INSTANCE_METHOD(UnityEngine::Material*, get_coreMaterial);
    DECLARE_INSTANCE_METHOD(UnityEngine::Material*, get_frameMaterial);
    DECLARE_INSTANCE_METHOD(Array<UnityEngine::Material*>*, get_coreMaterialArray);
    DECLARE_INSTANCE_METHOD(Array<UnityEngine::Material*>*, get_frameMaterialArray);
    DECLARE_INSTANCE_METHOD(Array<UnityEngine::Material*>*, get_coreSharedMaterials);
    DECLARE_INSTANCE_METHOD(Array<UnityEngine::Material*>*, get_frameSharedMaterials);
    DECLARE_INSTANCE_METHOD(UnityEngine::Mesh*, get_coreMesh);
    DECLARE_INSTANCE_METHOD(UnityEngine::Mesh*, get_frameMesh);

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
)