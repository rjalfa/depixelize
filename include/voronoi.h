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
	vector<vector<vector<pair<float, float>>>> hullPts;
	vector<vector<vector<pair<float, float>>>> resultPts;
	map<pair<float,float>,int> valency;
	bool onBoundary(pair<float,float> p);
	public: 
		Voronoi(Image& inputImage) { imageRef = &inputImage;}
		void createDiagram(Graph& graph);
		void createRegions(Graph& graph);
		void removeUseless();
		void fixBoundaries();
		void printVoronoi();
		void collapseValence2();
		int collapseValence2nce(int x, int y);
		void convex_hull();
		vector<pair<float,float>> operator()(int i,int j);
		vector<pair<float,float>> getHull(int i,int j);
};

#endif