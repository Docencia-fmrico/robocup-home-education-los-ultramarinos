#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int32.h"
#include "geometry_msgs/Pose2D.h"
#include <math.h>
#include <string>
#include <iostream>
//#include "robocup_home_education_los_ultramarinos/Info.h"

int fr =  10;
int current_person;
//bool act;
geometry_msgs::Pose2D cSensorsData;
std_msgs::String nSensorsData;
std_msgs::String oSensorsData;
std_msgs::Int32 pSensorsData;
std_msgs::String msg;
std_msgs::Int32 pp;
std_msgs::Int32 pp2;
ros::Publisher talkPub;
ros::Publisher treePub;
ros::Publisher dumpPub;

struct Person {

    std::string color;
    std::string name;
    std::string object;
    int position;
    
};

Person people[3];
int people_counter = 0;
//Person persona;

void personReceived(const std_msgs::String::ConstPtr& rgb_)
{
    people[people_counter].color = rgb_->data;
    std::cout << rgb_->data;
}

void objectReceived(const std_msgs::String::ConstPtr& object_)
{
	oSensorsData = *object_;
    people[people_counter].object = oSensorsData.data;
}

void positionReceived(const std_msgs::Int32::ConstPtr& position_)
{
	pSensorsData = *position_;
    people[people_counter].position = pSensorsData.data;
   
}

void voiceReceived(const std_msgs::String::ConstPtr& name_)
{
	nSensorsData = *name_;
    people[people_counter].name = nSensorsData.data;
    people_counter++;
    ROS_INFO("People visited: %d", people_counter);
}

void dump(ros::Publisher talkPub, ros::Publisher dumpPub, Person people[], int people_counter)
{
    std_msgs::String msg;
    std::stringstream ss;
    std::stringstream status;
    for (int i = 0; i < people_counter; i++)
    {
        ss << people[i].name << " is at position " << people[i].position << ", is wearing a " << people[i].color << " shirt and is holding a " << people[i].object << ".\n";
    }
    msg.data = ss.str();
	talkPub.publish(msg);

	status << "SUCCESS";
	msg.data = status.str();
	dumpPub.publish(msg);
}

void dumpTree(const std_msgs::Int32::ConstPtr& pp2){
    dump(talkPub, dumpPub, people, people_counter);
}

void activacionTree(const std_msgs::Int32::ConstPtr& pp)
{ 
    current_person = pp->data;

    ROS_INFO("CURRENT PERSON: %d\n",current_person);

    ROS_INFO("PEOPLE COUNTER: %d\n",people_counter);

    if(people_counter != current_person){
        std::stringstream ss;
	    ss << "RUNNING";
	    msg.data = ss.str();
	    treePub.publish(msg);
    }
    else if (people_counter < 3){
        std::stringstream ss;
	    ss << "FAILURE";
	    msg.data = ss.str();
	    treePub.publish(msg);
    }
    else{
        std::stringstream ss;
	    ss << "SUCCESS";
	    msg.data = ss.str();
	    treePub.publish(msg);
    }
}











int main(int argc, char** argv)
{

	ros::init(argc, argv, "data_collector");
	ros::NodeHandle nh;
	ros::Rate loop_rate(fr);

    //ros::Publisher cPub = nh.advertise<geometry_msgs::Pose2D> ("/controller_instructions", fr, true);
	talkPub = nh.advertise<std_msgs::String>("/msg_to_say", fr);
	treePub = nh.advertise<std_msgs::String>("/status_data", fr);
    dumpPub = nh.advertise<std_msgs::String>("/status_dump", fr);

	ros::Subscriber colorSub = nh.subscribe<std_msgs::String>("/person_data", fr, personReceived);
	ros::Subscriber listenerSub = nh.subscribe<std_msgs::String>("/info_received", fr, voiceReceived);
    ros::Subscriber objectSub = nh.subscribe<std_msgs::String>("/object_data", fr, objectReceived);
    ros::Subscriber positionSub = nh.subscribe<std_msgs::Int32>("/position_data", fr, positionReceived);
	ros::Subscriber Activador = nh.subscribe("/control_data", fr, activacionTree);
    ros::Subscriber Dumper = nh.subscribe("/data_dump", fr, dumpTree);

    ros::spin();
}