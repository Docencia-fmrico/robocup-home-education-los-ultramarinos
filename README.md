[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-f059dc9a6f8d3a56e377f745f24479a46679e63a5d9fe6f495e02850cd0d8118.svg)](https://classroom.github.com/online_ide?assignment_repo_id=7421586&assignment_repo_type=AssignmentRepo)
# RoboCup
## Índice
- [Árbol](#Árbol)
- [Nodos](#Nodos)
- [Control y Coordinación](#Control-y-Coordinación)
- [Diálogo](#Diálogo)
- [Launch y Publisher](#Launch-y-Publisher)
- [Roslint](#Roslint)

## Árbol
![image](https://user-images.githubusercontent.com/90789825/167591318-2f4ab73a-b55f-4a79-a540-5e6442a55cc0.png)

## Nodos
### 1.1. Navegar
```
BT::NodeStatus Navegar::tick()
{
  if (a == 5)
  {
    std::cout << a << "\n";

    geometry_msgs::PoseStamped msg;

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
  a++;

  if (feedBack != "")
  {
    if (feedBack == "Goal reached.")
    {
      return BT::NodeStatus::SUCCESS;
    }
    else
    {
      return BT::NodeStatus::FAILURE;
    }
  }
  return BT::NodeStatus::RUNNING;
```
### 1.2. Navegar
```
std_msgs::Bool act;
act.data = true;
activador.publish(act);

if (feedBack == "RUNNING")
{
  return BT::NodeStatus::RUNNING;
}

if (feedBack == "SUCCESS")
{
  act.data = false;

  for (int i = 0; i < 5; i++)
  {
    activador.publish(act);
  }
  return BT::NodeStatus::SUCCESS;
}
```
### 1.3. Navegar2
```
if (a == 5)
{
  std::cout << a << "\n";

  geometry_msgs::PoseStamped msg;

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
a++;

if (feedBack != "")
{
  if (feedBack == "Goal reached.")
  {
    return BT::NodeStatus::SUCCESS;
  }
  else
  {
    return BT::NodeStatus::FAILURE;
  }
}
return BT::NodeStatus::RUNNING;
```
## Control y Coordinación

## Diálogo
### 2.1. TextToSpeech.cpp
```
if (forwarder.isListenEnabled())
  {
    forwarder.listen();
  }
return BT::NodeStatus::SUCCESS;
```
### 2.2. DialogInterface.cpp
```
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
```
### 2.3. MonologoDF.h
```
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
  info_pub = nh_.advertise<robocup_home_education_los_ultramarinos::Info>("/info_received", 1);
  std_msgs::Int32 msg;
  msg.data = 0;
  stop_pub.publish(msg);
}
```
## Launch y Publisher

Los publicadores de cada estado del diálogo.
```
rostopic pub /msg_receive std_msgs/String "Start"
rostopic pub /msg_receive std_msgs/String "Left"
rostopic pub /msg_receive std_msgs/String "Not_vision"
rostopic pub /msg_receive std_msgs/String "Stop"

rostopic pub /msg_receive std_msgs/String "Welcome"
```

Cambio en el /catkin_ws/src/dialog/dialogflow_ros/dialogflow_ros/scripts/dialogflow_ros/dialogflow_client.py
```py
# line 275: rospy.loginfo(output.print_result(response.query_result))
# line 324: rospy.loginfo(output.print_result(final_result))
```

Publicar puntos en el navegation

```py
rostopic pub /move_base_simple/goal geometry_msgs/PoseStamped '{ header: {stamp: now, frame_id: "map"}, pose: { position: {x: 0, y: 0, z: 0.0}, orientation: {w: 1.0}}}'
```

## Roslint

```py
catkin_make roslint_robocup_home_education_los_ultramarinos
```
