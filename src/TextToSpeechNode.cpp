// Copyright 2022 los ultramarinos
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <string>
#include <iostream>
#include "robocup_home_education/MonologoDF.h"
#include "ros/ros.h"
#include "std_msgs/String.h"


ros::Subscriber msg_sub;
ros::Subscriber msg_to_say_sub;
std::string str_speak;
std::string str_tell;
int fr = 10;



void messageReceivedCallback(const std_msgs::String::ConstPtr& msg)
{
  str_tell = msg->data;
}

void messageToSayCallback(const std_msgs::String::ConstPtr& msg)
{

  str_speak = msg->data;
}

int main(int argc, char** argv)
{

	ros::init(argc, argv, "TextToSpeech");

	ros::NodeHandle nh;
  ros::Rate loop_rate(fr);
  MonologoDF forwarder;
  str_speak = "";
  str_tell = "";

  msg_sub = nh.subscribe<std_msgs::String>("/msg_receive", 1000, messageReceivedCallback);
  msg_to_say_sub = nh.subscribe<std_msgs::String>("/msg_to_say", 1000, messageToSayCallback);

  while (ros::ok())
  {
    if(str_speak != "")
    {

      if (forwarder.isListenEnabled())
        {
        forwarder.stopListen();
        }
      forwarder.speak(str_speak);
      str_speak = "";
    }

    if(str_tell != "")
    {

      if (forwarder.isListenEnabled())
        {
        forwarder.stopListen();
        }
      forwarder.tell(str_tell);
      str_tell = "";

    }


  ros::spinOnce();
  loop_rate.sleep();

  }
}