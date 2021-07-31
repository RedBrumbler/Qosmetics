#include "Types/Trail/AltTrail.hpp"

#include "UnityEngine/Time.hpp"
#include "QosmeticsLogger.hpp"

#define INFO(value...) QosmeticsLogger::GetContextLogger("AltTrail").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("AltTrail").error(value)

#define LOGFLOAT(float) INFO("%s, %F", #float, float)
#define LOGPTR(pointer) INFO("%s, %p", #pointer, pointer)
#define LOGCOLOR(color) INFO("%s, %.2f, %.2f, %.2f, %.2f", #color, color.r, color.g, color.b, color.a)
DEFINE_TYPE(Qosmetics, AltTrail);

using namespace UnityEngine;

static constexpr const float frameRateTreshold = 1.0f / 90.0f;

static inline Vector3 Vector3Sub(Vector3& first, Vector3& second) {
    return Vector3(first.x - second.x, first.y - second.y, first.z - second.z);
}

static inline Vector3 Vector3SubVal(Vector3 first, Vector3 second) {
    return Vector3Sub(first, second);
}

static inline Vector3 Vector3Div(Vector3& vec, float val) {
    return Vector3(vec.x / val, vec.y / val, vec.z / val);
}

template<typename T>
requires(std::is_arithmetic_v<T>)
static inline T sqr(T a)
{
    return a * a;
} 

static inline float Magnitude(Vector3& vec) {
    return sqrtf(sqr(vec.x) + sqr(vec.y) + sqr(vec.z));
}

static inline float MagnitudeVal(Vector3 vec) {
    return Magnitude(vec);
}

static inline Vector3 Normalize(Vector3& vec) {
    float magnitude = Magnitude(vec);
    if (magnitude == 0.0f) return {0.0f, 0.0f, 0.0f};
    return Vector3Div(vec, magnitude);
}

static inline Vector3 NormalizeVal(Vector3 vec) {
    return Normalize(vec);
}


static inline Color ColorAdd(Color& first, Color& second) {
    return Color(first.r + second.r, first.g + second.g, first.b + second.b, first.a + second.a);
}

static inline Color ColorAddVal(Color first, Color second) {
    return ColorAdd(first, second);
}

static inline Color ColorMul(Color& col, float val) {
    return Color(col.r * val, col.g * val, col.b * val, col.a * val);
}

static inline Color LerpUnclamped(Color& first, Color& second, float t) {
    return ColorAddVal(ColorMul(first, (1.0f - t)), ColorMul(second, t));
}

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
        
        if (!spline) spline = new Spline();
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
        return MagnitudeVal(Vector3SubVal(PointStart->get_position(), PointEnd->get_position()));
    }
    
    UnityEngine::Vector3 AltTrail::get_CurHeadPos()
    {
        return Vector3SubVal(PointStart->get_position(), PointEnd->get_position()) / 2.0f;
    }

    void AltTrail::RefreshSpline()
    {
        auto& controlPoints = spline->get_ControlPoints();
        int diff = snapshotList.size() - controlPoints.size();

        // if control points smaller than snapshots, add new control points
        while (diff > 0)
        {
            spline->AddControlPoint(get_CurHeadPos(), Vector3SubVal(PointStart->get_position(), PointEnd->get_position()));
            diff--;
        }

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
            int baseIdx = vertexSegment.VertStart + i * 3;

            float uvSegment = (float) i / Granularity;

            auto pos = spline->InterpolateByLen(uvSegment);

            auto up = NormalizeVal(spline->InterpolateNormalByLen(uvSegment));
            auto pos0 = pos + up * get_TrailWidth() * 0.5f;
            auto pos1 = pos - up * get_TrailWidth() * 0.5f;

            Color color = MyColor;

            if (uvSegment < WhiteStep)
            {
                static Color white = {1.0f, 1.0f, 1.0f, 1.0f};
                color = LerpUnclamped(white, MyColor, uvSegment / WhiteStep);
            }
            
            pool->Vertices->values[baseIdx] = pos0;
            pool->UVs->values[baseIdx].x = 0.0f;

            pool->Vertices->values[baseIdx + 1] = pos;
            pool->UVs->values[baseIdx + 1].x = 0.5f;

            pool->Vertices->values[baseIdx + 2] = pos1;
            pool->UVs->values[baseIdx + 2].x = 1.0f;

            pool->Colors->values[baseIdx] = pool->Colors->values[baseIdx + 1] = pool->Colors->values[baseIdx + 2] = color;
            pool->UVs->values[baseIdx].y = pool->UVs->values[baseIdx + 1].y = pool->UVs->values[baseIdx + 2].y = uvSegment;

            /*
            // pos0
            pool->Vertices->values[baseIdx] = pos0;
            pool->Colors->values[baseIdx] = color;
            //uvCoord.x = 0.0f;
            //uvCoord.y = uvSegment;
            //pool->UVs->values[baseIdx] = uvCoord;
            pool->UVs->values[baseIdx].x = 0.0f;
            pool->UVs->values[baseIdx].y = uvSegment;`

            //pos
            pool->Vertices->values[baseIdx + 1] = pos;
            pool->Colors->values[baseIdx + 1] = color;
            //uvCoord.x = 0.5f;
            //uvCoord.y = uvSegment;
            //pool->UVs->values[baseIdx + 1] = uvCoord;
            pool->UVs->values[baseIdx].x = 0.0f;
            pool->UVs->values[baseIdx].y = uvSegment;

            //pos1
            pool->Vertices->values[baseIdx + 2] = pos1;
            pool->Colors->values[baseIdx + 2] = color;
            //uvCoord.x = 1.0f;
            //uvCoord.y = uvSegment;
            //pool->UVs->values[baseIdx + 2] = uvCoord;
            pool->UVs->values[baseIdx + 2].x = 0.0f;
            pool->UVs->values[baseIdx + 2].y = uvSegment;
            */
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
            snapshotList.insert((snapshotList.begin()++), elem);
        }
        else if (snapshotList.size() > TrailLength)
        {
            elemPool->Release(snapshotList.back());
            snapshotList.pop_back();
        }
        else
        {
            elemPool->Release(snapshotList.back());
            snapshotList.pop_back();
            snapshotList.insert((snapshotList.begin()++), elem);
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
            snapshotList.reserve(TrailLength);
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
        
        Collapse();
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