#pragma once

#include <include/types.hpp>
#include <drivers/framebuffer.hpp>
#include <include/graphics.hpp>

static Point2 mousePos = { 100, 100 };

void mouseInit(Framebuffer in);