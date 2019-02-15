#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/PoseWithCovariance.h>

float xx, yy, xX, yY;

void odomPublish(const nav_msgs::Odometry::ConstPtr& msg) {
    
    xx = msg->pose.pose.position.x;
    yy = msg->pose.pose.position.y;
}

void basePublish(const nav_msgs::Odometry::ConstPtr& msg) {
    
    xX = msg->pose.pose.position.x;
    yY = msg->pose.pose.position.y;
}
int main(int argc, char* argv[]){
    
    ros::init(argc, argv, "drift");
    ros::NodeHandle nh;

    ros::Subscriber pubOdom = nh.subscribe("/pioneer/odom",1000,&odomPublish);
    ros::Subscriber pubBase = nh.subscribe("/base_pose_ground_truth",1000,&basePublish);

    ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("/posedrift",1000);

    ros::Rate rate(1);

    geometry_msgs::Twist twistPub;
    while(ros::ok()){

        ros::spinOnce();

        twistPub.linear.x = xx - xX;
        twistPub.linear.y = yy - yY;
        pub.publish(twistPub);

        ROS_INFO_STREAM("X: " << twistPub.linear.x);
        ROS_INFO_STREAM("Y: " << twistPub.linear.y);

        rate.sleep();
    }
    return 0;
} 