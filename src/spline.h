#pragma once

#ifndef _SPLINE_H
#define _SPLINE_H

#include "voronoi.h"

#include <vector>
#include <utility>
#include <set>
#include <map>

//Class Spline: For handling path detection for drawing continuous curves and bspline generation

class Spline
{
	//Reference to Voronoi diagram
	Voronoi* diagram;

	//List of all edges that have sufficiently different colors at the 2 sides
	std::vector<std::pair<Edge,Pixel*> > activeEdges;

	//Contains a adjacency list representation for the above
	std::map<Point, std::set<std::pair<Point,Color> > > graph;
	public:
		//Parametric Constructor
		Spline(Voronoi* d) : diagram(d) {};

		//Default Constructor
		Spline() : diagram(nullptr) {};
		
		//Accessor
		std::vector<std::pair<Edge,Pixel*> >& getActiveEdges() {return activeEdges;}
		
		//Extract active edges from the diagram
		void extractActiveEdges();

		//Create Adjacency list from Active Edges
		void calculateGraph();

		//Traverse a continuous curve starting from p and following color similar to c
		std::vector<Point> traverseGraph(const Point& p, const Color& c);

		//Get quadratic uniform B-spline for 3 points
		std::vector<std::vector<float> > getSpline(std::vector<Point> points);
		
		//Get the traced paths for drawing splines
		std::vector<std::pair<std::vector<Point>,Color> > printGraph();
};

#endif
