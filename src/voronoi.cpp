#include "voronoi.h"

void Voronoi::createDiagram(Graph& graph)
{
	int h = imageRef->getHeight();
	int w = imageRef->getWidth();

	//Semantic
	// edges[i][j][k] -> denotes whether there is a an edge from (i,j) in kth direction in the graph

	for(int i = 0; i < w; i++) 
	{
		vector<vector<pair<bool,int>>> v1;
		vector<vector<pair<int,int>>> v2;
		for(int j = 0; j < h; j ++)
		{
			v1.push_back(vector<pair<bool,int>>(8,make_pair(false,0)));
			// v2.push_back(vector<pair<int,int>>(8,make_pair(-1,-1)));
			v2.push_back(vector<pair<int,int>>());
		}
		polygons.push_back(v1);
		voronoiPts.push_back(v2);
	}

	createRegions(graph);
	removeUseless();
}

void Voronoi::createRegions(Graph& graph)
{
	int x, y, z;
	float xcenter, ycenter;
	for(x = 0; x< graph.getWidth; x++)
	{
		for(y = 0; y < graph.getHeight; y++)
		{
			xcenter = x+0.5;
			ycenter = y = 0.5;

			// TOP
			if(graph.edge(x, y,TOP))
			{
				voronoiPts[x][y].push_back(make_pair(xcenter, ycenter-0.5));
			}
			else
			{
				voronoiPts[x][y].push_back(make_pair(xcenter, ycenter-0.25));
			}

			// BOTTOM
			if(graph.edge(x, y,BOTTOM))
			{
				voronoiPts[x][y].push_back(make_pair(xcenter, ycenter+0.5));
			}
			else
			{
				voronoiPts[x][y].push_back(make_pair(xcenter, ycenter+0.25));
			}

			// LEFT
			if(graph.edge(x, y,LEFT))
			{
				voronoiPts[x][y].push_back(make_pair(xcenter-0.5, ycenter));
			}
			else
			{
				voronoiPts[x][y].push_back(make_pair(xcenter-0.25, ycenter));
			}

			// RIGHT
			if(graph.edge(x, y,RIGHT))
			{
				voronoiPts[x][y].push_back(make_pair(xcenter+0.5, ycenter));
			}
			else
			{
				voronoiPts[x][y].push_back(make_pair(xcenter+0.25, ycenter));
			}

			// Diagonal edges

			// TOP LEFT
			if(graph.edge(x,y,TOP_LEFT))
			{
				voronoiPts[x][y].push_back(make_pair(xcenter-0.75, ycenter-0.25));
				voronoiPts[x][y].push_back(make_pair(xcenter-0.25, ycenter-0.75));
				// top edge and not of left edge or vice versa
				if((graph.edge(x, y,TOP) && !(graph.edge(x, y,LEFT))) || !(graph.edge(x, y,TOP) && (graph.edge(x, y,LEFT))))
				{
					voronoiPts[x][y].push_back(make_pair(xcenter-0.5, ycenter-0.5));
				}
					
			}
			else
			{
				// check if left node is connected to top node (left connected to its own top right node)
				if(graph.edge(x-1,y,TOP_RIGHT))
				{
					voronoiPts[x][y].push_back(make_pair(xcenter-0.25, ycenter-0.25));
				}
				else
				{
					voronoiPts[x][y].push_back(make_pair(xcenter-0.5, ycenter-0.5));
				}
			}

			// IMPORTANT: IF NO TOP LEFT IS THERE, ADD POINT X,Y TO THE VORONOIPTS(similarly for others)

			// DOWNLEFT
			if(graph.edge(x,y,BOTTOM_LEFT))
			{
				voronoiPts[x][y].push_back(make_pair(xcenter-0.75, ycenter+0.25));
				voronoiPts[x][y].push_back(make_pair(xcenter-0.25, ycenter+0.75));
				// Bottom edge and not of left edge or vice versa
				if((graph.edge(x, y, BOTTOM) && !(graph.edge(x, y,LEFT))) || !(graph.edge(x, y,TOP) && (graph.edge(x, y,LEFT))))
				{
					voronoiPts[x][y].push_back(make_pair(xcenter-0.5, ycenter+0.5));
				}
					
			}
			else
			{
				// check if left node is connected to top node (left connected to its own top right node)
				if(graph.edge(x-1,y,BOTTOM_RIGHT))
				{
					voronoiPts[x][y].push_back(make_pair(xcenter-0.25, ycenter+0.25));
				}
				else
				{
					voronoiPts[x][y].push_back(make_pair(xcenter-0.5, ycenter+0.5));
				}
			}

			// TOP RIGHT
			if(graph.edge(x,y,TOP_RIGHT))
			{
				voronoiPts[x][y].push_back(make_pair(xcenter+0.75, ycenter-0.25));
				voronoiPts[x][y].push_back(make_pair(xcenter+0.25, ycenter-0.75));
				// top edge and not of left edge or vice versa
				if((graph.edge(x, y,TOP) && !(graph.edge(x, y,RIGHT))) || !(graph.edge(x, y,TOP) && (graph.edge(x, y,RIGHT))))
				{
					voronoiPts[x][y].push_back(make_pair(xcenter+0.5, ycenter-0.5));
				}
					
			}
			else
			{
				// check if top node is connected to right node (top connected to its own bottom right node)
				if(graph.edge(x,y-1,BOTTOM_RIGHT))
				{
					voronoiPts[x][y].push_back(make_pair(xcenter+0.25, ycenter-0.25));
				}
				else
				{
					voronoiPts[x][y].push_back(make_pair(xcenter+0.5, ycenter-0.5));
				}
			}

			// DOWNRIGHT
			if(graph.edge(x,y,BOTTOM_RIGHT))
			{
				voronoiPts[x][y].push_back(make_pair(xcenter+0.75, ycenter+0.25));
				voronoiPts[x][y].push_back(make_pair(xcenter+0.25, ycenter+0.75));
				// Bottom edge and not of right edge or vice versa
				if((graph.edge(x, y, BOTTOM) && !(graph.edge(x, y,RIGHT))) || !(graph.edge(x, y,TOP) && (graph.edge(x, y,RIGHT))))
				{
					voronoiPts[x][y].push_back(make_pair(xcenter+0.5, ycenter+0.5));
				}
					
			}
			else
			{
				// check if bottom node is connected to right node (bottom connected to its own top right node)
				if(graph.edge(x,y+1,TOP_RIGHT))
				{
					voronoiPts[x][y].push_back(make_pair(xcenter+0.25, ycenter+0.25));
				}
				else
				{
					voronoiPts[x][y].push_back(make_pair(xcenter+0.5, ycenter+0.5));
				}
			}
		}
	}
}

void Voronoi::removeUseless()
{
	
}

void Voronoi::fixBoundaries()
{
	//Fix boundaries and complete the polygons to make all closed.
}