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
    
};

struct RGB {

    double r;
    double g;
    double b;
    std::string name;

};

Person people[3];
int people_counter = 0;
//Person persona;

RGB violet = {
    148,
    0,
    211,
    "VIOLET",
};

RGB purple = {
    75,
    0,
    130,
    "PURPLE",
};

RGB blue = {
    0,
    0,
    255,
    "BLUE",
};

RGB green = {
    0,
    255,
    0,
    "GREEN",
};

RGB yellow = {
    255,
    255,
    0,
    "YELLOW",
};

RGB orange = {
    255,
    127,
    0,
    "ORANGE",
};

RGB red = {
    255,
    0,
    0,
    "RED",
};

RGB black = {
    0,
    0,
    0,
    "BLACK",
};

RGB white = {
    255,
    255,
    255,
    "WHITE",  
};

RGB rainbow[] = {violet,purple,blue,green,yellow,orange,red,black,white};
double ColourDistance(RGB e1, RGB e2)
{
  long rmean = ( (long)e1.r + (long)e2.r ) / 2;
  long r = (long)e1.r - (long)e2.r;
  long g = (long)e1.g - (long)e2.g;
  long b = (long)e1.b - (long)e2.b;
  return sqrt((((512+rmean)*r*r)>>8) + 4*g*g + (((767-rmean)*b*b)>>8));
}

std::string getColor(RGB valores){
    double d_min;
    int result;
    for(int i = 0; i < 9; i++)
    {
        if (( i == 0) || (ColourDistance(valores, rainbow[i]) < d_min) )
        {
            d_min = ColourDistance(valores, rainbow[i]);
            result = i;
            }
        //ROS_INFO("%d", d_min);
    }
    //ROS_INFO("%d", result);
    //ROS_INFO("%s", rainbow[result].name);
    std::cout << rainbow[result].name << std::endl;
    return rainbow[result].name;

}

void personReceived(const geometry_msgs::Pose2DConstPtr& rgb_)
{
	cSensorsData = *rgb_;
    //persona = people[people_counter];

    RGB color ={
        cSensorsData.x,
        cSensorsData.y,
        cSensorsData.theta,
        "unkown",
    };
    people[people_counter].color = getColor(color);
}

void objectReceived(const std_msgs::String::ConstPtr& object_)
{
	oSensorsData = *object_;
    people[people_counter].object = oSensorsData.data;
}

void voiceReceived(const std_msgs::String::ConstPtr& name_)
{
	nSensorsData = *name_;
    people[people_counter].name = nSensorsData.data;
    people_counter++;
}

void dump(ros::Publisher talkPub, ros::Publisher dumpPub, Person people[], int people_counter)
{
    std_msgs::String msg;
    std::stringstream ss;
    std::stringstream status;
    for (int i = 0; i < people_counter; i++)
    {
        ss << people[i].name << " is wearing a " << people[i].color << " shirt and is holding a " << people[i].object << ".\n";
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

    if(people_counter != current_person){
        std::stringstream ss;
	    ss << "RUNNING";
	    msg.data = ss.str();
	    treePub.publish(msg);
    }
    else if (people_counter < 1){

        //talk(talkPub, people, people_counter);
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

	ros::Subscriber colorSub = nh.subscribe<geometry_msgs::Pose2D>("/person_data", fr, personReceived);
	ros::Subscriber listenerSub = nh.subscribe<std_msgs::String>("/info_received", fr, voiceReceived);
    ros::Subscriber objectSub = nh.subscribe<std_msgs::String>("/object_data", fr, objectReceived);
	ros::Subscriber Activador = nh.subscribe("/control_data", fr, activacionTree);
    ros::Subscriber Dumper = nh.subscribe("/data_dump", fr, dumpTree);

    ros::spin();
}