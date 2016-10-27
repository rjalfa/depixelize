#include "image.h"
#include "graph.h"
#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
	if(argc < 1)
	{
		cout << "Usage: ./depixel <<image_path>>\n";
		return 1;
	}
	//Image contains Pixel Data
	Image inputImage = Image(string(argv[1]));

	//Create Similarity Graph
	Graph similarity(inputImage); 

	//Planarize the graph

	//Create Voronoi diagram for reshaping the pixels

	//Create B-Splines on the end points of Voronoi edges.

	//Optimize B-Splines

	//Output Image
	return 0;
}