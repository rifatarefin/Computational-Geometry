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
        c=atan2(b,a)*(180/pi);
        c>0 ? c:c+360;
        np.angle=c;
        points.push_back(np);
    }
}

int main()
{

    input();
    sort(points.begin(),points.end(),compareByAngle);
    for(vector<point>::iterator it=points.begin();it!=points.end();it++)
    {
        point tr=*it;
        cout<<tr.x<<" "<<tr.y<<" "<<tr.angle<<endl;
    }
}
