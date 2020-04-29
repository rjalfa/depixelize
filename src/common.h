#pragma once

#ifndef _COMMON_H
#define _COMMON_H

#include <utility>
#include <ostream>
#include <map>
#include <set>
#include <vector>

#ifndef NDEBUG
#define DEBUG(X) std::cout << X
#else
#define DEBUG(X)
#endif


//Some type definitions for code brevity
using Point = std::pair<float,float>;
using IntPoint = std::pair<int, int>;
using Edge = std::pair<Point,Point>;

template<typename K1, typename K2>
K1 X(const std::pair<K1, K2>& a) { return a.first; }

template<typename K1, typename K2>
K2 Y(const std::pair<K1, K2>& a) { return a.second; }

//DIRECTION MACROS : Used for defining directions
enum Direction {
 TOP = 1,
 TOP_RIGHT = 2,
 TOP_LEFT = 0,
 LEFT = 3,
 RIGHT = 4,
 BOTTOM = 6,
 BOTTOM_RIGHT = 7,
 BOTTOM_LEFT = 5
};

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

//Pretty print C++ structures
template<class T,class V>
std::ostream& operator<<(std::ostream& out, const std::pair<T,V>& p)
{
	out << "{" << p.first << "," << p.second << "}";
	return out;
}

template<class T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v)
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
std::ostream& operator<<(std::ostream& out, const std::map<K,V>& v)
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
std::ostream& operator<<(std::ostream& out, const std::set<K>& s)
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

//http://stackoverflow.com/questions/6245735/pretty-print-stdtuple
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

#endif