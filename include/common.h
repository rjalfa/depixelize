#pragma once

#ifndef _COMMON_H
#define _COMMON_H

#include <cstdio>
#include <tuple>
#include <utility>
#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <stack>
#include <map>
#include <cassert>
#include <GL/glew.h>
#include <GL/glui.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
using namespace std;

//DIRECTION MACROS
#define TOP 1
#define TOP_RIGHT 2
#define TOP_LEFT 0
#define LEFT 3
#define RIGHT 4
#define BOTTOM 6
#define BOTTOM_RIGHT 7
#define BOTTOM_LEFT 5

const int direction[8][2] = 
{
	{-1,-1},
	{-1,0},
	{-1,1},
	{0,-1},
	{0,1},
	{1,-1},
	{1,0},
	{1,1}
};

template<class T,class V>
ostream& operator<<(ostream& out, pair<T,V>& p)
{
	out << "{" << p.first << "," << p.second << "}";
	return out;
}

template<class T>
ostream& operator<<(ostream& out, vector<T> v)
{
	out << "[";
	for(int i = 0 ; i < v.size() ; i++)
	{
		out << v[i];
		if(i < v.size()-1) out << ", ";
	}
	out << "]";
	return out;
}


typedef double coord_t;         // coordinate type
typedef double coord2_t;  // must be big enough to hold 2*max(|coordinate|)^2

struct Point {
	coord_t x, y;
public:
	Point(coord_t _x,coord_t _y) {x=_x;y=_y;}
	Point():x(0),y(0) {}
	bool operator <(const Point &p) const {
		return x < p.x || (x == p.x && y < p.y);
	}
};
vector<Point> convex_hull(vector<Point> P);

#endif