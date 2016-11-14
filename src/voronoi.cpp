#include "voronoi.h"
int height, width;

void Voronoi::createDiagram(Graph& graph)
{
	int h = imageRef->getHeight();
	int w = imageRef->getWidth();

	height = h;
	width = w;

	//Semantic
	// edges[i][j][k] -> denotes whether there is a an edge from (i,j) in kth direction in the graph

	for(int i = 0; i < w; i++) 
	{
		vector<vector<pair<bool,int>>> v1;
		vector<vector<pair<float,float>>> v2(h);
		for(int j = 0; j < h; j ++) v1.push_back(vector<pair<bool,int>>(8,make_pair(false,0)));
		polygons.push_back(v1);
		voronoiPts.push_back(v2);
		hullPts.push_back(v2);
		resultPts.push_back(v2);
	}
	createRegions(graph);
	removeUseless();
}

int Voronoi::valence(int x,int y)
{
	cout<<"hehe";
	// if(x < 0 || x >= this->image->getWidth()) return -1;
	// if(y < 0 || y >= this->image->getHeight()) return -1;

	// int cnt = 0;
	// for(int i = 0; i < 8 ; i++) if(voronoiPts[x][y][i] == true) cnt ++;
	// return cnt;
}

void Voronoi::printVoronoi()
{

	// cout<<"\nImage height = "<<height<<"\tImage width = "<<width << endl;
	// for(int i=0; i <width; i++)
	// {
	// 	for(int j=0; j< height; j++)
	// 	{
	// 		cout<<"voronoi["<<i<<"]["<<j<<"] = "<<voronoiPts[i][j]<<"\n";
	// 	}
	// }

}

void Voronoi::collapseValence2()
{

	for(int i=0; i <width; i++)
	{
		for(int j=0; j< height; j++)
		{
			if(valence(i,j) != 2)
			{

			}
		}
	}
}


void Voronoi::createRegions(Graph& graph)
{
	int x, y, z;
	float xcenter, ycenter;
	for(x = 0; x< width; x++)
	{
		for(y = 0; y < height; y++)
		{
			xcenter = x+0.5;
			ycenter = y+0.5;
			// TOP
			if(graph.edge(x, y,TOP)) voronoiPts[x][y].push_back(make_pair(xcenter, ycenter-0.5));
			else voronoiPts[x][y].push_back(make_pair(xcenter, ycenter-0.25));

			// BOTTOM
			if(graph.edge(x, y,BOTTOM)) voronoiPts[x][y].push_back(make_pair(xcenter, ycenter+0.5));
			else voronoiPts[x][y].push_back(make_pair(xcenter, ycenter+0.25));

			// LEFT
			if(graph.edge(x, y,LEFT)) voronoiPts[x][y].push_back(make_pair(xcenter-0.5, ycenter));
			else voronoiPts[x][y].push_back(make_pair(xcenter-0.25, ycenter));

			// RIGHT
			if(graph.edge(x, y,RIGHT)) voronoiPts[x][y].push_back(make_pair(xcenter+0.5, ycenter));
			else voronoiPts[x][y].push_back(make_pair(xcenter+0.25, ycenter));

			// Diagonal edges

			// TOP LEFT
			if(graph.edge(x,y,TOP_LEFT))
			{
				voronoiPts[x][y].push_back(make_pair(xcenter-0.75, ycenter-0.25));
				voronoiPts[x][y].push_back(make_pair(xcenter-0.25, ycenter-0.75));
				// top edge and not of left edge or vice versa
				if((graph.edge(x, y,TOP) && !(graph.edge(x, y,LEFT))) || !(graph.edge(x, y,TOP) && (graph.edge(x, y,LEFT)))) voronoiPts[x][y].push_back(make_pair(xcenter-0.5, ycenter-0.5));
			}
			else
			{
				if(graph.edge(x-1,y,TOP_RIGHT))	voronoiPts[x][y].push_back(make_pair(xcenter-0.25, ycenter-0.25));
				else voronoiPts[x][y].push_back(make_pair(xcenter-0.5, ycenter-0.5));
			}
			// IMPORTANT: IF NO TOP LEFT IS THERE, ADD POINT X,Y TO THE VORONOIPTS(similarly for others)

			// DOWNLEFT
			if(graph.edge(x,y,BOTTOM_LEFT))
			{
				voronoiPts[x][y].push_back(make_pair(xcenter-0.75, ycenter+0.25));
				voronoiPts[x][y].push_back(make_pair(xcenter-0.25, ycenter+0.75));
				// Bottom edge and not of left edge or vice versa
				if((graph.edge(x, y, BOTTOM) && !(graph.edge(x, y,LEFT))) || !(graph.edge(x, y,TOP) && (graph.edge(x, y,LEFT)))) voronoiPts[x][y].push_back(make_pair(xcenter-0.5, ycenter+0.5));
			}
			else
			{
				// check if left node is connected to top node (left connected to its own top right node)
				if(graph.edge(x-1,y,BOTTOM_RIGHT)) voronoiPts[x][y].push_back(make_pair(xcenter-0.25, ycenter+0.25));
				else voronoiPts[x][y].push_back(make_pair(xcenter-0.5, ycenter+0.5));
			}

			// TOP RIGHT
			if(graph.edge(x,y,TOP_RIGHT))
			{
				voronoiPts[x][y].push_back(make_pair(xcenter+0.75, ycenter-0.25));
				voronoiPts[x][y].push_back(make_pair(xcenter+0.25, ycenter-0.75));
				// top edge and not of left edge or vice versa
				if((graph.edge(x, y,TOP) && !(graph.edge(x, y,RIGHT))) || !(graph.edge(x, y,TOP) && (graph.edge(x, y,RIGHT)))) voronoiPts[x][y].push_back(make_pair(xcenter+0.5, ycenter-0.5));
			}
			else
			{
				// check if top node is connected to right node (top connected to its own bottom right node)
				if(graph.edge(x,y-1,BOTTOM_RIGHT)) voronoiPts[x][y].push_back(make_pair(xcenter+0.25, ycenter-0.25));
				else voronoiPts[x][y].push_back(make_pair(xcenter+0.5, ycenter-0.5));
			}

			// DOWNRIGHT
			if(graph.edge(x,y,BOTTOM_RIGHT))
			{
				voronoiPts[x][y].push_back(make_pair(xcenter+0.75, ycenter+0.25));
				voronoiPts[x][y].push_back(make_pair(xcenter+0.25, ycenter+0.75));
				// Bottom edge and not of right edge or vice versa
				if((graph.edge(x, y, BOTTOM) && !(graph.edge(x, y,RIGHT))) || !(graph.edge(x, y,TOP) && (graph.edge(x, y,RIGHT)))) voronoiPts[x][y].push_back(make_pair(xcenter+0.5, ycenter+0.5));
			}
			else
			{
				// check if bottom node is connected to right node (bottom connected to its own top right node)
				if(graph.edge(x,y+1,TOP_RIGHT)) voronoiPts[x][y].push_back(make_pair(xcenter+0.25, ycenter+0.25));
				else voronoiPts[x][y].push_back(make_pair(xcenter+0.5, ycenter+0.5));
			}
		}
	}

	// collapseValence2();			// removing useless points in the voronoi
	convex_hull();					// get all the convex hull points
	// fixBoundaries()				//fix boundaries
}

void Voronoi::convex_hull()
{
	for(int x = 0; x< width; x++)
	{
		for(int y = 0; y < height; y++)
		{

			//cout<<"\nvoronoi["<<x<<"]["<<y<<"] = "<<voronoiPts[x][y]<<"\n";

			vector<Point> Pts;
			for(pair<float,float> p : voronoiPts[x][y]) Pts.push_back(Point(p.first,p.second));
			vector<Point> hullPts = ::convex_hull(Pts);
			for(Point p : hullPts) (this->hullPts[x][y]).push_back(make_pair(p.x,p.y));
			//copy(hull.begin(), hull.end(),(this->hullPts[x][y]).begin());
			//for (int i = 0; i < hull.size(); i++)
        	//	cout << "(" << hull[i].first << ", "<< hull[i].second << ")\n";
		}
	}

}

vector<pair<float,float>> Voronoi::operator()(int i,int j)
{
	return voronoiPts[i][j];
}

vector<pair<float,float>> Voronoi::getHull(int i,int j)
{
	return hullPts[i][j];
}

void Voronoi::removeUseless()
{
	
}

void Voronoi::fixBoundaries()
{
	//Fix boundaries and complete the polygons to make all closed.
}