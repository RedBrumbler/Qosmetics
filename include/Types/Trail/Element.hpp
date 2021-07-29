#pragma once

#include "UnityEngine/Vector3.hpp"

struct Element {
    UnityEngine::Vector3 pointStart;
    UnityEngine::Vector3 pointEnd;
    UnityEngine::Vector3 get_pos() { return (pointStart + pointEnd) / 2.0f; }

    Element() {}
    Element(UnityEngine::Vector3 start, UnityEngine::Vector3 end) : pointStart(start), pointEnd(end) {};
};