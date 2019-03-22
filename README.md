# CSE180
Labs for Robotics

## Requirements
* Install ROS (Kinetic or newer) using the following command:
```
sudo apt-get update
sudo apt-get install ros-<your distribution>-desktop-full
```
* Ubuntu OS (16.04 or newer) is highly recommended. However, ROS can be installed on other operating systems.

## Building
Run `catkin_init_workspace` inside of the src directory and two folders will be created in the base workspace directory, build and devel. In order for ROS to know where your workspace is, you will either need to run `source devel/setup.bash` once per terminal, or put this command inside of your `.bashrc` within your home directory.

## Launching Gazebo
The labs require Gazebo simulation, which is built into ROS. Launch it by following the instructions below:

**Pioneer (Labs 1 - 3)**
```
roslaunch p2os_urdf pioneer3dx.gazebo.launch
```
**Husky (Lab 4 and 5)<br/>**

First install Husky:<br/>
* Kinetic:
```
sudo apt-get update
sudo apt-get install ros-kinetic-husky-*
```
* Melodic:
```
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install ros-melodic-twist-mux
sudo apt-get install ros-melodic-robot-localization
sudo apt-get install ros-melodic-imu*
sudo apt-get intalll ros-melodic-nmea*
sudo apt-get install ros-melodic-urdf*
sudo apt-get install ros-melodic-um7
cd ~/CSE180/src
wget http://robosrv.ucmerced.edu/husky.tgz
tar zxvf husky.tgz
cd ~/CSE180
catkin_make
```
For both distributions, launch Husky Gazebo the same way:
```
roslaunch husky_gazebo husky_playpen.launch
```

**Husky with Launch File (Lab 6)**

```
roslaunch assignments husky_navigate.launch
```

## Testing
To test in gazebo, enter the below input according to your node:
```
rosrun assignments <your-node>
```