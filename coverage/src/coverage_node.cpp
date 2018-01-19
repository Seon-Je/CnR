#include "ros/ros.h"
#include "coverage/task_info.h"
#include "coverage/coverage_info.h"
//----------------------------
#include "UTM.h"
#include <stdio.h>
#include "Parameters.h"
//----------------------------
#include <visualization_msgs/Marker.h>
//----------------------------

// Origin Point (redone technology(UTM) : (303064.25 , 3902275.00) )
float ORIGIN_X = 303064.25;
float ORIGIN_Y = 3902275.00;
// ************************************ 
// ********** For Simulation **********
float LTVs_UTMx[100] ={303064.25  , 303140.43 , 303126.76 , 303105.21 , 303165.42 , 303210.45 , 303037.9 , 303013.51 , 302997.73 , 302974.75 , 303004.23 , 303045.29 , 303073.14, 303076.96 , 303093.98 , 303119.93};
float LTVs_UTMy[100] ={3902275 , 3902236.99 , 3902216.86 , 3902187.59 , 3902264.08 , 3902312.7 , 3902328.12 , 3902349.62 , 3902364.95 , 3902388.31 , 3902416.86 , 3902466.57 , 3902508.25, 3902294.97 , 3902277.84 , 3902257.3};
// ************* For Test ************* 
//float LTVs_UTMx[100]={0};
//float LTVs_UTMy[100]={0};	
// ************************************ 

//=====for publish part
coverage::coverage_info LTVs_R1,LTVs_R2,LTVs_R3;
ros::Publisher coverage_pub1; // for 1st SmartCookie
ros::Publisher coverage_pub2; // for 2nd SmartCookie
ros::Publisher coverage_pub3; // for 3rd Smartcookie

visualization_msgs::Marker points_R1,points_R2,points_R3, line_R1, line_R2, line_R3;
ros::Publisher marker_pub1; // for display to rviz about 1st SmartCookie's LTVs(Location To Visit)
ros::Publisher marker_pub2; // for display to rviz about 2nd SmartCookie's LTVs(Location To Visit)
ros::Publisher marker_pub3;	// for display to rviz about 3rd SmartCookie's LTVs(Location To Visit)
//=====================

int main(int argc, char **argv)
{
  ros::init(argc, argv, "coverage_node");

  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("/smart_cookie/task_information", 1000, ReceiveTaskInfo);

	coverage_pub1 = n.advertise<coverage::coverage_info>("/0001/Coverage_Info",1000); 
	coverage_pub2 = n.advertise<coverage::coverage_info>("/0002/Coverage_Info",1000);
	coverage_pub3 = n.advertise<coverage::coverage_info>("/0003/Coverage_Info",1000);
	
	marker_pub1 = n.advertise<visualization_msgs::Marker>("/R1_coverage_points", 10);
	marker_pub2 = n.advertise<visualization_msgs::Marker>("/R2_coverage_points", 10);
	marker_pub3 = n.advertise<visualization_msgs::Marker>("/R3_coverage_points", 10);
  ros::spin();

  return 0;
}

void ReceiveTaskInfo(const coverage::task_info::ConstPtr& m)
{
// task command: (1:patrol),(2:?),(3:?),(4:?),(5:?),(:go home),(99:emergency)

// ************* Check data from Task_information topic *************
	ROS_INFO("Robot_num(%d), Point_num(%d), Task_Command(%d)",m->robot_num, m->point_num,m->task_command);
	for	(int i= 0; i < m->point_num ; i++)
		ROS_INFO("     lat(%.6f),lon(%.6f)",m->lat[i],m->lon[i]);
// ******************************************************************
	
// ************************************ 
// ********** For Simulation **********
	int temp_task =1;
	if(temp_task == 1)
// ************* For Test ************* 
//	if(m->task_command == 1)
// ************************************ 
	{
		index_r1 = 0;
		index_r2 = 1;
		index_r3 = 0;		

		double tempX = 0;
		double tempY = 0;

// ************************************ 
// ********** For Simulation **********
		for (int i=0; i < 16; i++) {
// ************* For Test ************* 
//		for (int i=0; i < m->point_num ; i++)
//		{
//			LatLonToUTMXY(m->lat[i], m->lon[i], zone, tempX, tempY);
//
//			LTVs_UTMx[i]=tempX;
//			LTVs_UTMy[i]=tempY;
// ************************************
			LTV_Assign(LTVs_UTMx[i],LTVs_UTMy[i],i);
		}
		LTV_Sorting();
	}

// ************************************ 
// ********** For Simulation **********
	else if(temp_task == 98) 
// ************* For Test ************* 
//	else if(m->task_command == 98) // go home!
// ************************************ 
	{
		index_r1 = index_r2 = index_r3 = 1;
		LTVs_x_R1[0] = LTVs_x_R2[0] = LTVs_x_R3[0] = ORIGIN_X; // Home Point : redone technologes
		LTVs_y_R1[0] = LTVs_y_R2[0] = LTVs_y_R3[0] = ORIGIN_Y; // Home Point : redone technologes
	}
// ************************************ 
// ********** For Simulation **********
	else if(temp_task == 99) 
// ************* For Test ************* 
//	else if(m->task_command == 99) // emergency situation !
// ************************************ 
	{
		index_r1 = index_r2 = index_r3 = 1;
		LTVs_x_R1[0] = LTVs_x_R2[0] = LTVs_x_R3[0] = ORIGIN_X; // Change Emergeny Point!! m->??
		LTVs_y_R1[0] = LTVs_y_R2[0] = LTVs_y_R3[0] = ORIGIN_Y; // Change Emergeny Point!! m->??
	}
// ************* for monitoring at console *************
	Display(); 
// *****************************************************

// ************* for publishing to 1st SmartCookie *************
	LTVs_R1.point_num = index_r1;
	LTVs_R1.task_command = m->task_command;
	LTVs_R1.UTMx.resize(index_r1);
	LTVs_R1.UTMy.resize(index_r1);
	for( int i=0 ; i < index_r1 ; i++)
	{
		LTVs_R1.UTMx[i]=LTVs_x_R1[i];
		LTVs_R1.UTMy[i]=LTVs_y_R1[i];
	}
	coverage_pub1.publish(LTVs_R1);
// *************************************************************
  	
// ************* for publishing to 2nd SmartCookie *************
	LTVs_R2.point_num = index_r2;
	LTVs_R2.task_command=m->task_command;
	LTVs_R2.UTMx.resize(index_r2);
	LTVs_R2.UTMy.resize(index_r2);
	for( int i=0 ; i < index_r2 ; i++)
	{
		LTVs_R2.UTMx[i]=LTVs_x_R2[i];
		LTVs_R2.UTMy[i]=LTVs_y_R2[i];
	}
	coverage_pub2.publish(LTVs_R2);
// *************************************************************
  
// ************* for publishing to 3nd SmartCookie *************
	LTVs_R3.point_num = index_r3;
	LTVs_R3.task_command=m->task_command;
	LTVs_R3.UTMx.resize(index_r3);
	LTVs_R3.UTMy.resize(index_r3);
	for( int i=0 ; i < index_r3 ; i++)
	{
		LTVs_R3.UTMx[i]=LTVs_x_R3[i];
		LTVs_R3.UTMy[i]=LTVs_y_R3[i];
	}
	coverage_pub3.publish(LTVs_R3);
// *************************************************************
  
//	ros::spinOnce();
//	loop_rate.sleep();

// ************* for visualizing at rviz *************
//	Publish2RVIZ();
// ***************************************************
  
}

void LTV_Assign(float UTM_X, float UTM_Y, int index) // assign LTVs to SmartCookies
{
// ********** Area division ********
	CalPoints2Line(); 
// *********************************	

// ********** Assign LTVs to each SmartCookies(no order) **********
	if (CheckLineUpDown(line810_A, line810_B, UTM_X, UTM_Y) <= 0)
	{
		LTVs_x_R1[index_r1] = UTM_X;
		LTVs_y_R1[index_r1] = UTM_Y;
		index_r1++;
	}
	else {}
 
	if (index==0) { LTVs_x_R2[0]=UTM_X;LTVs_y_R2[0]=UTM_Y;}
	if ((CheckLineUpDown(line911_A, line911_B, UTM_X, UTM_Y) >= 0) && (CheckLineUpDown(line12_A, line12_B, UTM_X, UTM_Y) <= 0))
	{
		LTVs_x_R2[index_r2] = UTM_X;
		LTVs_y_R2[index_r2] = UTM_Y;
		index_r2++;
		}
	else {}
 
	if ((CheckLineUpDown(line911_A, line911_B, UTM_X, UTM_Y) >= 0) && (CheckLineUpDown(line34_A, line34_B, UTM_X, UTM_Y) >= 0))
	{
		LTVs_x_R3[index_r3] = UTM_X;
		LTVs_y_R3[index_r3] = UTM_Y;
		index_r3++;
	}
	else {}
// **************************************************************
}

void LTV_Sorting() // Sorting SmartCookies' LTVs
{
	double temp_dis = 0;
	float temp_X = 0;
	float temp_Y = 0;
	int temp_index = 0;
	int i=0;
	int j=0;

// ********** Sorting 1st SmartCookie's LTVs **********
	for (i = 0; i < index_r1 - 1; i++) 
	{
		temp_dis = 9999;    
		temp_X = 0;
		temp_Y = 0;
		temp_index = 0;
		for (j = i + 1; j < index_r1; j++) 
        {
			if( DisPoint2Point(LTVs_x_R1[i], LTVs_y_R1[i], LTVs_x_R1[j], LTVs_y_R1[j]) <= temp_dis)
			{
				temp_dis = DisPoint2Point(LTVs_x_R1[i], LTVs_y_R1[i], LTVs_x_R1[j], LTVs_y_R1[j]);
				temp_index = j;
			}
		}
       
		temp_X = LTVs_x_R1[i + 1];
		temp_Y = LTVs_y_R1[i + 1];
		LTVs_x_R1[i + 1] = LTVs_x_R1[temp_index];
		LTVs_y_R1[i + 1] = LTVs_y_R1[temp_index];
		LTVs_x_R1[temp_index] = temp_X;
		LTVs_y_R1[temp_index] = temp_Y;
	}

	for( i = 0 ; i < index_r1 ; i++)
	{
		LTVs_x_R1[index_r1 + i] = LTVs_x_R1[index_r1 - i-2];
		LTVs_y_R1[index_r1 + i] = LTVs_y_R1[index_r1 - i-2];
	}
	index_r1 = 2 * index_r1 - 1;
// ****************************************************

// ********** Sorting 2nd SmartCookie's LTVs **********
	for (i = 0; i < index_r2 - 1; i++) 
	{
		temp_dis = 9999;    
		temp_X = 0;
		temp_Y = 0;
		temp_index = 0;
		for (j = i + 1; j < index_r2; j++) 
		{
			if( DisPoint2Point(LTVs_x_R2[i], LTVs_y_R2[i], LTVs_x_R2[j], LTVs_y_R2[j]) <= temp_dis)
			{
				temp_dis = DisPoint2Point(LTVs_x_R2[i], LTVs_y_R2[i], LTVs_x_R2[j], LTVs_y_R2[j]);
				temp_index = j;
			}
		}
		temp_X = LTVs_x_R2[i + 1];
		temp_Y = LTVs_y_R2[i + 1];
		LTVs_x_R2[i + 1] = LTVs_x_R2[temp_index];
		LTVs_y_R2[i + 1] = LTVs_y_R2[temp_index];
		LTVs_x_R2[temp_index] = temp_X;
		LTVs_y_R2[temp_index] = temp_Y;
	}

	for( i = 0 ; i < index_r2 ; i++)
	{
		LTVs_x_R2[index_r2 + i] = LTVs_x_R2[index_r2 - i-2];
		LTVs_y_R2[index_r2 + i] = LTVs_y_R2[index_r2 - i-2];
	}
	index_r2 = 2 * index_r2 - 1;
// ****************************************************

// ********** Sorting 3rd SmartCookie's LTVs **********
	for (i = 0; i < index_r3 - 1; i++) 
	{
		temp_dis = 9999;    
		temp_X = 0;
		temp_Y = 0;
		temp_index = 0;
		for (j = i + 1; j < index_r3; j++) 
		{
			if( DisPoint2Point(LTVs_x_R3[i], LTVs_y_R3[i], LTVs_x_R3[j], LTVs_y_R3[j]) <= temp_dis)
			{
				temp_dis = DisPoint2Point(LTVs_x_R3[i], LTVs_y_R3[i], LTVs_x_R3[j], LTVs_y_R3[j]);
				temp_index = j;
			}
		}
		temp_X = LTVs_x_R3[i + 1];
		temp_Y = LTVs_y_R3[i + 1];
		LTVs_x_R3[i + 1] = LTVs_x_R3[temp_index];
		LTVs_y_R3[i + 1] = LTVs_y_R3[temp_index];
		LTVs_x_R3[temp_index] = temp_X;
		LTVs_y_R3[temp_index] = temp_Y;
	}
	for( i = 0 ; i < index_r3 ; i++)
	{
		LTVs_x_R3[index_r3 + i] = LTVs_x_R3[index_r3 - i - 2];
		LTVs_y_R3[index_r3 + i] = LTVs_y_R3[index_r3 - i - 2];
	}
	index_r3 = 2 * index_r3 - 1;
// ****************************************************
}

void CalPoints2Line() // cal the line from 2 reference points
{
    //point 1 to 2 
    line12_A = (ref_point_2_y - ref_point_1_y) / (ref_point_2_x - ref_point_1_x);
    line12_B = -line12_A * ref_point_1_x + ref_point_1_y;
 
    //point 3 to 4 
    line34_A = (ref_point_4_y - ref_point_3_y) / (ref_point_4_x - ref_point_3_x);
    line34_B = -line34_A * ref_point_3_x + ref_point_3_y;
 
    //point 8 to 10 
    line810_A = (ref_point_10_y - ref_point_8_y) / (ref_point_10_x - ref_point_8_x);
    line810_B = -line810_A * ref_point_8_x + ref_point_8_y;
 
    //point 9 to 11 
    line911_A = (ref_point_11_y - ref_point_9_y) / (ref_point_11_x - ref_point_9_x);
    line911_B = -line911_A * ref_point_9_x + ref_point_9_y;
}
 
float CheckLineUpDown(float A, float B, float UTM_X, float UTM_Y) // Check where the points is
{
    // AX + B - Y = ?
//  printf("A*x : %f \n ",A*UTM_X + B);// - UTM_Y);
//  printf("B   : %f \n ",-UTM_Y);
    return A*UTM_X + B - UTM_Y;
}
 
float DisPoint2Point(float x1, float y1, float x2, float y2) // Calculate one point to another point
{
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void Display() // Display each SmartCookie's LTVs information at console 
{
	ROS_INFO("ROBOT 1's points(%d)",index_r1);
	for	(int i= 0; i <index_r1 ; i++) {	ROS_INFO("    (%.2f, %.2f)",LTVs_x_R1[i], LTVs_y_R1[i]); }

	ROS_INFO("ROBOT 2's points(%d)",index_r2);
	for	(int i= 0; i <index_r2 ; i++) { ROS_INFO("    (%.2f, %.2f)",LTVs_x_R2[i], LTVs_y_R2[i]); }

	ROS_INFO("ROBOT 3's points(%d)",index_r3);
	for	(int i= 0; i <index_r3 ; i++) {	ROS_INFO("    (%.2f, %.2f)",LTVs_x_R3[i], LTVs_y_R3[i]); }
}

void Publish2RVIZ() // for visualizing at rviz
{
	points_R1.header.frame_id = points_R2.header.frame_id = points_R3.header.frame_id = "/skku_monitoring";
	line_R1.header.frame_id = line_R2.header.frame_id = line_R3.header.frame_id = "/skku_monitoring";

	points_R1.header.stamp = points_R2.header.stamp = points_R3.header.stamp = ros::Time::now();
	line_R1.header.stamp = line_R2.header.stamp = line_R3.header.stamp = ros::Time::now();

	points_R1.ns = points_R2.ns = points_R3.ns = "coverage_node";
	line_R1.ns = line_R2.ns = line_R3.ns = "coverage_node";

	points_R1.action = points_R2.action = points_R3.action = visualization_msgs::Marker::ADD;
	line_R1.action = line_R2.action = line_R3.action = visualization_msgs::Marker::ADD;

	points_R1.pose.orientation.w = points_R2.pose.orientation.w = points_R3.pose.orientation.w = 1.0;
	line_R1.pose.orientation.w = line_R2.pose.orientation.w = line_R3.pose.orientation.w = 1.0;

	points_R1.id = points_R2.id =points_R3.id = 0;
	line_R1.id = line_R2.id =line_R3.id = 1;

	points_R1.type = points_R2.type = points_R3.type = visualization_msgs::Marker::POINTS;
	line_R1.type = line_R2.type = line_R3.type = visualization_msgs::Marker::LINE_STRIP;

	points_R1.scale.x = points_R2.scale.x = points_R3.scale.x = 5;
	points_R1.scale.y = points_R2.scale.y = points_R3.scale.y = 5;

	line_R1.scale.x = line_R2.scale.x = line_R3.scale.x = 3;

	points_R1.color.r = 1.0f; points_R1.color.g = 0.0f; points_R1.color.b = 0.0f; points_R1.color.a = 1.0;
	line_R1.color.r = 0.5f; line_R1.color.g = 0.0f; line_R1.color.b = 0.0f; line_R1.color.a = 1.0;

	points_R2.color.r = 0.0f; points_R2.color.g = 1.0f; points_R2.color.b = 0.0f; points_R2.color.a = 1.0;
	line_R2.color.r = 0.0f; line_R2.color.g = 0.5f; line_R2.color.b = 0.0f; line_R2.color.a = 1.0;

	points_R3.color.r = 0.0f; points_R3.color.g = 0.0f;	points_R3.color.b = 1.0f; points_R3.color.a = 1.0;
	line_R3.color.r = 0.0f; line_R3.color.g = 0.0f; line_R3.color.b = 0.5f; line_R3.color.a = 1.0;

	geometry_msgs::Point p1,p2,p3;

	for( int i=0 ; i < index_r1 ; i++)
	{
		p1.x = LTVs_R1.UTMx[i] - ORIGIN_X;
		p1.y = LTVs_R1.UTMy[i] - ORIGIN_Y;
		p1.z = 0;

		points_R1.points.push_back(p1);
		line_R1.points.push_back(p1);
	}

	for( int i=0 ; i < index_r2 ; i++)
	{
		p2.x = LTVs_R2.UTMx[i] - ORIGIN_X;
		p2.y = LTVs_R2.UTMy[i] - ORIGIN_Y;
		p2.z = 0;

		points_R2.points.push_back(p2);
		line_R2.points.push_back(p2);
	}

	for( int i=0 ; i < index_r3 ; i++)
	{
		p3.x = LTVs_R3.UTMx[i] - ORIGIN_X;
		p3.y = LTVs_R3.UTMy[i] - ORIGIN_Y;
		p3.z = 0;

		points_R3.points.push_back(p3);
		line_R3.points.push_back(p3);
	}

	marker_pub1.publish(points_R1);
	marker_pub1.publish(line_R1);

	marker_pub2.publish(points_R2);
	marker_pub2.publish(line_R2);

	marker_pub3.publish(points_R3);
	marker_pub3.publish(line_R3);
}
