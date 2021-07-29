#pragma once

#include "UnityEngine/Vector3.hpp"

struct Spline;

struct SplineControlPoint {
        int ControlPointIndex = -1;
        float Dist;

        UnityEngine::Vector3 Normal;
        UnityEngine::Vector3 Position;
        int SegmentIndex = -1;
        Spline* spline;

        SplineControlPoint* NextControlPoint();
        SplineControlPoint* PreviousControlPoint();

        UnityEngine::Vector3 NextPosition();
        UnityEngine::Vector3 PreviousPosition();
        UnityEngine::Vector3 NextNormal();
        UnityEngine::Vector3 PreviousNormal();

        bool IsValid();
        UnityEngine::Vector3 GetNext2Position();
        UnityEngine::Vector3 GetNext2Normal();
        UnityEngine::Vector3 Interpolate(float localF);
        UnityEngine::Vector3 InterpolateNormal(float localF);
        void Init(Spline* owner);

};