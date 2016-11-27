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
	vector<pair<Edge,_pixel*> > activeEdges;
	map<Point, vector<Point> > graph;
	public:
		Spline(Voronoi* d) : diagram(d) {};
		Spline() : diagram(nullptr) {};
		vector<pair<Edge,_pixel*> >& getActiveEdges() {return activeEdges;}
		void extractActiveEdges();
		void calculateGraph();
		vector<Point> traverseGraph(Point& p);
		vector<vector<float> > getSpline(vector<Point> points);
		vector<vector<Point> > printGraph();
};

#endif
