#pragma once

#include "sombrero/shared/Vector3Utils.hpp"

class Spline;

class SplineControlPoint {
    public:
        int ControlPointIndex = -1;
        float Dist;

        Sombrero::FastVector3 Normal;
        Sombrero::FastVector3 Position;
        int SegmentIndex = -1;
        Spline* spline;

        SplineControlPoint* NextControlPoint();
        SplineControlPoint* PreviousControlPoint();

        Sombrero::FastVector3 NextPosition();
        Sombrero::FastVector3 PreviousPosition();
        Sombrero::FastVector3 NextNormal();
        Sombrero::FastVector3 PreviousNormal();

        bool IsValid();
        Sombrero::FastVector3 GetNext2Position();
        Sombrero::FastVector3 GetNext2Normal();
        Sombrero::FastVector3 Interpolate(float localF);
        Sombrero::FastVector3 InterpolateNormal(float localF);
        void Init(Spline* owner);
};