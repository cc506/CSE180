#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose2D.h>
#include <tf2/utils.h>
#include <geometry_msgs/TransformStamped.h>
#include <tf2_ros/transform_listener.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_msgs/TFMessage.h>

// void TransformListener::regTransform(const tf2_msgs::TFMessageConstPtr& msg)
// {
//   subscription_callback_impl(msg, false);
// }
// void TransformListener::staticTransform(const tf2_msgs::TFMessageConstPtr& msg)
// {
//   subscription_callback_impl(msg, true);
// }

int main(int argc, char *argv[]){
    
    ros::init(argc, argv, "transformations");
    ros::NodeHandle nh;

    ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("pioneer/base_link",1000);

    //ros::Subscriber tfSub = nh.subscribe<tf2_msgs::TFMessage>("/tf_static", 1000, &(TransformListener::regTransform,this, _1));
    //ros::Subscriber tfSub1 = nh.subscribe<tf2_msgs::TFMessage>("/tf", 1000, &(TransformListener::staticTransform,this, _1));

    tf2_ros::Buffer buffer;
    tf2_ros::TransformListener listener(buffer);
    geometry_msgs::TransformStamped transformStamped;
        geometry_msgs::TransformStamped transformOdom;
    while (ros::ok()) {
        ros::spinOnce();

        try{
            transformStamped = buffer.lookupTransform("pioneer/base_link/front_sonar", "pioneer/base_link/back_sonar",
                               ros::Time(0));
            transformOdom = buffer.lookupTransform("pioneer/base_link/front_sonar", "pioneer/odom", ros::Time(0));
        }
        catch (tf2::TransformException &ex) {
            ROS_WARN("%s",ex.what());
            ros::Duration(1.0).sleep();
            continue;
        }
        tf2::Quaternion q1(transformStamped.transform.rotation.x,transformStamped.transform.rotation.y, 
                                transformStamped.transform.rotation.z, transformStamped.transform.rotation.w);
        tf2::Quaternion q2(transformOdom.transform.rotation.x,transformOdom.transform.rotation.y, 
                                transformOdom.transform.rotation.z, transformOdom.transform.rotation.w);
        
        double yaw2 = tf2::getYaw(q2);
        double yaw1 = tf2::getYaw(q1);

        
        
        ROS_INFO_STREAM("Obtained transformation " << transformOdom);
    }
}