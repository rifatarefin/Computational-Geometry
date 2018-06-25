#define XX -99999
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

class triangle
{public:
    point a,b,c;
    int idx;
    int opp_a=-1,opp_b=-1,opp_c=-1;
    int child1=-1,child2=-1,child3=-1;
    triangle(point x, point y, point z, int order)
    {
        a=x;
        b=y;
        c=z;
        idx=order;
    }
};

triangle tri_null({XX,XX},{XX,XX},{XX,XX},-1);
point p0= {-15,15};
point p1= {0,-30};
point p2= {15,15};

//class T
//{
//public:
//    triangle core;
//    //point aprime={XX,XX},bprime={XX,XX},cprime={XX,XX};
//    triangle atr=nullTr,btr=nullTr,ctr=nullTr;
//
//    T(point a, point b, point c)
//    {
//        core= {a,b,c};
//    }
//};

vector<triangle>trlist;
vector<point>points;
void printTriangle(triangle x)
{
    cout<<"\n";
    cout<<x.a.x<<" "<<x.a.y<<"\n";
    cout<<x.b.x<<" "<<x.b.y<<"\n";
    cout<<x.c.x<<" "<<x.c.y<<"\n";
    cout<<x.opp_a<<" "<<x.opp_b<<" "<<x.opp_c<<"\n"<<x.idx<<"\n\n";
}

inline bool operator==(const point& lhs, const point& rhs)
{
    return lhs.x == rhs.x  && lhs.y==rhs.y ;
}

inline bool operator!=(const point& lhs, const point& rhs)
{
    return lhs.x != rhs.x  || lhs.y!=rhs.y ;
}

inline bool operator==(const triangle& lhs, const triangle& rhs)
{
    return lhs.a==rhs.a && lhs.b==rhs.b && lhs.c==rhs.c ;
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

int position(triangle t, point p)
{
    if(checkTurn(t.a,t.b,p)>0 && checkTurn(t.b,t.c,p)>0 && checkTurn(t.c,t.a,p)>0)return 1;
    else if(checkTurn(t.a,t.b,p)>=0 && checkTurn(t.b,t.c,p)>=0 && checkTurn(t.c,t.a,p)>=0)return 2;
    else return -1;

}

void update_opposite(triangle t1, int t2)
{
    if(t2==-1)return;
    cout<<"ttttt2"<<t2<<endl;
    triangle tmp=trlist[t2];
    if(tmp.a!=t1.a && tmp.a!=t1.b && tmp.a!=t1.c)tmp.opp_a=t1.idx;
    else if(tmp.b!=t1.a && tmp.b!=t1.b && tmp.b!=t1.c)tmp.opp_b=t1.idx;
    else if(tmp.c!=t1.a && tmp.c!=t1.b && tmp.c!=t1.c)tmp.opp_c=t1.idx;
    trlist[t2]=tmp;
}

point getOpposite(triangle tr,point pr)
{
    triangle *tmp=nullptr;
    if(pr==tr.a)*tmp=trlist[tr.opp_a];
    else if(pr==tr.b)*tmp=trlist[tr.opp_b];
    else if(pr==tr.c)*tmp=trlist[tr.opp_c];

    point p;
    if(tmp->a!=tr.a && tmp->a!=tr.b && tmp->a!=tr.c)p=tmp->a;
    else if(tmp->b!=tr.a && tmp->b!=tr.b && tmp->b!=tr.c)p=tmp->b;
    else if(tmp->c!=tr.a && tmp->c!=tr.b && tmp->c!=tr.c)p=tmp->c;
    return p;
}

bool checkIllegal(triangle tr, point k)    //true=illegal
{

    point i,j;
    if(k==tr.a)
    {
        i=tr.b;
        j=tr.c;
    }
    else if(k==tr.b)
    {

        i=tr.c;
        j=tr.a;
    }
    else if(k==tr.c)
    {
        i=tr.a;
        j=tr.b;
    }
    if((i==p0 || i==p1 || i==p2) &&(j==p0 || j==p1 || j==p2))return false;

    point l=getOpposite(tr,k);

}
void legalize(triangle tr, point k)
{


    //checkIllegal(i,j,zr)
}
void splitA(triangle t0, point z1)
{
    int len=trlist.size();
    triangle t1(t0.a,t0.b,z1,len);
    triangle t2(t0.b,t0.c,z1,len+1);
    triangle t3(t0.c,t0.a,z1,len+2);

    t1.opp_a=t2.idx;
    t1.opp_b=t3.idx;
    t1.opp_c=t0.opp_c;
    update_opposite(t1,t0.opp_c);

    t2.opp_a=t3.idx;
    t2.opp_b=t1.idx;
    t2.opp_c=t0.opp_a;
    update_opposite(t2,t0.opp_a);

    t3.opp_a=t1.idx;
    t3.opp_b=t2.idx;
    t3.opp_c=t0.opp_b;
    update_opposite(t3,t0.opp_b);

    trlist[t0.idx].child1=len;
    trlist[t0.idx].child2=len+1;
    trlist[t0.idx].child3=len+2;
    trlist.push_back(t1);
    trlist.push_back(t2);
    trlist.push_back(t3);
    //Tlist.erase(remove(Tlist.begin(),Tlist.end(),t0),Tlist.end());
//    vector<T>::iterator it=find(Tlist.begin(),Tlist.end(),t0);
//    if(it!=Tlist.end())Tlist.erase(it);
}



//{

int drawaxes,draw1=1,draw2=1;

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

void specialKeyListener(int key, int x,int y)
{
    switch(key)
    {

    case GLUT_KEY_RIGHT:

        draw1=1-draw1;
        break;
    case GLUT_KEY_LEFT:
        draw2=1-draw2;

        break;
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



    if(draw1==1)
    {
        for(vector<triangle>::iterator it=trlist.begin(); it!=trlist.end(); it++)        //print
        {

            triangle t=(*it);
            if(t.child1!=-1)continue;
            glColor3f(1.0,0,0);
        glBegin(GL_LINES);
        {
            glVertex3f( t.a.x,t.a.y,0);
            glVertex3f(t.b.x,t.b.y,0);

            glVertex3f( t.b.x,t.b.y,0);
            glVertex3f(t.c.x,t.c.y,0);

            glVertex3f( t.c.x,t.c.y,0);
            glVertex3f(t.a.x,t.a.y,0);

        }
        glEnd();

        }
    }

    if(draw2==1)
    {
        ;
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
//}

int main(int argc, char **argv)
{


    input();


    triangle t0(p0,p1,p2,0);
    trlist.push_back(t0);
//    point z1=points[0];
//
//    splitA(t0,z1);

    int len=points.size();
    for(int i=0; i<len; i++)

    {
        point z1=points[i];
        //cout<<Tlist.size()<<"**********"<<endl;
        //T tmp;
        for(vector<triangle>::iterator it=trlist.begin(); it!=trlist.end(); it++)        //print
        {
            triangle t=(*it);
            if(t.child1!=-1)continue;
            if(position(t,z1)==1)
            {
                //printTriangle(t);
                splitA(t,z1);
                break;
            }

        }


    }
    for(vector<triangle>::iterator it=trlist.begin(); it!=trlist.end(); it++)        //print
        {

            triangle t=(*it);
            if(t.child1!=-1)continue;
            printTriangle(t);
            if(t.opp_a!=-1)printTriangle(trlist[t.opp_a]);
            if(t.opp_b!=-1)printTriangle(trlist[t.opp_b]);
            if(t.opp_c!=-1)printTriangle(trlist[t.opp_c]);
            cout<<"*****\n";

        }


    glutInit(&argc,argv);
    glutInitWindowSize(700, 600);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

    glutCreateWindow("My OpenGL Program");

    init();

    glEnable(GL_DEPTH_TEST);	//enable Depth Testing

    glutDisplayFunc(display);	//display callback function
    glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)
    glutSpecialFunc(specialKeyListener);

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

