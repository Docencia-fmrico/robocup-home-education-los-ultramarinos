//#include "Coordinador.h"
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Bool.h"
#include "std_msgs/Int32.h"
#include "geometry_msgs/Pose2D.h"
//#include "robocup-home-education-los-ultramarinos/RobotData.h"
//#include "robocup-home-education-los-ultramarinos/PolarPoint.h"
#include "sound_play/SoundRequest.h"
#include "tf/tf.h"
#include <cmath>
#include <memory>

	int fr =  10;

	geometry_msgs::Pose2D person;
	geometry_msgs::Pose2D ball;
	geometry_msgs::Pose2D mSensorsData;

	double latest_person;
	double latest_ball;
	double now;
	const double MAX_WAIT = 1;
	bool found_ball = false;
	bool found_person = false;
	bool done = false;
	bool act = false;	

	geometry_msgs::Pose2D lost_;
	std_msgs::String msg;

void activacionTree(const std_msgs::Bool::ConstPtr& pp)
{ 
  act = pp->data  ;
  ROS_INFO("LRALDSAKSLKSL");

}

void personReceived(const geometry_msgs::Pose2DConstPtr& msg)
{
	mSensorsData = *msg;
	if ((std::abs(mSensorsData.y - person.y) < 0.3 && std::abs(mSensorsData.x - person.x) < 0.3) || !found_person){
	person = *msg;
	latest_person = ros::Time::now().toSec();
	}
}

void ballReceived(const geometry_msgs::Pose2DConstPtr& msg)
{
	mSensorsData = *msg;
	if (std::abs(mSensorsData.x - ball.x) < 0.5 || !found_ball){///////
	ball = *msg;
	latest_ball = ros::Time::now().toSec();
	}//////CAMBIOS**********
	
}

bool lost(double latest_msg, double now){
	
	return(now - latest_msg > MAX_WAIT);
}

void voiceReceived(const std_msgs::Int32::ConstPtr& msg)
{
	if (msg->data == 1){
		done = true;
	}
}


int main(int argc, char** argv)
{

	ros::init(argc, argv, "Coordinador");
	ros::NodeHandle nh;
	ros::Rate loop_rate(fr);
    bool perdido_aviso = true ;
    ros::Publisher cPub = nh.advertise<geometry_msgs::Pose2D> ("/controller_instructions", fr, true);
	std::string lost_warnnig = "I cannot see you, you are too fast for me. Please come back" ;

	ros::Publisher 	hablar = nh.advertise<sound_play::SoundRequest>("/robotsound",10);

	ros::Publisher treePub = nh.advertise<std_msgs::String>("/status_seguimiento", fr);

	ros::Subscriber personSub = nh.subscribe<geometry_msgs::Pose2D>("/person_data", fr, personReceived);
	ros::Subscriber ballSub = nh.subscribe<geometry_msgs::Pose2D>("/ball_data", fr, ballReceived);
	ros::Subscriber listenerSub = nh.subscribe("/listener_data", fr, voiceReceived);
	ros::Subscriber Activador = nh.subscribe("/control_seguimiento", fr, activacionTree);
	
    // ############# CAMBIO JORGE ###############
	geometry_msgs::Pose2D lost_;
	lost_.x = 1;
	//ball.x = 1;
	// ##########################################
	// ***###### IMPORTANTE ########**
    // PONER EN EL MENSAJE LOST DE COORDINADOR EN LA .x EL VALOR DE LA DISTANCIA DE SEGURIDAD
	
	while (ros::ok())
	{

	if(act){
	now = ros::Time::now().toSec();

	sound_play::SoundRequest habla ;
	
	
    if (!lost(latest_ball, now))
    {
		std::cout << "found ball!" << std::endl;
		found_ball = true;
		found_person = false;
		
		//--------------------------------------
		if (!perdido_aviso) {
		sound_play::SoundRequest habla ;
		habla.sound = -1 ;
	    habla.command = 0 ;
	    habla.volume = 1 ;
	    habla.arg = lost_warnnig ;
        hablar.publish(habla) ;
		perdido_aviso = true ;	
		}
        // ----------------------------------------
	    cPub.publish(ball);
    }
    else if (!lost(latest_person, now))
    {
		found_ball = false;
		found_person = true;
		std::cout << "found person!" << std::endl;
		perdido_aviso = true ;
	    cPub.publish(person);
    }
	else
	{
		found_ball = false;
		found_person = false;
		std::cout << "lost all" << std::endl;

        //---------------------------------------------
		if (perdido_aviso) {
		sound_play::SoundRequest habla ;
		habla.sound = -3 ;
	    habla.command = 2 ;
	    habla.volume = 1 ;
	    habla.arg = lost_warnnig ;
        hablar.publish(habla) ;	
		perdido_aviso = false ;
		}
        // -----------------------------------------------
		cPub.publish(lost_);
	}

	if(!done)
	{
		std::stringstream ss;
        ss << "RUNNING";
        msg.data = ss.str();
		treePub.publish(msg);	
		}

	else
	{
		std::stringstream ss;
        ss << "SUCCESS";
        msg.data = ss.str();
		treePub.publish(msg);	
		}

	

	}

	ros::spinOnce();
	loop_rate.sleep();

	}

	return 0;

}