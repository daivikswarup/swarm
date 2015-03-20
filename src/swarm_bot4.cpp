#include "ros/ros.h"
#include <math.h>
#include <utility>
#include "geometry_msgs/Twist.h"
#include "std_msgs/String.h"
#include "gazebo_msgs/ModelState.h"
#include <gazebo_msgs/GetModelState.h>
#include <gazebo_msgs/GetWorldProperties.h>
#include<string.h>
#define cordx (getmodelstate.response.pose.position.x-desx)
#define cordtheta (normalizeAngle(2*acos(getmodelstate.response.pose.orientation.w)-destheta))
#define cordy (getmodelstate.response.pose.position.y-desy)
#define threshold 0.1
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
double normalizeAngle(double x)
{
	//return fmod(x,2*M_PI)-((fmod(x,2*M_PI)>M_PI)?2*M_PI:0);
	return x-2*M_PI*floor((x+M_PI)/(2*M_PI));
}
class path_parameters
{
	public:
	double k1;
	double k2;
	double k3;
	void set(double x1, double x2, double x3)
	{
		k1=x1;
		k2=x2;
		k3=x3;
	}
};

class polar
{
	public:
	double ro;
	double gamma;
	double delta;
	void set(double x, double y, double theta)
	{
		ro=sqrt(x*x+y*y);
		//gamma=atan2(y,x)-theta+M_PI;
		gamma=normalizeAngle(atan2(y,x)-theta+M_PI);			
		delta=normalizeAngle(gamma+theta);
	}
	double calc_v(path_parameters p)
	{
		double v;
		v=p.k1*ro*cos(gamma);
		return v;
	}
	double calc_w(path_parameters p)
	{
		double w;
		if(gamma==0)
			w=p.k1*p.k3*delta;
		else
			w=p.k2*gamma+p.k1*sin(gamma)*cos(gamma)*(gamma+p.k3*delta)/gamma;
		return w;
	}
};

int main(int argc, char **argv)
{	
	double desx;
	double desy;
	double destheta;
	polar polar_cord;
	std::string name;
	ros::init(argc, argv, name);
	ros::NodeHandle private_node_handle_("~");
	private_node_handle_.param("name", name, std::string("swarmbot0"));
  	private_node_handle_.param("desx", desx, double(0));
  	private_node_handle_.param("desy", desy, double(0));
	private_node_handle_.param("destheta", destheta, double(0));

  	path_parameters p;

	ros::NodeHandle n;
	ros::ServiceClient client = n.serviceClient<gazebo_msgs::GetWorldProperties>("/gazebo/get_world_properties");
	gazebo_msgs::GetWorldProperties prop;
	ros::Publisher vel_pub_0 = n.advertise<geometry_msgs::Twist>("/swarmbot0/cmd_vel", 1);
	ros::ServiceClient serv_client = n.serviceClient<gazebo_msgs::GetModelState>("/gazebo/get_model_state");
	gazebo_msgs::GetModelState getmodelstate;
	gazebo_msgs::ModelState modelstate;
	ros::Rate loop_rate(50);

	int count = 0;
	geometry_msgs::Twist cmd_vel;
	getmodelstate.request.model_name=name;
	serv_client.call(getmodelstate); 
	polar_cord.set(cordx,cordy,cordtheta);
	p.set(0.75,4,20);
	ROS_INFO("%lf\n",polar_cord.ro);
	while((ros::ok())&&(polar_cord.ro>threshold))
 	{
		serv_client.call(getmodelstate); 
		polar_cord.set(cordx,cordy,cordtheta);
		
		cmd_vel.linear.x=polar_cord.calc_v(p);
		cmd_vel.angular.z=polar_cord.calc_w(p);
		
		ROS_INFO("%lf %lf %lf %lf %lf",cordx, cordy, cordtheta,cmd_vel.linear.x,cmd_vel.angular.z);
		ROS_INFO("%lf %lf %lf",polar_cord.ro,polar_cord.gamma,polar_cord.delta);
    		vel_pub_0.publish(cmd_vel);
    		ros::spinOnce();

    		loop_rate.sleep();
    		++count;
	}
	
	cmd_vel.linear.x=0;
	cmd_vel.angular.z=0;
	vel_pub_0.publish(cmd_vel);
    	ros::spinOnce();
  	return 0;
}
