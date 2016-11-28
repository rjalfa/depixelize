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
		graph[edge.first.first].insert(make_pair(edge.first.second,edge.second->getColor()));
		graph[edge.first.second].insert(make_pair(edge.first.first,edge.second->getColor()));
	}
}

vector<pair<vector<Point>,Color> > Spline::printGraph()
{
	/*
	map<Point, vector<Point>>::const_iterator it = graph.begin();
	vector<pair<float, float>> visited;
	vector<pair<vector<Point>,Color> > mainOutLine;
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
			if( find(
					graph[points[points.size()-1]].begin(),
					graph[points[points.size()-1]].end(),
					points[0]) != graph[points[points.size()-1]].end()) {
				points.push_back(points[0]);
				if(points.size() > 1) points.push_back(points[1]);
			}
			mainOutLine.push_back(make_pair(points,Color(0.0f,0.0f,0.0f)));
		}
	}	
	return mainOutLine;
	*/
	//Adding 
	vector<pair<vector<Point>,Color> > mainOutLine;
	map<Point, set<pair<Point,Color> > >::iterator vertexPt = graph.begin();
	while(vertexPt != graph.end())
	{
		while((vertexPt->second).size())
		{
			Point src = (vertexPt->second).begin()->first;
			Color c = (vertexPt->second).begin()->second;
			vector<Point> v = traverseGraph(src, c);
			mainOutLine.push_back(make_pair(v,c));
			//cout << v <<" " << c << endl;
		}
		vertexPt ++;
	}
	return mainOutLine;
}

vector<Point > Spline::traverseGraph(const Point& p, const Color& c)
{
	//Contains nodes that have been visited
	vector<Point> points;
	Point x = p;
	bool found = true;
	while(true)
	{
		points.push_back(x);
		for(set<pair<Point,Color> >::iterator it = graph[x].begin(); it != graph[x].end(); it++) 
		{
			//If color of a node is similar to that of one vertex in the adj list, then connect tha node.
			if(isSimilar(it->second,c)) {
				Point p2 = it->first;
				//cerr << x << "->" << graph[x] << " " << it->first<< "->" << " " << graph[it->first] << endl;
				set<pair<Point,Color> >::iterator it1;
				for(it1 = graph[p2].begin(); it1 != graph[p2].end(); it1++) {
					if(isSimilar(c,it1->second) && it1->first == x) break;
				}
				assert(it1 != graph[p2].end());
				graph[x].erase(it);
				graph[p2].erase(it1);
				//cerr << x << "->" << graph[x] << " " << p2<< "->" << " " << graph[p2] << endl;
				//cerr << "======\n";
				x = p2;
				found = true;
				break;
			}
		}
		if(!found) break;
		found = false;
	}
	if(points.size() > 2 && *points.begin() == *points.rbegin()) 
	{
		points.push_back(points[1]);
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