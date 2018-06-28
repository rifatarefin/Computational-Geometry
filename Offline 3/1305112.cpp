#define XX -99999
#include<bits/stdc++.h>
#include<algorithm>
#include<GL/glut.h>
using namespace std;
#define pi (2*acos(0.0))
ifstream fin("input2.txt");

struct point
{
    double x,y;

};

class triangle
{
public:
    point a,b,c;
    int idx;
    map<point,int>opp_triangle_idx;
    int child1=-1,child2=-1,child3=-1;
    point center;
    triangle(point x, point y, point z, int order)
    {
        a=x;
        b=y;
        c=z;
        idx=order;
        // opp_triangle_idx.clear();
        opp_triangle_idx[a]=-1;
        opp_triangle_idx[b]=-1;
        opp_triangle_idx[c]=-1;
    }
};

triangle tri_null({XX,XX}, {XX,XX}, {XX,XX},-1);
point p0= {-1500,1500};
point p1= {0,-3000};
point p2= {1500,1500};



vector<triangle>trlist;
vector<point>points;
void printTriangle(triangle x)
{
    cout<<"\n";
    cout<<x.a.x<<" "<<x.a.y<<"\n";
    cout<<x.b.x<<" "<<x.b.y<<"\n";
    cout<<x.c.x<<" "<<x.c.y<<"\n";
}

inline bool operator==(const point& lhs, const point& rhs)
{
    return lhs.x == rhs.x  && lhs.y==rhs.y ;
}

inline bool operator!=(const point& lhs, const point& rhs)
{
    return lhs.x != rhs.x  || lhs.y!=rhs.y ;
}


inline bool operator<(const point& lhs, const point& rhs)
{
    return lhs.x < rhs.x || ((lhs.x == rhs.x)&&(lhs.y<rhs.y));
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


double dist(point a,point b)
{
    return (a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y);
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

point getOppositePoint(triangle tr,point pr)
{
    triangle *tmp;
    tmp=&trlist[tr.opp_triangle_idx[pr]];


    point p;
    if(tmp->a!=tr.a && tmp->a!=tr.b && tmp->a!=tr.c)p=tmp->a;
    else if(tmp->b!=tr.a && tmp->b!=tr.b && tmp->b!=tr.c)p=tmp->b;
    else if(tmp->c!=tr.a && tmp->c!=tr.b && tmp->c!=tr.c)p=tmp->c;
    return p;
}


//{
void lineFromPoints(point P, point Q, double &a,
                    double &b, double &c)
{
    a = Q.y - P.y;
    b = P.x - Q.x;
    c = a*(P.x)+ b*(P.y);
}

void perpendicularBisectorFromLine(point P, point Q,
                                   double &a, double &b, double &c)
{
    point mid_point = {(P.x + Q.x)/2,
                       (P.y + Q.y)/2
                      };

    // c = -bx + ay
    c = -b*(mid_point.x) + a*(mid_point.y);

    double temp = a;
    a = -b;
    b = temp;
}

point lineLineIntersection(double a1, double b1, double c1,
                           double a2, double b2, double c2)
{
    double determinant = a1*b2 - a2*b1;
    point a;
    if (determinant == 0)
    {
        // The lines are parallel. This is simplified
        // by returning a pair of FLT_MAX

        return a= {FLT_MAX, FLT_MAX};
    }

    else
    {
        double x = (b2*c1 - b1*c2)/determinant;
        double y = (a1*c2 - a2*c1)/determinant;
        return a= {x, y};
    }
}

point findCircumCenter(point P, point Q, point R)
{
    // Line PQ is represented as ax + by = c
    double a, b, c;
    lineFromPoints(P, Q, a, b, c);

    // Line QR is represented as ex + fy = g
    double e, f, g;
    lineFromPoints(Q, R, e, f, g);

    // Converting lines PQ and QR to perpendicular
    // vbisectors. After this, L = ax + by = c
    // M = ex + fy = g
    perpendicularBisectorFromLine(P, Q, a, b, c);
    perpendicularBisectorFromLine(Q, R, e, f, g);

    // The point of intersection of L and M gives
    // the circumcenter
    point circumcenter =
        lineLineIntersection(a, b, c, e, f, g);
    return circumcenter;


}
//}
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
    if((i==p0 || i==p1 || i==p2) &&(j==p0 || j==p1 || j==p2))return false; //false=legal

    point l=getOppositePoint(tr,k);
    if((i==p0 || i==p1 || i==p2) || (j==p0 || j==p1 || j==p2) || (k==p0 || k==p1 || k==p2) || (l==p0 || l==p1 || l==p2))
    {
        //cout<<l.x<<" "<<l.y<<endl;
        if((k==p0 || k==p1 || k==p2) || (l==p0 || l==p1 || l==p2))return false;
        point center=findCircumCenter(tr.a,tr.b,tr.c);
        if(dist(center,l)>=dist(center,tr.a))return false;
        return true;
    }


    point center=findCircumCenter(tr.a,tr.b,tr.c);
    if(dist(center,l)<dist(center,tr.a))return true;
    return false;
    //return true;////
}

void update_opposite(triangle t1, int t2)
{
    if(t2==-1)
    {

        return;
    }

    triangle tmp=trlist[t2];
    if(tmp.a!=t1.a && tmp.a!=t1.b && tmp.a!=t1.c)tmp.opp_triangle_idx[tmp.a]=t1.idx;
    else if(tmp.b!=t1.a && tmp.b!=t1.b && tmp.b!=t1.c)tmp.opp_triangle_idx[tmp.b]=t1.idx;
    else if(tmp.c!=t1.a && tmp.c!=t1.b && tmp.c!=t1.c)tmp.opp_triangle_idx[tmp.c]=t1.idx;
    trlist[t2]=tmp;
}

void legalize(triangle tr, point pr)
{
    if(checkIllegal(tr,pr)==false)return;
//    cout<<"legalizing "<<pr.x<<" "<<pr.y<<endl;
//    printTriangle(tr);
//    point f,g,h;
//    f=getOppositePoint(tr,tr.a);
//    g=getOppositePoint(tr,tr.b);
//    h=getOppositePoint(tr,tr.c);
//    cout<<"opposite"<<endl;
//    cout<<f.x<<" "<<f.y<<endl;
//    cout<<g.x<<" "<<g.y<<endl;
//    cout<<h.x<<" "<<h.y<<endl;



    point l=getOppositePoint(tr,pr);

    triangle tr2=trlist[tr.opp_triangle_idx[pr]];
    point i,j,k;

    if(pr==tr.a)
    {
        i=tr.b;
        j=tr.c;
        k=tr.a;
    }
    else if(pr==tr.b)
    {
        i=tr.c;
        j=tr.a;
        k=tr.b;
    }
    else if(pr==tr.c)
    {
        i=tr.a;
        j=tr.b;
        k=tr.c;
    }

    int len=trlist.size();
    triangle t1(k,l,j,len);
    triangle t2(k,i,l,len+1);

    t1.opp_triangle_idx[l]=tr.opp_triangle_idx[i];
    t1.opp_triangle_idx[j]=t2.idx;
    t1.opp_triangle_idx[k]=tr2.opp_triangle_idx[i];

    update_opposite(t1,tr.opp_triangle_idx[i]);
    update_opposite(t1,tr2.opp_triangle_idx[i]);

    t2.opp_triangle_idx[i]=t1.idx;
    t2.opp_triangle_idx[l]=tr.opp_triangle_idx[j];
    t2.opp_triangle_idx[k]=tr2.opp_triangle_idx[j];

    update_opposite(t2,tr.opp_triangle_idx[j]);
    update_opposite(t2,tr2.opp_triangle_idx[j]);

    trlist[tr.idx].child1=len;
    trlist[tr.idx].child2=len+1;

    trlist[tr2.idx].child1=len;
    trlist[tr2.idx].child2=len+1;

    trlist.push_back(t1);
    trlist.push_back(t2);

    legalize(trlist[t1.idx],k);
    legalize(trlist[t2.idx],k);
    //checkIllegal(i,j,zr)
}

void splitA(triangle t0, point z1)
{
    int len=trlist.size();
    triangle t1(t0.a,t0.b,z1,len);
    triangle t2(t0.b,t0.c,z1,len+1);
    triangle t3(t0.c,t0.a,z1,len+2);

    t1.opp_triangle_idx[t1.a]=t2.idx;
    t1.opp_triangle_idx[t1.b]=t3.idx;
    t1.opp_triangle_idx[t1.c]=t0.opp_triangle_idx[t0.c];
    update_opposite(t1,t0.opp_triangle_idx[t0.c]);

    t2.opp_triangle_idx[t2.a]=t3.idx;
    t2.opp_triangle_idx[t2.b]=t1.idx;
    t2.opp_triangle_idx[t2.c]=t0.opp_triangle_idx[t0.a];
    update_opposite(t2,t0.opp_triangle_idx[t0.a]);

    t3.opp_triangle_idx[t3.a]=t1.idx;
    t3.opp_triangle_idx[t3.b]=t2.idx;
    t3.opp_triangle_idx[t3.c]=t0.opp_triangle_idx[t0.b];
    update_opposite(t3,t0.opp_triangle_idx[t0.b]);

    trlist[t0.idx].child1=len;
    trlist[t0.idx].child2=len+1;
    trlist[t0.idx].child3=len+2;
    trlist.push_back(t1);
    trlist.push_back(t2);
    trlist.push_back(t3);
    legalize(trlist[t1.idx],z1);
    legalize(trlist[t2.idx],z1);
    legalize(trlist[t3.idx],z1);
    //Tlist.erase(remove(Tlist.begin(),Tlist.end(),t0),Tlist.end());
//    vector<T>::iterator it=find(Tlist.begin(),Tlist.end(),t0);
//    if(it!=Tlist.end())Tlist.erase(it);
}

void splitB(triangle tr,point pr)
{
    point i,j,k,l;
    if(checkTurn(tr.a,tr.b,pr)==0)
    {
        i=tr.a;
        j=tr.b;
        k=tr.c;
    }
    else if(checkTurn(tr.b,tr.c,pr)==0)
    {
        i=tr.b;
        j=tr.c;
        k=tr.a;
    }
    else if(checkTurn(tr.c,tr.a,pr)==0)
    {
        i=tr.c;
        j=tr.a;
        k=tr.b;
    }
    l=getOppositePoint(tr,k);
    triangle tr2=trlist[tr.opp_triangle_idx[k]];
    int len=trlist.size();
    triangle t1(k,pr,j,len);
    triangle t2(k,i,pr,len+1);
    triangle t3(pr,l,j,len+2);
    triangle t4(pr,i,l,len+3);

    t1.opp_triangle_idx[k]=t3.idx;
    t1.opp_triangle_idx[j]=t2.idx;
    t1.opp_triangle_idx[pr]=tr.opp_triangle_idx[i];
    update_opposite(t1,tr.opp_triangle_idx[i]);

    t2.opp_triangle_idx[k]=t4.idx;
    t2.opp_triangle_idx[i]=t1.idx;
    t2.opp_triangle_idx[pr]=tr.opp_triangle_idx[j];
    update_opposite(t2,tr.opp_triangle_idx[j]);

    t3.opp_triangle_idx[j]=t4.idx;
    t3.opp_triangle_idx[l]=t1.idx;
    t3.opp_triangle_idx[pr]=tr2.opp_triangle_idx[i];
    update_opposite(t3,tr2.opp_triangle_idx[i]);

    t4.opp_triangle_idx[l]=t2.idx;
    t4.opp_triangle_idx[i]=t3.idx;
    t4.opp_triangle_idx[pr]=tr2.opp_triangle_idx[j];
    update_opposite(t4,tr2.opp_triangle_idx[j]);

    trlist[tr.idx].child1=len;
    trlist[tr.idx].child2=len+1;
    trlist[tr2.idx].child1=len+2;
    trlist[tr2.idx].child2=len+4;
    trlist.push_back(t1);
    trlist.push_back(t2);
    trlist.push_back(t3);
    trlist.push_back(t4);



//    printTriangle(t1);
//    printTriangle(t2);
//    printTriangle(t3);
//    printTriangle(t4);
//    cout<<checkIllegal(t1,pr)<<endl;
    legalize(trlist[t1.idx],pr);
    legalize(trlist[t2.idx],pr);
    legalize(trlist[t3.idx],pr);
    legalize(trlist[t4.idx],pr);

}

//{

bool containCorner(triangle t)
{
    if(t.a==p0 || t.b==p0 || t.c==p0)return true;
    else if(t.a==p1 || t.b==p1 || t.c==p1)return true;
    else if(t.a==p2 || t.b==p2 || t.c==p2)return true;
    return false;
}
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
    case GLUT_KEY_DOWN:
        drawaxes=1-drawaxes;

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
            if(t.a==p0 || t.b==p0 || t.c==p0)continue;
            else if(t.a==p1 || t.b==p1 || t.c==p1)continue;
            else if(t.a==p2 || t.b==p2 || t.c==p2)continue;
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

            for(vector<triangle>::iterator it=trlist.begin(); it!=trlist.end(); it++)        //print
        {

            triangle t=(*it);
            if(t.child1!=-1)continue;
            if(t.a==p0 || t.b==p0 || t.c==p0)continue;
            else if(t.a==p1 || t.b==p1 || t.c==p1)continue;
            else if(t.a==p2 || t.b==p2 || t.c==p2)continue;
            glColor3f(0,1.0,0);
            triangle x1=trlist[t.opp_triangle_idx[t.a]];
            if(containCorner(x1)==false)
            {
                glBegin(GL_LINES);
                {
                    glVertex3f( t.center.x,t.center.y,0);
                    glVertex3f(x1.center.x,x1.center.y,0);

                }
                glEnd();
            }
            else
            {
                double vx1=t.c.x-t.b.x;
                double vy1=t.c.y-t.b.y;
                glBegin(GL_LINES);
                {
                    glVertex3f( t.center.x,t.center.y,0);
                    glVertex3f(t.center.x+vy1,t.center.y-vx1,0);

                }
                glEnd();
            }

            triangle x2=trlist[t.opp_triangle_idx[t.b]];
            if(containCorner(x2)==false)
            {
                glBegin(GL_LINES);
                {
                    glVertex3f( t.center.x,t.center.y,0);
                    glVertex3f(x2.center.x,x2.center.y,0);

                }
                glEnd();
            }
            else
            {
                double vx1=t.a.x-t.c.x;
                double vy1=t.a.y-t.c.y;
                glBegin(GL_LINES);
                {
                    glVertex3f( t.center.x,t.center.y,0);
                    glVertex3f(t.center.x+vy1,t.center.y-vx1,0);

                }
                glEnd();
            }

            triangle x3=trlist[t.opp_triangle_idx[t.c]];
            if(containCorner(x3)==false)
            {
                glBegin(GL_LINES);
                {
                    glVertex3f( t.center.x,t.center.y,0);
                    glVertex3f(x3.center.x,x3.center.y,0);

                }
                glEnd();
            }
            else
            {
                double vx1=t.b.x-t.a.x;
                double vy1=t.b.y-t.a.y;
                glBegin(GL_LINES);
                {
                    glVertex3f( t.center.x,t.center.y,0);
                    glVertex3f(t.center.x+vy1,t.center.y-vx1,0);

                }
                glEnd();
            }

        }
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

            triangle tq=trlist[0];
            while(true)
            {
                if(tq.child1==-1)break;

                if(position(trlist[tq.child1],z1)>0)
                    tq=trlist[tq.child1];

                else if(position(trlist[tq.child2],z1)>0)
                    tq=trlist[tq.child2];

                else tq=trlist[tq.child3];
            }
            if(position(tq,z1)==1)
                splitA(tq,z1);

            else if(position(tq,z1)==2)
                splitB(tq,z1);

            printTriangle(tq);

        }
        for(vector<triangle>::iterator it=trlist.begin(); it!=trlist.end(); it++)        //print
        {

            triangle t=(*it);
            if(t.child1!=-1)continue;
            point p=findCircumCenter(t.a,t.b,t.c);
            (*it).center=p;
//            printTriangle(t);
//            cout<<"opposite"<<endl;
//            point f,g,h;
//            f=getOppositePoint(t,t.a);
//            g=getOppositePoint(t,t.b);
//            h=getOppositePoint(t,t.c);
//            cout<<f.x<<" "<<f.y<<endl;
//            cout<<g.x<<" "<<g.y<<endl;
//            cout<<h.x<<" "<<h.y<<endl;
//            if(t.opp_triangle_idx[t.a]!=-1)printTriangle(trlist[t.opp_triangle_idx[t.a]]);
//            if(t.opp_triangle_idx[t.b]!=-1)printTriangle(trlist[t.opp_triangle_idx[t.b]]);
//            if(t.opp_triangle_idx[t.c]!=-1)printTriangle(trlist[t.opp_triangle_idx[t.c]]);

//        cout<<"*****\n";

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

