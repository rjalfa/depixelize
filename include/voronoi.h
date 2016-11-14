#pragma once

#ifndef _VORONOI_H
#define _VORONOI_H
#include "common.h"
#include "graph.h"

class Voronoi
{
	Image* imageRef;
	vector<vector<vector<pair<bool,int>>>> polygons;
	vector<vector<vector<pair<float, float>>>> voronoiPts;
	vector<vector<vector<pair<float, float>>>> resultPts;
	public: 
		Voronoi(Image& inputImage) { imageRef = &inputImage;}
		void createDiagram(Graph& graph);
		void createRegions(Graph& graph);
		void removeUseless();
		void fixBoundaries();
		void printVoronoi();
		int valence(int x, int y);
		void collapseValence2();
		int collapseValence2nce(int x, int y);
		void convex_hull();
};

#endif