#include <ros/ros.h>
#include <stdlib.h>
#include <geometry_msgs/PoseStamped.h>
#include <tf/tf.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <move_base_msgs/MoveBaseGoal.h>
#include <actionlib/client/simple_action_client.h>
#include <geometry_msgs/PoseArray.h>
#include <cmath>

float destX, destY, destZ;
float sumX, sumY, sumZ, sumOrienX, sumOrienY, sumOrienZ, sumOrienW = 0.0;
float avgX, avgY, avgZ, avgOrienX, avgOrienY, avgOrienZ, avgOrienW = 0.0;
float diffX, diffY, diffZ, diffOrienX, diffOrienY, diffOrienZ, diffOrienW = 0.0;
geometry_msgs::PoseWithCovarianceStamped destCoord;
geometry_msgs::PoseArray particleCoord;


void GetPos(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr &msg)
{
	destCoord = *msg;
	
	destX = destCoord.pose.pose.position.x;
	destY = destCoord.pose.pose.position.y;
	destZ = destCoord.pose.pose.position.z;
}



void RecieveParticles(const geometry_msgs::PoseArray::ConstPtr &msg){
	
	particleCoord = *msg;

}


int main (int argc, char **argv){
	
	ros::init (argc, argv, "averagepose");
	ros::NodeHandle nh;

	ros::Subscriber sub2 = nh.subscribe("/particlecloud", 1000, &RecieveParticles);
	ros::Subscriber sub1 = nh.subscribe("/amcl_pose", 1000, &GetPos); //recieve coordinates from tour.cpp
   

	ros::Rate rate(1); //set for approx. 1 second
	int i = 0;

	ros::Duration(3).sleep();

	while (ros::ok){

		 //make infinite loop
		
		for (i = 0; i < particleCoord.poses.size(); i++){
		sumX += particleCoord.poses[i].position.x;
		sumY += particleCoord.poses[i].position.y;
		sumZ += particleCoord.poses[i].position.z;

		sumOrienX += particleCoord.poses[i].orientation.x;
		sumOrienY += particleCoord.poses[i].orientation.y;
		sumOrienZ += particleCoord.poses[i].orientation.z;
		sumOrienW += particleCoord.poses[i].orientation.w;

		}

	}

	avgX = sumX/particleCoord.poses.size(); //compute average
	avgY = sumY/particleCoord.poses.size();
	avgZ = sumZ/particleCoord.poses.size();

	avgOrienX = sumOrienX/particleCoord.poses.size();
	avgOrienY = sumOrienY/particleCoord.poses.size();
	avgOrienZ = sumOrienZ/particleCoord.poses.size();
	avgOrienW = sumOrienW/particleCoord.poses.size();
    

    std::cout << "Average of coordinate x: " << avgX << std::endl;
	std::cout << "Average of coordinate y: " << avgY << std::endl;
	std::cout << "Average of coordinate z: " << avgZ << std::endl;

	std::cout << "Average of orientation x: " << avgOrienX << std::endl;
	std::cout << "Average of orientation y: " << avgOrienY << std::endl;
	std::cout << "Average of orientation z: " << avgOrienZ << std::endl;
	std::cout << "Average of orientation w: " << avgOrienW << std::endl;


	diffX = avgX - destCoord.pose.pose.position.x;
	diffY = avgY - destCoord.pose.pose.position.y;
	diffZ = avgZ - destCoord.pose.pose.position.z;

	diffOrienX = avgOrienX - destCoord.pose.pose.orientation.x;
	diffOrienY = avgOrienY - destCoord.pose.pose.orientation.y;
	diffOrienZ = avgOrienZ - destCoord.pose.pose.orientation.z;
	diffOrienW = avgOrienW - destCoord.pose.pose.orientation.w;


	std::cout << "Difference of coordinate x: " << diffX << std::endl;
	std::cout << "Difference of coordinate y: " << diffY << std::endl;
	std::cout << "Difference of coordinate z: " << diffZ << std::endl;

	std::cout << "Difference of orientation x: " << diffOrienX << std::endl;
	std::cout << "Difference of orientation y: " << diffOrienY << std::endl;
	std::cout << "Difference of orientation z: " << diffOrienZ << std::endl;
	std::cout << "Difference of orientation w: " << diffOrienW << std::endl;

	sumX = 0.0; //reset sum values for positions and orientations
	sumY = 0.0;
	sumZ = 0.0;
	sumOrienX = 0.0;
	sumOrienY = 0.0;
	sumOrienZ = 0.0;
	sumOrienW = 0.0;


    std::cout << "Done!" << std::endl;

	ros::spinOnce();
	
}

