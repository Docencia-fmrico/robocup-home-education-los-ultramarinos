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

#include <robocup_home_education/DialogInterface.h>

#include <string>
#include <utility>

DialogInterface::DialogInterface()
: nh_(),
  sc_(nh_, "/robotsound"),
  enable_listen(false)
{
  init();
}

void DialogInterface::init()
{
  if (!nh_.getParam("/dialogflow_client/results_topic", results_topic_))
    results_topic_ = "/dialogflow_client/results";
  if (!nh_.getParam("/dialogflow_client/start_srv", start_srv_))
    start_srv_ = "/dialogflow_client/start";
  // if (!nh_.getParam("/dialogflow_client/stop_srv", stop_srv_))
    stop_srv_ = "/dialogflow_client/stop";

  df_result_sub_ = nh_.subscribe(results_topic_, 1, &DialogInterface::dfCallback, this);
  listening_gui_ = nh_.advertise<std_msgs::Bool>("/dialog_gui/is_listening", 1, true);
  speak_gui_ = nh_.advertise<std_msgs::String>("/dialog_gui/talk", 1, true);
  tell_gui_ = nh_.advertise<std_msgs::String>("/dialogflow_client/requests/string_msg", 1, true);

  std_msgs::String str_msg;
  std_msgs::Bool bool_msg;
  str_msg.data = "";
  speak_gui_.publish(str_msg);
  bool_msg.data = false;
  listening_gui_.publish(bool_msg);
}

void DialogInterface::registerCallback(
  std::function<void(const DialogflowResult & result)> cb,
  std::string intent)
{
  registered_cbs_.insert(
    std::pair<std::string, std::function<void(const DialogflowResult & result)>>
    (intent, cb));
}

void DialogInterface::dfCallback(const DialogflowResult::ConstPtr& result)
{ 
  auto bool_msg = std_msgs::Bool();
  bool_msg.data = false;
  listening_gui_.publish(bool_msg);

  if (result->intent.size() > 0)
  {
    for (auto item : registered_cbs_)
    {
      std::regex intent_re = std::regex(item.first);
      if (std::regex_match(result->intent, intent_re))
      {
        item.second(*result);
      }
    }
  }
  else{
    auto item = registered_cbs_["Empty"];
    item(*result);
  }
}

bool DialogInterface::speak(std::string str)
{
  boost::replace_all(str, "_", " ");
  std_msgs::String msg;
  msg.data = str;
  speak_gui_.publish(msg);

  sc_.say(str);
  return true;
}

bool DialogInterface::tell(std::string str)
{
  boost::replace_all(str, "_", " ");
  std_msgs::String msg;
  msg.data = str;
  tell_gui_.publish(msg);
  return true;
}

bool DialogInterface::listen()
{
  std_srvs::Empty srv;
  std_msgs::Bool msg;
  msg.data = true;
  listening_gui_.publish(msg);
  ROS_INFO("[DialogInterface] listening...");
  ros::ServiceClient df_srv = nh_.serviceClient<std_srvs::Empty>(start_srv_, 1);
  df_srv.call(srv);
  return true;
}

bool DialogInterface::stopListen()
{
  std_srvs::Empty srv;
  std_msgs::Bool msg;
  msg.data = false;
  listening_gui_.publish(msg);
  ROS_INFO("[DialogInterface] Stopping listening");
  enable_listen = false;
  // ros::ServiceClient df_srv = nh_.serviceClient<std_srvs::Empty>(stop_srv_, 1);
  // df_srv.call(srv);
  return true;
}

void DialogInterface::enableListen()
{
  enable_listen = true;
}

void DialogInterface::disableListen()
{
  enable_listen = false;
}

bool DialogInterface::isListenEnabled()
{
  return enable_listen;
}
