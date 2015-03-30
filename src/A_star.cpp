#include<vector>
#include "ros/ros.h"
#include<math.h>
#include "swarm_simulator/obstacleList.h"
#include "swarm_simulator/obstacleData.h"
#include<queue>
#include<stdio.h>
#include<stdlib.h>
#define gridx1 -12
#define gridx2 12
#define gridy1 -12
#define gridy2 12
#define maxx 100
#define maxy 100


typedef struct _pix
{
    double x;
    double y;
}pix;

class qi//CLASS FOR MEMBERS OF THE QUEUE
{
    public:
    pix p;
    double distance;//estimated total distance

};

struct comp//COMPARISON OPERATOR FOR THE PRIORITY QUEUE
 {
   bool operator()(const qi& l, const qi& r)
   {
       return l.distance > r.distance;
   }
 };

//DEFINING OPERATORS FOR PIX STRUCTURE
bool operator==(const pix& lhs, const pix& rhs)
    {
        return ((lhs.x==rhs.x)&&(lhs.y==rhs.y));
    }
bool operator!=(const pix& lhs, const pix& rhs)
    {
        return ((lhs.x!=rhs.x)||(lhs.y!=rhs.y));
    }


double dist(pix a, pix b)//CALCULATE DISTANCE BETWEEN a AND b
{
    return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}

//CHECK IF PIX P LIES ON AN OBSTACLE OR IF IT IS OUTSIDE THE GRID
int check(pix p,std::vector<swarm_simulator::obstacleData > obs )
{
    pix temp;
    if(p.x<0||p.x>maxx)
        return 0;
    if(p.y<0||p.y>maxy)
        return 0;
    for(std::vector<swarm_simulator::obstacleData>::iterator it=obs.begin();it!=obs.end();it++)
    {
        temp={(*it).x,(*it).y};
        if(dist(temp,p)<(*it).radius)
            return 0;
    }
    return 1;
}

//RETURNS A LINKED LIST OF THE REQUIRED INTERMEDIATE POINTS
std::list<pix > create(const std::vector<swarm_simulator::obstacleData > & obs,pix start, pix fin)
{
    int i,j,k,l;
    double temp,t2;
    qi curr,tempq;
    pix temppix;
    std::priority_queue< qi ,std::vector<qi> ,comp> q;
    std::vector<std::vector<pix> > parent(maxx+1);
    std::vector<std::vector<double> > distances(maxx+1);
    std::list<pix> sequence;
    std::vector<std::vector<int> > visited(maxx+1);

    for(i=0;i<=maxx;i++)
    {
        visited[i]=std::vector<int>(maxy+1);
        distances[i]=std::vector<double>(maxy+1);
        parent[i]=std::vector<pix>(maxy+1);
        for(j=0;j<=maxy;j++)
        {
            visited[i][j]=0;
            distances[i][j]=0;
        }
    }
    curr.p=start;
    curr.distance=dist(start,fin);
    distances[start.x][start.y]=0;
    q.push(curr);
//EXACT A-STAR ALGORITHM.
    while(!q.empty())
    {
        curr=q.top();
        q.pop();
        visited[curr.p.x][curr.p.y]=1;
        if(curr.p==fin)
        {
            break;
        }
        for(i=curr.p.x-1;i<=curr.p.x+1;i++)
        {
            for(j=curr.p.y-1;j<=curr.p.y+1;j++)
            {
                temppix={i,j};
                if(temppix==curr.p)
                    continue;
                if(check(temppix,obs)==1)
                {

                    t2=distances[curr.p.x][curr.p.y]+dist(temppix,curr.p);
                    temp=dist(temppix,fin)+t2;
                    if(visited[i][j]==1&&t2>=distances[i][j])
                        continue;
                    parent[i][j]=curr.p;
                    tempq.p=temppix;
                    tempq.distance=temp;
                    distances[i][j]=t2;
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
//SAMPLE OBSTACLE LIST
  pix start={20,20},fin={90,90};
  x[0].x=30;
  x[0].y=30;
  x[0].radius=10;
  x[1].x=45;
  x[1].y=15;
  x[1].radius=2;
  x[2].x=15;
  x[2].y=45;
  x[2].radius=2;
  sequence=create(x,start,fin);
  for(std::list<pix>::iterator it=sequence.begin();it!=sequence.end();it++)
  {
        printf("%lf %lf\n",(*it).x,(*it).y);
        }
  return 0;

}
