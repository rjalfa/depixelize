#pragma once

#ifndef _GRAPH_H
#define _GRAPH_H

#include "image.h"

class Node
{
	_pixel* info;
	Node* adjacent[8];
	public:
		Node(_pixel* p)
		{
			this->info = p;
			for(int i = 0; i < 8; i++) adjacent[i] = nullptr;
		}
		_pixel* getPixel() {return this->info;}
		Node* getAdjacent(int i)
		{
			return this->adjacent[i];
		}
		void setAdjacent(int i, Node* n)
		{
			this->adjacent[i] = n;
		}
		int valence()
		{
			int cnt = 0;
			for(int i = 0 ; i < 8 ; i ++) if(this->adjacent != nullptr) cnt++;
			return cnt;
		}
		
};

class Graph
{
	Image* image;
	vector<vector<vector<bool>>> edges;
	vector<vector<vector<int>>> weights;
	public:
		Graph(Image& image);
		void planarize();
		Graph()
		{
			image = nullptr;
			edges.clear();
			weights.clear();
		}
};

#endif