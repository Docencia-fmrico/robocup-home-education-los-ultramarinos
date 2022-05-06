// Copyright 2019 Intelligent Robotics Lab
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

#include "behavior_tree/Observar.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Bool.h"

#include "behaviortree_cpp_v3/behavior_tree.h"
#include "std_msgs/String.h"
#include "ros/ros.h"

namespace behavior_trees
{

Observar::Observar(const std::string& name , const BT::NodeConfiguration & config): BT::ActionNodeBase(name, config),nh_(),feedBack(" ")
{   
  activador = nh_.advertise<std_msgs::Bool>("/control_observador",10);
  talkPub = nh_.advertise<std_msgs::String>("/msg_receive",10);
  sub = nh_.subscribe<std_msgs::String>("/status_observador", 10, &Observar::messageCallback, this); 
  failures = 0;
}


void
Observar::messageCallback(const std_msgs::String::ConstPtr& msg)
{
  feedBack = msg->data;
  ROS_INFO("OBSERVER FEEDBACK RECEIVED");
}


void
Observar::halt()
{
  ROS_INFO("Observar halt");
}

BT::NodeStatus
Observar::tick()
{

  std_msgs::Bool act;
  if (a == 5){
  act.data = true;
  activador.publish(act);
  }
  a++;
  
  if (feedBack == "RUNNING") {
      failures++;
      if(failures > 200){
        act.data = false;
        activador.publish(act);
        failures = 0;
        a = 0;
        ROS_INFO("Observador failure\n");
         return BT::NodeStatus::FAILURE;
        }
        else{
        ROS_INFO("Observador failing\n");
         return BT::NodeStatus::RUNNING;
         }

  }
  else if (feedBack == "SUCCESS") {
    act.data = false;
    activador.publish(act);
    std_msgs::String msg;
    std::stringstream dialog;
	  dialog << "Welcome";
  	msg.data = dialog.str();
  	talkPub.publish(msg);
        failures = 0;
        a = 0;
        ROS_INFO("Observador success\n");
        feedBack = "FAILURE";
    return BT::NodeStatus::SUCCESS;
  }
  else if (feedBack == "FAILURE") {
      failures++;
      if(failures > 100){
        act.data = false;
        activador.publish(act);
        failures = 0;
        a = 0;
        ROS_INFO("Observador failure\n");
         return BT::NodeStatus::FAILURE;
         
        }
      else{
        ROS_INFO("Observador failing\n");
         return BT::NodeStatus::RUNNING;
         }
      }
    else{
        ROS_INFO("Observador no feedback\n");
         return BT::NodeStatus::RUNNING;
      }
     
}
}  // namespace behavior_trees


BT_REGISTER_NODES(factory)
{
  factory.registerNodeType<behavior_trees::Observar>("Observar");
}