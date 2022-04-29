//#include "Coordinador.h"
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int32.h"
#include "geometry_msgs/Pose2D.h"
//#include "robocup-home-education-los-ultramarinos/RobotData.h"
//#include "robocup-home-education-los-ultramarinos/PolarPoint.h"
#include "tf/tf.h"
#include <cmath>
#include <memory>

int fr = 10; 
bool first = true;
double start = 0;
double now = 0;
int wait = 0;
double time_passed = 0;
std_msgs::String msg;
std_msgs::Int32 mSensorsData;

void activacionTree(const std_msgs::Int32::ConstPtr& pp)
{ 
  mSensorsData = *pp;
  if(first)
  {
      start = ros::Time::now().toSec();
      wait = mSensorsData.data;
      first = false;
  }

  now = ros::Time::now().toSec();
  ROS_INFO("WAITING...");

}

int main(int argc, char** argv)
{

	ros::init(argc, argv, "Coordinador");
	ros::NodeHandle nh;
	ros::Rate loop_rate(fr);


	ros::Publisher treePub = nh.advertise<std_msgs::String>("/status_wait", fr);

	ros::Subscriber Activador = nh.subscribe("/control_wait", fr, activacionTree);

    while (ros::ok())
	{
	if(!first){
        time_passed = now - start;
        if (time_passed < wait)
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
            first = true;
            ROS_INFO("DONE");   
        }
    } 

	ros::spinOnce();
	loop_rate.sleep();

	}

	return 0;

}