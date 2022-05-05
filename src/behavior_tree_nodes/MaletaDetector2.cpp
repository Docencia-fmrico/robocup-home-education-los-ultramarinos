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

#include "behavior_tree/MaletaDetector2.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Bool.h"
#include "behaviortree_cpp_v3/behavior_tree.h"
#include "std_msgs/String.h"
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sound_play/SoundRequest.h"
#include "geometry_msgs/Pose2D.h"

namespace behavior_trees
{
MaletaDetector2::MaletaDetector2(const std::string& name , const BT::NodeConfiguration & config): BT::ActionNodeBase(name, config)
{ 
  int fr = 10 ;
  sub = nh_.subscribe("/movement_data", fr, &MaletaDetector2::messageCallback, this); 
  activador = nh_.advertise<std_msgs::Bool>("/control_maleta",fr);   
  audio = nh_.advertise<std_msgs::String>("/msg_receive",fr);  
}

void
MaletaDetector2::messageCallback(const geometry_msgs::Pose2D::ConstPtr& msg)
{
  feedBack = msg->x ;
}

void
MaletaDetector2::halt()
{
  ROS_INFO("Seguir halt");
}

BT::NodeStatus
MaletaDetector2::tick()
{  
  
  // espero un poco 
  
  if(a < 10 ){
   //ROS_INFO("Centrar RUNNING Esperansdo");
   ROS_INFO("ESperando MANO RUNNING ");
   std_msgs::Bool act; 
   act.data = true; 
   activador.publish(act);
   a++ ;
   return BT::NodeStatus::RUNNING;
  }

  // cuento durante 10 s

  if( a >=  10 && a < 110 ) {
    if(feedBack ==  1 ){
      derecha++; 
    }else if(feedBack == -1){
      izquierda++;
    }
    ROS_INFO("ESCANEANDO MANO RUNNING ");
    a++ ;
    return BT::NodeStatus::RUNNING;
  }
 
  std::cout << a ;
  
  ROS_INFO("Condiciones ");
  if (derecha > izquierda){
    if (!exito){
      std_msgs::Bool act; 
      act.data = false; 
      activador.publish(act);
      exito=true;

      std_msgs::String lado; 
      lado.data = "right" ;
      audio.publish(lado);

      
    } 
    ROS_INFO("Exito escaneo  Derecha ");  
    return BT::NodeStatus::SUCCESS;
  }
  else if(derecha <= izquierda){
    if (!exito){
      std_msgs::Bool act; 
      act.data = false; 
      activador.publish(act);
      exito=true;  
      
      std_msgs::String lado; 
      lado.data = "left" ;
      audio.publish(lado);
      
    } 
    ROS_INFO("Exito escaneo  Izquierda ");
    return BT::NodeStatus::SUCCESS;

  }else{

    return BT::NodeStatus::RUNNING;;

  }
  

}

}  // namespace behavior_trees


BT_REGISTER_NODES(factory)
{
  factory.registerNodeType<behavior_trees::MaletaDetector2>("MaletaDetector2");
}
