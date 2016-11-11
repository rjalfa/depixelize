#pragma once

#ifndef _VORONOI_H
#define _VORONOI_H
#include "graph.h"
#include <vector>
#include <utility>
using namespace std;

class Voronoi
{
	Image* imageRef;
	vector<vector<vector<pair<bool,int>>>> polygons;
	vector<vector<vector<pair<float, float>>>> voronoiPts;
	public: 
		Voronoi(Image& inputImage) { imageRef = &inputImage;}
		void createDiagram(Graph& graph);
		void createRegions(Graph& graph);
		void removeUseless();
		void fixBoundaries();
};

#endif