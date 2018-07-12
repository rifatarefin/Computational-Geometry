#define XX -99999
#include<bits/stdc++.h>
#include<algorithm>
#include<GL/glut.h>
using namespace std;
#define pi (2*acos(0.0))
ifstream fin("input.txt");

struct point
{
    double x,y;
    int lchild=-1,rchild=-1;
    int indx;
    double xmin=-9999,ymin=-9999;
    double xmax=9999, ymax=9999;
};

bool compareByY(const point &a, const point &b)
{
    return a.y < b.y;
}

bool compareByX(const point &a, const point &b)
{
    return a.x < b.x;
}
vector<point>points;



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
        np.indx=i;
        points.push_back(np);
    }
}


double dist(point a,point b)
{
    return (a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y);
}

int buildKDTree(vector<point>xpoints, vector<point>ypoints, int depth)
{
    if(xpoints.size()==1)return xpoints[0].indx;
    vector<point>baamx,daanx,baamy,daany;
    int order;
    if(depth%2==0)
    {
        int l=(xpoints.size()-1)/2;
        double x=xpoints[l].x;
        order=xpoints[l].indx;
        for(int i=0;i<=l;i++)
        {
            baamx.push_back(xpoints[i]);
        }
        for(int i=l+1;i<xpoints.size();i++)
        {
            daanx.push_back(xpoints[i]);
        }
        for(int i=0;i<ypoints.size();i++)
        {
            if(ypoints[i].x<=x)baamy.push_back(ypoints[i]);
            else daany.push_back(ypoints[i]);
        }

    }
    else
    {
        int l=(ypoints.size()-1)/2;
        double y=ypoints[l].y;
        order=ypoints[l].indx;
        for(int i=0;i<=l;i++)
        {
            baamy.push_back(ypoints[i]);
        }
        for(int i=l+1;i<ypoints.size();i++)
        {
            daany.push_back(ypoints[i]);
        }
        for(int i=0;i<xpoints.size();i++)
        {
            if(xpoints[i].y<=y)baamx.push_back(xpoints[i]);
            else daanx.push_back(xpoints[i]);
        }
    }
    int vleft=buildKDTree(baamx,baamy,depth+1);
    int vright=buildKDTree(daanx,daany,depth+1);
    points[order].lchild=vleft;
    points[order].rchild=vright;
    return points[order].indx;


}

void searchKDTree(int v, point a, point b)
{

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
        vector<point>xpoints=points;
        vector<point>ypoints=points;
        sort(xpoints.begin(),xpoints.end(),compareByX);
        sort(ypoints.begin(),ypoints.end(),compareByY);
        int root=buildKDTree(xpoints,ypoints,0);
        //cout<<points[root].rchild;
    cout<<endl;
    for(vector<point>::iterator it=points.begin();it!=points.end();it++)         //print
    {
        point tr=*it;
        cout<<tr.x<<" "<<tr.y<<" "<<tr.lchild<<" "<<tr.rchild<<endl;
    }cout<<endl;

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

