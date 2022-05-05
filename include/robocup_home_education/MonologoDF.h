
  
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

#ifndef ROBOCUP_HOME_EDUCATION_MONOLOGODF_H
#define ROBOCUP_HOME_EDUCATION_MONOLOGODF_H

#include <string>
#include "DialogInterface.h"
#include "std_msgs/Int32.h"
#include "std_msgs/String.h"

namespace ph = std::placeholders;

class MonologoDF: public DialogInterface
{
  public:
    MonologoDF(): nh_()
    {
      this->registerCallback(std::bind(&MonologoDF::noIntentCB, this, ph::_1), "Default Fallback Intent");
      this->registerCallback(std::bind(&MonologoDF::startIntentCB, this, ph::_1), "Start");
      this->registerCallback(std::bind(&MonologoDF::rightIntentCB, this, ph::_1), "Right");
      this->registerCallback(std::bind(&MonologoDF::leftIntentCB, this, ph::_1), "Left");
      this->registerCallback(std::bind(&MonologoDF::not_visionIntentCB, this, ph::_1), "Not_Vision");
      this->registerCallback(std::bind(&MonologoDF::stopIntentCB, this, ph::_1), "Stop");

      this->registerCallback(std::bind(&MonologoDF::welcomeIntentCB, this, ph::_1), "Welcome");

      this->registerCallback(std::bind(&MonologoDF::emptyIntentCB, this, ph::_1), "Empty");

      stop_pub = nh_.advertise<std_msgs::Int32>("/stop_received", 1);
      info_pub = nh_.advertise<std_msgs::String>("/info_received", 1);
      std_msgs::Int32 msg;
      msg.data = 0;
      stop_pub.publish(msg);
    }

    void noIntentCB(dialogflow_ros_msgs::DialogflowResult result)
    {
      ROS_INFO("Operator: %s", result.query_text.c_str());
      ROS_INFO("noIntentCB: %s\n", result.fulfillment_text.c_str());
      speak(result.fulfillment_text);
      // ros::Duration(3, 0).sleep();
      timer = nh_.createTimer(ros::Duration(3), &MonologoDF::timercallback, this, true);
      // enableListen();
    }
    void startIntentCB(dialogflow_ros_msgs::DialogflowResult result)
    {
      bool state = false;
      for (const auto & param : result.parameters)
      {
        if (param.param_name == "Carry")
        {
          state = true;
        }
      }
      if (state)
      {
        ROS_INFO("Operator: %s", result.query_text.c_str());
        ROS_INFO("startIntentCB: %s\n", result.fulfillment_text.c_str());
        speak(result.fulfillment_text);
        // timer = nh_.createTimer(ros::Duration(6), &MonologoDF::timercallback, this, true);
        disableListen();
      }
        
    }
    void rightIntentCB(dialogflow_ros_msgs::DialogflowResult result)
    {
      ROS_INFO("Operator: %s", result.query_text.c_str());
      ROS_INFO("rightIntentCB: %s\n", result.fulfillment_text.c_str());
      speak(result.fulfillment_text);
      timer = nh_.createTimer(ros::Duration(14), &MonologoDF::timercallback, this, true);
    }
    void leftIntentCB(dialogflow_ros_msgs::DialogflowResult result)
    {
      ROS_INFO("Operator: %s", result.query_text.c_str());
      ROS_INFO("leftIntentCB: %s\n", result.fulfillment_text.c_str());
      speak(result.fulfillment_text);
      timer = nh_.createTimer(ros::Duration(14), &MonologoDF::timercallback, this, true);
    }
    void not_visionIntentCB(dialogflow_ros_msgs::DialogflowResult result)
    {
      ROS_INFO("Operator: %s", result.query_text.c_str());
      ROS_INFO("not_visionIntentCB: %s\n", result.fulfillment_text.c_str());
      speak(result.fulfillment_text);
      timer = nh_.createTimer(ros::Duration(3), &MonologoDF::timercallback, this, true);
    }
    void stopIntentCB(dialogflow_ros_msgs::DialogflowResult result)
    {
      ROS_INFO("Operator: %s", result.query_text.c_str());
      ROS_INFO("stopIntentCB: %s\n", result.fulfillment_text.c_str());
      speak(result.fulfillment_text);
      disableListen();
      std_msgs::Int32 msg;
      msg.data = 1;
      stop_pub.publish(msg);
    }
    void welcomeIntentCB(dialogflow_ros_msgs::DialogflowResult result)
    {
      ROS_INFO("Operator: %s", result.query_text.c_str());
      ROS_INFO("welcomeIntentCB: %s\n", result.fulfillment_text.c_str());
      speak(result.fulfillment_text);

      std::string state = "", person = "", object = "";

      for (const auto & param : result.parameters)
      {
        if (param.param_name == "State")
        {
            std::cout << "\t" << param.value[0] << std::endl;
            state = param.value[0];
        }
        if (param.param_name == "person")
        {
            std::cout << "\t" << param.value[0] << std::endl;
            person = param.value[0];
        }
        if (param.param_name == "any")
        {
            std::cout << "\t" << param.value[0] << std::endl;
            object = param.value[0];
        }
      }
      std_msgs::String msg;
      msg.data = person;
      //msg.object = object;
      if (person != "")
      {
        disableListen();
        info_pub.publish(msg);
      }
      else
      {
        timer = nh_.createTimer(ros::Duration(2), &MonologoDF::timercallback, this, true);
      }
    }
    void emptyIntentCB(dialogflow_ros_msgs::DialogflowResult result)
    {
      // enableListen();
    }
    void timercallback(const ros::TimerEvent & event)
    {
      enableListen();
    }

  private:
    ros::NodeHandle nh_;
    ros::Publisher stop_pub;
    ros::Publisher info_pub;
    ros::Timer timer;
};
#endif  // ROBOCUP_HOME_EDUCATION_MONOLOGODF_H