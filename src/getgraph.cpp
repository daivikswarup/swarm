#include<vector>
#include "ros/ros.h"
#include<math.h>
#include "swarm_simulator/obstacleList.h"
#include "swarm_simulator/obstacleData.h"
#define gridx1 -12
#define gridx2 12
#define gridy1 -12
#define gridy2 12
#define maxx 1000
#define maxy 1000
typedef struct _pix
{
    int x;
    int y;
}pix;
class qi
{
    public:
    pix p;
    double distance;//estimated total distance
    bool operator<(const pix& rhs) const
    {
        return distance < rhs.distance;
    }
};
/*class graph
{
    public:
    //int create();
    static std::vector<pix> create(const swarm_simulator::obstacleList & obs,pix start, pix fin);
};*/
double dist(pix a, pix b)
{
    return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}
int check(pix p,const obstacleList & obs )
{
    pix temp;
    if(p.x<0||p.x>maxx)
        return 0;
    if(p.y<0||p.y>maxy)
        return 0;
    for(vector<swarm_simulator::obstacleList>::iterator it=obs.begin();it!=obs.end();it++)
    {
        temp={(*it).x,(*it).y};
        if(dist(temp,p)<(*it).radius)
            return 0;
    }
    return 1;
}
std::vector<pix> create(const swarm_simulator::obstacleList & obs,pix start, pix fin)
{
    int i,j,k,l;
    double temp;
    qi curr,tempq;
    pix temppix;
    std::priority_queue<qi> q;
    std::vector<std::vector<pix> > parent;
    std::vector<std::vector<double> > distances;
    std::list<pix> sequence;
    std::vector<std::vector<int> > visited(maxy);
    for(i=0;i<maxy;i++)
    {
        visited[i]=std::vector<int>(maxx);
        distances[i]=std::vector<double>(maxx);
        for(j=0;j<maxx;j++)
        {
            visited[i][j]=0;
            distances[i][j]=0;
        }
    }
    curr.p=start;
    curr.distance=dist(start,fin);
    distances[start.x][start.y]=curr.distance;
    q.push(curr);
    while(!q.empty())
    {
        curr=q.pop();
        visited[curr.p.x][curr.p.y]=1;
        if(curr.p==fin)
        {
            break;
        }
        for(i=curr.p.x-1;i<=curr.p.x+1;i++)
        {
            for(j=curr.p.y-1;j<=purr.p.y+1;j++)
            {
                temppix={i,j};
                if(check(temppix,obs)==1)
                {
                    parent[i][j]=curr.p;
                    temp=dist(temppix,fin)+curr.distance+dist(temppix,curr.p);
                    if(visited[i][j]==1&&temp>=distances[i][j])
                        continue;
                    tempq.p=temppix;
                    tempq.distance=temp;
                    distances[i][j]=temp;
                    q.push(tempq);
                }
            }
        }
    }
    temppix=fin;
    while(temppix!=start)
    {
        sequence.push_front(temppix);
        temppix=parent[temppix.x][temppix.y];
    }
    return sequence;

}
int main(int argc, char **argv)
{

  ros::init(argc, argv, "Bullshit");
  std::vector<swarm_simulator::obstacleData > x(3);
  std::list<pix> sequence;
  pix start={1,1},fin={100,100};
  x[0].x=40;
  x[0].y=40;
  x[0].radius=5;
  x[1].x=60;
  x[1].y=13;
  x[1].radius=7;
  x[2].x=70;
  x[2].y=90;
  x[2].radius=5;
  sequence=create(x,start,fin);
  for(std::list<pix>::iterator it=sequence.begin;it!=sequence.end();it++)
  {
        printf("%lf\t%lf\n",(*it).x,(*it).y);
        }
  return 0;

}
