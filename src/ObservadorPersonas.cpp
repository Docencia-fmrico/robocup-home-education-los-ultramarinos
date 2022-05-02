#include <iostream>
#include "ros/ros.h"
//#include "nav_msgs/Odometry.h"
#include "sensor_msgs/Image.h"
#include "cv_bridge/cv_bridge.h"
#include "darknet_ros_msgs/BoundingBoxes.h"
#include "tf/tf.h"
#include "visual_behavior_los_ultramarinos/RobotData.h"
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include "std_msgs/String.h"
#include "std_msgs/Bool.h"


#include <memory>

cv_bridge::CvImagePtr DepthImageData;
cv_bridge::CvImagePtr RgbImageData;
ros::Publisher mSensorsPublisher;
ros::Subscriber Activador;
ros::Publisher treePub;
ros::Publisher objectPub;
u_int R_ = 0;
u_int G_ = 0;
u_int B_ = 0;
float min_dist;
bool found_person;
bool found_object;
bool act = false;
double now;
int fr = 10;
geometry_msgs::Pose2D pp;
std_msgs::String msg;
std_msgs::String object;
float xmax;
float xmin;
float ymax;
float ymin;

void activacionTree(const std_msgs::Bool::ConstPtr& clave)
{ 
  act = clave->data;
  ROS_INFO("Observador Activado");

}


void callback_bbx(const sensor_msgs::ImageConstPtr& depth, const sensor_msgs::ImageConstPtr& rgb, const darknet_ros_msgs::BoundingBoxesConstPtr& boxes)
{
	if(act){
		DepthImageData = cv_bridge::toCvCopy(*depth, sensor_msgs::image_encodings::TYPE_32FC1);
		cv::Mat profundidadG_;
		/////////////////////
		//cv::GaussianBlur(mImageData->image,profundidadG_,cv::Size(33,33),0,0,0);
		
		cv::erode(DepthImageData->image, profundidadG_, cv::Mat());
		//////////////////////******************CAMBIOS*****************

		RgbImageData = cv_bridge::toCvCopy(*rgb, sensor_msgs::image_encodings::RGB16);
		//cv::Mat rgb = RgbImageData->image;
		//cv::erode(RgbImageData->image, rgb, cv::Mat());

		std::string tag = "sports ball";
		
		float px_center=DepthImageData->image.cols/2;
		float py_center=DepthImageData->image.rows/2;
		// #############################
		//min_dist=3;
		min_dist=0.5;
		// #####################################
		found_person = false;
		found_object = false;
		
		for(int i=0;i<boxes->bounding_boxes.size();i++)
		{
			if (boxes->bounding_boxes[i].Class == tag)
			{
				float px = (boxes->bounding_boxes[i].xmax + boxes->bounding_boxes[i].xmin) / 2;
				float py = 2 * (boxes->bounding_boxes[i].ymax + boxes->bounding_boxes[i].ymin) / 3;
				

				float dist = profundidadG_.at<float>(cv::Point(px, py)) * 0.001f;


				if (dist < min_dist)
				{
					min_dist = dist;
					//pp.x = dist;
					//float angle = (px - px_center)/(px_center);
					//pp.y = angle;
					cv::Vec3b vector_rgb = RgbImageData->image.at<cv::Vec3b>(cv::Point(px,py));

					R_ = vector_rgb[2];
					G_ = vector_rgb[1];
					B_ = vector_rgb[0];

					pp.x = R_;
					pp.y = G_;
					pp.theta = B_;

					xmax = boxes->bounding_boxes[i].xmax;
					xmin = boxes->bounding_boxes[i].xmin;
					ymax = boxes->bounding_boxes[i].ymax;
					ymin = boxes->bounding_boxes[i].ymin;


					if(pp.x != 0 || pp.y != 0 || pp.theta != 0){
						found_person = true;
					}

					}
			}
		}
		

		if (found_person)
		{
			for(int i=0;i<boxes->bounding_boxes.size();i++)
			{
				float px = (boxes->bounding_boxes[i].xmax + boxes->bounding_boxes[i].xmin) / 2;
				float py = (boxes->bounding_boxes[i].ymax + boxes->bounding_boxes[i].ymin) / 2;

				float s1x = xmax - xmin;
				float s2x = boxes->bounding_boxes[i].xmax - boxes->bounding_boxes[i].xmin;

				float s1y = ymax - ymin;
				float s2y = boxes->bounding_boxes[i].ymax - boxes->bounding_boxes[i].ymin;


				if( (xmin < px < xmax) && (ymin < py < ymax) && (s1x > s2x) && (s1y > s2y) )
				{
					std::stringstream ss;
					ss << boxes->bounding_boxes[i].Class;
					object.data = ss.str();
					objectPub.publish(object);

					std::stringstream ss2;
					ss2 << "SUCCESS";
					msg.data = ss2.str();
					treePub.publish(msg);

					mSensorsPublisher.publish(pp);
					found_object = true;
				}
			}
			if(!found_object)
			{
			std::stringstream ss;
			ss << "RUNNING";
			msg.data = ss.str();
			treePub.publish(msg);
			//Solo para tests
			mSensorsPublisher.publish(pp);
			}
		}
		else
		{
			std::stringstream ss;
			ss << "FAILURE";
			msg.data = ss.str();
			treePub.publish(msg);
		}
	}
}

int main(int argc, char** argv)
{

	ros::init(argc, argv, "ObservadorPersonas");

	ros::NodeHandle nh;

	message_filters::Subscriber<sensor_msgs::Image> image_depth_sub(nh, "/camera/depth/image_raw", fr);
	message_filters::Subscriber<sensor_msgs::Image> image_rgb_sub(nh, "/camera/rgb/image_raw", fr);
  	message_filters::Subscriber<darknet_ros_msgs::BoundingBoxes> bbx_sub(nh, "/darknet_ros/bounding_boxes", fr);
	

 	typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::Image, sensor_msgs::Image, darknet_ros_msgs::BoundingBoxes> Sync;
  	message_filters::Synchronizer<Sync> sync_bbx(Sync(10), image_depth_sub, image_rgb_sub, bbx_sub);

  	sync_bbx.registerCallback(boost::bind(&callback_bbx, _1, _2, _3));

	mSensorsPublisher = nh.advertise<geometry_msgs::Pose2D>("/person_data", fr);
	Activador = nh.subscribe("/control_observador", fr, activacionTree);
	treePub = nh.advertise<std_msgs::String>("/status_observador", fr);
	objectPub = nh.advertise<std_msgs::String>("/object_data", fr);

	ros::spin();
	

}