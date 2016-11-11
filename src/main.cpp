#include "image.h"
#include "graph.h"
#include "voronoi.h"
#include <iostream>
using namespace std;

void printGraph(Graph g)
{
	cout<<"Printing graph"<<endl;
	

}

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		cout << "Usage: ./depixel <<image_path>>\n";
		return 1;
	}
	//Image contains Pixel Data
	Image inputImage = Image(string(argv[1]));
	
	//Create Similarity Graph
	Graph similarity(inputImage); 

	//Planarize the graph
	similarity.planarize();
	
	//Test planarized similarity graph
	printGraph(similarity);

	//Create Voronoi diagram for reshaping the pixels
	// Voronoi diagram(inputImage);
	// diagram.createDiagram(similarity);

	//Create B-Splines on the end points of Voronoi edges.

	//Optimize B-Splines

	//Output Image
	return 0;
}