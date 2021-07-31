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
    DECLARE_INSTANCE_METHOD(void, Reset, bool addNewElemsToSnap);

    DECLARE_CTOR(ctor);
    DECLARE_DTOR(dtor);
    public:
        int SortingOrder;
        int TrailLength = 30;
        int Granularity = 60;
        float WhiteStep = 0.2f;
        UnityEngine::Color MyColor = {1.0f, 1.0f, 1.0f, 1.0f};
        static bool CapFps;
        UnityEngine::Vector3 get_CurHeadPos();
        void Setup(TrailInitData& initData, UnityEngine::Transform* pointStart, UnityEngine::Transform* pointEnd, UnityEngine::Material* material, bool editor);
        void SetColor(UnityEngine::Color color);
        void Collapse();
        static float trailIntensity;
        bool get_inited();
        
    private:
        float get_TrailWidth();
        std::vector<Element*> snapshotList;
        ElementPool* elemPool = nullptr;
        Spline* spline = nullptr;
        VertexSegment vertexSegment;
        bool inited = false;
        int skipFirstFrames = 4;
        int frameNum = 0;
        float time = 0.0f;
        
        void UpdateTrailData();
        void RefreshSpline();
        void UpdateVertex();
        void UpdateIndices();
        void UpdateHeadElem();
        void RecordCurElem();
)