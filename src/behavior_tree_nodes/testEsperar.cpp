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

#include "behavior_tree/testEsperar.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Int32.h"
#include "behaviortree_cpp_v3/behavior_tree.h"
#include "std_msgs/String.h"
#include "ros/ros.h"

namespace behavior_trees
{

testEsperar::testEsperar(const std::string& name , const BT::NodeConfiguration & config): BT::ActionNodeBase(name, config),nh_(),feedBack(" ")
{ 
   
  activador = nh_.advertise<std_msgs::Int32>("/control_wait",10);
  sub = nh_.subscribe<std_msgs::String>("/status_wait", 10, &testEsperar::messageCallback, this);
  
}


void    
testEsperar::messageCallback(const std_msgs::String::ConstPtr& msg)
{
  feedBack = msg->data ;
  std::cout << msg->data  ;
}


void
testEsperar::halt()
{
  ROS_INFO("testEsperar halt");
}

BT::NodeStatus
testEsperar::tick()
{
 
  std_msgs::Int32 WAIT ;
  WAIT.data = 10 ;
 
  
  activador.publish(WAIT);
  
  if (feedBack == "RUNNING") {
     
     activador.publish(WAIT);

     return BT::NodeStatus::RUNNING;
  } 

  if (feedBack == "SUCCESS") {
 
    return BT::NodeStatus::SUCCESS;
  }
    
  
}

}  // namespace behavior_trees


BT_REGISTER_NODES(factory)
{
  factory.registerNodeType<behavior_trees::testEsperar>("testEsperar");
}