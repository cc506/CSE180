#include <ros/ros.h>
#include <nav_msgs/Odometry.h> 
#include <tf/tf.h>
#include <tf/transform_datatypes.h>
#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/Pose2D.h>
#include <cmath>

//Robot Pose
float robot_pose_x;								
float robot_pose_y;								
float quat_x, quat_y, quat_z, quat_w;
float goal_x, goal_y;
float theta;	

void scanMessageReceived(const sensor_msgs::LaserScan &msg) 
{   
    int closest = msg.ranges[0];
    int closeIndex = 0;
    ROS_INFO_STREAM("Recieved Scan! Finding Closest Obstacle");
    for (int i = 0; i < msg.ranges.size(); i++){
        if(msg.ranges[i] < closest){
            closest = msg.ranges[i];
        }
    }
    closeIndex = closest;
}

//Odometry message Callback function
void poseMessageReceived(const nav_msgs::Odometry&msg) {
    //Extracting Current Position (x,y) of Robot
	robot_pose_x = msg.pose.pose.position.x;				
	robot_pose_y = msg.pose.pose.position.y;

    //Extracting current Quaternion Values of Robot
	quat_x = msg.pose.pose.orientation.x;
	quat_y = msg.pose.pose.orientation.y;
	quat_z = msg.pose.pose.orientation.z;
	quat_w = msg.pose.pose.orientation.w;

	tf::Quaternion q(quat_x, quat_y, quat_z, quat_w);		//Create Quaternion to extract robot_orientation angle
    tf::Matrix3x3 m(q);
    double roll, pitch, yaw;
    m.getRPY(roll, pitch, yaw);

	theta = yaw;

}

int main(int argc, char *argv[]){

    ros::init(argc, argv, "goto");
    ros::NodeHandle nh;

    ros::Subscriber subScan = nh.subscribe("/scan", 1000, &scanMessageReceived);
    ros::Subscriber subOdom = nh.subscribe("/odometry/filtered", 1000, &poseMessageReceived);

	goal_x = 5.0;
	goal_y = 5.0;

    ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("husky_velocity_controller/cmd_vel", 1000);

    ros::Rate rate(10);
	
	geometry_msgs::Twist msg;
    while (ros::ok()) {
        ros::spinOnce();

        double diff_x = goal_x - robot_pose_x;
		double diff_y = goal_y - robot_pose_y;
		double dot = quat_x*diff_x + quat_y*diff_y; 
		double det = quat_x*diff_y - quat_y*diff_x;
		double movetoZ = atan2(det, dot); 

		ROS_INFO_STREAM("(x, y) " << robot_pose_x << " , " << robot_pose_y);
		double distanceToGoal = sqrt(pow(diff_x,2)+pow(diff_y,2));

		if(robot_pose_x != goal_x && robot_pose_y != goal_y){
			msg.linear.x = 0.2;
			msg.angular.z = 0.0;
		}
		else 
		{
			ROS_INFO_STREAM("\n\nGOAL REACHED!!\n\n");
 			msg.linear.x = 0;
			msg.angular.z = 0;
		}
        pub.publish(msg);
		rate.sleep();

    }
}