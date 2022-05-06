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

#include "behavior_tree/Navegar3.h"


geometry_msgs::PoseStamped positions[7];

namespace behavior_trees
{
Navegar3::Navegar3(const std::string& name, const BT::NodeConfiguration & config):
BT::ActionNodeBase(name, config), nh_(), feedBack(" ")
{
  activador = nh_.advertise<geometry_msgs::PoseStamped>("move_base_simple/goal", 10);
  dataDumper = nh_.advertise<std_msgs::Int32>("/data_dump", 10);
  dataPub = nh_.advertise<std_msgs::Int32>("/position_data", 10);
  sub = nh_.subscribe("/move_base/result", 10, &Navegar3::messageCallback, this);



   
   //P1
    positions[0].header.stamp = i;
    positions[0].header.frame_id = "map";

    positions[0].pose.position.x = 1.5;
    positions[0].pose.position.y = 7.2;
    positions[0].pose.orientation.x = 0;
    positions[0].pose.orientation.y = 0;
    positions[0].pose.orientation.z = 0.707;
    positions[0].pose.orientation.w = 0.707;


    //P2
    positions[1].header.stamp = i;
    positions[1].header.frame_id = "map";

    positions[1].pose.position.x = -1;
    positions[1].pose.position.y = 7;

    positions[1].pose.orientation.x = 0;
    positions[1].pose.orientation.y = 0;
    positions[1].pose.orientation.z = 0.707;
    positions[1].pose.orientation.w = 0.707;


    //P3
        positions[0].header.stamp = i;
    positions[0].header.frame_id = "map";

    positions[2].pose.position.x = -1;
    positions[2].pose.position.y = 7;
    positions[2].pose.orientation.x = 0;
    positions[2].pose.orientation.y = 0;
    positions[2].pose.orientation.z = 1;
    positions[2].pose.orientation.w = 0;

    //P4
    positions[3].header.stamp = i;
    positions[3].header.frame_id = "map";

    positions[3].pose.position.x = 0;
    positions[3].pose.position.y = 4;
    positions[3].pose.orientation.x = 0;
    positions[3].pose.orientation.y = 0;
    positions[3].pose.orientation.z = 1;
    positions[3].pose.orientation.w = 0;

    //P5
        positions[4].header.stamp = i;
    positions[4].header.frame_id = "map";

    positions[4].pose.position.x = 0.4;
    positions[4].pose.position.y = 3;
    positions[4].pose.orientation.x = 0;
    positions[4].pose.orientation.y = 0;
    positions[4].pose.orientation.z = 0.707;
    positions[4].pose.orientation.w = -0.707;

    //P6
    positions[5].header.stamp = i;
    positions[5].header.frame_id = "map";

    positions[5].pose.position.x = 2;
    positions[5].pose.position.y = 5;
    positions[5].pose.orientation.x = 0;
    positions[5].pose.orientation.y = 0;
    positions[5].pose.orientation.z = 0.707;
    positions[5].pose.orientation.w = -0.707;

    //P7
    positions[6].header.stamp = i;
    positions[6].header.frame_id = "map";

    positions[6].pose.position.x = 5;
    positions[6].pose.position.y = 0;
    positions[6].pose.orientation.x = 0;
    positions[6].pose.orientation.y = 0;
    positions[6].pose.orientation.z = 0;
    positions[6].pose.orientation.w = 1;

    
}

void Navegar3::messageCallback(const move_base_msgs::MoveBaseActionResult::ConstPtr& msg)
{
  feedBack = msg->status.text;
  std::cout << "Resultado Navegacion : " << feedBack << "\n";
}

void Navegar3::halt()
{
  ROS_INFO("Seguir halt");
}

BT::NodeStatus Navegar3::tick()
{
  if (counter >= 7){
      std_msgs::Int32 algo;
      algo.data = 1;
      dataDumper.publish(algo);
    return BT::NodeStatus::SUCCESS;
  }
  if (a == 5)
  {
    std::cout << a << "\n";

    geometry_msgs::PoseStamped msg;

    msg = positions[counter];

    activador.publish(msg);
  }
  a++;

  if (feedBack != "")
  {
    if (feedBack == "Goal reached.")
    {
      std_msgs::Int32 pp;
      a = 0;
      counter++;
      pp.data = counter;
      ROS_INFO("POSOTION %d REACHED", counter);
      dataPub.publish(pp);

      feedBack = "";
      return BT::NodeStatus::SUCCESS;
    }
    else
    {
      return BT::NodeStatus::FAILURE;
    }
  }
  return BT::NodeStatus::RUNNING;
}
}  // namespace behavior_trees

BT_REGISTER_NODES(factory)
{
  factory.registerNodeType<behavior_trees::Navegar3>("Navegar3");
}
