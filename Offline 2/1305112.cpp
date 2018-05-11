#define START 1
#define END 2
#define SPLIT 3
#define MERGE 4
#define LEFT_REGULAR 5
#define RIGHT_REGULAR 6

#include<bits/stdc++.h>
#include<algorithm>
#include<GL/glut.h>
using namespace std;
#define pi (2*acos(0.0))
ifstream fin("input1.txt");

struct point
{
    double x,y;

};

int vertexType[1000];
set<point>T;


inline bool operator<(const point& lhs, const point& rhs)
{
    return lhs.x < rhs.x || ((lhs.x==rhs.x) && (lhs.y>rhs.y) );
}

inline bool operator==(const point& lhs, const point& rhs)
{
    return lhs.x == rhs.x  && lhs.y==rhs.y ;
}

bool compareByY(const point &a, const point &b)
{
    return (a.y > b.y) ||((a.y==b.y) && (a.x<b.x));
}

vector<point>points,sorted,newEdges;
map<point,point>helper;

void input()
{
    int n;
    double a, b;
    point np;
    fin>>n;
    for(int i=0; i<n; i++)
    {
        fin>>a>>b;
        np.x=a;
        np.y=b;

        points.push_back(np);
    }
}

double checkTurn(point a, point b, point c)
{
    double v1x=b.x-a.x;
    double v1y=b.y-a.y;

    double v2x=c.x-a.x;
    double v2y=c.y-a.y;

    return v1x*v2y-v1y*v2x;
}

void CheckVertexType()
{
    point a,b,c;
    for(vector<point>::iterator it=points.begin();it!=points.end();it++)
    {
        if(it==points.begin())a=points[points.size()-1];
        else a=*(it-1);

        if(it==points.end()-1)c=points[0];
        else c=*(it+1);

        b=*it;
        int i=it-points.begin();
        if(checkTurn(a,b,c)>0 && b.y>a.y && b.y>c.y)vertexType[i]=START;
        else if(checkTurn(a,b,c)>0 && b.y<a.y && b.y<c.y)vertexType[i]=END;
        else if(checkTurn(a,b,c)<0 && b.y>a.y && b.y>c.y)vertexType[i]=SPLIT;
        else if(checkTurn(a,b,c)<0 && b.y<a.y && b.y<c.y)vertexType[i]=MERGE;
        else if(a.y>c.y)vertexType[i]=LEFT_REGULAR;
        else if(c.y>a.y)vertexType[i]=RIGHT_REGULAR;//    cout<<"right vertex "<<b.x<<" "<<b.y<<endl;


    }

}

void handleStartVertex(point v)
{
    T.insert(v);
    helper[v]=v;
    cout<<v.x<<" "<<v.y<<endl;
}

void iterate()
{
    for(vector<point>::iterator it=sorted.begin();it!=sorted.end();it++)
    {
        point v=*it;
        vector<point>::iterator x=find(points.begin(),points.end(),v);
        //cout<<v.x<<" "<<v.y<<" "<<x-points.begin()<<endl;
        int id=x-points.begin();
        if(vertexType[id]==START)
        {

            handleStartVertex(v);
        }
    }
}

int drawaxes;

void drawAxes()
{
    if(drawaxes==1)
    {
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINES);
        {
            glVertex3f( 100,0,0);
            glVertex3f(-100,0,0);

            glVertex3f(0,-100,0);
            glVertex3f(0, 100,0);

            glVertex3f(0,0, 100);
            glVertex3f(0,0,-100);
        }
        glEnd();
    }
}



void display()
{

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
    gluLookAt(0,0,200,	0,0,0,	0,1,0);


    //again select MODEL-VIEW
    glMatrixMode(GL_MODELVIEW);


    /****************************
    / Add your objects from here
    ****************************/
    //add objects


    glScalef(10,10,1);
    glPointSize(3);
    for(vector<point>::iterator it=points.begin(); it!=points.end(); it++)       //print
    {
        point tr=*it;
        glColor3f(1.0, 1.0, 0);
        glBegin(GL_POINTS);
        {

            glVertex3f(tr.x,tr.y,0);

        }
        glEnd();

    }

    for(vector<point>::iterator it=points.begin()+1; it!=points.end(); it++)
    {
        point tr=*it;
        //cout<<tr.x<<" "<<tr.y<<" "<<endl;//tr.angle<<endl;
        point nx=*(it-1);

        glColor3f(1.0,0,0);
        glBegin(GL_LINES);
        {
            glVertex3f( nx.x,nx.y,0);
            glVertex3f(tr.x,tr.y,0);

        }
        glEnd();
    }

    point tr=*(points.begin());
    //cout<<tr.x<<" "<<tr.y<<" "<<endl;//tr.angle<<endl;
    point nx=points[points.size()-1];

    glColor3f(1.0,0,0);
    glBegin(GL_LINES);
    {
        glVertex3f( nx.x,nx.y,0);
        glVertex3f(tr.x,tr.y,0);

    }
    glEnd();



    drawAxes();



    //ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
    glutSwapBuffers();
}


void animate()
{
    //angle+=0.05;
    //codes for any changes in Models, Camera
    glutPostRedisplay();
}

void init()
{
    //codes for initialization
    drawaxes=1;


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


    input();

    CheckVertexType();
    sorted=points;
    sort(sorted.begin(),sorted.end(),compareByY);
    iterate();
    for(set<point>::iterator it=T.begin();it!=T.end();it++)
    {
        point s=*it;
        cout<<s.x<<" "<<s.y<<endl;
    }



    glutInit(&argc,argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

    glutCreateWindow("My OpenGL Program");

    init();

    glEnable(GL_DEPTH_TEST);	//enable Depth Testing

    glutDisplayFunc(display);	//display callback function
    glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

    glutMainLoop();



}


//    for(vector<point>::iterator it=points.begin();it!=points.end();it++)         //print
//    {
//        point tr=*it;
//        cout<<tr.x<<" "<<tr.y<<" "<<endl;
//    }

//
//cout<<bottom.x<<" "<<bottom.y<<endl;                           //prinnt
//    cout<<rt.x<<" "<<rt.y<<endl;
//    cout<<top.x<<" "<<top.y<<endl;
//    cout<<lf.x<<" "<<lf.y<<endl;

