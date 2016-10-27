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
		
};

class Graph
{
	vector<Node> nodes;
	public:
		Graph(Image& image);
};

#endif