#include <ros/ros.h>
#include <nav_msgs/Odometry.h> 
#include <tf/tf.h>
#include <tf/transform_datatypes.h>
#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/Pose2D.h>
#include <geometry_msgs/Vector3.h>
#include <std_msgs/UInt32.h>
#include <cmath>

using namespace std;

     float xX = 0;
	float yY = 0;

     int change = 0;

     double position[] = {1,2,5,6,0,7};
     int npositions = 3; 
void returnChange(const std_msgs::UInt32::ConstPtr &msg){
     change = msg->data;
}

int main(int argc, char *argv[]){

    ros::init(argc, argv, "tour");
    ros::NodeHandle nh;

    ros::Publisher pubPosition = nh.advertise<geometry_msgs::Vector3>("targetpose", 1000);
    ros::Subscriber subBool = nh.subscribe<std_msgs::UInt32>("sendBool", 1000, &returnChange);

    geometry_msgs::Vector3 vec;
    std_msgs::UInt32 returns;
    while(nh.ok()){
         int i = 0;
         while(i < npositions*2){
               xX = position[i];
               yY = position[i+1];
     	
               vec.x = xX;
               vec.y = yY; 
               vec.z = 0;

               returns.data = change;

               //Debug
               cout << vec.x << endl;
               cout << vec.y << endl;
               cout << vec.z << endl;
               
               pubPosition.publish(vec);

               //returns to gotopoint when change is granted
               if(change == 1){
                    returns.data = 0;
                    pubPosition.publish(returns);
                    break;
               }
               
               continue;
               i += 2;
         }
         
}
    }
    
