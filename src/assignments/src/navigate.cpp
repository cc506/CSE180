# include <cmath>
# include <cstdlib>
# include <ros/ros.h>
# include <geometry_msgs/PoseWithCovarianceStamped.h>
# include <geometry_msgs/PoseStamped.h>
# include <move_base_msgs/MoveBaseAction.h>
# include <actionlib/client/simple_action_client.h>

//typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

using namespace std;

float goalx;
float goaly;

double positions[] = {-5,-5,-5,5,5,5,5,-5};
int npositions = 4; 

void goToGoal(double goalx, double goaly, ros::Publisher pub){

    geometry_msgs::PoseStamped goal;
    goal.header.frame_id = "map";
    goal.pose.position.x = goalx;
    goal.pose.position.y = goaly;
    goal.pose.position.z = 0;
    goal.pose.orientation.w = 1;
    pub.publish(goal);

    cout << goalx << endl;
    cout << goaly << endl;
}

// void amcl_location(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr &msg) 
// {	
//     x = msg->pose.pose.position.x;
//     y = msg->pose.pose.position.y;
// }

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "navigate");
    ros::NodeHandle nh;
    ros::Rate rate(1);

    ros::Publisher pub = nh.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal", 1000);
    //ros::Subscriber subPos = nh.subscribe("/amcl_pose", 1000, &amcl_location);

    ros::Duration(3).sleep();
    goToGoal(-5.0,-5.0, pub);
    ros::Duration(30).sleep();
    goToGoal(-5.0,5.0, pub);
    ros::Duration(30).sleep();
    goToGoal(5.0,5.0, pub);
    ros::Duration(30).sleep();
    goToGoal(5.0,-5.0, pub);

    //MoveBaseClient ac("move_base", true);
    ros::spin();
}