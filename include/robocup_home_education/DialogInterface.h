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

#ifndef ROBOCUP_HOME_EDUCATION_DIALOGINTERFACE_H
#define ROBOCUP_HOME_EDUCATION_DIALOGINTERFACE_H

#include <ros/ros.h>
#include <string>
#include <dialogflow_ros_msgs/DialogflowResult.h>
#include <boost/algorithm/string/replace.hpp>
#include <std_srvs/Empty.h>
#include <std_msgs/Bool.h>
#include <std_msgs/String.h>
#include <sound_play/sound_play.h>
#include <regex>
#include <map>

class DialogInterface
{
public:
  using DialogflowResult = dialogflow_ros_msgs::DialogflowResult;

  DialogInterface();

  bool speak(std::string str);
  bool listen();
  bool stopListen();
  bool tell(std::string str);
  void enableListen();
  void disableListen();
  bool isListenEnabled();

  virtual void listenCallback(DialogflowResult result) {}
  void registerCallback(
    std::function<void(const DialogflowResult & result)> cb,
    std::string intent = "(.*)");

private:
  bool idle_;
  bool enable_listen;
  ros::NodeHandle nh_;
  std::string results_topic_, start_srv_, stop_srv_;
  ros::ServiceClient sound_client_;
  ros::Subscriber df_result_sub_;
  ros::Publisher listening_gui_, speak_gui_, tell_gui_;
  std::regex intent_re_;

  std::map<std::string, std::function<void(const DialogflowResult & result)>> registered_cbs_;

  sound_play::SoundClient sc_;

  void init();
  void dfCallback(const DialogflowResult::ConstPtr& result);
};
#endif  // ROBOCUP_HOME_EDUCATION_DIALOGINTERFACE_H
