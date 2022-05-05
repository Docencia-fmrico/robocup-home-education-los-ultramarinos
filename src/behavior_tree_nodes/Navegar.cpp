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
#include "behavior_tree/Navegar.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Bool.h"
#include "geometry_msgs/PoseStamped.h"
#include "move_base_msgs/MoveBaseActionResult.h"
#include "std_msgs/Int64.h"
#include "behaviortree_cpp_v3/behavior_tree.h"
#include "std_msgs/String.h"
#include "ros/ros.h"


namespace behavior_trees
{

Navegar::Navegar(const std::string& name , const BT::NodeConfiguration & config): BT::ActionNodeBase(name, config),nh_(),feedBack(-100)
{ 
  int fr = 10 ;
  activador = nh_.advertise<geometry_msgs::PoseStamped>("/navigate_to",fr);
  sub = nh_.subscribe("/resultado_navegacion", fr, &Navegar::messageCallback, this);
}

void
Navegar::messageCallback(const std_msgs::Int64::ConstPtr& msg_)
{
  feedBack = msg_->data ;
}

void
Navegar::halt()
{
  ROS_INFO("Seguir halt");
}

BT::NodeStatus
Navegar::tick()
{

  if (ac == 5) 
  {
    //ROS_INFO(" POSICION ARBITRO COMUNICADA ");

    geometry_msgs::PoseStamped msg;
    
    // donde estara el arbitro 
    msg.header.stamp = i;
    msg.header.frame_id = "map";

    msg.pose.position.x = 0;
    msg.pose.position.y = 3.0;
    msg.pose.position.z = 0.0;

    msg.pose.orientation.x = 0.0;
    msg.pose.orientation.y = 0.0;
    msg.pose.orientation.z = 0.9238795;
    msg.pose.orientation.w = -0.3826834;

    activador.publish(msg);
  }
  ac++;
  //std::cout << "Feedback : " <<  feedBack ;
  
  if(feedBack == 1 )
  {
      
    ROS_INFO(" ARBITRO ALCANSADO ");
    return BT::NodeStatus::SUCCESS;

  }else if(feedBack == -1 ) 
  {

    ROS_INFO(" FALLO EN LA NAVEGACION ");
    return BT::NodeStatus::FAILURE;
  }
    
  ROS_INFO(" LLEGANDO A LA POSICION ARBITRO ");
  return BT::NodeStatus::RUNNING;
 
}

}  // namespace behavior_trees


BT_REGISTER_NODES(factory)
{
  factory.registerNodeType<behavior_trees::Navegar>("Navegar");
}
