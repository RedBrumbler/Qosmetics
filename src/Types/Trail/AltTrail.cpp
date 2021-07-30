#include "Types/Trail/AltTrail.hpp"

#include "UnityEngine/Time.hpp"
#include "QosmeticsLogger.hpp"

#define INFO(value...) QosmeticsLogger::GetContextLogger("AltTrail").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("AltTrail").error(value)

#define LOGPTR(pointer) INFO("%s, %p", #pointer, pointer)
#define LOGCOLOR(color) INFO("%s, %.2f, %.2f, %.2f, %.2f", #color, color.r, color.g, color.b, color.a)
DEFINE_TYPE(Qosmetics, AltTrail);

using namespace UnityEngine;

static constexpr const float frameRateTreshold = 1.0f / 90.0f;

static inline Vector3 Vector3Sub(Vector3& first, Vector3& second)
{
    return Vector3(first.x - second.x, first.y - second.y, first.z - second.z);
}

static inline Vector3 Vector3SubVal(Vector3 first, Vector3 second)
{
    return Vector3Sub(first, second);
}

namespace Qosmetics
{
    bool AltTrail::CapFps = false;
    float AltTrail::trailIntensity = 1.0f;
    
    void AltTrail::ctor()
    {
        INVOKE_CTOR();
    }

    void AltTrail::Setup(TrailInitData& initData, UnityEngine::Transform* pointStart, UnityEngine::Transform* pointEnd, UnityEngine::Material* material, bool editor)
    {
        PointStart = pointStart;
        PointEnd = pointEnd;
        MyMaterial = material;
        
        LOGPTR(PointStart);
        LOGPTR(PointEnd);
        LOGPTR(MyMaterial);
        
        MyColor = initData.TrailColor;
        Granularity = initData.Granularity;
        TrailLength = initData.TrailLength;
        WhiteStep = initData.Whitestep;

        get_gameObject()->set_layer(12);
        if(editor) SortingOrder = 3;
        else SortingOrder = 0;
        
        if (inited) return;
        
        if (!spline) spline = new Spline();
        if (!elemPool) elemPool = new ElementPool(TrailLength);
        if (!vertexPool) vertexPool = *il2cpp_utils::New<VertexPool*>(MyMaterial, this);


        vertexSegment = vertexPool->GetVertices(Granularity * 3, (Granularity - 1) * 12);
        UpdateIndices();

        vertexPool->SetMeshObjectActive(false);

        inited = true;
    }

    void AltTrail::OnDisable()
    {
        if (vertexPool) vertexPool->SetMeshObjectActive(false);
        Collapse();
    }

    void AltTrail::OnEnable()
    {
        if (vertexPool) vertexPool->SetMeshObjectActive(true);
    }

    void AltTrail::LateUpdate()
    {
        if (!inited) return;

        if (CapFps)
        {
            time += Time::get_deltaTime();
            if (time < frameRateTreshold) return;
            time  = 0.0f;
        }

        frameNum ++;

        if (frameNum == skipFirstFrames + 1)
        {
            Reset(true);
            vertexPool->SetMeshObjectActive(true);
        }
        else if (frameNum < (skipFirstFrames + 1)) return;

        
        UpdateHeadElem();
        RecordCurElem();
        RefreshSpline();
        UpdateVertex();
        vertexPool->LateUpdate();
    }

    void AltTrail::OnDestroy()
    {
        if (!inited || !vertexPool) return;
        vertexPool->Destroy();
        
        // empty snapshot list
        for (auto snap : snapshotList)
        {
            elemPool->Release(snap);
        }
        snapshotList.clear();

        // delete pool
        delete (elemPool);
        // delete spline
        delete (spline);
    }

    float AltTrail::get_TrailWidth()
    {
        return Vector3SubVal(PointStart->get_position(), PointEnd->get_position()).get_magnitude();
    }
    
    UnityEngine::Vector3 AltTrail::get_CurHeadPos()
    {
        return Vector3SubVal(PointStart->get_position(), PointEnd->get_position()) / 2.0f;
    }

    void AltTrail::RefreshSpline()
    {
        auto& controlPoints = spline->get_ControlPoints();
        int index = 0;
        for (auto snap : snapshotList)
        {
            controlPoints[index]->Position = snap->get_pos();
            controlPoints[index]->Normal = Vector3Sub(snap->pointEnd, snap->pointStart);
            index++;
        }

        spline->RefreshSpline();
    }

    void AltTrail::UpdateVertex()
    {
        auto* pool = vertexSegment.Pool;

        for (int i = 0; i < Granularity; i++)
        {
            auto baseIdx = vertexSegment.VertStart + i * 3;

            auto uvSegment = (float) i / Granularity;

            auto uvCoord = Vector2::get_zero();

            auto pos = spline->InterpolateByLen(uvSegment);

            auto up = spline->InterpolateNormalByLen(uvSegment);
            auto pos0 = pos + up.get_normalized() * get_TrailWidth() * 0.5f;
            auto pos1 = pos - up.get_normalized() * get_TrailWidth() * 0.5f;

            Color color = MyColor;

            if (uvSegment < WhiteStep)
            {
                color = Color::LerpUnclamped(Color::get_white(), MyColor, uvSegment / WhiteStep);
            }
            
            // pos0
            pool->Vertices->values[baseIdx] = pos0;
            pool->Colors->values[baseIdx] = color;
            uvCoord.x = 0.0f;
            uvCoord.y = uvSegment;
            pool->UVs->values[baseIdx] = uvCoord;

            //pos
            pool->Vertices->values[baseIdx + 1] = pos;
            pool->Colors->values[baseIdx + 1] = color;
            uvCoord.x = 0.5f;
            uvCoord.y = uvSegment;
            pool->UVs->values[baseIdx + 1] = uvCoord;

            //pos1
            pool->Vertices->values[baseIdx + 2] = pos1;
            pool->Colors->values[baseIdx + 2] = color;
            uvCoord.x = 1.0f;
            uvCoord.y = uvSegment;
            pool->UVs->values[baseIdx + 2] = uvCoord;
        }

        vertexSegment.Pool->UVChanged = true;
        vertexSegment.Pool->VertChanged = true;
        vertexSegment.Pool->ColorChanged = true;
    }

    void AltTrail::UpdateIndices()
    {
        INFO("Updating indices");
        auto* pool = vertexSegment.Pool;

        for (int i = 0; i < Granularity - 1; i++)
        {
            int baseIdx = vertexSegment.VertStart + i * 3;
            int nextBaseIdx = vertexSegment.VertStart + (i + 1) * 3;

            int iidx = vertexSegment.IndexStart + i * 12;
            //triangle left
            pool->Indices->values[iidx + 0] = nextBaseIdx;
            pool->Indices->values[iidx + 1] = nextBaseIdx + 1;
            pool->Indices->values[iidx + 2] = baseIdx;
            pool->Indices->values[iidx + 3] = nextBaseIdx + 1;
            pool->Indices->values[iidx + 4] = baseIdx + 1;
            pool->Indices->values[iidx + 5] = baseIdx;
            
            //triangle right
            pool->Indices->values[iidx + 6] = nextBaseIdx + 1;
            pool->Indices->values[iidx + 7] = nextBaseIdx + 2;
            pool->Indices->values[iidx + 8] = baseIdx + 1;
            pool->Indices->values[iidx + 9] = nextBaseIdx + 2;
            pool->Indices->values[iidx + 10] = baseIdx + 2;
            pool->Indices->values[iidx + 11] = baseIdx + 1;
        }

        pool->IndiceChanged = true;
    }

    void AltTrail::UpdateHeadElem()
    {
        snapshotList.front()->pointStart = PointStart->get_position();
        snapshotList.front()->pointEnd = PointEnd->get_position();
    }

    void AltTrail::RecordCurElem()
    {
        Element* elem = elemPool->Get();

        elem->pointStart = PointStart->get_position();
        elem->pointEnd = PointEnd->get_position();

        if (snapshotList.size() < TrailLength)
        {
            snapshotList.insert(snapshotList.begin()++, elem);
        }
        else
        {
            elemPool->Release(snapshotList.back());
            snapshotList.pop_back();
            snapshotList.insert(snapshotList.begin()++, elem);
        }
    }

    void AltTrail::SetColor(Color color)
    {
        color.a *= trailIntensity;
        MyColor = color;
    }

    void AltTrail::dtor()
    {

    }

    void AltTrail::Reset(bool addNewElemsToSnap)
    {
        if (!spline) spline = new Spline();
        bool poolExisted = elemPool;
        if (!poolExisted)
        {
            elemPool = new ElementPool(TrailLength);
        }

        spline->Granularity = Granularity;
        spline->Clear();

        for (int i = 0; i < TrailLength; i++)
        {
            spline->AddControlPoint(get_CurHeadPos(), Vector3SubVal(PointStart->get_position(), PointEnd->get_position()));
        }

        // if snapshot list contains elements, that is an issue, remove them!
        if (snapshotList.size() > 0)
        {
            for (auto snap : snapshotList)
            {
                elemPool->Release(snap);
            }
            snapshotList.clear();
        }

        if (poolExisted && !addNewElemsToSnap)
        {
            snapshotList.push_back(elemPool->Get());
            snapshotList.push_back(elemPool->Get());
        }
        else
        {
            snapshotList.push_back(new Element(PointStart->get_position(), PointEnd->get_position()));
            snapshotList.push_back(new Element(PointStart->get_position(), PointEnd->get_position()));
        }

    }

    void AltTrail::Collapse()
    {
        // makes all parts of the trail end up at the same place, making it basically 0 length
        for (auto snap : snapshotList)
        {
            snap->pointStart = PointStart->get_position();
            snap->pointEnd = PointEnd->get_position();
        }
    }
}