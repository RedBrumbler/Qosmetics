#pragma once

#include <vector>
#include "UnityEngine/Vector3.hpp"
 
class SplineControlPoint;

class Spline {
    public:
        std::vector<SplineControlPoint*> controlPoints;
        std::vector<SplineControlPoint*> segments;
        int Granularity = 20;
    
        SplineControlPoint* operator[](int index);
        std::vector<SplineControlPoint*>& get_ControlPoints();
        SplineControlPoint* NextControlPoint(SplineControlPoint* controlPoint);
        SplineControlPoint* PreviousControlPoint(SplineControlPoint* controlPoint);
    
        UnityEngine::Vector3 NextPosition(SplineControlPoint* controlPoint);
        UnityEngine::Vector3 PreviousPosition(SplineControlPoint* controlPoint);
    
        UnityEngine::Vector3 NextNormal(SplineControlPoint* controlPoint);
        UnityEngine::Vector3 PreviousNormal(SplineControlPoint* controlPoint);
    
        SplineControlPoint* LenToSegment(float t, float& localF);
    
        static UnityEngine::Vector3 CatmulRom(UnityEngine::Vector3 T0, UnityEngine::Vector3 P0, UnityEngine::Vector3 P1, UnityEngine::Vector3 T1, float f);
        UnityEngine::Vector3 InterpolateByLen(float tl);
        UnityEngine::Vector3 InterpolateNormalByLen(float tl);
        SplineControlPoint* AddControlPoint(UnityEngine::Vector3 pos, UnityEngine::Vector3 up);
        void Clear();
        void RefreshSpline();
    
        ~Spline();
    private: 
        void RefreshDistance();
};