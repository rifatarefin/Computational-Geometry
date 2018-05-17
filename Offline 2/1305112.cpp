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
ifstream fin("input2.txt");

struct point
{
    double x,y;
    int order,vertexType;
    mutable int helperIndex;

};
int N;
set<point>T;
vector<point>points,sorted,newEdges;

point nextPoint(point p)
{
    int id=p.order;
    point tmp;
    if(id==points.size()-1)tmp=points[0];
    else tmp =points[id+1];
    return tmp;
}

point prevPoint(point p)
{
    int id=p.order;
    point tmp;
    if(id==0)tmp=points[points.size()-1];
    else tmp =points[id-1];
    return tmp;
}

inline bool operator<(const point& lhs, const point& rhs)
{

    point nxt=nextPoint(lhs);
    double v1x=lhs.x-rhs.x;
    double v1y=lhs.y-rhs.y;

    double v2x=nxt.x-rhs.x;
    double v2y=nxt.y-rhs.y;
    return v1x*v2y-v1y*v2x>0;

}

inline bool operator==(const point& lhs, const point& rhs)
{
    return lhs.x == rhs.x  && lhs.y==rhs.y ;
}

bool compareByY(const point &a, const point &b)
{
    return (a.y > b.y) ||((a.y==b.y) && (a.x<b.x));
}



void input()
{
    int n;
    double a, b;
    point np;
    fin>>n;
    points.clear();
    for(int i=0; i<n; i++)
    {
        fin>>a>>b;
        np.x=a;
        np.y=b;
        np.order=i;
        points.push_back(np);
    }
}

bool below(point a, point b)        //a below b
{
    return a.y<b.y || (a.y==b.y && a.x>b.x);
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
        b=*it;
        a=prevPoint(b);

        c=nextPoint(b);
        (*it).helperIndex=b.order;
        if(checkTurn(a,b,c)>0 && below(a,b) && below(c,b))
        {
            (*it).vertexType=START;
            cout<<b.x<<" "<<b.y<<" Start"<<endl;
        }
        else if(checkTurn(a,b,c)>0 && below(b,a) && below(b,c))
        {
            (*it).vertexType=END;
            cout<<b.x<<" "<<b.y<<" End"<<endl;
        }
        else if(checkTurn(a,b,c)<0 && below(a,b) && below(c,b))
        {
            (*it).vertexType=SPLIT;
            cout<<b.x<<" "<<b.y<<" Split"<<endl;
        }
        else if(checkTurn(a,b,c)<0 && below(b,a) && below(b,c))
        {
            (*it).vertexType=MERGE;//cout<<b.x<<" "<<b.y<<endl;}
            cout<<b.x<<" "<<b.y<<" Merge"<<endl;
    }
        else if(below(c,a))
        {
            (*it).vertexType=LEFT_REGULAR;
            cout<<b.x<<" "<<b.y<<" Left"<<endl;
        }
        else if(below(a,c))
        {
            (*it).vertexType=RIGHT_REGULAR;//    cout<<"right vertex "<<b.x<<" "<<b.y<<endl;
            cout<<b.x<<" "<<b.y<<" Right"<<endl;
        }


    }

}

void handleStartVertex(point v)
{

    v.helperIndex=v.order;
    T.insert(v);
    //cout<<v.x<<" "<<v.y<<endl;
}

void handleSplitVertex(point v)
{

    set<point>::iterator it=T.lower_bound(v);
    it--;
    point e=*it;
    point helper=points[e.helperIndex];
    newEdges.push_back(v);
    newEdges.push_back(helper);
    (*it).helperIndex=v.order;
    v.helperIndex=v.order;

    vector<point>::iterator vt=find(points.begin(),points.end(),*it);
    (*vt).helperIndex=v.order;

    T.insert(v);


}

void handleMergeVertex(point v)
{
    point pre=prevPoint(v);
    point prehelper=points[pre.helperIndex];

    if(prehelper.vertexType==MERGE)
    {
        newEdges.push_back(v);
        newEdges.push_back(prehelper);
    }


    set<point>::iterator xt=find(T.begin(),T.end(),pre);
    if(xt!=T.end())T.erase(xt);
    xt=T.lower_bound(v);
    xt--;
    point left=*xt;
    point lefthelper=points[left.helperIndex];
    if(lefthelper.vertexType==MERGE)
    {
        newEdges.push_back(v);
        newEdges.push_back(lefthelper);
        cout<<"dds"<<endl;
    }
    (*xt).helperIndex=v.order;
    vector<point>::iterator vt=find(points.begin(),points.end(),*xt);
    (*vt).helperIndex=v.order;


}

void handleLeftRegularVertex(point v)
{
    point pre=prevPoint(v);
    point prehelper=points[pre.helperIndex];
    if(prehelper.vertexType==MERGE)
    {
        newEdges.push_back(v);
        newEdges.push_back(prehelper);
    }

    set<point>::iterator xt=find(T.begin(),T.end(),pre);
    if(xt!=T.end())T.erase(xt);
    v.helperIndex=v.order;
    T.insert(v);

}

void handleRightRegularVertex(point v)
{
    set<point>::iterator it=T.lower_bound(v);
    it--;
    point e=*it;
    point helper=points[e.helperIndex];
    if(helper.vertexType==MERGE)
    {
        newEdges.push_back(v);
        newEdges.push_back(helper);
    }
    (*it).helperIndex=v.order;

    vector<point>::iterator vt=find(points.begin(),points.end(),*it);
    (*vt).helperIndex=v.order;
}

void handleEndVertex(point v)
{
    point pre=prevPoint(v);
    point prehelper=points[pre.helperIndex];
    if(prehelper.vertexType==MERGE)
    {
        newEdges.push_back(v);
        newEdges.push_back(prehelper);
    }
    set<point>::iterator xt=find(T.begin(),T.end(),pre);
    if(xt!=T.end())T.erase(xt);
}



void iterate()
{
    for(vector<point>::iterator it=sorted.begin();it!=sorted.end();it++)
    {
        point v=*it;

        if(v.vertexType==START)
        {
            //cout<<v.x<<" "<<v.y<<endl;
            handleStartVertex(*it);
        }
        else if(v.vertexType==SPLIT)
        {

            handleSplitVertex(v);
        }
        else if(v.vertexType==MERGE)
        {

            handleMergeVertex(v);
        }
        else if(v.vertexType==LEFT_REGULAR)
        {

            handleLeftRegularVertex(v);
        }
        else if(v.vertexType==RIGHT_REGULAR)
        {

            handleRightRegularVertex(v);
        }
        else if(v.vertexType==END)
        {

            handleEndVertex(v);
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

    for(vector<point>::iterator it=newEdges.begin(); it!=newEdges.end(); it+=2)
    {
        point tr=*it;
        //cout<<tr.x<<" "<<tr.y<<" "<<endl;//tr.angle<<endl;
        point nx=*(it+1);

        glColor3f(0,0,1.0);
        glBegin(GL_LINES);
        {
            glVertex3f( nx.x,nx.y,0);
            glVertex3f(tr.x,tr.y,0);

        }
        glEnd();
    }

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


    for(set<point>::iterator it=T.begin();it!=T.end();it++)cout<<(*it).x<<" "<<(*it).y<<endl;



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

