#pragma once

#ifndef _VORONOI_H
#define _VORONOI_H
#include "common.h"
#include "graph.h"

//Class Voronoi: Handling Voronoi diagram creation (Reshaping of cells)

class Voronoi
{
	//Reference to image
	Image* imageRef;

	//Contains voronoi points around every pixel (x,y), in clockwise order starting from top-left
	vector<vector<vector<pair<float, float>>>> voronoiPts;

	//Valency of each voronoi point for collapsing
	map<pair<float,float>,int> valency;

	//Function to check if the voronoi point is on the boundary of the image.
	bool onBoundary(pair<float,float> p);
	public: 
		//Parametric Constructor
		Voronoi(Image& inputImage) { imageRef = &inputImage;}
		
		//Creates Voronoi Diagram
		void createDiagram(Graph& graph);
		
		//Create Regions, subfunction to above
		void createRegions(Graph& graph);

		//Debugging function
		void printVoronoi();

		//Collapsing valence 2 nodes for smoother voronoi
		void collapseValence2();

		//Accessors
		vector<pair<float,float>> operator()(int i,int j);
		vector<pair<float,float>> getHull(int i,int j);
		Image* getImage() {return imageRef;};
};

#endif