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
#include "sound_play/SoundRequest.h"
#include "std_msgs/String.h"
#include "diagnostic_msgs/DiagnosticArray.h"

namespace behavior_tree
{

  class TextToSpeech2 : public BT::ActionNodeBase
  {
    public:
      explicit TextToSpeech2(const std::string& name, const BT::NodeConfiguration& config);

      void halt();

      BT::NodeStatus tick();

      static BT::PortsList providedPorts()
      {
        return { BT::InputPort<std::string>("message")};
      }
      
      void messageCallback(const diagnostic_msgs::DiagnosticArray::ConstPtr& msg);
      
    private:
      ros::NodeHandle nh;
      ros::Subscriber sub;
      ros::Publisher ad;      
      std::string charla ;
      
      std::string feedback ;
      bool exito = false ;
      int ac = 0;
      
  };

}  // namespace behavior_tree

#endif  // BEHAVIOR_TREE_TEXTTOSPEECH_H
