#pragma once

namespace Qosmetics { class VertexPool; }

struct VertexSegment {
    int VertStart = 0;
    int IndexStart = 0;
    int VertCount = 0;
    int IndexCount = 0;
    Qosmetics::VertexPool* Pool = nullptr;

    VertexSegment(int start, int count, int istart, int icount, Qosmetics::VertexPool* pool) : VertStart(start), IndexStart(count), VertCount(istart), IndexCount(icount), Pool(pool) {}
    VertexSegment() {};
};
