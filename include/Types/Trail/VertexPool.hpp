#pragma once

#include "VertexSegment.hpp"
#include "custom-types/shared/macros.hpp"

//#include "UnityEngine/Vector3.hpp"
//#include "UnityEngine/Vector2.hpp"
//#include "UnityEngine/Color.hpp"

#include "sombrero/shared/Vector3Utils.hpp"
#include "sombrero/shared/Vector2Utils.hpp"
#include "sombrero/shared/ColorUtils.hpp"


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

    DECLARE_INSTANCE_FIELD(Array<Sombrero::FastVector3>*, Vertices);
    DECLARE_INSTANCE_FIELD(Array<int>*, Indices);
    DECLARE_INSTANCE_FIELD(Array<Sombrero::FastVector2>*, UVs);
    DECLARE_INSTANCE_FIELD(Array<Sombrero::FastColor>*, Colors);

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
        void SetMaterial(UnityEngine::Material* material);

    private:
        int vertexTotal = 0;
        int vertexUsed = 0;
        int indexTotal = 0;
        int indexUsed = 0;
        bool vertCountChanged;

        Qosmetics::AltTrail* owner = nullptr;
        
        void CreateMeshObj(AltTrail* owner, UnityEngine::Material* material);
        void InitArrays();
)