
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

float LTVs_x_R1[100]={0};
float LTVs_y_R1[100]={0};

float LTVs_x_R2[100]={0};
float LTVs_y_R2[100]={0};

float LTVs_x_R3[100]={0};
float LTVs_y_R3[100]={0};

int index_r1 = 0;
int index_r2 = 0;
int index_r3 = 0;		


void ReceiveTaskInfo(const coverage::task_info::ConstPtr& m);
void LTV_Assign(float UTM_X, float UTM_Y, int index);
void CalPoints2Line();
void LTV_Sorting();
void Display();
void Publish2Robots();
void Publish2RVIZ();

float CheckLineUpDown(float A, float B, float UTM_X, float UTM_Y);
float DisPoint2Point(float x1, float y1, float x2, float y2);
