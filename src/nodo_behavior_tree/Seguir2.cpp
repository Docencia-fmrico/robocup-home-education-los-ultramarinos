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

#include "nodo_behavior_tree/Seguir.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Bool.h"
#include "behaviortree_cpp_v3/behavior_tree.h"
#include "ros/ros.h"

namespace behavior_trees
{

Seguir::Seguir(const std::string& name , const BT::NodeConfiguration & config): BT::ActionNodeBase(name, config), counter_(0),A(0.0)
{
  ros::Publisher pub_vel_ = nh_.advertise<std_msgs::Bool>("/seguirdor",10);
}

void
Seguir::halt()
{
  ROS_INFO("Seguir halt");
}

BT::NodeStatus
Seguir::tick()
{
  
  
  std::cout << "Running  \n" ;

  std_msgs::Bool algo;
  algo.data = true;
  pub_vel_.publish(algo);

  return BT::NodeStatus::SUCCESS;
 
  
}

}  // namespace behavior_trees

#include "behaviortree_cpp_v3/bt_factory.h"
BT_REGISTER_NODES(factory)
{
  factory.registerNodeType<behavior_trees::Seguir>("Seguir");
}
