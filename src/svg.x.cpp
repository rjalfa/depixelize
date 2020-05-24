#include "common.h"
#include "image.h"
#include "graph.h"
#include "voronoi.h"
#include "spline.h"
#include "simple-svg.hpp"

#include <iostream>

using namespace std;
unsigned IMAGE_SCALE = 10;

uint8_t rotateLevel = 0;

//Global vars for use in render()
Image* gImage = nullptr;
Graph* gSimilarity = nullptr;
Voronoi* gDiagram = nullptr;
Spline* gCurves = nullptr;
vector<pair<vector<Point>,Color> > mainOutLine;

//Pretty Print graph to std::cout
void printGraph(Graph& g)
{
	std::cout<<"Printing graph"<<endl;
	Image* img = g.getImage();
	int width = img->getWidth();
	int height = img->getHeight();
	std::cout<<"(height, width) = ("<<height<<", "<<width<<")\n";
	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			for(int k = 0;k<8; k++)
			{
				std::cout<<"("<<i<<","<<j<<","<<k<<") = "<<g.edge(i,j,(Direction)k)<<"\t";
			}
			std::cout<<"\n";
		}
	}
}

#define draw(x, y) svg::Point(IMAGE_SCALE * x, IMAGE_SCALE * y)
#define HALF_UNIT IMAGE_SCALE * 0.5f

//Function to draw a closed convex polygon with fill color.
void drawPolygon(svg::Document &doc, std::vector<pair<float,float> > hull, const Color& c)
{
	svg::Polygon polygon(svg::Color(c.R, c.G, c.B));
	for (const auto& point : hull) polygon << draw(X(point), Y(point));
	doc << polygon;
}

// calculates 1(x1)+t(x2)+t^2(x3) for bspline points
inline float evalBspline(float x1, float x2, float x3, float t) { return x1 + x2*t + x3*t*t; }

// Function to draw a q-u-b spline curve from 3 control points
void drawSpline(svg::Document &doc, pair<float,float> p1, pair<float,float> p2, pair<float,float> p3, const Color &color)
{
	vector<vector<float>> matrix = gCurves->getSpline({p1,p2,p3});
	//T is extroplated a little for intersecting pieces

	// parameter t that will generate all the points
	float t = -0.1f;

	// increment t by steps
	float step = 0.01f;

	// store x,y after multiplying with 1, t and t^2
	float xcor, ycor;

	svg::Polyline poly_line(svg::Stroke(1, svg::Color(color.R, color.G, color.B)));
	for(;t<=1.1f; t+=step) poly_line << draw(evalBspline(matrix[0][0],matrix[1][0],matrix[2][0],t), evalBspline(matrix[0][1],matrix[1][1],matrix[2][1],t));
	doc << poly_line;
}

void drawCell(svg::Document& doc, int x, int y, const Color& c) {
	float cx = x + 0.5f;
	float cy = y + 0.5f;
	doc << (svg::Polygon(svg::Color(c.R, c.G, c.B))
		<< draw(cx - HALF_UNIT, cy - HALF_UNIT)
		<< draw(cx + HALF_UNIT, cy - HALF_UNIT)
		<< draw(cx + HALF_UNIT, cy + HALF_UNIT)
		<< draw(cx - HALF_UNIT, cy + HALF_UNIT)
	);
}

//Render Function
void drawImage(svg::Document &doc)
{
	//Draw Voronoi Diagrams
	for(int x = 0 ; x < gImage->getWidth(); x++)
	for(int y = 0 ; y < gImage->getHeight(); y++)
	{
		auto hull = (*gDiagram)(x,y);
		//Fill Polygon
		drawPolygon(doc, hull, (*gImage)(x,y)->color());
	}

	for(pair<vector<Point>,Color> curve: mainOutLine)
	{
		auto points = curve.first;
		auto color = curve.second;
		if(points.size() < 2) continue;
		for(int i = 0; i < points.size()-2; i++)
			drawSpline(doc, points[i],points[(i+1)%points.size()],points[(i+2)%points.size()], color);
	}
}

int main(int argc, char** argv)
{
	if(argc < 3)
	{
		std::cout << "Usage: " << argv[0] << " <<image_path>> <<output_path>>\n";
		return 1;
	}
	std::string output_path = std::string(argv[2]);

	//Image contains Pixel Data
	Image inputImage = Image(string(argv[1]));
	gImage = &inputImage;

	////Create Similarity Graph
	Graph similarity(inputImage);
	gSimilarity = &similarity;
	//Planarize the graph
	similarity.planarize();

	////Test planarized similarity graph
	////printGraph(similarity);

	//Create Voronoi diagram for reshaping the pixels
	Voronoi diagram(inputImage);
	gDiagram = &diagram;
	diagram.createDiagram(similarity);
	//diagram.printVoronoi();

	////Create B-Splines on the end points of Voronoi edges.
	Spline curves(&diagram);
	gCurves = &curves;
	curves.extractActiveEdges();
	curves.calculateGraph();

	//// Check the graph here

	//// mainOutLine contains all the outline edges where we will fit the b-splines
	mainOutLine = curves.printGraph();
	////std::cout << mainOutLine << endl;
	//Optimize B-Splines

	//Output Image
	svg::Dimensions dimensions(IMAGE_SCALE * inputImage.getWidth(), IMAGE_SCALE * inputImage.getHeight());
	svg::Document doc(output_path, svg::Layout(dimensions, svg::Layout::TopLeft));

	drawImage(doc);

	doc.save();
	return 0;
}