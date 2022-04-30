
#include "ros/ros.h"
#include "geometry_msgs/Pose2D.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Bool.h"
#include "visual_behavior/Controlador.hpp"
#include "visual_behavior/PIDController.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string>

double dg = 0 ;
double da = 0 ;
const int fr = 10 ; // frecuencia
double v = 0;
double w = 0;
bool act = false ;

void messageCallback(const geometry_msgs::Pose2D::ConstPtr& msg)
{
  
  dg = msg->y; 
  da = msg->x;

}


void activacionTree(const std_msgs::Bool::ConstPtr& pp)
{ 
  act = pp->data  ;


}

int main(int argc, char** argv){
	ros::init(argc, argv, "Control");

	ros::NodeHandle nh;
	Controlador controlador = Controlador();
    
    ros::Subscriber Activador = nh.subscribe("/control_seguimiento", fr, activacionTree);


	ros::Subscriber sub = nh.subscribe("/controller_instructions", fr, messageCallback);
	ros::Publisher pub_vel_ = nh.advertise<geometry_msgs::Twist>("mobile_base/commands/velocity",fr);

	ros::Publisher pub_err_ = nh.advertise<geometry_msgs::Twist>("errores",fr);

	


    geometry_msgs::Twist cmd;
	geometry_msgs::Twist err;

	ros::Rate loop_rate(fr);
	
    while(ros::ok()){
    if(act)
	{
	
		double errg = controlador.errorGiro(dg) ; 
        double erra = controlador.errorAvance(da) ;


		 err.angular.x = errg;
		 err.linear.y = erra;

     cmd.angular.z = w;
     cmd.linear.x = v;
      
		 w = controlador.velocidadAngular( errg );
     v = controlador.velocidadLineal( erra );


    
	   cmd.angular.z = w;
	   cmd.linear.x = v;
       
	   
	   pub_err_.publish(err);
	   pub_vel_.publish(cmd);     
	}
	
	ros::spinOnce();
    loop_rate.sleep();
}
			
	return 0;
}