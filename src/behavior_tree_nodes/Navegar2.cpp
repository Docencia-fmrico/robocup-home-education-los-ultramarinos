#include <string>
#include "behavior_tree/Navegar2.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Bool.h"
#include "geometry_msgs/PoseStamped.h"
#include "move_base_msgs/MoveBaseActionResult.h"
#include "std_msgs/Int64.h"
#include "behaviortree_cpp_v3/behavior_tree.h"
#include "std_msgs/String.h"
#include "ros/ros.h"


namespace behavior_trees
{

Navegar2::Navegar2(const std::string& name , const BT::NodeConfiguration & config): BT::ActionNodeBase(name, config),nh_(),feedBack(-100)
{ 
  int fr = 10 ;
  activador = nh_.advertise<geometry_msgs::PoseStamped>("/navigate_to",fr);
  sub = nh_.subscribe("/resultado_navegacion", fr, &Navegar2::messageCallback, this);
}

void
Navegar2::messageCallback(const std_msgs::Int64::ConstPtr& msg_)
{
  feedBack = msg_->data ;
}

void
Navegar2::halt()
{
  ROS_INFO("Seguir halt");
}

BT::NodeStatus
Navegar2::tick()
{

  if (ac == 5) 
  {

    ROS_INFO(" VUELTA AL ORIGEN ACTIVADA ");

    geometry_msgs::PoseStamped msg;
    
    // donde estara el arbitro 
    msg.header.stamp = i;
    msg.header.frame_id = "map";

    msg.pose.position.x = 0.0;
    msg.pose.position.y = 0.0;
    msg.pose.position.z = 0.0;

    msg.pose.orientation.x = 0.0;
    msg.pose.orientation.y = 0.0;
    msg.pose.orientation.z = 0.0;
    msg.pose.orientation.w = 1.0;

    activador.publish(msg);
  }
  ac++;
  std::cout << "Feedback : " <<  feedBack ;
  
  if(feedBack == 1 )
  {
      
    ROS_INFO(" ORIGEN ALCANSADO ");
    return BT::NodeStatus::SUCCESS;

  }else if(feedBack == -1 ) 
  {

    ROS_INFO(" FALLO EN LA NAVEGACION ");
    return BT::NodeStatus::FAILURE;
  }
    
  ROS_INFO(" LLEGANDO AL ORIGEN ");
  return BT::NodeStatus::RUNNING;
 
}

}  // namespace behavior_trees


BT_REGISTER_NODES(factory)
{
  factory.registerNodeType<behavior_trees::Navegar2>("Navegar2");
}
