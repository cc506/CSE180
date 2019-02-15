#include <ros/ros.h>
#include <stdlib.h>
#include <geometry_msgs/Twist.h>

int main(int argc, char **argv){
    
    ros::init(argc, argv, "moverobot");
    ros::NodeHandle nh;

    ros::Publisher pubMove = nh.advertise<geometry_msgs::Twist>("/pioneer/cmd_vel",1000);
    ros::Publisher pubRotate = nh.advertise<geometry_msgs::Twist>("/pioneer/cmd_vel",1000);

    ros::Rate rate(5);

    geometry_msgs::Twist twistMove;
	geometry_msgs::Twist twistRotate;
   
while (ros::ok()) {
    int count = 0;

	twistMove.linear.x = 1.0;
	twistMove.angular.z = 0;
	pubMove.publish(twistMove);
	ros::Duration(1).sleep();

	twistRotate.linear.x = 0;
	twistRotate.linear.z = M_PI/2;
	pubRotate.publish(twistRotate);
	ros::Duration(1).sleep();

	}
}
