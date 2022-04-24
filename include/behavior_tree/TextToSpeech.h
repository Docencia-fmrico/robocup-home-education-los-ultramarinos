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

#ifndef BEHAVIOR_TREE_TEXTTOSPEECH_H
#define BEHAVIOR_TREE_TEXTTOSPEECH_H

#include <string>
#include "behaviortree_cpp_v3/behavior_tree.h"
#include "behaviortree_cpp_v3/bt_factory.h"
#include "robocup_home_education/MonologoDF.h"
#include "ros/ros.h"

namespace behavior_tree
{

  class TextToSpeech : public BT::ActionNodeBase
  {
    public:
      explicit TextToSpeech(const std::string& name, const BT::NodeConfiguration& config);

      void halt();

      BT::NodeStatus tick();

      static BT::PortsList providedPorts()
      {
        return { BT::OutputPort<std::string>("object")};
      }
    private:
      ros::NodeHandle nh;
      ros::Subscriber msg_sub;
      MonologoDF forwarder;

      void messageReceivedCallback(const std_msgs::String::ConstPtr& msg);
  };

}  // namespace behavior_tree

#endif  // BEHAVIOR_TREE_TEXTTOSPEECH_H
