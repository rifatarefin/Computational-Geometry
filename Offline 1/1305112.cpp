#include<bits/stdc++.h>
using namespace std;
#define pi (2*acos(0.0))
ifstream fin("in.txt");

struct point
{
    int x,y;
    double angle;
};

bool compareByAngle(const point &a, const point &b)
{
    return a.angle < b.angle;
}
point bottom;
vector<point>points;

void input()
{
    int n, a, b, c;
    point np;
    fin>>n;
    for(int i=0;i<n;i++)
    {
        fin>>a>>b;
        np.x=a;
        np.y=b;
//        c=atan2(b,a)*(180/pi);
//        c>0 ? c:c+360;
//        np.angle=c;
        points.push_back(np);
    }
}

double angle(point a)
{
    point q;

    q.x=a.x-bottom.x;
    q.y=a.y-bottom.y;

    double ang = atan2(q.y,q.x)*(180/pi);
    return ang>0 ? ang:ang+360;
}

double dist(point a)
{
    return (a.x-bottom.x)*(a.x-bottom.x)+(a.y-bottom.y)*(a.y-bottom.y);
}

int main()
{

    input();
    int len;
    point top,rt,lf;
    top.y=INT_MIN;
    bottom.y=INT_MAX;
    lf.x=INT_MAX;
    rt.x=INT_MIN;
    len=points.size();

    for(int i=0;i<len;i++)
    {
        if(points[i].x<lf.x)lf=points[i];
        if(points[i].x>rt.x)rt=points[i];
        if(points[i].y>top.y)top=points[i];
        if(points[i].y<=bottom.x)
        {
            if(points[i].x>bottom.x)bottom=points[i];
        }

    }
    for(int i=0;i<len;i++)
    {
        points[i].angle=angle(points[i]);
    }
    sort(points.begin(),points.end(),compareByAngle);
    points.erase(points.begin()+len-1);
    len--;

    for(vector<point>::iterator it=points.begin()+1;it!=points.end();)
    {
        point a=*(it-1);
        point b=*it;

        if(a.angle==b.angle)
        {
            double w=dist(a);
            double z=dist(b);
            if(w<z)it=points.erase(it-1)+1;
            else it=points.erase(it);
        }
        else it++;
    }

    for(vector<point>::iterator it=points.begin();it!=points.end();it++)
    {
        point tr=*it;
        cout<<tr.x<<" "<<tr.y<<" "<<tr.angle<<endl;
    }
}
