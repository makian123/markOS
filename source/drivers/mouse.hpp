#pragma once

#include <include/types.hpp>
#include <drivers/framebuffer.hpp>
#include <game/game.hpp>

static Vertex2D mousePos = { 0, 0 };

void mouseInit(Framebuffer in);