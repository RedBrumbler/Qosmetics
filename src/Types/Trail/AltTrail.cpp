#include "Types/Trail/AltTrail.hpp"

#include "UnityEngine/Time.hpp"
#include "QosmeticsLogger.hpp"

#define INFO(value...) QosmeticsLogger::GetContextLogger("AltTrail").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("AltTrail").error(value)

DEFINE_TYPE(Qosmetics, AltTrail);

using namespace UnityEngine;
using namespace Sombrero;

static constexpr const float frameRateTreshold = 1.0f / 90.0f;

namespace Qosmetics
{
    bool AltTrail::CapFps = false;
    float AltTrail::trailIntensity = 1.0f;
    
    void AltTrail::ctor()
    {
        INVOKE_CTOR();
        INFO("ctor");
    }

    bool AltTrail::get_inited()
    {
        return inited;
    }

    void AltTrail::Setup(TrailInitData& initData, UnityEngine::Transform* pointStart, UnityEngine::Transform* pointEnd, UnityEngine::Material* material, bool editor)
    {
        INFO("setup");
        PointStart = pointStart;
        PointEnd = pointEnd;
        MyMaterial = material;
        
        LOGPTR(PointStart);
        LOGPTR(PointEnd);
        LOGPTR(MyMaterial);
        
        MyColor = initData.TrailColor;
        Granularity = initData.Granularity;
        // if it's size is not at least 4 it breaks
        TrailLength = initData.TrailLength;
        WhiteStep = initData.Whitestep;

        get_gameObject()->set_layer(12);
        if(editor) SortingOrder = 3;
        else SortingOrder = 0;
        
        if (!spline) spline = new Spline(TrailLength);
        if (!elemPool) elemPool = new ElementPool(TrailLength);
        if (!vertexPool) vertexPool = *il2cpp_utils::New<VertexPool*>(MyMaterial, this);
        snapshotList.reserve(TrailLength);

        vertexPool->SetMeshObjectActive(false);
        vertexSegment = vertexPool->GetVertices(Granularity * 3, (Granularity - 1) * 12);
        UpdateIndices();

        if (inited) UpdateTrailData();

        inited = true;
    }

    void AltTrail::UpdateTrailData()
    {
        vertexPool->SetMaterial(MyMaterial);
        Collapse();
        vertexPool->SetMeshObjectActive(true);

        /*
        Reset(false);
        Collapse();
        */
    }

    void AltTrail::OnDisable()
    {
        if (!inited) return;
        Collapse();
        if (vertexPool) vertexPool->SetMeshObjectActive(false);
    }

    void AltTrail::OnEnable()
    {
        if (!inited) return;
        Collapse();
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
            UpdateHeadElem();
            RecordCurElem();
            RefreshSpline();
            UpdateVertex();

            vertexPool->LateUpdate();
            vertexPool->SetMeshObjectActive(true);
            return;
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
        return ((FastVector3)PointStart->get_position() - (FastVector3)PointEnd->get_position()).Magnitude() * 0.5f;
    }
    
    Sombrero::FastVector3 AltTrail::get_CurHeadPos()
    {
        return ((FastVector3)PointStart->get_position() - (FastVector3)PointEnd->get_position()) / 2.0f;
    }

    void AltTrail::RefreshSpline()
    {
        auto& controlPoints = spline->get_ControlPoints();
        int diff = snapshotList.size() - controlPoints.size();
        
        
        // if control points smaller than snapshots, add new control points
        while (diff > 0)
        {
            spline->AddControlPoint(get_CurHeadPos(), (FastVector3)PointStart->get_position() - (FastVector3)PointEnd->get_position());
            diff--;
        }
        

        int index = 0;
        for (auto snap : snapshotList)
        {
            controlPoints[index]->Position = snap->get_pos();
            controlPoints[index]->Normal = snap->pointEnd - snap->pointStart;
            index++;
        }

        spline->RefreshSpline();
    }

    void AltTrail::UpdateVertex()
    {
        auto* pool = vertexSegment.Pool;

        for (int i = 0; i < Granularity; i++)
        {
            int baseIdx = vertexSegment.VertStart + i * 3;

            float uvSegment = (float) i / Granularity;

            FastColor color = MyColor;

            if (uvSegment < WhiteStep)
            {
                color = FastColor::LerpUnclamped(FastColor::white(), MyColor, uvSegment / WhiteStep);
            }

            FastVector3 pos = spline->InterpolateByLen(uvSegment);
            FastVector3 mul = FastVector3::Normalize(spline->InterpolateNormalByLen(uvSegment)) * get_TrailWidth();//(NormalizeVal(spline->InterpolateNormalByLen(uvSegment)), get_TrailWidth());

            // offset half a trail width from pos
            pool->Vertices->values[baseIdx] = pos + mul;;
            pool->UVs->values[baseIdx].x = 0.0f;

            // center
            pool->Vertices->values[baseIdx + 1] = pos;
            pool->UVs->values[baseIdx + 1].x = 0.5f;

            // offset half a trail width from pos
            pool->Vertices->values[baseIdx + 2] = pos - mul;
            pool->UVs->values[baseIdx + 2].x = 1.0f;

            pool->Colors->values[baseIdx] = pool->Colors->values[baseIdx + 1] = pool->Colors->values[baseIdx + 2] = color;
            pool->UVs->values[baseIdx].y = pool->UVs->values[baseIdx + 1].y = pool->UVs->values[baseIdx + 2].y = uvSegment;
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
        // update the first element
        snapshotList.front()->pointStart = PointStart->get_position();
        snapshotList.front()->pointEnd = PointEnd->get_position();
    }

    void AltTrail::RecordCurElem()
    {
        // get an element
        Element* elem = elemPool->Get();

        // set it to the current position
        elem->pointStart = PointStart->get_position();
        elem->pointEnd = PointEnd->get_position();

        // if list is small
        if (snapshotList.size() < TrailLength)
        {
            // add it after the first element
            snapshotList.insert((snapshotList.begin()++), elem);
        }
        // if list too big, just release the back element
        else if (snapshotList.size() > TrailLength)
        {
            elemPool->Release(elem);
            elemPool->Release(snapshotList.back());
            snapshotList.pop_back();
        }
        // if list exactly right, remove the last list element and put the new element after the first
        else // if (snapshotList.size() == TrailLength)
        {
            elemPool->Release(snapshotList.back());
            snapshotList.pop_back();
            snapshotList.insert((snapshotList.begin()++), elem);
        }
    }

    void AltTrail::SetColor(FastColor color)
    {
        color.a *= trailIntensity;
        MyColor = color;
    }

    void AltTrail::dtor()
    {

    }

    void AltTrail::Reset(bool addNewElemsToSnap)
    {
        if (!spline) spline = new Spline(TrailLength);
        bool poolExisted = elemPool;
        if (!poolExisted)
        {
            elemPool = new ElementPool(TrailLength);
        }

        spline->Granularity = Granularity;
        spline->Clear();

        // make sure the spline contains enough control points for the trail length
        for (int i = 0; i < TrailLength; i++)
        {
            spline->AddControlPoint(get_CurHeadPos(), (FastVector3)PointStart->get_position() - (FastVector3)PointEnd->get_position());
        }

        // if snapshot list contains elements, that is an issue, remove them!
        if (snapshotList.size() > 0)
        {
            for (auto snap : snapshotList)
            {
                elemPool->Release(snap);
            }
            snapshotList.clear();
            snapshotList.reserve(TrailLength);
        }

        // if hte pool already existed and we dont want to force new elements in, use this
        if (poolExisted && !addNewElemsToSnap)
        {
            auto elem = elemPool->Get();
            elem->pointStart = PointStart->get_position();
            elem->pointStart = PointEnd->get_position();
            snapshotList.push_back(elem);

            elem = elemPool->Get();
            elem->pointStart = PointStart->get_position();
            elem->pointStart = PointEnd->get_position();
            snapshotList.push_back(elem);
        }
        else
        {
            snapshotList.push_back(new Element(PointStart->get_position(), PointEnd->get_position()));
            snapshotList.push_back(new Element(PointStart->get_position(), PointEnd->get_position()));
        }
        // collapse the trail to be basically 0 length
        Collapse();
    }

    void AltTrail::Collapse()
    {
        FastVector3 start = PointStart->get_position();
        FastVector3 end = PointEnd->get_position();
        // makes all parts of the trail end up at the same place, making it basically 0 length
        for (auto snap : snapshotList)
        {
            snap->pointStart = start;
            snap->pointEnd = end;
        }

        RefreshSpline();
    }
}