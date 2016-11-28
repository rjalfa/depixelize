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
#include <set>
#include <queue>
#include <cassert>
#include <GL/glew.h>
#include <GL/glui.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
using namespace std;

using Point = pair<float,float>;
using Edge = pair<Point,Point>;
using Color = tuple<unsigned int,unsigned int,unsigned int>;

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
ostream& operator<<(ostream& out,const pair<T,V>& p)
{
	out << "{" << p.first << "," << p.second << "}";
	return out;
}

template<class T>
ostream& operator<<(ostream& out,const vector<T>& v)
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

template<class K,class V>
ostream& operator<<(ostream& out, const map<K,V>& v)
{
	out << "Map{";
	for(auto it = v.begin(); it != v.end(); it++)
	{
		out << it->first << "="<<it->second;
		out << "\n";
	}
	out << "}";
	return out;
}

template<class K>
ostream& operator<<(ostream& out, const set<K>& s)
{
	out << "{";
	for(auto it = s.begin(); it != s.end(); it ++)
	{
		out << *it;
		if(next(it) != s.end()) out << ", ";
	}
	out << "}";
	return out;
}

namespace aux{
template<std::size_t...> struct seq{};

template<std::size_t N, std::size_t... Is>
struct gen_seq : gen_seq<N-1, N-1, Is...>{};

template<std::size_t... Is>
struct gen_seq<0, Is...> : seq<Is...>{};

template<class Ch, class Tr, class Tuple, std::size_t... Is>
void print_tuple(std::basic_ostream<Ch,Tr>& os, Tuple const& t, seq<Is...>){
  using swallow = int[];
  (void)swallow{0, (void(os << (Is == 0? "" : ", ") << std::get<Is>(t)), 0)...};
}
} // aux::

template<class Ch, class Tr, class... Args>
auto operator<<(std::basic_ostream<Ch, Tr>& os, std::tuple<Args...> const& t)
    -> std::basic_ostream<Ch, Tr>&
{
  os << "(";
  aux::print_tuple(os, t, aux::gen_seq<sizeof...(Args)>());
  return os << ")";
}

void convertYUV(const Color& colors,double& y,double& u, double& v);
bool isSimilar(const Color& a, const Color& b);

#endif