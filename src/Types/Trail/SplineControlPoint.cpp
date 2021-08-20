#include "Types/Trail/SplineControlPoint.hpp"
#include "Types/Trail/Spline.hpp"
#include "sombrero/shared/MiscUtils.hpp"

using namespace Sombrero;

SplineControlPoint* SplineControlPoint::NextControlPoint()
{
    return spline->NextControlPoint(this);
}

SplineControlPoint* SplineControlPoint::PreviousControlPoint()
{
    return spline->PreviousControlPoint(this);
}

FastVector3 SplineControlPoint::NextPosition()
{
    return spline->NextPosition(this);
}

FastVector3 SplineControlPoint::PreviousPosition()
{
    return spline->PreviousPosition(this);
}

FastVector3 SplineControlPoint::NextNormal()
{
    return spline->NextNormal(this);
}

FastVector3 SplineControlPoint::PreviousNormal()
{
    return spline->PreviousNormal(this);
}

bool SplineControlPoint::IsValid()
{
    return NextControlPoint();
}

FastVector3 SplineControlPoint::GetNext2Position()
{
    auto cp = NextControlPoint();
    if (cp)
        return cp->NextPosition();
    return NextPosition();
}

FastVector3 SplineControlPoint::GetNext2Normal()
{
    auto cp = NextControlPoint();
    if (cp)
        return cp->NextNormal();
    return NextNormal();
}

FastVector3 SplineControlPoint::Interpolate(float localF)
{
    return Spline::CatmulRom(PreviousPosition(), Position, NextPosition(), GetNext2Position(), Clamp01(localF));
}

FastVector3 SplineControlPoint::InterpolateNormal(float localF)
{
    return Spline::CatmulRom(PreviousNormal(), Normal, NextNormal(), GetNext2Normal(), Clamp01(localF));
}

void SplineControlPoint::Init(Spline* owner)
{
    spline = owner;
    SegmentIndex = -1;
}