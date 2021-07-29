#include "Types/Trail/SplineControlPoint.hpp"
#include "Types/Trail/Spline.hpp"

using Vector3 = UnityEngine::Vector3;

SplineControlPoint* SplineControlPoint::NextControlPoint()
{
    return spline->NextControlPoint(this);
}

SplineControlPoint* SplineControlPoint::PreviousControlPoint()
{
    return spline->PreviousControlPoint(this);
}

Vector3 SplineControlPoint::NextPosition()
{
    return spline->NextPosition(this);
}

Vector3 SplineControlPoint::PreviousPosition()
{
    return spline->PreviousPosition(this);
}

Vector3 SplineControlPoint::NextNormal()
{
    return spline->NextNormal(this);
}

Vector3 SplineControlPoint::PreviousNormal()
{
    return spline->PreviousNormal(this);
}

bool SplineControlPoint::IsValid()
{
    return NextControlPoint();
}

Vector3 SplineControlPoint::GetNext2Position()
{
    auto cp = NextControlPoint();
    if (cp)
        return cp->NextPosition();
    return NextPosition();
}

Vector3 SplineControlPoint::GetNext2Normal()
{
    auto cp = NextControlPoint();
    if (cp)
        return cp->NextNormal();
    return NextNormal();
}

Vector3 SplineControlPoint::Interpolate(float localF)
{
    localF = localF > 1.0f ? 1.0f : localF;
    localF = localF < 0.0f ? 0.0f : localF;
    return Spline::CatmulRom(PreviousPosition(), Position, NextPosition(), GetNext2Position(), localF);
}

Vector3 SplineControlPoint::InterpolateNormal(float localF)
{
    localF = localF > 1.0f ? 1.0f : localF;
    localF = localF < 0.0f ? 0.0f : localF;
    return Spline::CatmulRom(PreviousNormal(), Normal, NextNormal(), GetNext2Normal(), localF);
}

void SplineControlPoint::Init(Spline* owner)
{
    spline = owner;
    SegmentIndex = -1;
}