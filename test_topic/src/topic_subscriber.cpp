#include "ros/ros.h"        // ROS 기본 헤더파일
#include "test_topic/MsgTutorial.h"
// MsgTutorial 메시지 파일 헤더 (빌드 후 자동 생성됨)
// 메시지 콜백 함수로써, 밑에서 설정한 ros_tutorial_msg라는 이름의 토픽
// 메시지를 수신하였을 때 동작하는 함수이다
// 입력 메시지로는 ros_tutorials_topic 패키지의 MsgTutorial 메시지를 받도록 되어있다
void msgCallback(const test_topic::MsgTutorial::ConstPtr& msg)
{
    ROS_INFO("recieve msg = %d", msg->stamp.sec);   // stamp.sec 메시지를 표시한다
    ROS_INFO("recieve msg = %d", msg->stamp.nsec);  // stamp.nsec 메시지를 표시한다
    ROS_INFO("recieve msg(data) = %d", msg->data);        // data 메시지를 표시한다
    //ROS_INFO("recieve msg(text) = %s", msg->text);
}
int main(int argc, char **argv)
{
    ros::init(argc, argv, "topic_subscriber");
    ros::NodeHandle nh;
    ros::Subscriber ros_tutorial_sub = nh.subscribe("test_msg", 100, msgCallback);
    ros::spin();
    return 0;
}
