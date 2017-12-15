#include "ros/ros.h"
#include "test_topic/MsgTutorial.h"

int main(int argc, char **argv)
{
    ros::init(argc,argv,"topic_pub");
    ros::NodeHandle nh;
    ros::Publisher ros_tutorial_pub = nh.advertise<test_topic::MsgTutorial>("test_msg", 100);
    ros::Rate loop_rate(10);
    test_topic::MsgTutorial msg;
    int count = 0;



    while (ros::ok())
    {
        msg.stamp = ros::Time::now();
        msg.data = count;
        //msg.text = std::string("h");
        // 현재 시간을 msg의 하위 stamp 메시지에 담는다
        // count라는 변수 값을 msg의 하위 data 메시지에 담는다
        ROS_INFO("send msg = %d", msg.stamp.sec);
        ROS_INFO("send msg = %d", msg.stamp.nsec);
        ROS_INFO("send msg = %d", msg.data); // stamp.sec 메시지를 표시한다
        //ROS_INFO("send msg = %s", msg.text);
        // stamp.nsec 메시지를 표시한다
        // data 메시지를 표시한다
        ros_tutorial_pub.publish(msg); // 메시지를 발행한다
        loop_rate.sleep(); // 위에서 정한 루프 주기에 따라 슬립에 들어간다
        ++count; // count 변수 1씩 증가
    }
    return 0;
}

//typedef coordinate
//typedef robot
/*
int MAX_LTVs = 10;
int MAX_Robot = 3;

double L[MAX_LTVs][MAX_LTVs] = {0};
int S[MAX_LTVs][MAX_LTVs] = {0};
double L_min = 1000;
double t_o = 0;

double Cal_Distance(double coordinate_1, double coordinate_2) // 좌표형태의 구조체 필요
{
  return sqrt(pow((coordinate_1.x-coordinate_2.x),2) + pow((coordinate_1.y-coordinate_2.y),2);
}

int Cal_L_min(LTVs[],state) // LTVs
{
  double L_min; // 매우 큰 초기값 설정 필요
  for(int j=0; j<MAX_LTVs; j++)
  {
    for(int k=0; k<MAX_LTVs; k++)
    {
      if(j!=k)
      {
        L[j][k] = Cal_Distance(LTVs[j],LTVs[k]);
        if(L[j][k] < L_min)
        {
          L_min = L[j][k];
        }
        else
        {
          L_min=L_min;
        }
    }
  }
  return 1;
}

int Cal_S(L[][], double L_min, int state)
{
  int b=1;

  for(int j=0; j<MAX_LTVs; j++)
  {
    for(int k=0; k<MAX_LTVs; k++)
    {
      if(j!=k)
      {
        if(L[j][k]%L_min==0)
        {
          S[j][k] = int(L[j][k]/L_min);
        }
        else
        {
          S[j][k] = int(L[j][k]/L_min)+b;
        }
      }
    }
  }
  return 1;
}

int Cal_t_L_min(robot, L_min, L_i_max,state)
{
  for(int i=0; i<MAX_Robot; i++)
  {
    L_max[i]=robot.v_max * (2*robot.v_max/robot.a_max);
    if (L_min <= L_i_max[i])
    {
      t_L_min[i]=2*sqrt(L_min/robot.a_max);
    }
    else
    {
      t_L_min[i]=L_min/robot.v_max + L_max[i]/robot.v_max;
    }
  }
  return 1;
}

int Sel_t_o(t_L_min[i],state)
{
  for(int i=0; i<MAX_Robot; i++)
    {
    if(t_L_min[i] >= t_o)
    {
      t_o = t_L_min[i];
    }
    else
    {
      t_o=t_o;
    }
  }
  return 1;
}

int Cal_eta(t_o, t_L_min[i],state)
{
  for(int i=0; i<MAX_Robot; i++)
  {
    eta[i]=int(t_o/t_L_min[i]);
  }
  return 1;
}
*/
