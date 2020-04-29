#include "common.h"
#include "image.h"
#include "graph.h"
#include "voronoi.h"
#include "spline.h"

#define PIXELS

#include <GL/freeglut.h>
#include <GL/gl.h>
#include <iostream>

using namespace std;
float IMAGE_SCALE = 1.0f;

uint8_t rotateLevel = 0;

//Global vars for use in render()
Image* gImage = nullptr;
Graph* gSimilarity = nullptr;
Voronoi* gDiagram = nullptr;
Spline* gCurves = nullptr;
vector<pair<vector<Point>,Color> > mainOutLine;

int majorwindow;

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

//keyboard() for ESC functionality
void keyboard(unsigned char key, int x, int y)
{
	switch(key)
  	{
		case 27:exit(0);break;
		case 'r':rotateLevel = (rotateLevel + 1) % 4; break;
  	}
}

//Converts (0,w) -> (-1, 1)
float convCoordX(float x)
{
	return (2*x)/(IMAGE_SCALE*(gImage->getWidth())) - 1;
}


//Converts (0,h) -> (-1, 1)
float convCoordY(float y)
{
	return (2*y)/(IMAGE_SCALE*(gImage->getHeight())) - 1;
}

// Converts {[0,width],[0, height]} -> {[-1,1], [-1,1]}
// Map :: [0,0] -> [-1, 1]
// Map :: [width, height] -> [1, -1]
void draw(float px, float py) {
	float width = gImage->getWidth();
	float height = gImage->getHeight();
	float x = (2 * px) / width - 1;
	float y = 1 - (2 * py) / height;
	glVertex2f(x, y);
}

void draw(Point p) {
	draw(X(p), Y(p));
}

//Function to draw a closed convex polygon with fill color.
void drawPolygon(vector<pair<float,float> > hull, float r, float g, float b)
{
	glColor3f(r,g,b);
	//Need to tessellate for handling concaves
	glBegin(GL_POLYGON);
	for (const auto& point : hull) draw(point);
	if (hull.size()) draw(hull[0]);
	glEnd();

}

// calculates 1(x1)+t(x2)+t^2(x3) for bspline points
inline float evalBspline(float x1, float x2, float x3, float t) { return x1 + x2*t + x3*t*t; }

//Function to draw a q-u-b spline curve from 3 control points
void drawSpline(pair<float,float> p1, pair<float,float> p2, pair<float,float> p3)
{
	vector<vector<float>> matrix = gCurves->getSpline({p1,p2,p3});
	//T is extroplated a little for intersecting pieces

	// parameter t that will generate all the points
	float t = -0.1f;

	// increment t by steps
	float step = 0.01f;

	// store x,y after multiplying with 1, t and t^2
	float xcor, ycor;
	for(;t<=1.1f; t+=step) draw(evalBspline(matrix[0][0],matrix[1][0],matrix[2][0],t), evalBspline(matrix[0][1],matrix[1][1],matrix[2][1],t));
}

std::pair<float, float> getCenter(int x, int y, int width, int height) {
	float cx = (2 * x - 1.0f - width) / width;
	float cy = (height - 2 * y + 1.0f) / height;
	return std::make_pair(cx, cy);
}

void drawCell(int x, int y) {
	float cx = x + 0.5f;
	float cy = y + 0.5f;
	draw(cx - 0.5f, cy - 0.5f);
	draw(cx + 0.5f, cy - 0.5f);
	draw(cx + 0.5f, cy + 0.5f);
	draw(cx - 0.5f, cy + 0.5f);
}

// cx, cy is a coord with TL (1,1) and BR (w, h)
void drawLine(int x1, int y1, int x2, int y2, int width, int height) {
	auto centerPoint1 = getCenter(x1, y1, width, height);
	float cx1 = centerPoint1.first;
	float cy1 = centerPoint1.second;
	auto centerPoint2 = getCenter(x2, y2, width, height);
	float cx2 = centerPoint2.first;
	float cy2 = centerPoint2.second;
	
	float invwidth = 1.0f / width;
	float invheight = 1.0f / height;
	glVertex2f(cx1, cy1);
	glVertex2f(cx2, cy2);
}

//Render Function
void display()
{
	bool check = true;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0,1.0,1.0,0.0);
	// glEnable( GL_MULTISAMPLE );

	//Print Image [PIXELS]
#ifndef PIXELS
	glBegin(GL_QUADS);
	for(int x = 0 ; x < gImage->getWidth(); x++)
	for(int y = 0 ; y < gImage->getHeight(); y++)
	{
		auto color = (*gImage)(x,y)->color();
		float r = color.R/255.0;
		float g = color.G/255.0;
		float b = color.B/255.0;
		glColor3f(r, g, b);
		drawCell(x, y);
	}
	glEnd();
#endif

	//Draw Voronoi Diagrams
	for(int x = 0 ; x < gImage->getWidth(); x++)
	for(int y = 0 ; y < gImage->getHeight(); y++)
	{
		auto color = (*gImage)(x, y)->color();
		float r = color.R / 255.0;
		float g = color.G / 255.0;
		float b = color.B / 255.0;
		auto hull = (*gDiagram)(x,y);

		//Fill Polygon
		drawPolygon(hull, r,g,b);

		//Draw boundaries of reshaped cells
		/*glColor3f(0.0f, 0.0f, 0.0f);
		glBegin(GL_LINE_LOOP);
		for(const auto& point : hull) draw(point);
		if(hull.size()) draw(hull[0]);
		glEnd();*/
	}


#if 0
	// Print Similarity graph
	glBegin(GL_LINES);
	for(int x = 0 ; x < gImage->getWidth(); x++)
	for(int y = 0 ; y < gImage->getHeight(); y++)
	{
		for(int k = 0 ; k < 8; k++) if(gSimilarity->edge(x,y,(Direction)k))
		{
			glColor3f(0.5,0.5,1.0);
			auto adjPixel = gImage->getAdjacent(x, y, (Direction)k);
			if (adjPixel) {
				drawLine(x, y, adjPixel->X(), adjPixel->Y(), gImage->getWidth(), gImage->getHeight());
			}
				
		}
	}
	glEnd();

	//Draws Voronoi Points around the pixels
	for(int x = 0 ; x < gImage->getWidth(); x++)
	for(int y = 0 ; y < gImage->getHeight(); y++)
	{
		glColor3f(0.0f,0.0f,1.0f);
		glPointSize(3);
		glBegin(GL_POINTS);
		for(const Point& pt : (*gDiagram)(x,y)) draw(pt);
		glEnd();
	}
	
#endif
	//Draws active edges selected by Spline for curve tracing
	glLineWidth(5.0);
	glBegin(GL_LINES);
	glColor3f(0.0,0.0,1.0);
	for(auto edge : gCurves->getActiveEdges())
	{
		auto color = (edge.second)->color();
		float r = color.R / 255.0;
		float g = color.G / 255.0;
		float b = color.B / 255.0;
		glColor3f(r,g,b);
		draw(edge.first.first);
		draw(edge.first.second);
	}
	glEnd();

	glLineWidth(1.0f);
	
	//Draw BSPLINE CURVES
	#ifndef BSPLINE_OVERLAY
	glLineWidth(10.0f);
	for(pair<vector<Point>,Color> curve: mainOutLine)
	{
		auto points = curve.first;
		auto color = curve.second;
		if(points.size() < 2) continue;
		float r = color.R/255.0;
		float g = color.G/255.0;
		float b = color.B/255.0;
		glColor3f(r,g,b);
		glBegin(GL_LINE_STRIP);
		for(int i = 0; i < points.size()-2; i++)
		{
			drawSpline(points[i],points[(i+1)%points.size()],points[(i+2)%points.size()]);
			//i++;
		}
		glEnd();
		//break;
	}
	//drawSpline(make_pair(1.f,7.f),make_pair(1.25f,6.25f),make_pair(1.f,8.f));
	glLineWidth(1.0f);
	#endif

	#ifdef FINAL
	#endif
	glutSwapBuffers();
}



void idleFunction()
{
	glutSetWindow(majorwindow);
	glutPostRedisplay();
}

void drawImage(int argc, char* argv[], int width, int height) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(2, 2);
	majorwindow = glutCreateWindow("Depixelize!");
	glutKeyboardFunc(keyboard);
	glutInitWindowSize(width, height);
	glutIdleFunc(idleFunction);
	glutDisplayFunc(display);
	glutMainLoop();
}

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		std::cout << "Usage: " << argv[0] << " <<image_path>>\n";
		return 1;
	}
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
	#ifndef NO_RENDER
	drawImage(argc, argv, inputImage.getWidth(), inputImage.getHeight());
	//draw_square(argc, argv, 50, 50);
	#endif
	return 0;
}