#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose2D.h>
#include <geometry_msgs/TransformStamped.h>
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h>
#include <tf/LinearMath/Quaternion.h>
#include <tf/tfMessage.h>

using namespace std;

	float x;
	float y;
	float theta;
    bool recieved;

void poseRecieved(const geometry_msgs::Pose2D&msg){
	x = msg.x;
	y = msg.y;
	theta = msg.theta;
    recieved = true;
}

int main(int argc,char ** argv) {
	ros::init(argc,argv,"gotoposition");
	ros::NodeHandle nh;

	ros::Publisher pubDirection = nh.advertise<geometry_msgs::Pose2D>("targetpose", 1000);

	geometry_msgs::Pose2D poser;
	tf::Quaternion q;
    while(ros::ok()){
    	cin >> x;
		cin >> y;
		cin >> theta;
		q.setRPY(0,0,theta);
		pubDirection.publish(poser);
    }
	return 0;
}