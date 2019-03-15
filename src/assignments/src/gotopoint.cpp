#include <ros/ros.h>
#include <nav_msgs/Odometry.h> 
#include <tf/tf.h>
#include <tf/transform_datatypes.h>
#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/Vector3.h>
#include <std_msgs/UInt32.h>
#include <cmath>

using namespace std;

//Robot Pose
float robot_pose_x;								
float robot_pose_y;								
float quat_x, quat_y, quat_z;
float goal_x, goal_y, goal_z;
float theta;
int closeIndex = 0;

void goalPoseRecieved(const geometry_msgs::Vector3 &msg){
	goal_x = msg.x;
	goal_y = msg.y;
	goal_z = msg.z;

	ROS_INFO_STREAM("X: " << goal_x);
	ROS_INFO_STREAM("Y: " << goal_y);
	ROS_INFO_STREAM("Z: " << goal_z);
}

void scanMessageReceived(const sensor_msgs::LaserScan &msg) 
{   
    int closest = msg.ranges[0];

    ROS_INFO_STREAM("Recieved Scan! Finding Closest Obstacle");
    for (int i = 0; i < msg.ranges.size(); i++){
        if(msg.ranges[i] < closest){
            closest = msg.ranges[i];
        }
    }
    closeIndex = closest;
}

//Odometry message Callback function
void poseMessageReceived(const nav_msgs::Odometry::ConstPtr &msg) {
    //Extracting Current Position (x,y) of Robot
	robot_pose_x = msg->pose.pose.position.x;				
	robot_pose_y = msg->pose.pose.position.y;

    //Extracting current Quaternion Values of Robot
	tf::Pose pose;
  	tf::poseMsgToTF(msg->pose.pose, pose);
  	quat_z = tf::getYaw(pose.getRotation());
	quat_x = cos(quat_z);
	quat_y = sin(quat_z);

}

int main(int argc, char *argv[]){

    ros::init(argc, argv, "gotopoint");
    ros::NodeHandle nh;

	
    ros::Subscriber subScan = nh.subscribe("/scan", 1000, &scanMessageReceived);
    ros::Subscriber subOdom = nh.subscribe("/odometry/filtered", 1000, &poseMessageReceived);
	ros::Subscriber goalPose = nh.subscribe("targetpose", 1000, &goalPoseRecieved);

    ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("husky_velocity_controller/cmd_vel", 1000);
	ros::Publisher pubBool = nh.advertise<std_msgs::UInt32>("sendBool", 1000);

    ros::Rate rate(1);
	
	geometry_msgs::Twist msg;
	std_msgs::UInt32 change;
    while (ros::ok()) {

        double diff_x = goal_x - robot_pose_x;
		double diff_y = goal_y - robot_pose_y;
		double dot = quat_x*diff_x + quat_y*diff_y; 
		double det = quat_x*diff_y - quat_y*diff_x;
		double movetoZ = atan2(det, dot);
		double movetoX = cos(movetoZ);
		double movetoY = sin(movetoZ); 

		ROS_INFO_STREAM("(x, y) " << robot_pose_x << " , " << robot_pose_y);
		double distanceToGoal = sqrt(pow(diff_x,2)+pow(diff_y,2));

		if(distanceToGoal >= 0.2){

			if(closeIndex <= 0.5){
				msg.linear.z = 0.3;
				pub.publish(msg);
			}
			
			msg.linear.x = 0.3;
			msg.linear.z = 0.0;
			pub.publish(msg);
		}
		else 
		{
			ROS_INFO_STREAM("\n\nGOAL REACHED!!\n\n");
 			msg.linear.x = 0;
			msg.angular.z = 0;

			change.data = 1;
			pubBool.publish(change);
			pub.publish(msg);
			rate.sleep();
		}

    }
	ros::spinOnce();
}