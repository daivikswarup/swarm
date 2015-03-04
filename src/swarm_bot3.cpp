#include "ros/ros.h"
#include <math.h>
#include "geometry_msgs/Twist.h"
#include "std_msgs/String.h"
#include "gazebo_msgs/ModelState.h"
#include <gazebo_msgs/GetModelState.h>
#include <gazebo_msgs/GetWorldProperties.h>
#include<string.h>
//#define desx 0
//#define desy 0
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
//To obtain command line parameters	
	double desx;
	double desy;
	std::string name;
ros::init(argc, argv, name);
	ros::NodeHandle private_node_handle_("~");
	private_node_handle_.param("name", name, std::string("swarmbot0"));
  	private_node_handle_.param("desx", desx, double(0));
  	private_node_handle_.param("desy", desy, double(0));

  

  ros::NodeHandle n;
   ros::ServiceClient client = n.serviceClient<gazebo_msgs::GetWorldProperties>("/gazebo/get_world_properties");
  gazebo_msgs::GetWorldProperties prop;
  ros::Publisher vel_pub_0 = n.advertise<geometry_msgs::Twist>("/swarmbot0/cmd_vel", 1);
//std::vector<std::string> s;
  //if(client.call(prop))
  //  s=prop.response.model_names;
std::string s="swarmbot0";//How to find name of current process????
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

cmd_vel.linear.z = 0;
    cmd_vel.angular.x = 0;
    cmd_vel.angular.y = 0;
    cmd_vel.angular.z = 0;
serv_client.call(getmodelstate);
ROS_INFO("Moving in X direction");

//while ((ros::ok())&&(abs(cordx)<=abs(prev))&&cordx!=0)
//while((ros::ok())&&(abs(cordx)>0.15)) 
while(ros::ok())
 {
serv_client.call(getmodelstate); 
	if(abs(cordx)<0.2)
		break;
	else
 		cmd_vel.linear.x = (cordx<0)?1:-1;
    //cmd_vel.linear.y = (cordy<0)?1*signbit(cordy):0;
	cmd_vel.linear.y=0;   
	cmd_vel.linear.z = 0;
    cmd_vel.angular.x = 0;
    cmd_vel.angular.y = 0;
    cmd_vel.angular.z = 0.4*sin(count);
   
 ROS_INFO("%lf %lf",cordx,cordy);

    vel_pub_0.publish(cmd_vel);
    ros::spinOnce();

    loop_rate.sleep();
//prev=cordx;
   // serv_client.call(getmodelstate);

    ++count;
  }
ROS_INFO("Turning +90 degrees");
ROS_INFO("%lf %lf %lf %lf",getmodelstate.response.pose.orientation.x,getmodelstate.response.pose.orientation.y, getmodelstate.response.pose.orientation.z, getmodelstate.response.pose.orientation.w);
cmd_vel.linear.x=0;
cmd_vel.linear.x=0;
cmd_vel.linear.x=0;
cmd_vel.angular.z=0.1;
	vel_pub_0.publish(cmd_vel);
while(ros::ok())
{
serv_client.call(getmodelstate); 
ROS_INFO("%lf",getmodelstate.response.pose.orientation.w);
//if(abs(abs(getmodelstate.response.pose.orientation.w)-0.707)<0.1)
if((getmodelstate.response.pose.orientation.w)<0.707)
{
	cmd_vel.angular.z=0;
	vel_pub_0.publish(cmd_vel);
	break;
}
ros::spinOnce();
loop_rate.sleep();
}

ROS_INFO("Moving in Y direction");
while(ros::ok())
 {
serv_client.call(getmodelstate); 
	if(abs(cordy)<0.2)
		break;
	else
 		cmd_vel.linear.x = (cordy<0)?-1:1;
    //cmd_vel.linear.y = (cordy<0)?1*signbit(cordy):0;
	cmd_vel.linear.y=0;   
	cmd_vel.linear.z = 0;
    cmd_vel.angular.x = 0;
    cmd_vel.angular.y = 0;
    cmd_vel.angular.z = 0.4*sin(count);
   
 ROS_INFO("%lf %lf",cordx,cordy);

    vel_pub_0.publish(cmd_vel);
    ros::spinOnce();

    loop_rate.sleep();
//prev=cordx;
   // serv_client.call(getmodelstate);

    ++count;
  }
cmd_vel.linear.x=0;
cmd_vel.angular.z=0;
vel_pub_0.publish(cmd_vel);
//while ((ros::ok())&&(abs(cordy)>=abs(prev))&&cordy!=0)
/*serv_client.call(getmodelstate);
prev=cordy;
ROS_INFO("Moving in Y direction!");
while((ros::ok())&&(abs(cordy)>0.15))
  {
prev=cordy;
  serv_client.call(getmodelstate);
 cmd_vel.linear.y = -1000*signbit(cordy);
    cmd_vel.linear.x = 0;
    cmd_vel.linear.z = 0;
    cmd_vel.angular.x = 0;
    cmd_vel.angular.y = 0;
    cmd_vel.angular.z = 0.4*sin(count);

 ROS_INFO("%lf %lf %lf",cordx,cordy,cmd_vel.linear.y);

    vel_pub_0.publish(cmd_vel);
    ros::spinOnce();

    loop_rate.sleep();
//prev=cordy;
   // serv_client.call(getmodelstate);

    ++count;
  }*/
  return 0;
}
