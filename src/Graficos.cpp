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


cv_bridge::CvImagePtr filtrada;

void messageCallback(const  sensor_msgs::Image::ConstPtr&  msg)
{
  cv_bridge::CvImagePtr filtrada = cv_bridge::toCvCopy(*msg , sensor_msgs::image_encodings::BGR8);
   
  cv::Mat img_ ;
  cv::Mat img_t ;
  cv::Mat combine;
  
  cv::cvtColor(filtrada->image, img_, cv::COLOR_RGB2RGBA );
  cv::Mat blueImage(1000, 1000, 24, cv::Scalar(255, 255, 255));

  std::cout << img_.type() << "\n";
  std::cout << blueImage.type()<< "\n" ;
  
  //cv::hconcat(img_,blueImage,combine);
  cv::Rect r(200, 500, img_.cols, img_.rows);
  cv::Point org(250, 100);
  cv::putText(blueImage, "EJEJEJ", org,
            cv::FONT_HERSHEY_DUPLEX, 2.2,
            cv::Scalar(0, 0, 0), 2, cv::LINE_AA);
  
  img_.copyTo(blueImage(r));
  
  cv::imshow("Canvas",blueImage);
  
  
  cv::waitKey(3);
  

}

int main(int argc, char **argv)
{

  ros::init(argc, argv, "Graficos");
  ros::NodeHandle nh;

  ros::Subscriber sub = nh.subscribe("/camera/rgb/image_raw", 10, messageCallback);
  
  ros::spin();

  return 0;
}
