#include "ros/ros.h"
#include "std_msgs/String.h"
#include "tutorials/Num.h"

#include <sstream>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "talker");
//  ros::NodeHandle n;
//  ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);

  ros::NodeHandle nh;
  ros::Publisher tutorials_pub = nh.advertise<tutorials::Num>("tutorials_msg", 100);
  tutorials::Num m;

  ros::Rate loop_rate(10);
  int count = 0;

  while (ros::ok())
  {
//    std_msgs::String msg;

    std::stringstream ss;
//    ss << "hello world " << count;
//    msg.data = ss.str();
    m.num = 10;
    m.RobotNum=5;

//    ROS_INFO("%s", msg.data.c_str());
    ROS_INFO("num = %d", m.num);
    ROS_INFO("The Number of Robot = %d", m.RobotNum);
//    chatter_pub.publish(msg);
    tutorials_pub.publish(m);
    ros::spinOnce();

    loop_rate.sleep();
    ++count;
  }


  return 0;
}
