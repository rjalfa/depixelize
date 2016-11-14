#include "common.h"
#include "image.h"
#include "graph.h"
#include "voronoi.h"
using namespace std;

Image* gImage = nullptr;
Graph* gSimilarity = nullptr;
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

float convCoordX(float x)
{
	return (2*x)/(4*(gImage->getWidth())) - 1;
}

float convCoordY(float y)
{
	return (2*y)/(4*(gImage->getHeight())) - 1;
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0,1.0,1.0,0.0);
	float IMAGE_SCALE = 4.0f;
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
	for(int x = 0 ; x < gImage->getWidth(); x++)
	for(int y = 0 ; y < gImage->getHeight(); y++)
	{
		for(int k = 0 ; k < 8; k++) if(gSimilarity->edge(x,y,k))
		{
			glBegin(GL_LINES);
			glColor3f(0.0,0.0,1.0);
			glVertex2f(convCoordX(IMAGE_SCALE*(x+0.5)), convCoordY(IMAGE_SCALE*(y+0.5)));
			glVertex2f(convCoordX(IMAGE_SCALE*(x+0.5+direction[k][0])), convCoordY(IMAGE_SCALE*(y+0.5+direction[k][1])));
			glEnd();
		}
	}
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
	diagram.createDiagram(similarity);

	diagram.printVoronoi();

	//Create B-Splines on the end points of Voronoi edges.

	//Optimize B-Splines

	//Output Image
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(50*inputImage.getWidth(),50*inputImage.getHeight());
	majorwindow = glutCreateWindow("Depixelize!");

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