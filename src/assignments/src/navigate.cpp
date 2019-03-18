# include <cmath>
# include <cstdlib>
# include <ros/ros.h>
# include <geometry_msgs/Pose2D.h>
# include <move_base_msgs/MoveBaseAction.h>
# include <actionlib/client/simple_action_client.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
using namespace std;

float x;
float y;
float theta;
bool msgs;

double position[] = {-5,-5,-5,5,5,5,5,-5};
int npositions = 4; 

void gotolocation(const geometry_msgs::Pose2D&msg) 
{	
    int i = 0;
    while(i < npositions*2){
        x = position[i];
        y = position[i+1];
	    theta = 0;
	    msgs = true;

        cout << x << endl;
        cout << y << endl;
        cout << theta << endl;

        if(!msgs){
            i += 2;
        }
    }
}

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "navigate");
    ros::NodeHandle nh;
    ros::Subscriber subPos = nh.subscribe("targetpose", 1000, &gotolocation);
    
    MoveBaseClient ac("move_base", true);

    while(ros::ok())
    {
        while (!ac.waitForServer()) 
        {
            ROS_INFO("Waiting for the move_base action server to come up");
            exit(0);
        }
        while(!msgs)
        {
            ros::spinOnce();
        }

        move_base_msgs::MoveBaseGoal goal;
        goal.target_pose.header.frame_id = "map";
        goal.target_pose.header.stamp = ros::Time::now();
        goal.target_pose.pose.position.x = x;
        goal.target_pose.pose.position.y = y;
        goal.target_pose.pose.orientation.z = sin(theta/2);
        goal.target_pose.pose.orientation.w = cos(theta/2);

        ac.sendGoal(goal);
        ac.waitForResult();

        if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
            ROS_INFO_STREAM("\t\t\t\tGOAL Reached!!!!");
        else
            ROS_INFO_STREAM("\t\t\t\tFailure");
        msgs = false;
    }
    return 0;
}