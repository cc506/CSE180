#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>
#include <tf/tf.h>
#include <math.h>

//Goal Pose
float goalX = 0;								
float goalY = 0;								
float goalTHETA = 0;	

//Robot Pose
float robot_pose_x;								
float robot_pose_y;								
float quat_x, quat_y, quat_z, quat_w;		
double robot_orientation;									
						
//flags to move
bool rot = true;
bool move = false;
bool rot2 = false;

//Function to return the distance between 2 points
float distance(float x1, float y1, float x2, float y2) {
	float vect_x = x2 - x1;
	float vect_y = y2 - y1;
	float sum_square = (((vect_x) * (vect_x)) + ((vect_y) * (vect_y)));
	float dist = sqrt(sum_square);
	return dist;
}

//Odometry message Callback function
void pose_groundMessageReceived(const nav_msgs::Odometry&msg) {		
	robot_pose_x = msg.pose.pose.position.x;				//Extracting Current Position (x,y) of Robot
	robot_pose_y = msg.pose.pose.position.y;

	quat_x = msg.pose.pose.orientation.x;				//Extracting current Quaternion Values of Robot
	quat_y = msg.pose.pose.orientation.y;
	quat_z = msg.pose.pose.orientation.z;
	quat_w = msg.pose.pose.orientation.w;

	tf::Quaternion q(quat_x, quat_y, quat_z, quat_w);		//Create Quaternion to extract robot_orientation angle

	robot_orientation = tf::getYaw(q);			//Define robot_orientation angle between positive x-axis and target point

}

int main(int argc,char ** argv) {
	ros::init(argc,argv,"gotoposition");
	ros::NodeHandle nh_;
	
	ros::Subscriber pose_global = nh_.subscribe("/base_pose_ground_truth",1000,&pose_groundMessageReceived);

	ros::Publisher pubVel = nh_.advertise<geometry_msgs::Twist>("pioneer/cmd_vel",1000);

	//Twist velocity variable
	geometry_msgs::Twist vel;

	std::cout<<"Please indicate the goal Coordinates."<<std::endl;
	std::cout<<"goalX: "<<std::endl;
	std::cin>>goalX;
	std::cout<<"goalY: "<<std::endl;
	std::cin>>goalY;
	std::cout<<"goalTHETA: "<<std::endl;
	std::cin>>goalTHETA;

	ros::spinOnce();
	//Calculate vector to goal
	float vect_x = abs(goalX) - abs(robot_pose_x);				
	float vect_y = abs(goalY) - abs(robot_pose_y);

	int sign=vect_x/abs(vect_x);
	vect_x=sign*vect_x;
	sign=vect_y/abs(vect_y);
	vect_y=sign*vect_y;

	float angle_diff = atan2(vect_y,vect_x);	//atan2(y,x) returns the angle (-pi and pi) of the x-axis and the Goal Point(goalX,goalY) 
	float goal_dist = distance(goalX, goalY, robot_pose_x, robot_pose_y);						//Distance from Goal Point and Origin (WORLD FRAME)
	float init_robot_x=	robot_pose_x;
	float init_robot_y=	robot_pose_y;

	while (ros::ok()) {
		ros::spinOnce();
	
		float dist_remainder = distance(init_robot_x, init_robot_y, robot_pose_x, robot_pose_y);		//Distance from Origin (WORLD FRAME) and Robot Ground Pose Coordinates

		//Rotate the robot to goal robot_orientation orientation 
		if(rot == true) {
			vel.angular.z = 0.5;		//While rot is true, rotate robot counterclockwise
			pubVel.publish(vel);

			std::cout<<"First Alignment. Current robot's angle: " << robot_orientation << " vs goal's angle: " << angle_diff<<std::endl;

			if(((robot_orientation + .1) >= angle_diff) && ((robot_orientation - .1) <= angle_diff)) {			//When robot is facing target orientation, stop rotating
				rot = false;
				vel.angular.z = 0;
				pubVel.publish(vel);
				std::cout<<"STOP"<<std::endl;
				move = true;															//Move is set to true
			}
		}

		//Move the robot the goal distance (the distance between original frame and target point)
		if(move == true) {	
			vel.linear.x = 0.5;			//While move is true, move robot forward
			pubVel.publish(vel);

			std::cout<<"Reaching goal. " << dist_remainder << "  TARGET: " << goal_dist<<std::endl;

			if(dist_remainder >= goal_dist) {			//When robot reaches target distance, stop moving
				move = false;
				vel.linear.x = 0;
				vel.angular.z = 0;
				pubVel.publish(vel);
				std::cout<<"STOP"<<std::endl;
				rot2 = true;						//rot2 is set to true for 2nd rotation
			}
		}

		//Rotate robot to  goalTHETA 
		if(rot2 == true) {			
			vel.angular.z = 0.5;		//While rot2 is equal to true, rotate robot counterclockwise
			pubVel.publish(vel);


			std::cout<<"Final Alignment.  Current robot's angle: " << robot_orientation << " vs goal's angle: " << goalTHETA<<std::endl;

			if(((robot_orientation + .1) >= goalTHETA) && ((robot_orientation - .1) <= goalTHETA)) {			//WHen robot is facing target goalTHETA, stop rotating
				rot2 = false;
				vel.angular.z = 0;
				pubVel.publish(vel);
				std::cout<<"Robot arrived to the goal"<<std::endl;
			}
		}
	}
}
