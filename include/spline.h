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
	map<Point, set<pair<Point,Color> > > graph;
	public:
		Spline(Voronoi* d) : diagram(d) {};
		Spline() : diagram(nullptr) {};
		vector<pair<Edge,_pixel*> >& getActiveEdges() {return activeEdges;}
		void extractActiveEdges();
		void calculateGraph();
		vector<Point > traverseGraph(const Point& p, const Color& c);
		vector<vector<float> > getSpline(vector<Point> points);
		vector<pair<vector<Point>,Color> > printGraph();
};

#endif
