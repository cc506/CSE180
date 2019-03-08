#include <ros/ros.h>
#include <nav_msgs/Odometry.h> 
#include <tf/tf.h>
#include <tf/transform_datatypes.h>
#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/Pose2D.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> goal;

//Robot Pose
float robot_pose_x;								
float robot_pose_y;								
float quat_x, quat_y, quat_z, quat_w;
float goal_x, goal_y;		
double robot_orientation;	

void gotolocation(const geometry_msgs::Pose2D&msg) 
{	
	goal_x = 5;
    goal_y = 5;
}

void scanMessageReceived(const sensor_msgs::LaserScan &msg) 
{   
    int closest = msg.ranges[0];
    int closeIndex = 0;
    ROS_INFO_STREAM("Recieved Scan! Finding Closest Obstacle");
    for (int i = 0; i < msg.ranges.size(); i++){
        if(msg.ranges[i] < closest){
            closest = msg.ranges[i];
            closeIndex = i;
        }
    }
    ROS_INFO_STREAM("Closest Obstacle: " << closest);
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

	robot_orientation = tf::getYaw(q);			//Define robot_orientation angle between positive x-axis and target point

}

int main(int argc, char *argv[]){

    ros::init(argc, argv, "goto");
    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("goalpose", 1000 , &gotolocation);
    ros::Subscriber subScan = nh.subscribe("/scan", 1000, &scanMessageReceived);
    ros::Subscriber subOdom = nh.subscribe("/odometry/filtered", 1000, &poseMessageReceived);

    ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("husky_velocity_controller/cmd_vel", 1000);

    goal ac("move_base", true);

    ros::Rate rate(10);

    while (ros::ok()) {
        ros::spinOnce();

        if(robot_pose_x != goal_x && robot_pose_y != goal_y){
            
        }else{

        }



    }
}