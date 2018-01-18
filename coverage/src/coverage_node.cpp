#include "ros/ros.h"
#include "coverage/task_info.h"
#include "coverage/coverage_info.h"
//----------------------------
#include "UTM.h"
#include <stdio.h>

double utm_x = 0;
double utm_y = 0;

int zone = 52;

float ref_point_1_x = 303117.84;
float ref_point_1_y = 3902232.15;
                      
float ref_point_2_x = 302967.45;
float ref_point_2_y = 3902366.72;
                      
float ref_point_3_x = 303141.97;
float ref_point_3_y = 3902261.81;
                      
float ref_point_4_x = 302993.29;
float ref_point_4_y = 3902386.58;
 
float ref_point_5_x = 303034.99;
float ref_point_5_y = 3902282.57;
 
float ref_point_6_x = 3032007.59;
float ref_point_6_y = 3902540.38;
 
float ref_point_7_x = 302896.95;
float ref_point_7_y = 3902043.98;
 
float ref_point_8_x = 303075.75;
float ref_point_8_y = 3902281.24;
 
float ref_point_9_x = 303056.73;
float ref_point_9_y = 3902294.30;
 
float ref_point_10_x = 303089.15;
float ref_point_10_y = 3902303.03;
 
float ref_point_11_x = 303075.42;
float ref_point_11_y = 3902311.87;

float line12_A = 0;
float line12_B = 0;
 
float line34_A = 0;
float line34_B = 0;
 
float line810_A = 0;
float line810_B = 0;
 
float line911_A = 0;
float line911_B = 0;

float LTVs_UTMx[100]={0};
float LTVs_UTMy[100]={0};

float LTVs_x_R1[100]={0};
float LTVs_y_R1[100]={0};

float LTVs_x_R2[100]={0};
float LTVs_y_R2[100]={0};

float LTVs_x_R3[100]={0};
float LTVs_y_R3[100]={0};

int index_r1 = 0;
int index_r2 = 0;
int index_r3 = 0;		

// for Testing
//float LTVs_UTMx[100] ={303064.25  , 303140.43 , 303126.76 , 303105.21 , 303165.42 , 303210.45 , 303037.9 , 303013.51 , 302997.73 , 302974.75 , 303004.23 , 303045.29 , 303073.14, 303076.96 , 303093.98 , 303119.93};
//float LTVs_UTMy[100] ={3902275 , 3902236.99 , 3902216.86 , 3902187.59 , 3902264.08 , 3902312.7 , 3902328.12 , 3902349.62 , 3902364.95 , 3902388.31 , 3902416.86 , 3902466.57 , 3902508.25, 3902294.97 , 3902277.84 , 3902257.3};
 

void ReceiveTaskInfo(const coverage::task_info::ConstPtr& m);
void LTV_Assign(float UTM_X, float UTM_Y, int index);
void CalPoints2Line();
void LTV_Sorting();
float CheckLineUpDown(float A, float B, float UTM_X, float UTM_Y);
float DisPoint2Point(float x1, float y1, float x2, float y2);

//=====for publish part
	coverage::coverage_info LTVs_R1,LTVs_R2,LTVs_R3;
	//ros::NodeHandle nh;
	ros::Publisher coverage_pub1;// = nh.advertise<coverage::coverage_info>("/0001/Coverage_Info",10);
	ros::Publisher coverage_pub2;// = nh.advertise<coverage::coverage_info>("/0002/Coverage_Info",10);
	ros::Publisher coverage_pub3;// = nh.advertise<coverage::coverage_info>("/0003/Coverage_Info",10);
	//os::Rate loop_rate(100);	
//=====================


int main(int argc, char **argv)
{
  ros::init(argc, argv, "coverage_node");

  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("/smart_cookie/task_information", 1000, ReceiveTaskInfo);

	coverage_pub1 = n.advertise<coverage::coverage_info>("/0001/Coverage_Info",1000);
	coverage_pub2 = n.advertise<coverage::coverage_info>("/0002/Coverage_Info",1000);
	coverage_pub3 = n.advertise<coverage::coverage_info>("/0003/Coverage_Info",1000);

  ros::spin();

  return 0;
}


void ReceiveTaskInfo(const coverage::task_info::ConstPtr& m)
{

	
	ROS_INFO("Robot_num(%d), Point_num(%d)",m->robot_num, m->point_num);
	for	(int i= 0; i < m->point_num ; i++) { ROS_INFO("lat(%.2f),lon(%.2f)",m->lat[i],m->lon[i]);}
	index_r1 = 0;
	index_r2 = 1;
	index_r3 = 0;		

	double tempX = 0;
	double tempY = 0;

//	for (int i=0; i < 16; i++) // for testing
	for (int i=0; i < m->point_num ; i++) 
	{

		LatLonToUTMXY(m->lat[i], m->lon[i], zone, tempX, tempY);

		LTVs_UTMx[i]=tempX;
		LTVs_UTMy[i]=tempY;

		LTV_Assign(LTVs_UTMx[i],LTVs_UTMy[i],i);
	}
	LTV_Sorting();
	
	ROS_INFO("ROBOT 1's points(%d)",index_r1);
	for	(int i= 0; i <index_r1 ; i++) {	ROS_INFO("    (%.2f, %.2f)",LTVs_x_R1[i], LTVs_y_R1[i]); }

	ROS_INFO("ROBOT 2's points(%d)",index_r2);
	for	(int i= 0; i <index_r2 ; i++) { ROS_INFO("    (%.2f, %.2f)",LTVs_x_R2[i], LTVs_y_R2[i]); }

	ROS_INFO("ROBOT 3's points(%d)",index_r3);
	for	(int i= 0; i <index_r3 ; i++) {	ROS_INFO("    (%.2f, %.2f)",LTVs_x_R3[i], LTVs_y_R3[i]); }


//=====for publish part

	LTVs_R1.point_num = index_r1;
	LTVs_R1.task_command=m->task_command;
	LTVs_R1.UTMx.resize(index_r1);
	LTVs_R1.UTMy.resize(index_r1);
	for( int i=0 ; i < index_r1 ; i++)
	{
		LTVs_R1.UTMx[i]=LTVs_x_R1[i];
		LTVs_R1.UTMy[i]=LTVs_y_R1[i];
	}
	coverage_pub1.publish(LTVs_R1);
  	

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
//	ros::spinOnce();
    ///loop_rate.sleep();
//=====================
	
}

void LTV_Assign(float UTM_X, float UTM_Y, int index)
{
	CalPoints2Line();
	
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
// Sorting Robot 1's LTVs
}
void LTV_Sorting()
{
	double temp_dis = 0;
	float temp_X = 0;
	float temp_Y = 0;
	int temp_index = 0;
	int i=0;
	int j=0;

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

// Sorting Robot 2's LTVs
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
// Sorting Robot 3's LTVs

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
}

void CalPoints2Line()
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
 
float CheckLineUpDown(float A, float B, float UTM_X, float UTM_Y)
{
    // AX + B - Y = ?
//  printf("A*x : %f \n ",A*UTM_X + B);// - UTM_Y);
//  printf("B   : %f \n ",-UTM_Y);
    return A*UTM_X + B - UTM_Y;
}
 
float DisPoint2Point(float x1, float y1, float x2, float y2)
{
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}
