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
int main(int argc, char **argv)
{

  ros::init(argc, argv, "SwarmSimu");

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
serv_client.call(getmodelstate);

cmd_vel.linear.x=-0.5*(cordx)/(sqrt(cordx*cordx+cordy*cordy);
cmd_vel.linear.y=-0.5*(cordy)/(sqrt(cordx*cordx+cordy*cordy);

cmd_vel.linear.z = 0;
    cmd_vel.angular.x = 0;
    cmd_vel.angular.y = 0;
    cmd_vel.angular.z = 0.4*sin(count);
  
while ((ros::ok()&&(abs(cordx-desx)>0.5))
  {
    /*cmd_vel.linear.x = 0.5;
    cmd_vel.linear.y = 0;
    cmd_vel.linear.z = 0;
    cmd_vel.angular.x = 0;
    cmd_vel.angular.y = 0;
    cmd_vel.angular.z = 0.4*sin(count);*/


    vel_pub_0.publish(cmd_vel);
    ros::spinOnce();

    loop_rate.sleep();
    serv_client.call(getmodelstate);

    ++count;
  }
  return 0;
}
