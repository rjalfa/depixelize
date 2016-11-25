#pragma once

#ifndef _SPLINE_H
#define _SPLINE_H

#include "common.h"
#include "image.h"
#include "graph.h"
#include "voronoi.h"

class Spline
{
	Voronoi* diagram;
	public:
		Spline(Voronoi* d) : diagram(d) {};
		Spline() : diagram(nullptr) {};
};

#endif
