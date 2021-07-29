#pragma once

namespace Qosmetics { class VertexPool; }

struct VertexSegment {
    int VertStart;
    int IndexStart;
    int VertCount;
    int IndexCount;
    Qosmetics::VertexPool* Pool;

    VertexSegment(int start, int count, int istart, int icount, Qosmetics::VertexPool* pool) : VertStart(start), IndexStart(count), VertCount(istart), IndexCount(icount), Pool(pool) {}
};
