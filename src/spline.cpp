#include "spline.h"

_pixel* darker(_pixel* a, _pixel* b)
{
	if(a->getColor() > b->getColor()) return a;
	return b;
}

void Spline::extractActiveEdges()
{
	if(this->diagram == nullptr) return;
	map<pair<pair<float,float>,pair<float,float> >,_pixel*> edgeEnum;
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

void Spline::printGraph()
{
	map<pair<float,float>, vector<pair<float,float>>>::const_iterator it = graph.begin();
	vector<pair<float, float>> visited;
	vector<vector<pair<float,float>>> mainOutLine;
	for(it; it!=graph.end(); ++it)
	{
		pair<float, float> passMe = it->first;
		// cout<<"\n("<<it->first.first<<", "<<it->first.second<<") = ";
		if(find(visited.begin(), visited.end(), passMe)!= visited.end())
		{
			// cout<<"present";
		}
		else
		{
			cout<<"\n\nnot";
			visited.push_back(passMe);
			vector<pair<float,float>> points = traverseGraph(passMe);
			for(pair<float, float> list: points)
			{
				cout<<"("<< list.first<<", "<< list.second<<"), ";
				visited.push_back(list);
			}
			mainOutLine.push_back(points);
		}


		
		
	}
}

vector<pair<float,float> > Spline::traverseGraph(pair<float,float>& p)
{
	//Contains nodes that have been visited
	set<pair<float,float> > visitedNodes;
	vector<pair<float,float> > points;
	queue<pair<float,float> > q;
	q.push(p);
	while(!q.empty())
	{
		auto x = q.front();
		q.pop();
		visitedNodes.insert(x);
		points.push_back(x);
		for(auto pt : graph[x]) if(visitedNodes.find(pt) == visitedNodes.end()) q.push(pt);
	}
	return points;
}

vector<vector<float> > Spline::getSpline(vector<pair<float,float> > points) // For 3 points
{
	assert(points.size() == 3);
	//Basis Matrix for quadratic uniform b-spline
	float B[3][3] = {{1, 1, 0},{ -2, 2, 0},{1, -2, 1}};

	vector<vector<float> > a(3,vector<float>(2,0));
	for(int i = 0 ; i < 3; i++)
	{
		for(int j = 0; j < 2; j++)
		{
			for(int k = 0 ; k < 3; k ++) 
			{
				if(j == 0) a[i][j] += B[i][k]*points[k].first;
				else a[i][j] += B[i][k]*points[k].second;
			}
		}
	}
	return a;
}