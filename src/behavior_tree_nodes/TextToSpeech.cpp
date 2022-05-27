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

#include "behavior_tree/TextToSpeech.h"

namespace behavior_tree
{

TextToSpeech::TextToSpeech(const std::string& name,  const BT::NodeConfiguration & config)
: BT::ActionNodeBase(name, config)
{
  msg_sub = nh.subscribe<std_msgs::String>("/msg_receive", 1000, &TextToSpeech::messageReceivedCallback, this);
  msg_to_say_sub = nh.subscribe<std_msgs::String>("/msg_to_say", 1000, &TextToSpeech::messageToSayCallback, this);
}

void TextToSpeech::messageReceivedCallback(const std_msgs::String::ConstPtr& msg)
{
  if (forwarder.isListenEnabled())
  {
    forwarder.stopListen();
  }

  forwarder.tell(msg->data);
}

void TextToSpeech::messageToSayCallback(const std_msgs::String::ConstPtr& msg)
{
  if (forwarder.isListenEnabled())
  {
    forwarder.stopListen();
  }

  forwarder.speak(msg->data);
}

void TextToSpeech::halt()
{
  // ROS_INFO("TextToSpeech halt");
}

BT::NodeStatus TextToSpeech::tick()
{
  // ROS_INFO("TextToSpeech tick");
  if (forwarder.isListenEnabled())
  {
    forwarder.listen();
  }
  return BT::NodeStatus::SUCCESS;
}

}  // namespace behavior_tree


BT_REGISTER_NODES(factory)
{
  factory.registerNodeType<behavior_tree::TextToSpeech>("TextToSpeech");
}
