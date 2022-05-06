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
#include <string>
#include <math.h>

#include <memory>

cv_bridge::CvImagePtr DepthImageData;
cv_bridge::CvImagePtr RgbImageData;
ros::Publisher mSensorsPublisher;
ros::Subscriber Activador;
ros::Publisher treePub;
ros::Publisher objectPub;
ros::Publisher talkPub;
const int max_vals = 10;
double R_ = 0;
double G_ = 0;
double B_ = 0;
double Rs [max_vals];
double Gs [max_vals];
double Bs [max_vals];
int values_counter = 0;
float min_dist;
bool found_person;
bool found_object = false;
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

struct RGB {

    double r;
    double g;
    double b;
    std::string name;

};

RGB blue = {
    0,
    0,
    255,
    "BLUE",
};

RGB orange = {
    255,
    127,
    0,
    "ORANGE",
};

RGB red = {
    255,
    0,
    0,
    "RED",
};

RGB rainbow[] = {blue,orange,red};
double ColourDistance(RGB e1, RGB e2)
{
  long rmean = ( (long)e1.r + (long)e2.r ) / 2;
  long r = (long)e1.r - (long)e2.r;
  long g = (long)e1.g - (long)e2.g;
  long b = (long)e1.b - (long)e2.b;
  return sqrt((((512+rmean)*r*r)>>8) + 4*g*g + (((767-rmean)*b*b)>>8));
}

std::string getColor(RGB valores){
    double d_min;
    int result;
    for(int i = 0; i < 9; i++)
    {
        if (( i == 0) || (ColourDistance(valores, rainbow[i]) < d_min) )
        {
            d_min = ColourDistance(valores, rainbow[i]);
            result = i;
            }
        //ROS_INFO("%d", d_min);
    }
    //ROS_INFO("%d", result);
    //ROS_INFO("%s", rainbow[result].name);
    std::cout << rainbow[result].name << std::endl;
    return rainbow[result].name;
}

void activacionTree(const std_msgs::Bool::ConstPtr& clave)
{ 
  act = clave->data;
  if(act){
  ROS_INFO("Observador Activado");
  }
  else{
  ROS_INFO("Observador Desactivado");	  
  }

}

double hacerMedia(double arr[])
{
	double result = 0;
	
	for(int i = 0;i < max_vals; i++){
		result = result + arr[i];
		//ROS_INFO("Media: %d", result);
	}
	result = result/max_vals;
	ROS_INFO("Media: %f", result);
	return result;
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

		std::string tag = "person";
		
		float px_center=DepthImageData->image.cols/2;
		float py_center=DepthImageData->image.rows/2;
		// #############################
		//min_dist=3;
		min_dist=1.5;
		// #####################################
		found_person = false;
		//found_object = false;
		
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

					//pp.x = R_;
					//pp.y = G_;
					//pp.theta = B_;

					xmax = boxes->bounding_boxes[i].xmax;
					xmin = boxes->bounding_boxes[i].xmin;
					ymax = boxes->bounding_boxes[i].ymax;
					ymin = boxes->bounding_boxes[i].ymin;


					if( (R_ != 0 || G_ != 0 || B_ != 0 ) && values_counter < max_vals){
						Rs[values_counter] = R_;
						Gs[values_counter] = G_;
						Bs[values_counter] = B_;

						//ROS_INFO("R = %d", Rs[values_counter]);
						//ROS_INFO("G = %d", Gs[values_counter]);
						//ROS_INFO("B = %d", Bs[values_counter]);
						
						values_counter++;
						//ROS_INFO("values = %d", values_counter);
					}

					found_person = true;

					}
			}
		}
		

		if (found_person)
		{
			for(int i=0;i<boxes->bounding_boxes.size();i++)
			{
				tag = boxes->bounding_boxes[i].Class;
				if(tag == "sports ball" || tag == "bottle" || tag == "banana"){
					std::stringstream ss;
					ss << boxes->bounding_boxes[i].Class;
					object.data = ss.str();

					found_object = true;
				}
			}
			//if(!found_object || values_counter < max_vals)
			if(values_counter < max_vals)
			{
			std::stringstream ss;
			ss << "RUNNING";
			msg.data = ss.str();
			treePub.publish(msg);
			//Solo para tests
			//mSensorsPublisher.publish(pp);
			}
			else{

				double r = hacerMedia(Rs);
				double g = hacerMedia(Gs);
				double b = hacerMedia(Bs);

				std::vector<float> rojo = {255,0 ,0} ;
				std::vector<float> azul = {0,0 ,255} ;
				std::vector<float> naranja = {255,127 ,0} ;

				float dist_rojo = sqrt(pow((rojo[0]-r),2)+pow((rojo[1]-g),2)+pow((rojo[2]-b),2));
				float dist_azul = sqrt(pow((azul[0]-r),2)+pow((azul[1]-g),2)+pow((azul[2]-b),2));
				float dist_nar = sqrt(pow((naranja[0]-r),2)+pow((naranja[1]-g),2)+pow((naranja[2]-b),2));

                std::vector<float> dists = {dist_rojo,dist_azul,dist_nar};
                std::vector<std::string> colores = {"red","blue","orange"};   
			    
				float dist_min =  100000 ;
				std::string color_parecido = "orange";

				for(int i = 0 ; i < 3 ; i++){
                    
					if(dist_min <= dists[i]){
                       dist_min = dists[i] ;
					   color_parecido = colores[i];
					}
				}

				msg.data = color_parecido ;

				mSensorsPublisher.publish(msg);
				objectPub.publish(object);

				std::stringstream ss;
				ss << "SUCCESS";
				msg.data = ss.str();
				treePub.publish(msg);

				std::cout << "SUCCESS\n";

				

				values_counter = 0;
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

	mSensorsPublisher = nh.advertise<std_msgs::String>("/person_data", fr);
	Activador = nh.subscribe("/control_observador", fr, activacionTree);
	treePub = nh.advertise<std_msgs::String>("/status_observador", fr);
	objectPub = nh.advertise<std_msgs::String>("/object_data", fr);
	talkPub = nh.advertise<std_msgs::String>("/msg_receive", fr);

	ros::spin();
}