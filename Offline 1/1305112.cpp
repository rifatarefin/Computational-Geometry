#include<bits/stdc++.h>
using namespace std;
#define pi (2*acos(0.0))
ifstream fin("in.txt");

struct point
{
    int x,y;
    double angle;
};

inline bool operator<(const point& lhs, const point& rhs)
{
    return lhs.x != rhs.x || lhs.y!=rhs.y;
}

bool compareByAngle(const point &a, const point &b)
{
    return a.angle < b.angle;
}
point bottom;
vector<point>points;
vector<point>hull;
set<point>con;

void input()
{
    int n, a, b, c;
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

int checkTurn(point z)
{
    int len=hull.size();
    point b=hull[len-1];
    point a=hull[len-2];
    int v1x=b.x-a.x;
    int v1y=b.y-a.y;
    int v2x=z.x-a.x;
    int v2y=z.y-a.y;
    return v1x*v2y-v1y*v2x;


}

bool checkBoundery(point z, vector<point>v)
{
    for(vector<point>::iterator it=v.begin()+1; it!=v.end(); it++)
    {
        point a=*(it-1);
        point b=*it;
        int v1x=b.x-a.x;
        int v1y=b.y-a.y;
        int v2x=z.x-a.x;
        int v2y=z.y-a.y;

        if(v1x*v2y-v1y*v2x<0)
        {
            return false;
        }
        if(it==v.end()-1)
        {
            b=*(v.begin());
            a=*it;
            int v1x=b.x-a.x;
            int v1y=b.y-a.y;
            int v2x=z.x-a.x;
            int v2y=z.y-a.y;

            if(v1x*v2y-v1y*v2x<0)
            {
                return false;
            }
        }

    }
    //cout<<"true "<<z.x<<" "<<z.y<<endl;
    return true;
}

int main()
{

    input();
    int len;
    point top,rt,lf;
    top.y=INT_MIN;
    top.x=INT_MAX;
    bottom.y=INT_MAX;
    bottom.x=INT_MIN;
    lf.x=INT_MAX;
    lf.y=INT_MAX;
    rt.x=INT_MIN;
    rt.y=INT_MIN;
    len=points.size();

    for(int i=0; i<len; i++)
    {
        if(points[i].x<=lf.x)
        {
            if(points[i].x<lf.x || points[i].y<lf.y)lf=points[i];
        }
        if(points[i].x>=rt.x)
        {
            if(points[i].x>rt.x || points[i].y>rt.y)rt=points[i];
        }
        if(points[i].y>=top.y)
        {
            if(points[i].y>top.y || points[i].x<top.x)top=points[i];
        }
        if(points[i].y>top.y || points[i].y<=bottom.y)
        {
            if(points[i].x>bottom.x)bottom=points[i];
        }

    }


    con.insert(top);
    con.insert(bottom);
    con.insert(lf);
    con.insert(rt);


    if(con.size()>2)
    {
        vector<point>v;
        v.push_back(bottom);
        v.push_back(rt);
        v.push_back(top);
        v.push_back(lf);

        for(vector<point>::iterator it=points.begin(); it!=points.end(); )
        {
            if(checkBoundery(*it,v)==true)
            {
                points.erase(it);
            }
            else it++;
        }
        points.push_back(bottom);
        points.push_back(rt);
        points.push_back(top);
        points.push_back(lf);


    }

    len=points.size();

    for(int i=0; i<len; i++)
    {
        points[i].angle=angle(points[i]);
    }
    sort(points.begin(),points.end(),compareByAngle);
    points.erase(points.begin()+len-1);



    for(vector<point>::iterator it=points.begin()+1; it!=points.end();)
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


    hull.push_back(bottom);
    hull.push_back(points[0]);
    points.erase(points.begin());

    for(vector<point>::iterator it=points.begin(); it!=points.end(); it++)
    {
        int turn=checkTurn(*it);
        if(turn>0)hull.push_back(*it);
        while(turn<0)
        {
            hull.pop_back();
            turn=checkTurn(*it);
            if(turn>0)hull.push_back(*it);
        }
    }
    cout<<"Hull size: "<<hull.size()<<endl<<endl;

    for(vector<point>::iterator it=hull.begin(); it!=hull.end(); it++)
    {
        point tr=*it;
        cout<<tr.x<<" "<<tr.y<<" "<<endl;//tr.angle<<endl;
    }
}


//    for(vector<point>::iterator it=points.begin();it!=points.end();it++)         //print
//    {
//        point tr=*it;
//        cout<<tr.x<<" "<<tr.y<<" "<<tr.angle<<endl;
//    }

//
//cout<<bottom.x<<" "<<bottom.y<<endl;                           //prinnt
//    cout<<rt.x<<" "<<rt.y<<endl;
//    cout<<top.x<<" "<<top.y<<endl;
//    cout<<lf.x<<" "<<lf.y<<endl;
