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
	vector<pair<pair<pair<float,float>,pair<float,float> >,_pixel*> > activeEdges;
	map<pair<float,float>, vector<pair<float,float> > > graph;
	public:
		Spline(Voronoi* d) : diagram(d) {};
		Spline() : diagram(nullptr) {};
		vector<pair<pair<pair<float,float>,pair<float,float> >,_pixel*> >& getActiveEdges() {return activeEdges;}
		void extractActiveEdges();
		void calculateGraph();
		vector<pair<float,float> > traverseGraph(pair<float,float>& p);
		vector<vector<float> > getSpline(vector<pair<float,float> > points);
		void printGraph();
};

#endif
