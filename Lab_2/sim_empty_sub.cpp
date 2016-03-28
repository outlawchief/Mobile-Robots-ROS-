#include "ros/ros.h"
#include "nav_msgs/Odometry.h"
#include "sim_empty_sub_cmdline.h"
#include "ros/time.h"
//#include "turtlebot_node/turtlebotsensorstate.h"
#include <fstream>
#include "std_msgs/Empty.h"
#include "sim_odom-pub.h"


using namespace ros;
void wipeCallback(const std_msgs::Empty::ConstPtr& msg)
{

	std::ofstream stream;
	
	/*stream.open("data.txt",std::ios::app);
	stream << ros::Time::now() << "\n";
	stream << msg->pose.pose;
	stream.close();
	std::cout << "pose = " << msg->pose.pose << std::endl;
	*/
	//std::cout << "pose = " << msg->Empty << std::endl;

	extern linx = 0;
	extern liny = 0;

}


int main( int argc, char* argv[])
{
	init( argc, argv, "listener");
	NodeHandle n;
	Subscriber sub = n.subscribe<std_msgs::Empty>("/mobile_base/commands/reset_odometry",100, wipeCallback);


	ros::spin();

}
