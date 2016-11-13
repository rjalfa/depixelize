#include "common.h"
#include "image.h"
#include "graph.h"
#include "voronoi.h"
using namespace std;

void printGraph(Graph& g)
{
	cout<<"Printing graph"<<endl;
	Image* img = g.getImage();
	int width = img->getWidth();
	int height = img->getHeight();
	cout<<"(height, width) = ("<<height<<", "<<width<<")\n";
	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			for(int k = 0;k<8; k++)
			{
				cout<<"("<<i<<","<<j<<","<<k<<") = "<<g.edge(i,j,k)<<"\t";
			}
			cout<<"\n";
		}
	}
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
	Voronoi diagram(inputImage);
	diagram.createDiagram(similarity);
	diagram.printVoronoi();

	//Create B-Splines on the end points of Voronoi edges.

	//Optimize B-Splines

	//Output Image
	return 0;
}