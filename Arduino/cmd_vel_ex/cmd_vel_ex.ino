
//======================================================================
#include <ros.h>
#include <geometry_msgs/Twist.h>
//======================================================================
#include <Servo.h>

//=============Setup Drive motors=======================================
Servo left_wheel;
Servo right_wheel; 

//======================================================================
//================Setup node handler and global vars====================
//======================================================================

// Handles startup and shutdown of ROS
ros::NodeHandle nh;

const double whbase = .622;
const double m = 64.286;
const double b = 90;

//======================================================================
//============Setup callback function===================================
//======================================================================

// Take the velocity command as input and calculate the PWM values.
void getvel(const geometry_msgs::Twist& cmdVel) 
{
   
  // Calculate the PWM value given the desired velocity 
  double linear = cmdVel.linear.x;
  double angular = cmdVel.angular.z;

  double vleft = linear - ((angular * whbase) / 2);
  double vright = linear + ((angular * whbase) / 2);

  double left_speed = m*vleft+b;
  double right_speed = m*vright+b;

  drive(left_speed,right_speed);

}

//============Setup Subscriber function=================================

ros::Subscriber<geometry_msgs::Twist> subCmdVel("cmd_vel", &getvel );

//======================================================================

void setup() 
{
  Serial.begin(115200);
  
  // ROS Setup
  nh.getHardware()->setBaud(115200);
  nh.initNode();
  nh.subscribe(subCmdVel); 
   
  left_wheel.attach(13, 1000, 2000);
  right_wheel.attach(12, 1000, 2000);
}

void loop() 
{
  nh.spinOnce();
  delay(10);
}