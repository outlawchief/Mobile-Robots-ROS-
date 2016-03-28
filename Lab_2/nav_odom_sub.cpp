#include "ros/ros.h"
#include "nav_msgs/Odometry.h"
#include "nav_odom_sub_cmdline.h"
#include "ros/time.h"
#include <fstream>


using namespace ros;
void callB(const nav_msgs:: Odometry::ConstPtr& msg)
{
	std::ofstream stream;
	
	stream.open("data.txt",std::ios::app);
	stream << ros::Time::now() << "\n";
	stream << msg->pose.pose;
	stream.close();
	std::cout << "pose = " << msg->pose.pose << std::endl;
	
}


int main( int argc, char* argv[])
{
	init( argc, argv, "listener");
	NodeHandle n;
	Subscriber sub = n.subscribe( "/odom", 100, callB);


	ros::spin();

}
