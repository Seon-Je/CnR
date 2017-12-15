#include <stdio.h>
#include "ros/ros.h"        // ROS 기본 헤더파일
#include "skku_coverage/skku_coverage_msg.h"
#include "math.h"
#include <ros/console.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>

const int MAX_LTVs = 13;
const int MAX_robots = 3;

#define V MAX_LTVs

int S[MAX_LTVs][MAX_LTVs]={0};
int sum_S = 0;
int new_i = 0;
double L[MAX_LTVs][MAX_LTVs] ={0};

double L_min = 1000;
double t_o = 0;

bool state =0;

typedef struct
{
  double x;
  double y;
} Coordinate;

typedef struct
{
  double v_max;
  double a_max;
  double t_L_min;
  double L_max;
  int eta;
  int o;
  Coordinate rLTV;
} ROBOT;

using namespace std;

ROBOT robot[MAX_robots];
Coordinate M[MAX_LTVs]={0};
// for simulation
//Coordinate LTVs[MAX_LTVs]={{0,0},{10,20},{20,10},{30,50},{40,40},{30,10},{50,40},{10,50}};//,{5,4},{6,3},{7,2},{8,1},{9,0}};
//Coordinate LTVs[MAX_LTVs]={{0,0},{10,5},{30,5},{45,5},{45,25},{45,35},{45,45},{35,45},{4,46},{4,25},{15,25},{4,15},{15,15}};
Coordinate LTVs[MAX_LTVs]={{0,0},{10,5},{4,15},{15,15},{4,25},{15,25},{4,46},{30,5},{45,5},{45,35},{35,45},{45,45},{45,25}};

//

void msgCallback(const skku_coverage::skku_coverage_msg::ConstPtr& msg);
void Robot_Init();
double Cal_Distance(Coordinate P1, Coordinate P2); // 좌표형태의 구조체 필요
int Cal_L_min(Coordinate *LTV ,bool state); // LTVs
int Cal_S(/*double *Length, double L_min, */int state);
int Cal_t_L_min(ROBOT *Robot,int state);
int Sel_t_o(ROBOT *Robot,int state);
int Cal_eta(ROBOT *Robot, int state);
int Cal_O(ROBOT *Robot, int state);
int Assign_LTVs();
void DISPLAY();
void printSolution(int path[]);
bool isSafe(int v, int graph[V][V], int path[], int pos);
bool hamCycleUtil(int graph[V][V], int path[], int pos);
bool hamCycle(int graph[V][V]);
int Modify_S(); // map data, LTVs, S

int main(int argc, char **argv)
{
    /*ros::init(argc, argv, "topic_subscriber");
    ros::NodeHandle nh;
    ros::Subscriber ros_tutorial_sub = nh.subscribe("ros_tutorial_msg", 100, msgCallback);
    ros::spin();*/
    Robot_Init();

    state = Cal_L_min(LTVs,state);
    state = Cal_S(state);
    state = Modify_S();
    state = Cal_t_L_min(robot,state);
    state = Sel_t_o(robot, state);
    state = Cal_eta(robot, state);
    hamCycle(S);
    new_i=0;
    state = Cal_O(robot,state);
    //DISPLAY();
    //hamCycle(S);

    DISPLAY();
    return 0;
}

void DISPLAY()
{
  cout<<"*L_min = "<<L_min<<endl<<endl;
  cout<<"*L_jk  = "<<endl;
  for(int j=0; j<MAX_LTVs;j++)
    for(int k=0; k<MAX_LTVs;k++)
      {
        cout<<"  "<< setw(6)<<setprecision(4)<<L[j][k];
        if(k == MAX_LTVs-1) cout<<endl;
      }
  cout<<endl;

  cout<<"*S_jk  = "<<endl;
  for(int j=0; j<MAX_LTVs;j++)
    for(int k=0; k<MAX_LTVs;k++)
      {
        cout<<"  "<< setw(6)<<setprecision(4)<<S[j][k];
        if(k == MAX_LTVs-1) cout<<endl;
      }
  cout<<endl;

  for(int i=0; i<MAX_robots;i++)
  {
    cout<<"*ROBOT "<< i <<endl;
    cout<<" |----L_max   = "<< robot[i].L_max<<endl;
    cout<<" |----t_L_min = "<< robot[i].t_L_min<<endl;
    cout<<" |----eta     = "<< robot[i].eta<<endl;
    cout<<" |----O       = "<< robot[i].o<<endl;
    cout<<endl;
  }
  cout<<endl;

  cout<<"LTVs[]=";
  for(int j=0; j<MAX_LTVs;j++)
    cout<< "(" << LTVs[j].x<< "," <<LTVs[j].y << ") ";

  cout<<endl;

  cout<<"M[]   =";
  for(int j=0; j<MAX_LTVs;j++)
    cout<< "(" << M[j].x<< "," <<M[j].y << ") ";
  cout<<endl;


}

void msgCallback(const skku_coverage::skku_coverage_msg::ConstPtr& msg)
{
    ROS_INFO("recieve msg = %d", msg->data);        // data 메시지를 표시한다
}

double Cal_Distance(Coordinate P1, Coordinate P2) // 좌표형태의 구조체 필요
{
  return sqrt(pow((P1.x-P2.x),2) + pow((P1.y-P2.y),2));
}
void Robot_Init()
{
  robot[0].v_max = 1.5;  // m/s
  robot[0].a_max = 1;  // m/s^2
  robot[0].L_max = 0;  // m
  robot[0].t_L_min = 0;  //s
  robot[0].eta = 0;

  robot[1].v_max = 0.5;  // m/s
  robot[1].a_max = 0.5;  // m/s^2
  robot[1].L_max = 0;  // m
  robot[1].t_L_min = 0;  //s
  robot[1].eta = 0;

  robot[2].v_max = 1.5;  // m/s
  robot[2].a_max = 1;  // m/s^2
  robot[2].L_max = 0;  // m
  robot[2].t_L_min = 0;  //s
  robot[2].eta = 0;
}

int Cal_L_min(Coordinate *LTV ,bool state) // LTVs
{
  for(int j=0; j<MAX_LTVs; j++)
  {
    for(int k=0; k<MAX_LTVs; k++)
    {
      if(j!=k)
      {
        L[j][k] = Cal_Distance(LTV[j],LTV[k]);
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
  }
  return 1;
}

int Cal_S(int state)
{
  int b=1;

  for(int j=0; j<MAX_LTVs; j++)
  {
    for(int k=0; k<MAX_LTVs; k++)
    {
      if(j!=k)
      {
        if(fmod(L[j][k],L_min) == 0)
        {
        //  S[j][k] = int(round(L[j][k]/L_min));
          S[j][k] = int((L[j][k]/L_min));

        }
        else
        {
        //  S[j][k] = int(round(L[j][k]/L_min))+b;
          S[j][k] = int((L[j][k]/L_min))+b;
        }
      }
    }
  }
  return 1;
}

int Cal_t_L_min(ROBOT *Robot,int state)
{
  for(int i=0; i<MAX_robots; i++)
  {
    Robot[i].L_max = Robot[i].v_max * (2*Robot[i].v_max/Robot[i].a_max);
    if (L_min <= Robot[i].L_max)
    {
      Robot[i].t_L_min = 2*sqrt(L_min/Robot[i].a_max);
    }
    else
    {
      Robot[i].t_L_min = L_min/Robot[i].v_max + Robot[i].L_max/Robot[i].v_max;
    }
  }
  return 1;
}

int Sel_t_o(ROBOT *Robot,int state)
{
  for(int i=0; i<MAX_robots; i++)
    {
    if(Robot[i].t_L_min >= t_o)
    {
      t_o = Robot[i].t_L_min;
    }
    else
    {
      t_o=t_o;
    }
  }
  return 1;
}

int Cal_eta(ROBOT *Robot, int state)
{
  for(int i=0; i<MAX_robots; i++)
  {
    Robot[i].eta = int(round(t_o/Robot[i].t_L_min));
  }
  return 1;
}

void printSolution(int path[])
{
    cout<<" Solution Exists:";
    cout<<" Following is one Hamiltonian Cycle"<<endl;
    for (int i = 0; i < V; i++)
        cout<<path[i]<<"-";

    cout<<endl;
    //cout<< path[0]<<endl;
    cout<<endl<<"S value  : "<<sum_S<<endl;
}

bool isSafe(int v, int graph[V][V], int path[], int pos)
{
    if (graph [path[pos-1]][v] == 0)
        return false;
   for (int i = 0; i < pos; i++)
        if (path[i] == v)
            return false;
    return true;
}


bool hamCycleUtil(int graph[V][V], int path[], int pos)
{


    if (pos == V) return 1; // Hamilton Path
/*    {
        if (graph[ path[pos-1] ][ path[0] ] > 0) // == 1)
      	{
      	   sum_S = sum_S + graph[ path[pos-1] ][ path[0] ]; // 20171214
           return true;

	      }
        else
            return false;
    }
*/ // Hamintol Cycle

    for (int v = 1; v < V; v++)
    {
        if (isSafe(v, graph, path, pos))
        {
            path[pos] = v;
            if (hamCycleUtil (graph, path, pos+1) == true)
		        {
                sum_S = sum_S + graph[ path[pos-1] ][ path[pos] ]; // 20171214
                //M[0]=LTVs[0];
                M[path[pos]]=LTVs[MAX_LTVs-new_i-1];
                new_i++;
                //cout<<LTVs[path[pos-1]].x<<","<<LTVs[path[pos-1]].y<<"--"<<path[pos-1]<<endl;
           		 	//cout<<"PATH index : "<<path[pos-1]<<"  " <<path[pos]<<endl;
               	return true;
		        }
            path[pos] = -1;
        }
    }
    return false;
}

/* solves the Hamiltonian Cycle problem using Backtracking.*/

bool hamCycle(int graph[V][V])
{
    int *path = new int[V];
    for (int i = 0; i < V; i++)
        path[i] = -1;
    path[0] = 0;
    if (hamCycleUtil(graph, path, 1) == false)
    {
        cout<<"\nSolution does not exist"<<endl;
        return false;
    }
    printSolution(path);
    return true;
}

int Modify_S()
{
  int CheckObstacle=0; // 나중에 함수로 구현

  for(int j=0; j<MAX_LTVs; j++)
  {
    for(int k=0; k<MAX_LTVs; k++)
    {
      if(j!=k)
      {
      /*  if (j==0 && k==1) CheckObstacle=1;
        if (j==0 && k==3) CheckObstacle=1;
        if (j==0 && k==4) CheckObstacle=1;
        if (j==0 && k==5) CheckObstacle=1;
        if (j==0 && k==6) CheckObstacle=1;

        if (j==1 && k==3) CheckObstacle=1;
        if (j==1 && k==4) CheckObstacle=1;
        if (j==1 && k==6) CheckObstacle=1;

        if (j==2 && k==5) CheckObstacle=1;
        if (j==2 && k==7) CheckObstacle=1;

        if if (j==3 && k==4) CheckObstacle=1;
        if if (j==3 && k==5) CheckObstacle=1;*/
    //  if   if (j==3 && k==2) CheckObstacle=0;
/*
        if (j==0  &&  k==4)  CheckObstacle=1;
        if (j==0  &&  k==5) CheckObstacle=1;
        if (j==0  &&  k==6) CheckObstacle=1;
        if (j==0  &&  k==7) CheckObstacle=1;
        if (j==1  &&  k==4) CheckObstacle=1;
        if (j==1  &&  k==5) CheckObstacle=1;
        if (j==1  &&  k==6) CheckObstacle=1;
        if (j==1  &&  k==7) CheckObstacle=1;
        if (j==2  &&  k==4) CheckObstacle=1;
        if (j==2  &&  k==5) CheckObstacle=1;
        if (j==2  &&  k==6) CheckObstacle=1;
        if (j==2  &&  k==7) CheckObstacle=1;
        if (j==2  &&  k==8) CheckObstacle=1;
        if (j==2  &&  k==9) CheckObstacle=1;
        if (j==2  &&  k==10) CheckObstacle=1;
        if (j==2  &&  k==12) CheckObstacle=1;
        if (j==3  &&  k==5) CheckObstacle=1;
        if (j==3  &&  k==7) CheckObstacle=1;
        if (j==3  &&  k==8) CheckObstacle=1;
        if (j==3  &&  k==9) CheckObstacle=1;
        if (j==3  &&  k==10) CheckObstacle=1;
        if (j==3  &&  k==11) CheckObstacle=1;
        if (j==3  &&  k==12) CheckObstacle=1;
        if (j==4  &&  k==8) CheckObstacle=1;
        if (j==4  &&  k==11) CheckObstacle=1;
        if (j==4  &&  k==12) CheckObstacle=1;
        if (j==5  &&  k==8) CheckObstacle=1;
        if (j==5  &&  k==11) CheckObstacle=1;
        if (j==5  &&  k==12) CheckObstacle=1;
        if (j==6  &&  k==9) CheckObstacle=1;
        if (j==6  &&  k==10) CheckObstacle=1;
        if (j==6  &&  k==11) CheckObstacle=1;
        if (j==6  &&  k==12) CheckObstacle=1;
        if (j==7  &&  k==9) CheckObstacle=1;
        if (j==7  &&  k==10) CheckObstacle=1;
        if (j==7  &&  k==11) CheckObstacle=1;
        if (j==7  &&  k==12) CheckObstacle=1;
        if (j==8  &&  k==10) CheckObstacle=1;
        if (j==8  &&  k==12) CheckObstacle=1;*/

        if (j==0  &&  k==9)  CheckObstacle=1;
        if (j==0  &&  k==12) CheckObstacle=1;
        if (j==0  &&  k==11) CheckObstacle=1;
        if (j==0  &&  k==10) CheckObstacle=1;
        if (j==1  &&  k==9) CheckObstacle=1;
        if (j==1  &&  k==12) CheckObstacle=1;
        if (j==1  &&  k==11) CheckObstacle=1;
        if (j==1  &&  k==10) CheckObstacle=1;
        if (j==2  &&  k==8) CheckObstacle=1;
        if (j==2  &&  k==9) CheckObstacle=1;
        if (j==2  &&  k==12) CheckObstacle=1;
        if (j==2  &&  k==11) CheckObstacle=1;
        if (j==2  &&  k==10) CheckObstacle=1;
        if (j==3  &&  k==7) CheckObstacle=1;
        if (j==3  &&  k==8) CheckObstacle=1;
        if (j==3  &&  k==9) CheckObstacle=1;
        if (j==3  &&  k==12) CheckObstacle=1;
        if (j==3  &&  k==11) CheckObstacle=1;
        if (j==3  &&  k==10) CheckObstacle=1;
        if (j==3  &&  k==6) CheckObstacle=1;
        if (j==4  &&  k==7) CheckObstacle=1;
        if (j==4  &&  k==8) CheckObstacle=1;
        if (j==4  &&  k==10) CheckObstacle=1;
        if (j==4  &&  k==11) CheckObstacle=1;
        if (j==5  &&  k==6) CheckObstacle=1;
        if (j==5  &&  k==7) CheckObstacle=1;
        if (j==5  &&  k==8) CheckObstacle=1;
        if (j==5  &&  k==10) CheckObstacle=1;
        if (j==5  &&  k==11) CheckObstacle=1;
        if (j==6  &&  k==12) CheckObstacle=1;
        if (j==6 && k==11) CheckObstacle =1;
        if (j==6  &&  k==9) CheckObstacle=1;
        if (j==6  &&  k==8) CheckObstacle=1;
        if (j==6  &&  k==7) CheckObstacle=1;
        if (j==7  &&  k==9) CheckObstacle=1;
        if (j==7  &&  k==12) CheckObstacle=1;
        if (j==7  &&  k==10) CheckObstacle=1;
        if (j==7  &&  k==11) CheckObstacle=1;
        if (j==8  &&  k==9) CheckObstacle=1;
        if (j==8  &&  k==10) CheckObstacle=1;

        if (CheckObstacle==1)
        {

          S[j][k]=0;
          S[k][j]=0;
          CheckObstacle=0;//임시로
        }
        //LTVs[j].x, LTVs[j].y, LTVs[k].x, LTVs[k].y,mapdata
      }
    }
  }
  return 1;
}


int Cal_O(ROBOT *Robot, int state)
{
  int temp = 0;
  for (int i=0 ; i<MAX_robots ; i++)
    temp = temp + Robot[i].eta;

  for (int i=0 ; i<MAX_robots ; i++)
    Robot[i].o = (int)(sum_S * (double)Robot[i].eta / (double)temp);

//   cout<<"Sum eta : "<<temp<<endl;
//   cout<<"Sum S : "<<sum_S<<endl;

    return 1;

}

int Assign_LTVs()
{

}
