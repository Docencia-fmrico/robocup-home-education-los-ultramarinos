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

#include "behavior_tree/MaletaDetector.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Bool.h"
#include "behaviortree_cpp_v3/behavior_tree.h"
#include "std_msgs/String.h"
#include "ros/ros.h"
#include "std_msgs/String.h"

namespace behavior_trees
{

MaletaDetector::MaletaDetector(const std::string& name , const BT::NodeConfiguration & config): BT::ActionNodeBase(name, config)
{ 
  int fr = 10 ;
  sub = nh_.subscribe<std_msgs::String>("/status_maleta", fr, &MaletaDetector::messageCallback, this); 
  activador = nh_.advertise<std_msgs::Bool>("/control_maleta",fr);
}

void
MaletaDetector::messageCallback(const std_msgs::String::ConstPtr& msg)
{
  feedBack = msg->data ;
  std::cout << msg->data  ;
}

void
MaletaDetector::halt()
{
  ROS_INFO("Seguir halt");
}

BT::NodeStatus
MaletaDetector::tick()
{ 
  std_msgs::Bool act ;
  act.data = true ;
  activador.publish(act);
  
  if (feedBack == "RUNNING") {
    ROS_INFO("ESCANEANDO DIRECCION ") ;
    return BT::NodeStatus::RUNNING;
  } 

  if (feedBack == "SUCCESS") {
    act.data = false ;
    activador.publish(act);
    ROS_INFO("DIRECCION ENCONTRADA ") ;
    return BT::NodeStatus::SUCCESS;
  }
  
   return BT::NodeStatus::FAILURE;
}

}  // namespace behavior_trees


BT_REGISTER_NODES(factory)
{
  factory.registerNodeType<behavior_trees::MaletaDetector>("MaletaDetector");
}
