#include "Types/Trail/VertexPool.hpp"

#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Object.hpp"
#include "UnityEngine/Time.hpp"
#include "UnityEngine/Rendering/ShadowCastingMode.hpp"

#include "Types/Trail/AltTrail.hpp"

#include "QosmeticsLogger.hpp"

#define INFO(value...) QosmeticsLogger::GetContextLogger("VertexPool").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("VertexPool").error(value)

DEFINE_TYPE(Qosmetics, VertexPool);

using namespace UnityEngine;
using namespace Sombrero;

namespace Qosmetics
{
    void VertexPool::ctor(Material* material, AltTrail* owner)
    {
        INVOKE_CTOR();
        vertexTotal = vertexUsed = indexUsed = indexTotal = 0;
        vertCountChanged = false;
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
        if ((vertexUsed + vcount) >= vertexTotal)
        {
            vertNeed = ((vcount / BlockSize) + 1) * BlockSize;
        }

        if ((indexUsed + icount) >= indexTotal)
        {
            indexNeed = ((icount / BlockSize) + 1) * BlockSize;
        }

        vertexUsed += vcount;
        indexUsed += icount;

        if (vertNeed != 0 || indexNeed != 0)
        {
            EnlargeArrays(vertNeed, indexNeed);
            vertexTotal += vertNeed;
            indexTotal += indexNeed;
        }
        return VertexSegment(vertexUsed - vcount, vcount, indexUsed - icount, icount, this);
    }

    void VertexPool::EnlargeArrays(int count, int icount)
    {
        int length = Vertices->Length();
        auto tempVertices = Vertices;
        Vertices = Array<Sombrero::FastVector3>::NewLength(length + count);
        tempVertices->CopyTo(Vertices, 0);

        length = UVs->Length();
        auto tempUVs = UVs;
        UVs = Array<Sombrero::FastVector2>::NewLength(length + count);
        tempUVs->CopyTo(UVs, 0);

        length = Colors->Length();
        auto tempColors = Colors;
        Colors = Array<Sombrero::FastColor>::NewLength(length + count);
        tempColors->CopyTo(Colors, 0);

        length = Indices->Length();
        auto tempIndices = Indices;
        Indices = Array<int>::NewLength(length + icount);
        tempIndices->CopyTo(Indices, 0);
        
        vertCountChanged = true;
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

        if (vertCountChanged) mymesh->Clear();

        mymesh->set_vertices(reinterpret_cast<Array<Vector3>*>(Vertices));
        if (UVChanged) mymesh->set_uv(reinterpret_cast<Array<Vector2>*>(UVs));
        if (ColorChanged) mymesh->set_colors(reinterpret_cast<Array<Color>*>(Colors));
        if (IndiceChanged) mymesh->set_triangles(Indices);

        ElapsedTime += Time::get_deltaTime();

        if (ElapsedTime > BoundsScheduleTime || FirstUpdate)
        {
            RecalculateBounds();
            ElapsedTime = 0.0f;
        }

        if (ElapsedTime > BoundsScheduleTime)
            FirstUpdate = false;
        
        vertCountChanged = false;
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

        _gameObject->get_transform()->set_position(FastVector3::zero());
        _gameObject->get_transform()->set_rotation(Quaternion::get_identity());
        
        meshrenderer->set_shadowCastingMode(UnityEngine::Rendering::ShadowCastingMode::Off);
        meshrenderer->set_receiveShadows(false);
        meshrenderer->set_sharedMaterial(material);
        // not available on quest
        //meshrenderer->set_sortingLayerName(this->owner->SortingLayerName);
        meshrenderer->set_sortingOrder(this->owner->SortingOrder);
        _meshFilter->set_sharedMesh(Mesh::New_ctor());

        SetMeshObjectActive(false);
    }

    void VertexPool::InitArrays()
    {
        Vertices = Array<Sombrero::FastVector3>::NewLength(4);
        UVs = Array<Sombrero::FastVector2>::NewLength(4);
        Colors = Array<Sombrero::FastColor>::NewLength(4);
        Indices = Array<int>::NewLength(6);
        vertexTotal = 4;
        indexTotal = 6;
    }

    void VertexPool::SetMaterial(Material* material)
    {
        if (_gameObject)
        {
            _material = material;
            _gameObject->GetComponent<Renderer*>()->set_sharedMaterial(material);
        }
    }
}