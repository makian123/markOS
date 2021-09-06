#pragma once

#include <include/types.hpp>
#include <drivers/framebuffer.hpp>

struct Vertex2D{
    long x, y;
};

struct Vertex3D{
    long x, y, z;
};

struct Triangle{
    Vertex3D p1, p2, p3;
};

void gameInit(Framebuffer fb);