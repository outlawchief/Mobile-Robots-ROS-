#include "ros/ros.h"
#include "kobuki_msgs/Sound.h"
#include "geometry_msgs/Twist.h"
#include "ros_publisher_cmdline.h"
#include <math.h>

/* Twist format
geometry_msgs/Vector3 linear
  float64 x
  float64 y
  float64 z
geometry_msgs/Vector3 angular
  float64 x
  float64 y
  float64 z


*/

using namespace ros;

int main( int argc, char* argv[])
{
	init( argc, argv, "talker");

	gengetopt_args_info args;
	cmdline_parser( argc, argv, &args);

	double v = args.linearVelocity_arg;

	NodeHandle n;
	Publisher pub = n.advertise<geometry_msgs::Twist>("/cmd_vel_mux/input/navi", 1000);

	//kobuki_msgs::Sound msg;
	geometry_msgs::Twist msg;

	//Publisher pub = n.advertise<msg_folder::msg_type>("topic name",1);


	msg.linear.x =0;

	double t = 10.0;

	ros::Rate loop_rate(10);

	int count = 0;
	int max_count = t*10;

	while( ros::ok() && count < max_count )
	{
		msg.angular.z = 1*sin(count/10.0);
		pub.publish(msg);
		ros:spinOnce();
		loop_rate.sleep() ;
		count++;

	}

}
