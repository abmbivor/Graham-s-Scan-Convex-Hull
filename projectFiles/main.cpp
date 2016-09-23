#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include <stack>
#include <windows.h>
#include <GL/gl.h>
#include <glut.h>

using namespace std;

struct point
{
    double x,y;
};

point originP;
int nodesNo;
point inputGraph [1000];
point finalGraph [1000];
int solutionSize;

double dist(point a,point b)
{
    double distance=0;
    distance= sqrt( pow(a.x-b.x,2) + pow(a.y-b.y,2)  );
    return distance;
}

bool comparator(point a,point b)
{
    double thetaA,thetaB;
    thetaA = atan2( a.y-originP.y , a.x-originP.x );
    thetaB = atan2( b.y-originP.y , b.x-originP.x );
    if(thetaA==thetaB)return dist(originP,a)<dist(originP,b);
    else return thetaA<thetaB;
}

int minPoint(point input[],int nodesN)
{
    int nodesNo,resultNode;
    double minVal;
    vector<int> tempResult;
    nodesNo = nodesN;
    minVal = 9999;
    //finding out the min ORDINATE
    for(int i=0;i<nodesNo;i++)
    {
        if(input[i].y< minVal)
        {
            minVal = input[i].y;
            resultNode=i;
        }
    }
    //checking if multiple
    int countMin=0;
    for(int i=0;i<nodesNo;i++)
    {
        if(input[i].y==minVal)
        {
            countMin++;
            tempResult.push_back(i);
        }
    }
    minVal = 9999;
    if(countMin>1) // multiple ,now sort on the basis of ABSCISSA
    {
        for(int i=0;i<countMin;i++)
        {
            if(input[( tempResult.at(i) )].x < minVal )
            {
                minVal = input[( tempResult.at(i) )].x;
                resultNode = tempResult.at(i);
            }
        }
        return resultNode;
    }
    else return resultNode;

}

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double moveX,moveY;
double angle;

void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{

	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSS()
{
    glColor3f(1,0,0);
    //glutSolidCube(20);
    int scale=100;
    glLineWidth(8);
    glPointSize(8);
    for(int i=0;i<nodesNo;i++)
    {
        glBegin(GL_POINTS);
        {
            glVertex3f(scale*inputGraph[i].x,scale*inputGraph[i].y,0);
        }
        glEnd();
    }

    glColor3f(0,1,0);
    for(int i =0;i<solutionSize-1;i++)
    {
        glBegin(GL_LINES);
        {
            glVertex3f(scale*finalGraph[i].x,scale*finalGraph[i].y,0);
            glVertex3f(scale*finalGraph[i+1].x,scale*finalGraph[i+1].y,0);
        }
        glEnd();
    }
    glFlush();

}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			drawgrid=1-drawgrid;
			break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
            moveY-=0.5;
			break;
		case GLUT_KEY_UP:		// up arrow key
            moveY+=0.5;
			break;

		case GLUT_KEY_RIGHT:
			moveX += 0.5;
			break;
		case GLUT_KEY_LEFT:
			moveX -= 0.5;
			break;
        case GLUT_KEY_PAGE_UP:
            cameraHeight += 3.0;
			break;
		case GLUT_KEY_PAGE_DOWN:
            cameraHeight -= 3.0;
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(0,0,cameraHeight,	moveX,moveY,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

    drawSS();


	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	//angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=70.0;
	cameraAngle=1.0;
	moveX=0.0;
	moveY=0.0;
	angle=0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv)
{
    ifstream input;
    input.open("testcase_sample3.txt");

    nodesNo;
    input>> nodesNo;

    cout << "Total nodes No : "<<nodesNo <<'\n';
    cout<<'\n';
    //point inputGraph[nodesNo];
    point forSortingGraph[nodesNo];
    //graph inputGraph(nodesNo);


    for(int i = 0;i<nodesNo;i++)
    {
        input>>inputGraph[i].x; input>>inputGraph[i].y;
        forSortingGraph[i].x = inputGraph[i].x;
        forSortingGraph[i].y = inputGraph[i].y;
        //inputGraph.set_values(x,y);
        cout<< "x: " << inputGraph[i].x <<"   y: " << inputGraph[i].y<<'\n';

    }
    cout<<'\n';

    int pointP= minPoint(inputGraph,nodesNo);
    originP = inputGraph[pointP];


    cout<<"The Selected point to start with is : "<<originP.x<<" "<<originP.y<<'\n';
    std::sort(forSortingGraph,forSortingGraph+nodesNo,comparator);

    cout<<'\n';
    cout<<"The sorted order with respect to "<<originP.x<<" , "<<originP.y<<" is : "<<'\n';
    cout<<'\n';
    for (int i=0;i<nodesNo;i++)
    {
        cout<<forSortingGraph[i].x<<" "<<forSortingGraph[i].y<<'\n';
    }
    cout<<'\n';

    stack<point> solution;
    solution.push(forSortingGraph[nodesNo-1]);
    solution.push(originP);
    solution.push(forSortingGraph[1]);

    point beforeTop;
    beforeTop = originP;
    //beforeTop = forSortingGraph[nodesNo-1];
    point first=originP;
    int n=2;
    //for(int i=2;i<nodesNo;i++)
    while(n!=nodesNo)
    {
        double x1,x2,y1,y2;
        x1 = solution.top().x - beforeTop.x;
        y1 = solution.top().y - beforeTop.y;
        x2 = forSortingGraph[n].x - beforeTop.x;
        y2 = forSortingGraph[n].y - beforeTop.y;
        double xMult = (x1*y2) - (x2*y1);
        if(xMult>0)
        {
            beforeTop = solution.top(); // the current top becomes the 2nd top
            solution.push(forSortingGraph[n]);
            n++;
        }
        else
        {
            solution.pop();   // popping the concaved or boundary one
            point top = solution.top();
            solution.pop();
            beforeTop = solution.top();
            //solution.push(forSortingGraph[i]); // pushing the latest point
            solution.push(top);
        }


    }

    solutionSize= solution.size();
    //point finalGraph[solutionSize];

    for(int i=0;i<solutionSize;i++)
    {
        finalGraph[solutionSize-1-i] = solution.top();
        //cout<<"solution : "<< solution.top().x<<" "<<solution.top().y<<'\n';
        solution.pop();
    }

    cout<<"The Points in the Convex Hull are : "<<"\n\n";
    for(int i =0;i<solutionSize;i++)
    {
        cout<< "x: " << finalGraph[i].x <<"   y: " << finalGraph[i].y<<'\n';
    }

    glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(400, 100);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Convex Hull");


	init();


	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

    return 0;
}

