
#include "ros/ros.h"
#include "geometry_msgs/Pose2D.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Bool.h"
#include "std_msgs/String.h"
#include "visual_behavior/Controlador.hpp"
#include "visual_behavior/PIDController.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "diagnostic_msgs/DiagnosticArray.h"
#include "sound_play/SoundRequest.h"



const int fr = 10 ; // frecuencia
std::string feedback = "";
std::string charla = "" ;

void messageCallback(const diagnostic_msgs::DiagnosticArray::ConstPtr& msg)
{ 
  std::cout << msg->status[0].message << "\n" ;
  feedback = msg->status[0].message ;
}

void messageCallback_(const std_msgs::String::ConstPtr& msg)
{ 
  std::cout << msg->data << "\n" ;
  charla = msg->data ;
}


int main(int argc, char** argv){
	ros::init(argc, argv, "Speaker");

	ros::NodeHandle nh;
    ros::Subscriber sub = nh.subscribe("/diagnostics", fr, messageCallback);
	ros::Subscriber sub_ = nh.subscribe("/charla", fr, messageCallback_);
	
	ros::Publisher pub = nh.advertise<sound_play::SoundRequest>("/robotsound",fr);
	
	while(ros::ok()){
         
		if(feedback == "0 sounds playing"){

			sound_play::SoundRequest habla ;
			habla.sound = -3 ;
			habla.command = 1 ;
			habla.volume = 1 ;
			habla.arg = charla ;
			pub.publish(habla) ;
		}
    

	ros::spinOnce();
	}

	return 0;
}