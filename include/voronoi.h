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
	map<pair<float,float>,int> valency;
	bool onBoundary(pair<float,float> p);
	public: 
		Voronoi(Image& inputImage) { imageRef = &inputImage;}
		void createDiagram(Graph& graph);
		void createRegions(Graph& graph);
		void printVoronoi();
		void collapseValence2();
		vector<pair<float,float>> operator()(int i,int j);
		vector<pair<float,float>> getHull(int i,int j);
		Image* getImage() {return imageRef;};
};

#endif