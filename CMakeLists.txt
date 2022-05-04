cmake_minimum_required(VERSION 3.0.2)
project(robocup_home_education_los_ultramarinos)

add_compile_options(-std=c++17)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  #dialogflow_ros_msgs
  roslint
  std_srvs
  std_msgs
  #sound_play
  actionlib
  actionlib_msgs
  geometry_msgs 
  message_generation 
  cv_bridge 
  image_transport
  tf
  behaviortree_cpp_v3
)

catkin_package(
	INCLUDE_DIRS include
	LIBRARIES robocup_home_education_los_ultramarinos behaviortree_cpp_v3
	CATKIN_DEPENDS roscpp dialogflow_ros_msgs actionlib_msgs
	DEPENDS
)

###########
## Build ##
###########

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${PCL_INCLUDE_DIRS}
  ${ZMQ_INCLUDE_DIRS}
)

link_directories(
	${PCL_LIBRARY_DIRS}
)

add_definitions(${PCL_DEFINITIONS})

add_library(robocup_home_education_los_ultramarinos
  src/robocup_home_education/PIDController.cpp
  src/robocup_home_education/Controlador.cpp
)

list(APPEND plugin_libs
  
)

#add_library(TextToSpeech SHARED src/behavior_tree_nodes/TextToSpeech.cpp)
add_library(Seguir SHARED src/behavior_tree_nodes/Seguir.cpp)
add_library(Navegar SHARED src/behavior_tree_nodes/Navegar.cpp)
add_library(Esperar SHARED src/behavior_tree_nodes/Esperar.cpp)
add_library(Navegar2 SHARED src/behavior_tree_nodes/Navegar2.cpp)
add_library(Centrar SHARED src/behavior_tree_nodes/Centrar.cpp)
add_library(Iniciar SHARED src/behavior_tree_nodes/Iniciar.cpp)


add_dependencies(robocup_home_education_los_ultramarinos ${catkin_EXPORTED_TARGETS} )#sound_play_generate_messages_cpp
target_link_libraries(robocup_home_education_los_ultramarinos ${catkin_LIBRARIES})
## Declare a C++ library
# add_library(${PROJECT_NAME}
#   src/${PROJECT_NAME}/robocup-home-education-los-ultramarinos.cpp
# )

## Declare a C++ executable
## With catkin_make all packages are built within a single CMake context
## The recommended prefix ensures that target names across packages don't collide

#add_executable(ObservadorPersonas_rc src/robocup_home_education/ObservadorPersonas.cpp)
add_executable(Coordinador_rc src/robocup_home_education/Coordinador.cpp)
add_executable(ObservadorPelotasHSV_rc src/robocup_home_education/ObservadorPelotasHSV.cpp)
add_executable( Control_rc src/robocup_home_education/Control.cpp ) 

add_executable( Graficos_rc src/robocup_home_education/Graficos.cpp ) 

add_executable( identificadorLado src/robocup_home_education/identificadorLado.cpp ) 
add_executable( MoveDetector src/robocup_home_education/MoveDetector.cpp ) 


target_link_libraries( ObservadorPelotasHSV_rc
${catkin_LIBRARIES}
)

#target_link_libraries( ObservadorPersonas_rc
#${catkin_LIBRARIES}
#)

target_link_libraries( Coordinador_rc
${catkin_LIBRARIES}
)


target_link_libraries( Control_rc 
${catkin_LIBRARIES} 
${PROJECT_NAME}
 )


target_link_libraries( Graficos_rc 
${catkin_LIBRARIES} 
${PROJECT_NAME}
 )

target_link_libraries(  MoveDetector
${catkin_LIBRARIES} 
${PROJECT_NAME}
 )

target_link_libraries( identificadorLado
${catkin_LIBRARIES}
)

## Rename C++ executable without prefix
## The above recommended prefix causes long target names, the following renames the
## target back to the shorter version for ease of user use
## e.g. "rosrun someones_pkg node" instead of "rosrun someones_pkg someones_pkg_node"
# set_target_properties(${PROJECT_NAME}_node PROPERTIES OUTPUT_NAME node PREFIX "")

## Add cmake target dependencies of the executable
## same as for the library above
# add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Specify libraries to link a library or executable target against
# target_link_libraries(${PROJECT_NAME}_node
#   ${catkin_LIBRARIES}
# )


add_executable(Monologo_node src/Find_My_Mates.cpp)
add_executable(Carry_My_Luggage_bt_node src/Carry_My_Luggage_node.cpp)

add_dependencies(Monologo_node ${catkin_EXPORTED_TARGETS} )
add_dependencies(Carry_My_Luggage_bt_node ${catkin_EXPORTED_TARGETS} )

target_link_libraries(Monologo_node ${catkin_LIBRARIES} robocup_home_education_los_ultramarinos ${ZMQ_LIBRARIES})
target_link_libraries(Carry_My_Luggage_bt_node ${catkin_LIBRARIES} robocup_home_education_los_ultramarinos ${ZMQ_LIBRARIES})

list(APPEND plugin_libs
  #TextToSpeech
  Seguir
  Navegar
  Esperar
  Navegar2
  Centrar
  Iniciar
)

foreach(bt_plugin ${plugin_libs})
  target_link_libraries(${bt_plugin} ${catkin_LIBRARIES} robocup_home_education_los_ultramarinos)
  target_compile_definitions(${bt_plugin} PRIVATE BT_PLUGIN_EXPORT)
endforeach()

roslint_cpp(
  src/Carry_My_Luggage_node.cpp
  src/Find_My_Mates.cpp

  src/robocup_home_education/Control.cpp
  src/robocup_home_education/Controlador.cpp
  src/robocup_home_education/Coordinador.cpp
  src/robocup_home_education/Graficos.cpp
  src/robocup_home_education/identificadorLado.cpp
  src/robocup_home_education/MoveDetector.cpp
  src/robocup_home_education/ObservadorPelotasHSV.cpp
  src/robocup_home_education/ObservadorPersonas.cpp
  src/robocup_home_education/PIDController.cpp

  src/behavior_tree_nodes/Esperar.cpp
  src/behavior_tree_nodes/Navegar2.cpp
  src/behavior_tree_nodes/Navegar.cpp
  src/behavior_tree_nodes/Seguir.cpp
  src/behavior_tree_nodes/TextToSpeech.cpp
  src/behavior_tree_nodes/Iniciar.cpp

  include/robocup_home_education/Controlador.h
  include/robocup_home_education/MonologoDF.h
  include/robocup_home_education/PIDController.h
)
