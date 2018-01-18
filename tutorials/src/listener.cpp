#include "ros/ros.h"
#include "tutorials/Num.h"

void listener_back(const tutorials::Num::ConstPtr& m)
{
  /*ROS_INFO("Task : %d | # of Robot : %d | # of LTVs : %d", m->task_command,m->robot_num, m->point_num);
  ROS_INFO("point1 : (%.6f , %.6f) , point2 : (%.6f , %.6f)", m->lat[0], m->lon[0], m->lat[1], m->lon[1]);*/

	ROS_INFO("%d, %.2f, %.2f", m->robot_num, m->UTMx[0], m->UTMy[0]);
	ROS_INFO("hi1");
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");

  ros::NodeHandle n;

  //ros::Subscriber sub = n.subscribe("/smart_cookie/task_information", 1000, listener_back);
ROS_INFO("hi2");
  ros::Subscriber sub = n.subscribe("/Coverage_information", 1000, listener_back);
ROS_INFO("hi3");
		
  ros::spin();

  return 0;
}
