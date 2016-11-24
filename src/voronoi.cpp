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
	collapseValence2();
}

bool Voronoi::onBoundary(pair<float,float> p)
{
	return p.first == 0 || p.first == width || p.second == 0 || p.second == height;
}

void Voronoi::printVoronoi()
{

	cout<<"\nImage height = "<<height<<"\tImage width = "<<width << endl;
	for(int i=0; i <width; i++)
	{
		for(int j=0; j< height; j++)
		{
			cout<<"voronoi["<<i<<"]["<<j<<"] = "<<voronoiPts[i][j] << ",Pixel:";
			(*imageRef)(i,j)->print(cout);
			cout<<"\n";
		}
	}

}

void Voronoi::collapseValence2()
{

	for(int x=0; x <width; x++)
	{
		for(int y=0; y< height; y++)
		{
			for(int i = 0 ; i < voronoiPts[x][y].size(); i++) 
			{
				int l = i;
				int r = (i+1)%(voronoiPts[x][y].size());
				if(valency.find(voronoiPts[x][y][l]) != valency.end()) {valency[voronoiPts[x][y][l]] += 1;}
				else valency[voronoiPts[x][y][l]] = 1;
				if(valency.find(voronoiPts[x][y][r]) != valency.end()) {valency[voronoiPts[x][y][r]] += 1;}
				else valency[voronoiPts[x][y][r]] = 1;
			}
		}
	}
	for(int x = 0; x < width; x++)
	{
		for(int y=0; y< height; y++)
		{
			vector<pair<float,float> > temp;
			for(int i = 0 ; i < voronoiPts[x][y].size(); i++) 
			{
				if(valency[voronoiPts[x][y][i]] != 4 || onBoundary(voronoiPts[x][y][i])) temp.push_back(voronoiPts[x][y][i]);  
			}
			voronoiPts[x][y].clear();
			voronoiPts[x][y] = vector<pair<float,float> >(temp);
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

			xcenter = x + 0.5;
			ycenter = y + 0.5;
			//TOPLEFT
			if(graph.edge(x,y,TOP_LEFT))
			{
				voronoiPts[x][y].push_back(make_pair(xcenter-0.25,ycenter-0.75));
				voronoiPts[x][y].push_back(make_pair(xcenter-0.75,ycenter-0.25));
			}
			else if(graph.edge(x+direction[TOP][0],y+direction[TOP][1],BOTTOM_LEFT)) voronoiPts[x][y].push_back(make_pair(xcenter-0.25,ycenter-0.25)); 
			else voronoiPts[x][y].push_back(make_pair(xcenter-0.5,ycenter-0.5));
			
			//TOP
			voronoiPts[x][y].push_back(make_pair(xcenter-0.5,ycenter));

			//TOPRIGHT
			if(graph.edge(x,y,TOP_RIGHT))
			{
				voronoiPts[x][y].push_back(make_pair(xcenter-0.75,ycenter+0.25));
				voronoiPts[x][y].push_back(make_pair(xcenter-0.25,ycenter+0.75));
			}
			else if(graph.edge(x+direction[TOP][0],y+direction[TOP][1],BOTTOM_RIGHT)) voronoiPts[x][y].push_back(make_pair(xcenter-0.25,ycenter+0.25)); 
			else voronoiPts[x][y].push_back(make_pair(xcenter-0.5,ycenter+0.5));
			
			//RIGHT
			voronoiPts[x][y].push_back(make_pair(xcenter,ycenter+0.5));

			//BOTTOMRIGHT
			if(graph.edge(x,y,BOTTOM_RIGHT))
			{
				voronoiPts[x][y].push_back(make_pair(xcenter+0.25,ycenter+0.75));
				voronoiPts[x][y].push_back(make_pair(xcenter+0.75,ycenter+0.25));
			}
			else if(graph.edge(x+direction[BOTTOM][0],y+direction[BOTTOM][1],TOP_RIGHT)) voronoiPts[x][y].push_back(make_pair(xcenter+0.25,ycenter+0.25)); 
			else voronoiPts[x][y].push_back(make_pair(xcenter+0.5,ycenter+0.5));
			
			//BOTTOM
			voronoiPts[x][y].push_back(make_pair(xcenter+0.5,ycenter));

			//BOTTOMLEFT
			if(graph.edge(x,y,BOTTOM_LEFT))
			{
				voronoiPts[x][y].push_back(make_pair(xcenter+0.75,ycenter-0.25));
				voronoiPts[x][y].push_back(make_pair(xcenter+0.25,ycenter-0.75));
			}
			else if(graph.edge(x+direction[BOTTOM][0],y+direction[BOTTOM][1],TOP_LEFT)) voronoiPts[x][y].push_back(make_pair(xcenter+0.25,ycenter-0.25)); 
			else voronoiPts[x][y].push_back(make_pair(xcenter+0.5,ycenter-0.5));
			
			//LEFT
			voronoiPts[x][y].push_back(make_pair(xcenter,ycenter-0.5));
		}
	}
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
			//for(Point p : hullPts) (this->hullPts[x][y]).push_back(make_pair(p.x,p.y));
			copy(voronoiPts[x][y].begin(), voronoiPts[x][y].end(),(this->hullPts[x][y]).begin());
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