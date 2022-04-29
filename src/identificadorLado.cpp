//#include "Coordinador.h"
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Bool.h"
#include "geometry_msgs/Pose2D.h"
//#include "robocup-home-education-los-ultramarinos/RobotData.h"
//#include "robocup-home-education-los-ultramarinos/PolarPoint.h"
#include "tf/tf.h"
#include <cmath>
#include <memory>
bool act = false;
int data_number = 0;
int orientacion = 0;
int fr = 10;
const int max_data = 200;
const int min_dif = max_data/10;

std_msgs::String msg;
geometry_msgs::Pose2D mSensorsData;

void activacionTree(const std_msgs::Bool::ConstPtr& pp)
{ 
  act = pp->data  ;
  ROS_INFO("IDENTIFICADOR ACTIVADO");

}

void movementReceived(const geometry_msgs::Pose2DConstPtr& lado)
{
    if(act)
    {

        mSensorsData = *lado;
        data_number++;
        orientacion = orientacion + mSensorsData.x;

    }
	
}

int main(int argc, char** argv)
{

	ros::init(argc, argv, "identificadorLado");
	ros::NodeHandle nh;
    ros::Rate loop_rate(fr);


	ros::Publisher talkPub = nh.advertise<std_msgs::String>("/nodo_hablar", fr);
	ros::Publisher treePub = nh.advertise<std_msgs::String>("/status_seguimiento", fr);

	ros::Subscriber movementSub = nh.subscribe<geometry_msgs::Pose2D>("/movement_data", fr, movementReceived);
	ros::Subscriber Activador = nh.subscribe("/control_maleta", fr, activacionTree);
	
    while(ros::ok()){
    if(act){
	if(data_number > max_data)
        {
            if(orientacion > min_dif){

                std::stringstream result;
                result << "DERECHA";
                msg.data = result.str();
		        talkPub.publish(msg);

                std::stringstream status;
                status << "SUCCESS";
                msg.data = status.str();
		        treePub.publish(msg);

            }

            else if(orientacion < -min_dif){

                std::stringstream result;
                result << "IZQUIERDA";
                msg.data = result.str();
		        talkPub.publish(msg);

                std::stringstream status;
                status << "SUCCESS";
                msg.data = status.str();
		        treePub.publish(msg);

            }

            else if(-min_dif <= orientacion <= min_dif){

                std::stringstream result;
                result << "FAIL";
                msg.data = result.str();
		        talkPub.publish(msg);

                std::stringstream status;
                status << "FAILURE";
                msg.data = status.str();
		        treePub.publish(msg);

            }

            data_number = 0;
            orientacion = 0;

        }
        else{

            std::stringstream status;
            status << "RUNNING";
            msg.data = status.str();
		    treePub.publish(msg);

        }
    }

    ros::spinOnce();
	loop_rate.sleep();

    }
	
    return 0;

}