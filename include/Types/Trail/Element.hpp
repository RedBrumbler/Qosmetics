#pragma once

#include "sombrero/shared/Vector3Utils.hpp"

struct Element {
    Sombrero::FastVector3 pointStart;
    Sombrero::FastVector3 pointEnd;
    Sombrero::FastVector3 get_pos() { return (pointStart + pointEnd) * 0.5f; }

    Element() {}
    Element(Sombrero::FastVector3 start, Sombrero::FastVector3 end) : pointStart(start), pointEnd(end) {};
};