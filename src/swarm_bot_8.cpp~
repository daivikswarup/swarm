#include "ros/ros.h"
#include <math.h>
#include "geometry_msgs/Twist.h"
#include "std_msgs/String.h"
#include "gazebo_msgs/ModelState.h"
#include <gazebo_msgs/GetModelState.h>
#include <gazebo_msgs/GetWorldProperties.h>
#define desx 0
#define desy 0
#define cordx getmodelstate.response.pose.position.x-desx
#define cordy getmodelstate.response.pose.position.y-desy
/**
 * This tutorial demonstrates simple sending of velocity commands to the IRobot Create in Gazebo.
 */
int signbit(double x)
{
	if(x>=0)
		return 0;
	else
		return 1;
}
double abs(double x)
{
	if(x>=0)
		return x;
	return -x;
}
int main(int argc, char **argv)
{
	std::string name;
  ros::init(argc, argv, "SwarmSimu");
ros::NodeHandle private_node_handle_("~");
	private_node_handle_.param("name", name, std::string("swarmbot0"));
  ros::NodeHandle n;
   ros::ServiceClient client = n.serviceClient<gazebo_msgs::GetWorldProperties>("/gazebo/get_world_properties");
  gazebo_msgs::GetWorldProperties prop;
  ros::Publisher vel_pub_0 = n.advertise<geometry_msgs::Twist>("/swarmbot0/cmd_vel", 1);
//std::vector<std::string> s;
  //if(client.call(prop))
  //  s=prop.response.model_names;
std::string s=name;//How to find name of current process????
  ros::ServiceClient serv_client = n.serviceClient<gazebo_msgs::GetModelState>("/gazebo/get_model_state");
  gazebo_msgs::GetModelState getmodelstate;
  gazebo_msgs::ModelState modelstate;

  // ros::Publisher vel_pub_1 = n.advertise<geometry_msgs::Twist>("/robot0/cmd_vel", 1);
  ros::Rate loop_rate(5);

  int count = 0;
geometry_msgs::Twist cmd_vel;
  getmodelstate.request.model_name=s;

//cmd_vel.linear.x=-0.5*(cordx)/(sqrt(cordx*cordx+cordy*cordy));
//cmd_vel.linear.y=-0.5*(cordy)/(sqrt(cordx*cordx+cordy*cordy));
//ROS_INFO("%lf %lf %lf",cmd_vel.linear.x,cmd_vel.linear.y,(double)sqrt(cordx*cordx+cordy*cordy));
	cmd_vel.linear.x = 1;
	cmd_vel.linear.y = 0;
	cmd_vel.linear.z = 0;
    cmd_vel.angular.x = 0;
    cmd_vel.angular.y = 0;
    cmd_vel.angular.z = -1;
//Radius = 1 unit[length]
	vel_pub_0.publish(cmd_vel);
serv_client.call(getmodelstate);
ROS_INFO("Loop#1");

//while ((ros::ok())&&(abs(cordx)<=abs(prev))&&cordx!=0)
//while((ros::ok())&&(abs(cordx)>0.15)) 
count=0;
int flag=0;
while(ros::ok())
 {
	if(getmodelstate.response.pose.orientation.w<-0.99)
	{
		break;
	}
serv_client.call(getmodelstate); 
    //cmd_vel.linear.y = (cordy<0)?1*signbit(cordy):0;
   
 ROS_INFO("%lf %lf %lf",cordx,cordy,getmodelstate.response.pose.orientation.w);

	vel_pub_0.publish(cmd_vel);//test
    ros::spinOnce();

    loop_rate.sleep();
//prev=cordx;
   // serv_client.call(getmodelstate);

    ++count;
  }

	cmd_vel.linear.x = 1;
	cmd_vel.linear.y = 0;
	cmd_vel.linear.z = 0;
    cmd_vel.angular.x = 0;
    cmd_vel.angular.y = 0;
    cmd_vel.angular.z = 1;
//Radius = 1 unit[length]
	vel_pub_0.publish(cmd_vel);
serv_client.call(getmodelstate);
ROS_INFO("Loop#2");

//while ((ros::ok())&&(abs(cordx)<=abs(prev))&&cordx!=0)
//while((ros::ok())&&(abs(cordx)>0.15)) 
count=0;
while(ros::ok())
 {
	if(getmodelstate.response.pose.orientation.w>0.99)
	{
		break;
	}
serv_client.call(getmodelstate); 
    //cmd_vel.linear.y = (cordy<0)?1*signbit(cordy):0;
   
	vel_pub_0.publish(cmd_vel);
 ROS_INFO("%lf %lf %lf",cordx,cordy,getmodelstate.response.pose.orientation.w);

    ros::spinOnce();

    loop_rate.sleep();
//prev=cordx;
   // serv_client.call(getmodelstate);

    ++count;
  }
cmd_vel.linear.x = 0;
	cmd_vel.linear.y = 0;
	cmd_vel.linear.z = 0;
    cmd_vel.angular.x = 0;
    cmd_vel.angular.y = 0;
    cmd_vel.angular.z = 0;
	vel_pub_0.publish(cmd_vel);
  return 0;
}
