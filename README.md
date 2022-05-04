[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-f059dc9a6f8d3a56e377f745f24479a46679e63a5d9fe6f495e02850cd0d8118.svg)](https://classroom.github.com/online_ide?assignment_repo_id=7421586&assignment_repo_type=AssignmentRepo)
# RoboCup

## Launch y Publisher del Monologo
El launch para lanzar solo el nodo monologo con el arbol de TextToSpeech.
```
roslaunch robocup_home_education_los_ultramarinos Main.launch
```

Los publicadores de cada estado.
```
rostopic pub /msg_receive std_msgs/String "Start"
rostopic pub /msg_receive std_msgs/String "Left"
rostopic pub /msg_receive std_msgs/String "Not_vision"
rostopic pub /msg_receive std_msgs/String "Stop"
```

Cambio en el /catkin_ws/src/dialog/dialogflow_ros/dialogflow_ros/scripts/dialogflow_ros/dialogflow_client.py
```py
# line 275: rospy.loginfo(output.print_result(response.query_result))
# line 324: rospy.loginfo(output.print_result(final_result))
```

## Publicar puntos en el navegation

```py
rostopic pub /move_base_simple/goal geometry_msgs/PoseStamped '{ header: {stamp: now, frame_id: "map"}, pose: { position: {x: 0, y: 0, z: 0.0}, orientation: {w: 1.0}}}'
```

## Roslint

```py
catkin_make roslint_robocup_home_education_los_ultramarinos
```
