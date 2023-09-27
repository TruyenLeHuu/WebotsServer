#include <stdio.h>
#include <math.h>
#include <webots/robot.h>
#include <webots/motor.h>
#include <webots/distance_sensor.h>
#include <webots/led.h>
#include <webots/camera.h>
#include <webots/supervisor.h>
#define TIME_STEP 32

// 8 IR ground color sensors
#define NB_GROUND_SENS 8
#define NB_LEDS 5

// IR Ground Sensors
WbDeviceTag gs[NB_GROUND_SENS];

// LEDs 
WbDeviceTag led[NB_LEDS];

// Motors
WbDeviceTag left_motor, right_motor;

/* Phần code được phép chỉnh sửa cho phù hợp */
// Định nghĩa các tín hiệu của xe
#define NOP  -1
#define MID   0
#define LEFT  1
#define RIGHT 2
#define FULL_SIGNAL 3
#define BLANK_SIGNAL 4
#define STOP_SIGNAL 5

// Điểu chỉnh tốc độ phù hợp
#define MAX_SPEED 15
// Khai báo biến
unsigned short threshold[NB_GROUND_SENS] = { 350 , 350 , 350 , 350 , 350 , 350 , 350 , 350 };
unsigned int filted[8] = {0 , 0 , 0 , 0 , 0 , 0 , 0 , 0};

int pos = MID;
int circle_flag = 0;
int pos_Signal = 0;
int DieuKien = 0;

double time_to_turn = 0;
double time_to_stop = 0;

unsigned int intersectionDirect = BLANK_SIGNAL;
unsigned int pre_intersectionDirect = BLANK_SIGNAL;

// Biến lưu giá trị tỉ lệ tốc độ của động cơ
double left_ratio = 0.0;
double right_ratio = 0.0;

//min = 0 , max = 10
void constrain(double *value, double min, double max) {
  if (*value > max) *value = max;
  if (*value < min) *value = min;
}

//Hàm đọc giá trị sensors
void ReadSensors(){
  unsigned short gs_value[NB_GROUND_SENS] = {0, 0, 0, 0, 0, 0, 0, 0};
  
  for(int i=0; i<NB_GROUND_SENS; i++){
    gs_value[i] = wb_distance_sensor_get_value(gs[i]);
    // So sánh giá trị gs_value với threshold -> chuyển đổi sang nhị phân
    if (gs_value[i] < threshold[i])
      filted[i] = 1;
    else filted[i] = 0;
  }
  // Tắt comment để gỡ lỗi
  /*for(int i=0; i<NB_GROUND_SENS; i++){printf("%d\t", gs_value[i]);}*/
}

//xu li tin hieu, flag la tin hieu chi cho pos
int DeterminePosition(unsigned int filted[])
{
  int i = 0;
  int flag_Left = 0, flag_Right = 0 , flag_Sum;
    for (; i < 4 ; i ++)
    {
      if (filted[i] == 1 )  flag_Left ++;
      if (filted[NB_GROUND_SENS - i - 1] == 1) flag_Right++;
    }
    flag_Sum = flag_Left + flag_Right;  // tính tổng tín hiệu 1
    
    //printf("\nflag left : %d", flag_Left);
    //printf ("\nflag Right : %d", flag_Right);
    //printf("\n flag SUm : %d", flag_Sum);
    
    //Xác định blank với mid
    if (flag_Left == flag_Right)
    {
      if (flag_Left == 0)
        return BLANK_SIGNAL;
      else if (flag_Left == 1)
        return MID;
    }
    
    // Xác định bên phải bên trái
    if (flag_Left == 0 && 0 < flag_Right && flag_Right <4)
      return LEFT;
    if (flag_Left > 0 && flag_Left < 4 && flag_Right == 0)
      return RIGHT;
       
     //xac dinh huong re cua nga tu
     if (flag_Sum == 4)
    {
      if (flag_Left > flag_Right)
        intersectionDirect = LEFT;
      if (flag_Left < flag_Right)
        intersectionDirect = RIGHT;
    }
    
    if (flag_Sum > 4 && flag_Sum < 7)
    {
      if ((flag_Left - flag_Right) > 0 && filted[4] != 0)
        intersectionDirect = LEFT;
      else if ((flag_Left - flag_Right) < 0 && filted[3] != 0)
        intersectionDirect = RIGHT;
    }
    
    // Xác định full tín hiệu
    if (flag_Sum >= 6)  
    {
      switch (i)
      {
        case 1 : 
        case 2 :
        case 3 :
        case 4 :
        case 5 :
        case 6 :
        {
          if (filted[i] == 0)
          return BLANK_SIGNAL;
        }
        default : 
        {
          pre_intersectionDirect = FULL_SIGNAL;
          time_to_stop = wb_robot_get_time();
          return FULL_SIGNAL;
        }
      }
    }
  return NOP;
}

void read_Pos ()
{
  wb_robot_step(TIME_STEP);
  ReadSensors();
  pos = DeterminePosition(filted);
}

//hàm giảt tăng tốc cho động cơ với flag = 0 là động cơ trái = 1 là đông cơ phải
void Acceleration(double* first_velo , double last_velo , int flag)
{
    while (fabs(*first_velo - last_velo) > 0.02 )
    {
        *first_velo += (last_velo - *first_velo)/10;
        wb_motor_set_velocity((flag == 0 ) ? left_motor : right_motor , *first_velo * MAX_SPEED);
    }
}

void GoStraight() {
  Acceleration (&left_ratio, 1.6 , 0);
  Acceleration (&right_ratio, 1.6 , 1);
}

void TurnLeft(unsigned int filted[]) {
  for (int i = 0 ; i < 4 ; i ++)
  { 
      if (filted[i] == 1)
      {
        pos_Signal = i;
      }
  }
  Acceleration(&right_ratio,1.58, 1);
  Acceleration(&left_ratio, -0.122*pow(pos_Signal,2) + 0.86*pos_Signal , 0);
}

void TurnRight(unsigned int filted[]) {
    for (int i = 0 ; i < 4 ; i ++)
  { 
      if (filted[NB_GROUND_SENS - i - 1] == 1)
      {
        pos_Signal = i;
      }
  }
  Acceleration (&left_ratio,1.58, 0);
  Acceleration(&right_ratio,-0.122*pow(pos_Signal,2) + 0.86*pos_Signal , 1);
}

void TurnLeftCorner() 
{
  Acceleration(&left_ratio , 0.43 , 0);
  Acceleration(&right_ratio , 1.84 , 1);
}

void TurnRightCorner()
{
  Acceleration(&right_ratio , 0.43 , 1);
  Acceleration(&left_ratio , 1.84 , 0);
}

void Stop()
{
  left_ratio = 0;
  right_ratio = 0;
}

/*
 * This is the main program.
 */
int main() {
  
  /* necessary to initialize webots stuff */
  wb_robot_init();

  /* get and enable the camera and accelerometer */
  WbDeviceTag camera = wb_robot_get_device("camera");
  wb_camera_enable(camera, 64);

  /* initialization */
  char name[20];
  for (int i = 0; i < NB_GROUND_SENS; i++) {
    sprintf(name, "gs%d", i);
    gs[i] = wb_robot_get_device(name); /* ground sensors */
    wb_distance_sensor_enable(gs[i], TIME_STEP);
  }

  for (int i = 0; i < NB_LEDS; i++) {
    sprintf(name, "led%d", i);
    led[i] = wb_robot_get_device(name);
    wb_led_set(led[i], 1);
  }  
  // motors
  left_motor = wb_robot_get_device("left wheel motor");
  right_motor = wb_robot_get_device("right wheel motor");
  wb_motor_set_position(left_motor, INFINITY);
  wb_motor_set_position(right_motor, INFINITY);
  wb_motor_set_velocity(left_motor, 0.0);
  wb_motor_set_velocity(right_motor, 0.0);
    
  // Chương trình sẽ được lặp lại vô tận trong hàm for(;;)
  for (;;)
  {
    read_Pos();
    Stop();
    wb_motor_set_velocity(left_motor, 0);
    wb_motor_set_velocity(right_motor, 0);
    // Run one simulation step
    
    
     //stop
    
       
  }
  wb_robot_cleanup();
  return 0;
}