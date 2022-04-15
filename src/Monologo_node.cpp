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

namespace ph = std::placeholders;

class MonologoDF: public DialogInterface
{
  public:
    MonologoDF(): nh_()
    {
      // this->registerCallback(std::bind(&MonologoDF::noIntentCB, this, ph::_1));
      this->registerCallback(
        std::bind(&MonologoDF::noIntentCB, this, ph::_1),
        "Default Fallback Intent");
      this->registerCallback(
        std::bind(&MonologoDF::startIntentCB, this, ph::_1),
        "Start");
      this->registerCallback(
        std::bind(&MonologoDF::start_followingIntentCB, this, ph::_1),
        "Start_Following");
      this->registerCallback(
        std::bind(&MonologoDF::not_visionIntentCB, this, ph::_1),
        "Not_Vision");
      this->registerCallback(
        std::bind(&MonologoDF::followingIntentCB, this, ph::_1),
        "Following");
      this->registerCallback(
        std::bind(&MonologoDF::stopIntentCB, this, ph::_1),
        "Stop");
    }

    void noIntentCB(dialogflow_ros_msgs::DialogflowResult result)
    {
      ROS_INFO("noIntentCB: %s", result.intent.c_str());
      ROS_INFO("noIntentCB: %s\n", result.fulfillment_text.c_str());
      speak(result.fulfillment_text);
      ros::Duration(3, 0).sleep();
      listen();
    }
    void startIntentCB(dialogflow_ros_msgs::DialogflowResult result)
    {
      ROS_INFO("Operator: %s", result.query_text.c_str());
      ROS_INFO("startIntentCB: %s\n", result.fulfillment_text.c_str());
      speak(result.fulfillment_text);
      ros::Duration(9, 0).sleep();
      listen();
    }
    void start_followingIntentCB(dialogflow_ros_msgs::DialogflowResult result)
    {
      ROS_INFO("Operator: %s", result.query_text.c_str());
      ROS_INFO("start_followingIntentCB: %s\n", result.fulfillment_text.c_str());
      speak(result.fulfillment_text);
      ros::Duration(11, 0).sleep();
      listen();
    }
    void not_visionIntentCB(dialogflow_ros_msgs::DialogflowResult result)
    {
      ROS_INFO("Operator: %s", result.query_text.c_str());
      ROS_INFO("not_visionIntentCB: %s\n", result.fulfillment_text.c_str());
      speak(result.fulfillment_text);
    }
    void followingIntentCB(dialogflow_ros_msgs::DialogflowResult result)
    {
      ROS_INFO("Operator: %s", result.query_text.c_str());
      ROS_INFO("followingIntentCB: %s\n", result.fulfillment_text.c_str());
      speak(result.fulfillment_text);
    }
    void stopIntentCB(dialogflow_ros_msgs::DialogflowResult result)
    {
      ROS_INFO("Operator: %s", result.query_text.c_str());
      ROS_INFO("stopIntentCB: %s\n", result.fulfillment_text.c_str());
      speak(result.fulfillment_text);
    }

  private:
    ros::NodeHandle nh_;
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "Monologo_node");
  MonologoDF forwarder;
  forwarder.tell("Start");
  forwarder.listen();
  // forwarder.tell("Not_Vision");
  // forwarder.tell("Following");
  forwarder.listen();
  ros::spin();
  return 0;
}