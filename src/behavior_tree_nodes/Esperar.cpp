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

#include "behavior_tree/Esperar.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Bool.h"
#include "behaviortree_cpp_v3/behavior_tree.h"
#include "std_msgs/String.h"
#include "ros/ros.h"

namespace behavior_trees
{

Esperar::Esperar(const std::string& name , const BT::NodeConfiguration & config): BT::ActionNodeBase(name, config)
{ 
  
}




void
Esperar::halt()
{
  ROS_INFO("Seguir halt");
}

BT::NodeStatus
Esperar::tick()
{ 
 
  if ( i < 10 ) { 
  ROS_INFO("Espera");
  sleep(1);
  return BT::NodeStatus::RUNNING;
  }
  i++;
   ROS_INFO("Fin de la espera");
  return BT::NodeStatus::SUCCESS;
    
  
}

}  // namespace behavior_trees


BT_REGISTER_NODES(factory)
{
  factory.registerNodeType<behavior_trees::Esperar>("Esperar");
}
