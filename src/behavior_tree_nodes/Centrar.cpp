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
#include "behavior_tree/Centrar.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Bool.h"
#include "geometry_msgs/PoseStamped.h"
#include "geometry_msgs/Pose2D.h"
#include "behaviortree_cpp_v3/behavior_tree.h"
#include "std_msgs/String.h"
#include "ros/ros.h"
#include <math.h>


namespace behavior_trees
{

Centrar::Centrar(const std::string& name , const BT::NodeConfiguration & config): BT::ActionNodeBase(name, config),nh_(),feedBack(" ")
{    
  sub = nh_.subscribe("/errores", 10, &Centrar::messageCallback, this);
  ad = nh_.advertise<std_msgs::Bool>("/control_seguimiento",10);
}

void
Centrar::messageCallback(const geometry_msgs::Twist::ConstPtr& msg)
{
  dg = msg->angular.x ;
  da = msg->linear.y ;
 
}

void
Centrar::halt()
{
  ROS_INFO("Seguir halt");
}

BT::NodeStatus
Centrar::tick()
{

  
  //esperamos un poco

  if(a < 10){
  //ROS_INFO("Centrar RUNNING Esperansdo");
  std_msgs::Bool act ;
  act.data = true ;
  ad.publish(act);
  a++ ;
  return BT::NodeStatus::RUNNING;
  }
  
  // 
  bool centrado = abs(dg) <= lim_g && abs(da) <= lim_a ;
  //std::cout << "Error distacia : " << da << " Error angulo : " << dg << "Condicion abs(dg) <= lim_g :" << centrado << " \n";

  if (centrado) {
    
    if(!exito){
     for(int i = 0 ; i < 5 ; i++ ){  
      std_msgs::Bool act ;
      act.data = false;
      ROS_INFO("Exito en el centrado");
      ad.publish(act);
    }
    exito = true;
    }

    return BT::NodeStatus::SUCCESS;
  }

  
  //ROS_INFO("Centrar RUNNING");
  return BT::NodeStatus::RUNNING;
    

}  // namespace behavior_trees


BT_REGISTER_NODES(factory)
{
  factory.registerNodeType<behavior_trees::Centrar>("Centrar");
}

}