#include <ros/ros.h>
#include <stdlib.h>
#include <geometry_msgs/Twist.h>

int main(int argc, char **argv){
    
    ros::init(argc, argv, "moverobot");
    ros::NodeHandle nh;

    ros::Publisher pubMove = nh.advertise<geometry_msgs::Twist>("/pioneer/cmd_vel",1000);
    ros::Publisher pubRotate = nh.advertise<geometry_msgs::Twist>("/pioneer/cmd_vel",1000);

    ros::Rate rate(8);
    geometry_msgs::Twist twistSquare;
   
while (ros::ok()) {
    int count = 0;

	while(count < 4){

		twistSquare.linear.x = 1.0;
		twistSquare.angular.z = 0 ;
		count++;
		pubMove.publish(twistSquare);
		rate.sleep();
	}
	count = 0;
	while(count < 3){
        
		twistSquare.linear.x = 0;
		twistSquare.angular.z = 30.0 ;
		pubRotate.publish(twistSquare);
		count++;
		rate.sleep();
		}
	}
}
