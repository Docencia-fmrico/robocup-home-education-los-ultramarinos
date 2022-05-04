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

#include "behavior_tree/Iniciar.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Bool.h"
#include "geometry_msgs/PoseStamped.h"

#include "geometry_msgs/Pose2D.h"

#include "behaviortree_cpp_v3/behavior_tree.h"
#include "std_msgs/String.h"
#include "ros/ros.h"


namespace behavior_trees
{

Iniciar::Iniciar(const std::string& name , const BT::NodeConfiguration & config): BT::ActionNodeBase(name, config),nh_(),feedBack(" ")
{ 
   
  sub = nh_.subscribe("/errores", 10, &Iniciar::messageCallback, this);

  ad = nh_.advertise<std_msgs::Bool>("/control_seguimiento",10);


}

void
Iniciar::messageCallback(const geometry_msgs::Twist::ConstPtr& msg)
{
  dg = msg->angular.x ;
  da = msg->linear.y ;

  
  
}


  

void
Iniciar::halt()
{
  ROS_INFO("Seguir halt");
}

BT::NodeStatus
Iniciar::tick()
{
  std::cout << da << " - " << dg << "\n";
  
  std_msgs::Bool act ;
  act.data = true ;
  ad.publish(act);

  if (dg != 0 || da != 0) {
    act.data = false;
    ROS_INFO("INICIANDO");
    ad.publish(act);
    return BT::NodeStatus::SUCCESS;
  }

  
  ROS_INFO("WAITING");
  return BT::NodeStatus::RUNNING;
    

}  // namespace behavior_trees


BT_REGISTER_NODES(factory)
{
  factory.registerNodeType<behavior_trees::Iniciar>("Iniciar");
}

}