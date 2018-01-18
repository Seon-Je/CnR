#include "ros/ros.h"
#include "tutorials/Num.h"

int main(int argc, char **argv)
{
  ros::init(argc, argv, "talker");

  ros::NodeHandle nh;
  ros::Publisher tutorials_pub = nh.advertise<tutorials::Num>("tutorials_msg", 100);
  tutorials::Num m;

  ros::Rate loop_rate(10);
  int count = 0;

  while (ros::ok())
  {


    m.lat = 10;
    m.lon = 5;
    m.Num_Robot = 3;
    m.task = 1;

    ROS_INFO("Task : %d | # of Robot : %d | lat : %.6f | lon : %.6f", m.task,m.Num_Robot, m.lat, m.lon);

    tutorials_pub.publish(m);
    ros::spinOnce();

    loop_rate.sleep();
  }


  return 0;
}


// topic name : /smart_cookie/task_information
