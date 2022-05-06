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

#include "behavior_tree/DataCheck.h"

namespace behavior_trees
{

DataCheck::DataCheck(const std::string& name , const BT::NodeConfiguration & config):
BT::ActionNodeBase(name, config), nh_(), feedBack(" ")
{
  activador = nh_.advertise<std_msgs::Int32>("/control_data", 10);
  sub = nh_.subscribe<std_msgs::String>("/status_data", 10, &DataCheck::messageCallback, this);
}

void DataCheck::messageCallback(const std_msgs::String::ConstPtr& msg)
{
  feedBack = msg->data;
  ROS_INFO("DATA FEEDBACK RECEIVED");
}

void DataCheck::halt()
{
  //ROS_INFO("Seguir halt");
  //std_msgs::Bool act;
  //act.data = false;
  //activador.publish(act);
}

BT::NodeStatus DataCheck::tick()
{
  
  std_msgs::Int32 person;
  person.data = person_counter;
  activador.publish(person);

  if (a < 10){
    ROS_INFO("WAITING %d", a);
    a++;
    return BT::NodeStatus::RUNNING;
  }

  if (feedBack == "FAILURE")
  {
    ROS_INFO("LOOKING FOR MORE PEOPLE");
    a = 0;
    person_counter++;
    return BT::NodeStatus::FAILURE;
    
  }

  else if (feedBack == "SUCCESS")
  {
    ROS_INFO("GOING HOME");
    return BT::NodeStatus::SUCCESS;
    //person_counter++;
  }
  else
  {
    ROS_INFO("TALKING");
    return BT::NodeStatus::RUNNING;
    //person_counter++;
  }
}
}  // namespace behavior_trees


BT_REGISTER_NODES(factory)
{
  factory.registerNodeType<behavior_trees::DataCheck>("DataCheck");
}
