#include "common.h"
#include "image.h"
#include "graph.h"
#include "voronoi.h"
#include "spline.h"
using namespace std;
float IMAGE_SCALE = 1.0f;

Image* gImage = nullptr;
Graph* gSimilarity = nullptr;
Voronoi* gDiagram = nullptr;
Spline* gCurves = nullptr;
int majorwindow;
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

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
  	{
		case 27:exit(0);break; 
  	}
}

float convCoordX(float x)
{
	return (2*x)/(IMAGE_SCALE*(gImage->getWidth())) - 1;
}

float convCoordY(float y)
{
	return (2*y)/(IMAGE_SCALE*(gImage->getHeight())) - 1;
}

void drawPolygon(vector<pair<float,float> > hull, float r, float g, float b)
{
	glColor3f(r,g,b);
	//Need to tessellate for handling concaves
	glBegin(GL_POLYGON);
	for(int i = 0 ; i < hull.size() ; i++) glVertex2f(convCoordX(IMAGE_SCALE*hull[i].first),convCoordY(IMAGE_SCALE*hull[i].second));
	if(hull.size()) glVertex2f(convCoordX(IMAGE_SCALE*hull[0].first),convCoordY(IMAGE_SCALE*hull[0].second));
	glEnd();

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0,1.0,1.0,0.0);
	
	//Print Image 
	#ifdef PIXELS
	glBegin(GL_QUADS);
	for(int x = 0 ; x < gImage->getWidth(); x++)
	for(int y = 0 ; y < gImage->getHeight(); y++)
	{
		auto color = (*gImage)(x,y)->getColor();
		float r = get<0>(color)/255.0;
		float g = get<1>(color)/255.0;
		float b = get<2>(color)/255.0;
		glColor3f(r, g, b);
		glVertex2f(convCoordX(IMAGE_SCALE*x), convCoordY(IMAGE_SCALE*y));
		glVertex2f(convCoordX(IMAGE_SCALE*(x+1)), convCoordY(IMAGE_SCALE*y));
		glVertex2f(convCoordX(IMAGE_SCALE*(x+1)), convCoordY(IMAGE_SCALE*(y+1)));
		glVertex2f(convCoordX(IMAGE_SCALE*x), convCoordY(IMAGE_SCALE*(y+1)));
	}
	glEnd();
	#endif

	//Draw Voronoi Diagrams
	#ifdef VORONOI
	for(int x = 0 ; x < gImage->getWidth(); x++)
	for(int y = 0 ; y < gImage->getHeight(); y++)
	{
		auto color = (*gImage)(x,y)->getColor();
		float r = get<0>(color)/255.0;
		float g = get<1>(color)/255.0;
		float b = get<2>(color)/255.0;
		auto hull = (*gDiagram)(x,y);

		//Fill Polygon
		drawPolygon(hull, r,g,b);

		glColor3f(0.0f, 0.0f, 0.0f);
		glBegin(GL_LINE_LOOP);
		for(int i = 0 ; i < hull.size() ; i++) glVertex2f(convCoordX(IMAGE_SCALE*hull[i].first),convCoordY(IMAGE_SCALE*hull[i].second));
		if(hull.size()) glVertex2f(convCoordX(IMAGE_SCALE*hull[0].first),convCoordY(IMAGE_SCALE*hull[0].second));
		glEnd();
	}
	#endif

	// Print Similarity
	#ifdef SIM
	for(int x = 0 ; x < gImage->getWidth(); x++)
	for(int y = 0 ; y < gImage->getHeight(); y++)
	{
		for(int k = 0 ; k < 8; k++) if(gSimilarity->edge(x,y,k))
		{
			glBegin(GL_LINES);
			glColor3f(0.5,0.5,1.0);
			glVertex2f(convCoordX(IMAGE_SCALE*(x+0.5)), convCoordY(IMAGE_SCALE*(y+0.5)));
			glVertex2f(convCoordX(IMAGE_SCALE*(x+0.5+direction[k][0])), convCoordY(IMAGE_SCALE*(y+0.5+direction[k][1])));
			glEnd();
		}
	}
	#endif
	
	//Draws Voronoi Points 
	#ifdef POINTS
	for(int x = 0 ; x < gImage->getWidth(); x++)
	for(int y = 0 ; y < gImage->getHeight(); y++)
	{
		glColor3f(0.0f,0.0f,1.0f);
		glPointSize(3);
		glBegin(GL_POINTS);
		for(pair<float,float> pt : (*gDiagram)(x,y)) glVertex2f(convCoordX(IMAGE_SCALE*pt.first),convCoordY(IMAGE_SCALE*pt.second));
		glEnd();
	}
	#endif

	#ifdef ACTIVE_EDGES
	glLineWidth(50.0);
	glBegin(GL_LINES);
	glColor3f(0.0,0.0,1.0);
	for(auto edge : gCurves->getActiveEdges())
	{
		auto color = (edge.second)->getColor();
		float r = get<0>(color)/255.0;
		float g = get<1>(color)/255.0;
		float b = get<2>(color)/255.0;
		glColor3f(r,g,b);
		glVertex2f(convCoordX(IMAGE_SCALE*(edge.first.first.first)), convCoordY(IMAGE_SCALE*(edge.first.first.second)));
		glVertex2f(convCoordX(IMAGE_SCALE*(edge.first.second.first)), convCoordY(IMAGE_SCALE*(edge.first.second.second)));
	}
	glEnd();
	glLineWidth(1.0f);
	#endif

	#ifdef BSPLINE_OVERLAY
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

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		cout << "Usage: ./depixel <<image_path>>\n";
		return 1;
	}
	//Image contains Pixel Data
	Image inputImage = Image(string(argv[1]));
	gImage = &inputImage;

	//Create Similarity Graph
	Graph similarity(inputImage); 
	gSimilarity = &similarity;
	//Planarize the graph
	similarity.planarize();
	
	//Test planarized similarity graph
	//printGraph(similarity);

	//Create Voronoi diagram for reshaping the pixels
	Voronoi diagram(inputImage);
	gDiagram = &diagram;
	diagram.createDiagram(similarity);
	//diagram.printVoronoi();

	//Create B-Splines on the end points of Voronoi edges.
	Spline curves(&diagram);
	gCurves = &curves;
	curves.extractActiveEdges();
	curves.calculateGraph();

	// Check the graph here
	// curves.printGraph();

	vector<vector<pair<float,float>>> points; 

	// points.push_back(curves.traverseGraph());

	//Optimize B-Splines

	//Output Image
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(50*inputImage.getWidth(),50*inputImage.getHeight());
	majorwindow = glutCreateWindow("Depixelize!");
	glutKeyboardFunc(keyboard);

	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK) {
		cerr << "Error: glewInit: " << glewGetErrorString(glew_status) << endl;
		return 1;
	}

	glutIdleFunc(idleFunction);
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}