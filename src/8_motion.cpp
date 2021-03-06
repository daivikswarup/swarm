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
 * To execute an 8 motion. The parameters v and w can be taken from the command line.
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
	double v,w;
  	ros::init(argc, argv, "SwarmSimu");

	//TO ACCEPT PARAMETERS FROM THE COMMAND LINE
	ros::NodeHandle private_node_handle_("~");
	private_node_handle_.param("name", name, std::string("swarmbot0"));
  	private_node_handle_.param("v", v, double(1));//DEFAULT v=1
  	private_node_handle_.param("w", w, double(1));//DEFAULT w=1

	//DECLARING CLIENT AND PUBLISHER
  	ros::NodeHandle n;
   	ros::ServiceClient client = n.serviceClient<gazebo_msgs::GetWorldProperties>("/gazebo/get_world_properties");
  	gazebo_msgs::GetWorldProperties prop;
  	ros::Publisher vel_pub_0 = n.advertise<geometry_msgs::Twist>("/swarmbot0/cmd_vel", 1);
	std::string s=name;
  	ros::ServiceClient serv_client = n.serviceClient<gazebo_msgs::GetModelState>("/gazebo/get_model_state");
  	gazebo_msgs::GetModelState getmodelstate;
  	gazebo_msgs::ModelState modelstate;


  	ros::Rate loop_rate(5);

  	int count = 0;
	geometry_msgs::Twist cmd_vel;
  	getmodelstate.request.model_name=s;//ENABLES serv_client.call() TO FETCH THE CORRECT MODEL DATA.

	//1ST LOOP
	cmd_vel.linear.x = v;
	cmd_vel.linear.y = 0;
	cmd_vel.linear.z = 0;
    	cmd_vel.angular.x = 0;
    	cmd_vel.angular.y = 0;
    	cmd_vel.angular.z = -w;

	vel_pub_0.publish(cmd_vel);
	serv_client.call(getmodelstate);
	ROS_INFO("Loop#1");

	count=0;
	int flag=0;
	while(ros::ok())
 	{
		if(getmodelstate.response.pose.orientation.w<-0.99)//CHECK IF 1 LOOP IS COMPLETED
		{
			break;
		}
		serv_client.call(getmodelstate); 
   
 		ROS_INFO("%lf %lf %lf",cordx,cordy,getmodelstate.response.pose.orientation.w);

		vel_pub_0.publish(cmd_vel);//NOT NESSECARY
    		ros::spinOnce();
    		loop_rate.sleep();
    		++count;
  	}

	//LOOP 2
	cmd_vel.linear.x = v;
	cmd_vel.linear.y = 0;
	cmd_vel.linear.z = 0;
    	cmd_vel.angular.x = 0;
    	cmd_vel.angular.y = 0;
    	cmd_vel.angular.z = w;
	vel_pub_0.publish(cmd_vel);
	serv_client.call(getmodelstate);
	ROS_INFO("Loop#2");
	count=0;
	while(ros::ok())
 	{
		if(getmodelstate.response.pose.orientation.w>0.99)
		{
			break;
		}
		serv_client.call(getmodelstate); 
		vel_pub_0.publish(cmd_vel);
 		ROS_INFO("%lf %lf %lf",cordx,cordy,getmodelstate.response.pose.orientation.w);
    		ros::spinOnce();
   		loop_rate.sleep();
    		++count;
  	}

	//STOP THE BOT
	cmd_vel.linear.x = 0;
	cmd_vel.linear.y = 0;
	cmd_vel.linear.z = 0;
    	cmd_vel.angular.x = 0;
    	cmd_vel.angular.y = 0;
    	cmd_vel.angular.z = 0;
	vel_pub_0.publish(cmd_vel);
  	return 0;
}
