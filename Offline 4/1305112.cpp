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
    int indx,depth;
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
point a,b,query,nn;
int cnt=0;
int root;
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
    return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}

void reportSubTree(int indx)
{
    point v=points[indx];
    if(v.lchild==-1 && v.rchild==-1)
    {
        cout<<v.x<<" "<<v.y<<endl;
        return;
    }
    reportSubTree(v.lchild);
    reportSubTree(v.rchild);

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
    point np=points[order];
    np.lchild=vleft;
    np.rchild=vright;
    np.depth=depth;
    np.indx=points.size();
    points.push_back(np);
    return np.indx;


}

void region(int root,int depth)
{
    point q=points[root];


    if(q.lchild==-1 && q.rchild==-1)return;
    if(depth%2==0)
    {
        points[q.lchild].xmax=q.x;
        points[q.lchild].xmin=q.xmin;
        points[q.rchild].xmin=q.x;
        points[q.rchild].xmax=q.xmax;
    }
    else
    {
        points[q.lchild].ymax=q.y;
        points[q.lchild].ymin=q.ymin;
        points[q.rchild].ymin=q.y;
        points[q.lchild].ymax=q.ymax;
    }
    region(q.lchild,depth+1);
    region(q.rchild,depth+1);
}
void searchKDTree(int v, point a, point b)
{
    point qr=points[v];
    if(qr.lchild==-1 && qr.rchild==-1)
    {
        if((qr.x>=a.x && qr.x<=b.x) && (qr.y>=a.y && qr.y<=b.y))
        {
            cout<<"("<<qr.x<<" "<<qr.y<<")"<<endl;
            cnt++;
        }
        return;
    }
    if(qr.xmin>=b.x || qr.xmax<a.x || qr.ymin>=b.y || qr.ymax<a.y)return;
    point l=points[qr.lchild];
    point r=points[qr.rchild];
    if((l.xmin>a.x && l.xmax<=b.x) && (l.ymin>a.y && l.ymax<=b.y))
    {
        reportSubTree(l.indx);
        return;
    }
    else
    {
        searchKDTree(l.indx,a,b);
    }
    if((r.xmin>a.x && r.xmax<=b.x) && (r.ymin>a.y && r.ymax<=b.y))
    {
        reportSubTree(r.indx);
        return;
    }
    else
    {
        searchKDTree(r.indx,a,b);
    }


}

point nearestNeighbor(int root, point query)
{
    point tp=points[root];
    if(tp.lchild==-1 && tp.rchild==-1)
    {
        //cout<<"************"<<tp.x<<" "<<tp.y<<endl;
        return tp;
    }
    if(tp.depth%2==0)
    {

        if(query.x<=tp.x)
        {
            point p1=nearestNeighbor(tp.lchild,query);
            point p2;
            double d=points[tp.rchild].x-query.x;
            double dst1=dist(p1,query);
            if(d<=dst1)
            {
                p2=nearestNeighbor(tp.rchild,query);
            }
            point best=dst1<=dist(p2,query)?p1:p2;
            return best;
        }
        else
        {
            point p1=nearestNeighbor(tp.rchild,query);
            point p2;
            double d=query.x-points[tp.lchild].x;
            double dst1=dist(p1,query);
            if(d<=dst1)
            {
                p2=nearestNeighbor(tp.lchild,query);
            }
            point best=dst1<=dist(p2,query)?p1:p2;
            return best;
        }
    }
    else
    {
        if(query.y<=tp.y)
        {
            point p1=nearestNeighbor(tp.lchild,query);
            point p2;
            double d=points[tp.rchild].y-query.y;
            double dst1=dist(p1,query);
            if(d<=dst1)
            {
                p2=nearestNeighbor(tp.rchild,query);
            }
            point best=dst1<=dist(p2,query)?p1:p2;
            return best;
        }
        else
        {
            point p1=nearestNeighbor(tp.rchild,query);
            point p2;
            double d=query.y-points[tp.lchild].y;
            double dst1=dist(p1,query);
            if(d<=dst1)
            {
                p2=nearestNeighbor(tp.lchild,query);
            }
            point best=dst1<=dist(p2,query)?p1:p2;
            return best;
        }
    }
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
void DrawCircle(float cx, float cy, float r, int num_segments)
{
	glBegin(GL_LINE_LOOP);
	for(int ii = 0; ii < num_segments; ii++)
	{
		float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle

		float x = r * cosf(theta);//calculate the x component
		float y = r * sinf(theta);//calculate the y component

		glVertex2f(x + cx, y + cy);//output vertex

	}
	glEnd();
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
        string s;
        fin>>s;
        if(s=="R")
        {
            fin>>a.x>>a.y>>b.x>>b.y;
            cnt=0;
            searchKDTree(root,a,b);
            cout<<cnt<<endl;
        }
        else if(s=="N")
        {
            fin>>query.x>>query.y;

            nn=nearestNeighbor(root,query);
            cout<<dist(query,nn)<<"("<<nn.x<<" "<<nn.y<<")"<<endl;
        }

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
    glColor3f(1, 0, 0);

    glBegin(GL_POINTS);
    {

        glVertex3f(query.x,query.y,0);

    }
    glEnd();

    if(draw1==1)
    {
        glColor3f(0, 1.0, 0);
        glBegin(GL_LINES);
        {
            glVertex3f( a.x,a.y,0);
            glVertex3f(b.x,a.y,0);

            glVertex3f(b.x,a.y,0);
            glVertex3f(b.x,b.y,0);

            glVertex3f(b.x,b.y, 0);
            glVertex3f(a.x,b.y,0);

            glVertex3f(a.x,b.y, 0);
            glVertex3f(a.x,a.y,0);
        }
        glEnd();
    }
    if(draw2==1)
    {
        glColor3f(1, 0, 0);
        DrawCircle(query.x,query.y,dist(query,nn),100);
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
        a.x=-100,a.y=-100,b.x=-200,b.y=-200,query.x=-200,query.y=-200,nn.x=-200,nn.y=-200;

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
        root=buildKDTree(xpoints,ypoints,0);
        //cout<<root<<endl;
        //reportSubTree(root);
        region(root,0);


//        cout<<endl;
//        for(vector<point>::iterator it=points.begin();it!=points.end();it++)         //print
//        {
//            point tr=*it;
//            cout<<tr.x<<" "<<tr.y<<" "<<tr.lchild<<" "<<tr.rchild<<" "<<tr.xmin<<" "<<tr.xmax<<" "<<tr.ymin<<" "<<tr.ymax<<endl;
//        }cout<<endl;

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

