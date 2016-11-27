#include "spline.h"

_pixel* darker(_pixel* a, _pixel* b)
{
	if(a->getColor() < b->getColor()) return a;
	return b;
}

void Spline::extractActiveEdges()
{
	if(this->diagram == nullptr) return;
	map<Edge,_pixel*> edgeEnum;
	Image* imageRef = this->diagram->getImage(); 
	int width = imageRef->getWidth();
	int height = imageRef->getHeight();

	for(int x=0; x <width; x++)
	{
		for(int y=0; y< height; y++)
		{
			for(int i = 0 ; i < (*this->diagram)(x,y).size(); i++) 
			{
				int l = i;
				int r = (i+1)%((*this->diagram)(x,y).size());
				if(edgeEnum.find(make_pair((*this->diagram)(x,y)[r],(*this->diagram)(x,y)[l])) != edgeEnum.end()) 
				{
					auto p = edgeEnum[make_pair((*this->diagram)(x,y)[r],(*this->diagram)(x,y)[l])];
					if(p != (*imageRef)(x,y) && !p->isSimilar(*(*imageRef)(x,y))) activeEdges.push_back(make_pair(make_pair((*this->diagram)(x,y)[l],(*this->diagram)(x,y)[r]),darker(p,(*imageRef)(x,y)))); 
				}
				else edgeEnum[make_pair((*this->diagram)(x,y)[l],(*this->diagram)(x,y)[r])] = (*imageRef)(x,y);
			}
		}
	}
}

void Spline::calculateGraph()
{
	for(auto edge : activeEdges)
	{
		graph[edge.first.first].push_back(edge.first.second);
		graph[edge.first.second].push_back(edge.first.first);
	}
}

vector<vector<Point>> Spline::printGraph()
{
	map<Point, vector<Point>>::const_iterator it = graph.begin();
	vector<pair<float, float>> visited;
	vector<vector<Point>> mainOutLine;
	for(; it!=graph.end(); ++it)
	{
		pair<float, float> passMe = it->first;
		if(find(visited.begin(), visited.end(), passMe) == visited.end())
		{
			visited.push_back(passMe);
			vector<Point> points = traverseGraph(passMe);
			for(pair<float, float> list: points)
			{
				visited.push_back(list);
			}
			mainOutLine.push_back(points);
		}
	}	
	return mainOutLine;
}

vector<Point > Spline::traverseGraph(Point& p)
{
	//Contains nodes that have been visited
	set<Point > visitedNodes;
	vector<Point > points;
	stack<Point > q;
	q.push(p);
	while(!q.empty())
	{
		auto x = q.top();
		q.pop();
		visitedNodes.insert(x);
		points.push_back(x);
		for(auto pt : graph[x]) if(visitedNodes.find(pt) == visitedNodes.end()) q.push(pt);
	}
	return points;
}

//REF: http://math.stackexchange.com/questions/115241/manually-deducing-the-quadratic-uniform-b-spline-basis-functions
vector<vector<float> > Spline::getSpline(vector<Point > points) // For 3 points
{
	assert(points.size() == 3);
	//Basis Matrix for quadratic uniform b-spline
	float B[3][3] = {{1, 1, 0},{ -2, 2, 0},{1, -2, 1}};

	// 3x2 vector matrix initialized with 0
	vector<vector<float> > a(3,vector<float>(2,0));

	// multiply it with B-splines basis matrix
	for(int i = 0 ; i < 3; i++)
	{
		for(int j = 0; j < 2; j++)
		{
			for(int k = 0 ; k < 3; k ++) 
			{
				if(j == 0) a[i][j] += 0.5*B[i][k]*points[k].first;
				else a[i][j] += 0.5*B[i][k]*points[k].second;
			}
		}
	}
	return a;
}