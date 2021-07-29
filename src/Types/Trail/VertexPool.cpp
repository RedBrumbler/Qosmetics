#include "Types/Trail/VertexPool.hpp"

#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Object.hpp"
#include "UnityEngine/Time.hpp"
#include "UnityEngine/Rendering/ShadowCastingMode.hpp"

#include "Types/Trail/AltTrail.hpp"

DEFINE_TYPE(Qosmetics, VertexPool);

using namespace UnityEngine;

namespace Qosmetics
{
    void VertexPool::ctor(Material* material, AltTrail* owner)
    {
        _vertexTotal = _vertexUsed = 0;
        _vertCountChanged = false;
        this->owner = owner;
        CreateMeshObj(owner, material);
        _material = material;
        InitArrays();
        IndiceChanged = ColorChanged = UVChanged = UV2Changed = VertChanged = true;
    }

    void VertexPool::RecalculateBounds()
    {
        auto mesh = get_MyMesh();
        if (mesh) mesh->RecalculateBounds();
    }

    Mesh* VertexPool::get_MyMesh()
    {
        if (_meshFilter) return _meshFilter->get_sharedMesh();
        return nullptr;
    }

    void VertexPool::SetMeshObjectActive(bool flag)
    {
        if (!_meshFilter) return;
        _meshFilter->get_gameObject()->SetActive(flag);
    }

    void VertexPool::Destroy()
    {
        if (_gameObject) UnityEngine::Object::Destroy(_gameObject);
    }

    VertexSegment VertexPool::GetVertices(int vcount, int icount)
    {
        int vertNeed = 0;
        int indexNeed = 0;
        if (_vertexUsed + vcount >= _vertexTotal)
        {
            vertNeed = (vcount / BlockSize + 1) * BlockSize;
        }
        if (_indexUsed + icount >= _indexTotal)
        {
            indexNeed = (icount / BlockSize + 1) * BlockSize;
        }
        _vertexUsed += vcount;
        _indexUsed += icount;
        if (vertNeed != 0 || indexNeed != 0)
        {
            EnlargeArrays(vertNeed, indexNeed);
            _vertexTotal += vertNeed;
            _indexTotal += indexNeed;
        }
        return VertexSegment(_vertexUsed - vcount, vcount, _indexUsed - icount, icount, this);
    }

    void VertexPool::EnlargeArrays(int count, int icount)
    {
        auto tempVertices = Vertices;
        Vertices = Array<Vector3>::NewLength(Vertices->Length() + count);
        tempVertices->CopyTo(Vertices, 0);

        auto tempUVs = UVs;
        UVs = Array<Vector2>::NewLength(UVs->Length() + count);
        tempUVs->CopyTo(UVs, 0);
        auto tempColors = Colors;
        Colors = Array<Color>::NewLength(Colors->Length() + count);
        tempColors->CopyTo(Colors, 0);
        auto tempIndices = Indices;
        Indices = Array<int>::NewLength(Indices->Length() + icount);
        tempIndices->CopyTo(Indices, 0);
        
        _vertCountChanged = true;
        IndiceChanged = true;
        ColorChanged = true;
        UVChanged = true;
        VertChanged = true;
        UV2Changed = true;
    }

    void VertexPool::LateUpdate()
    {
        auto mymesh = get_MyMesh();
        if (!mymesh) return;

        if (_vertCountChanged) mymesh->Clear();

        mymesh->set_vertices(Vertices);
        if (UVChanged) mymesh->set_uv(UVs);
        if (ColorChanged) mymesh->set_colors(Colors);
        if (IndiceChanged) mymesh->set_triangles(Indices);

        ElapsedTime += Time::get_deltaTime();

        if (ElapsedTime > BoundsScheduleTime || FirstUpdate)
        {
            RecalculateBounds();
            ElapsedTime = 0.0f;
        }

        if (ElapsedTime > BoundsScheduleTime)
            FirstUpdate = false;
        
        _vertCountChanged = false;
        IndiceChanged = false;
        ColorChanged = false;
        UVChanged = false;
        UV2Changed = false;
        VertChanged = false;
    }

    void VertexPool::CreateMeshObj(AltTrail* owner, Material* material)
    {
        static Il2CppString* saberTrailName = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("SaberTrail");
        _gameObject = GameObject::New_ctor(saberTrailName);
        _gameObject->set_layer(owner->get_gameObject()->get_layer());
        _meshFilter = _gameObject->AddComponent<MeshFilter*>();
        auto meshrenderer = _gameObject->AddComponent<MeshRenderer*>();

        _gameObject->get_transform()->set_position(Vector3::get_zero());
        _gameObject->get_transform()->set_rotation(Quaternion::get_identity());
        
        meshrenderer->set_shadowCastingMode(UnityEngine::Rendering::ShadowCastingMode::Off);
        meshrenderer->set_receiveShadows(false);
        meshrenderer->set_sharedMaterial(material);
        //meshrenderer->set_sortingLayerName(this->owner->SortingLayerName);
        meshrenderer->set_sortingOrder(this->owner->SortingOrder);
        _meshFilter->set_sharedMesh(Mesh::New_ctor());
    }

    void VertexPool::InitArrays()
    {
        Vertices = Array<Vector3>::NewLength(4);
        UVs = Array<Vector2>::NewLength(4);
        Colors = Array<Color>::NewLength(4);
        Indices = Array<int>::NewLength(6);
        _vertexTotal = 4;
        _indexTotal = 6;
    }
}