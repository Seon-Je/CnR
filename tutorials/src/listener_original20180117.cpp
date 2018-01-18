#include "ros/ros.h"
#include "std_msgs/String.h"
#include "tutorials/Num.h"

//void chatterCallback1(const std_msgs::String::ConstPtr& msg)
//{
//  ROS_INFO("I heard: [%s]", msg->data.c_str());
//}

int a;

void listener_back(const tutorials::Num::ConstPtr& m)
{
  ROS_INFO("num = [%d]", m->num);
  ROS_INFO("The Number of Robot = [%d]", m->RobotNum);
  a = m->num;
    ROS_INFO("a is %d",a);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");

  ros::NodeHandle n;
  //ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback1);
//  ROS_INFO("a is %d",a);

  ros::Subscriber sub = n.subscribe("tutorials_msg", 1000, listener_back);

  ros::spin();

  return 0;
}
