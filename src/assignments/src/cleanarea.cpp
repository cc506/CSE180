# include <cmath>
# include <cstdlib>
# include <ros/ros.h>
# include <geometry_msgs/PoseWithCovarianceStamped.h>
# include <geometry_msgs/PoseStamped.h>
# include <move_base_msgs/MoveBaseAction.h>
# include <actionlib/client/simple_action_client.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

using namespace std;

float x ,y ,theta;
float goalx, goaly;

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

void amcl_location(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr &msg) 
{	
    x = msg->pose.pose.position.x;
    y = msg->pose.pose.position.y;
}

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "cleanarea");
    ros::NodeHandle nh;
    ros::Rate rate(1);

    ros::Subscriber subPos = nh.subscribe("/amcl_pose", 1000, &amcl_location);

    MoveBaseClient ac("move_base", true);


}