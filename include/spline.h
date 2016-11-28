#pragma once

#ifndef _SPLINE_H
#define _SPLINE_H

#include "common.h"
#include "image.h"
#include "graph.h"
#include "voronoi.h"

//Class Spline: For handling path detection for drawing continuous curves and bspline generation

class Spline
{
	//Reference to Voronoi diagram
	Voronoi* diagram;

	//List of all edges that have sufficiently different colors at the 2 sides
	vector<pair<Edge,_pixel*> > activeEdges;

	//Contains a adjacency list representation for the above
	map<Point, set<pair<Point,Color> > > graph;
	public:
		//Parametric Constructor
		Spline(Voronoi* d) : diagram(d) {};

		//Default Constructor
		Spline() : diagram(nullptr) {};
		
		//Accessor
		vector<pair<Edge,_pixel*> >& getActiveEdges() {return activeEdges;}
		
		//Extract active edges from the diagram
		void extractActiveEdges();

		//Create Adjacency list from Active Edges
		void calculateGraph();

		//Traverse a continuous curve starting from p and following color similar to c
		vector<Point > traverseGraph(const Point& p, const Color& c);

		//Get quadratic uniform B-spline for 3 points
		vector<vector<float> > getSpline(vector<Point> points);
		
		//Get the traced paths for drawing splines
		vector<pair<vector<Point>,Color> > printGraph();
};

#endif
