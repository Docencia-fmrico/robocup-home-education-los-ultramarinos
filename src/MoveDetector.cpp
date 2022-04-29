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
#include <iostream>
#include "ros/ros.h"
#include "sensor_msgs/Image.h"
#include "cv_bridge/cv_bridge.h"
#include "darknet_ros_msgs/BoundingBoxes.h"
#include "tf/tf.h"
#include "visual_behavior_los_ultramarinos/RobotData.h"
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <memory>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/types.hpp>
#include <math.h>
#include <opencv2/photo.hpp>
#include "std_msgs/Bool.h"
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <std_msgs/Float32.h>
#include <math.h>


cv_bridge::CvImagePtr filtrada;
cv::Mat frameAnterior ;
bool toFrames = false ;
geometry_msgs::Pose2D datos;
ros::Publisher pub;
float rango = 150 ;
int fr = 10;
bool act = false;

void activacionTree(const std_msgs::Bool::ConstPtr& pp)
{ 
  act = pp->data  ;
  ROS_INFO("MOVE DETECTOR ACTIVADO");

}

void messageCallback(const  sensor_msgs::Image::ConstPtr&  msg)
{
  if(act)
  {
    
  cv_bridge::CvImagePtr filtrada = cv_bridge::toCvCopy(*msg , sensor_msgs::image_encodings::BGR8);
  cv::Mat frameActual ;
  cv::cvtColor(filtrada->image, frameActual, cv::COLOR_RGB2RGBA);
 
  int step = frameActual.step;
  int height = frameActual.rows;
  int width = frameActual.cols;
  
  int contardoMovimientoDerecha = 0 ;
  int contardoMovimientoIzquierda = 0 ;

  cv::Mat Output ;
  frameActual.copyTo(Output);

  if (toFrames ) {

  for (int i = 0; i < height; i++) {   
            for (int j = 0; j < width; j++) {
 
            int posdata = i * step + j * 4;
              
            float r_a = frameActual.data[posdata]  ;
            float g_a = frameActual.data[posdata+1]  ;
            float b_a = frameActual.data[posdata+2]  ;

            float r_an = frameAnterior.data[posdata]  ;
            float g_an = frameAnterior.data[posdata+1]  ;
            float b_an = frameAnterior.data[posdata+2]  ;
            
            // diferencia entre pixels 

            float diff = sqrt( pow((r_a-r_an),2)+pow((g_a-g_an),2)+pow((b_a-b_an),2));

            
            if (diff < rango){

              // no hay movimiento en este pixel
              Output.data[posdata] = 0 ;
              Output.data[posdata+1] = 0 ;
              Output.data[posdata+2] = 0 ;

            }else{

              // hay movimiento
              Output.data[posdata] = 255 ;
              Output.data[posdata+1] = 255 ;
              Output.data[posdata+2] = 255 ;
              
              // miro en que semiplano se detecta movimiento
              if ( j > width/2 ){
                contardoMovimientoDerecha++;
                
              }else{
                contardoMovimientoIzquierda++;
                
              }

            }                   
  }
  }
  }

  cv::Point p1(width/2,0), p2(width/2,height);
  cv::Scalar colorLine(0,255,0); 
  int thicknessLine = 1;  
  cv::line(Output, p1, p2, colorLine, thicknessLine);
  toFrames  = true ;
  frameAnterior = frameActual ;
  cv::imshow("Canvas",Output);
  cv::waitKey(3);
  
  // CAMBIAR LOS RANGO ETC ..................................................
                
  //std::cout << "Contador derecho : " << contardoMovimientoDerecha << "\n";
  //std::cout << "Contador izquierdo : " << contardoMovimientoIzquierda  << "\n";

  
  bool diferencia = abs(contardoMovimientoDerecha - contardoMovimientoIzquierda ) > 50 ;

  if(diferencia){

    if (contardoMovimientoDerecha > contardoMovimientoIzquierda){
      std::cout << "DERECHA" << "\n" ;

      datos.x = 1;
    }else{
      std::cout << "IZQUIERDA" << "\n" ;
      datos.x = -1;

    }

  } else{
    std::cout << "NO MOVIMIENTO" << "\n" ;
    datos.x = 0;
  }

pub.publish(datos);

  
}
}


int main(int argc, char **argv)
{

  ros::init(argc, argv, "MoveDetector");
  ros::NodeHandle nh;

  ros::Subscriber sub = nh.subscribe("/camera/rgb/image_raw", 10, messageCallback);
  ros::Subscriber Activador = nh.subscribe("/control_maleta", fr, activacionTree);
  pub = nh.advertise<geometry_msgs::Pose2D>("/movement_data", fr);

  
  ros::spin();

  return 0;
}
