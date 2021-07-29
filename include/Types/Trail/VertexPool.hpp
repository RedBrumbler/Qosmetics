#pragma once

#include "VertexSegment.hpp"
#include "custom-types/shared/macros.hpp"

#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/MeshFilter.hpp"
#include "UnityEngine/Mesh.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Renderer.hpp"

#include <vector>

namespace Qosmetics {
    class AltTrail;
}

DECLARE_CLASS_CODEGEN(Qosmetics, VertexPool, Il2CppObject,

    DECLARE_INSTANCE_FIELD(Array<UnityEngine::Vector3>*, Vertices);
    DECLARE_INSTANCE_FIELD(Array<int>*, Indices);
    DECLARE_INSTANCE_FIELD(Array<UnityEngine::Vector2>*, UVs);
    DECLARE_INSTANCE_FIELD(Array<UnityEngine::Color>*, Colors);

    DECLARE_INSTANCE_FIELD(UnityEngine::MeshFilter*, _meshFilter);
    DECLARE_INSTANCE_FIELD(UnityEngine::Material*, _material);
    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, _gameObject);

    DECLARE_CTOR(ctor, UnityEngine::Material* material, AltTrail* owner);
    public:

        bool IndiceChanged = false;
        bool ColorChanged = false;
        bool UVChanged = false;
        bool VertChanged = false;
        bool UV2Changed = false;
        bool FirstUpdate = true;

        const int BlockSize = 108;

        float BoundsScheduleTime;
        float ElapsedTime;
    	
        void RecalculateBounds();
        UnityEngine::Mesh* get_MyMesh();
        void SetMeshObjectActive(bool flag);
        void Destroy();
        VertexSegment GetVertices(int vcount, int icount);
        void EnlargeArrays(int count, int icount);
        void LateUpdate();

    private:
        int _vertexTotal;
        int _vertexUsed;
        int _indexTotal;
        int _indexUsed;
        bool _vertCountChanged;

        Qosmetics::AltTrail* owner;
        
        void CreateMeshObj(AltTrail* owner, UnityEngine::Material* material);
        void InitArrays();
)