#pragma once

#include "Types/Trail/VertexPool.hpp"
#include "Types/Trail/ElementPool.hpp"
#include "Types/Trail/Spline.hpp"
#include "Types/Trail/SplineControlPoint.hpp"

#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/Color.hpp"

#include "Types/Trail/TrailConfig.hpp"
#include "Types/Trail/TrailInitData.hpp"


DECLARE_CLASS_CODEGEN(Qosmetics, AltTrail, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD(Il2CppString*, SortingLayerName);
    DECLARE_INSTANCE_FIELD(UnityEngine::Transform*, PointStart);
    DECLARE_INSTANCE_FIELD(UnityEngine::Transform*, PointEnd);
    DECLARE_INSTANCE_FIELD(UnityEngine::Material*, MyMaterial);
    
    DECLARE_INSTANCE_FIELD(Qosmetics::VertexPool*, vertexPool);

    DECLARE_INSTANCE_METHOD(void, OnDestroy);
    DECLARE_INSTANCE_METHOD(void, LateUpdate);
    DECLARE_INSTANCE_METHOD(void, OnDisable);
    DECLARE_INSTANCE_METHOD(void, OnEnable);

    DECLARE_CTOR(ctor);
    DECLARE_DTOR(dtor);
    public:
        int SortingOrder;
        int TrailLength = 30;
        int Granularity = 60;
        float WhiteStep;
        UnityEngine::Color MyColor = {1.0f, 1.0f, 1.0f, 1.0f};
        static bool CapFps;
        UnityEngine::Vector3 get_CurHeadPos();
        void Setup(TrailInitData& initData, UnityEngine::Transform* pointStart, UnityEngine::Transform* pointEnd, UnityEngine::Material* material, bool editor);

    private:
        float get_TrailWidth();
        std::vector<Element*> snapshotList;
        ElementPool* elemPool;
        Spline* spline;
        VertexSegment vertexSegment;
        bool inited;
        int skipFirstFrames = 4;
        int frameNum;
        float time;

        void RefreshSpline();
        void UpdateVertex();
        void UpdateIndices();
        void UpdateHeadElem();
        void RecordCurElem();
)