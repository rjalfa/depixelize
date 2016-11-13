#pragma once

#ifndef _COMMON_H
#define _COMMON_H

#include <cstdio>
#include <tuple>
#include <utility>
#include <fstream>
#include <vector>
#include <iostream>
#include <stack>
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

extern int directions[8][2];


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
		if(i < v.size() - 1 ) out << ", ";
	}
	out << "]";
	return out;
}

#endif