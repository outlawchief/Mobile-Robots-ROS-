#include "ros/ros.h"
#include "kobuki_msgs/Sound.h"
#include "geometry_msgs/Twist.h"
#include <math.h>
#include "nav_msgs/Odometry.h"
#include "std_msgs/Empty.h"
#include "ros/time.h"
#include <fstream>
#include "ros_navigator_cmdline.h"

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

void callB(const nav_msgs:: Odometry::ConstPtr& msg)
{
	std::ofstream stream;
	
	stream.open("data.txt",std::ios::app);
	stream << ros::Time::now() << "\n";
	stream << msg->pose.pose;
	stream.close();
	std::cout << "pose = " << msg->pose.pose << std::endl;
	
}


double q2rpy(double* quat)
{
	double R[3][3];

	double w = quat[3];
	double x = quat[0];
	double y = quat[1];
	double z = quat[2];


	R[1][1] = 2*(pow(w,2) + pow(x,2)) - 1;
	R[1][2] = 2*(x*y - w*z);
	R[2][1] = 2*(x*y + w*z);
	R[2][2] = 2*(pow(w,2) + pow(y,2)) -1;
	R[3][3] = 2*(pow(w,2) + pow(z,2)) -1;
	R[1][3] = 2*(x*z - w*y);
	R[2][3] = 2*(y*z - w*x);
	R[3][1] = 2*(x*z - w*y);
	R[3][2] = 2*(y*z - w*x);

	double yaw = acos(R[1][1]);

	return yaw;
}



int main( int argc, char* argv[])
{
	init( argc, argv, "talker");

	gengetopt_args_info args;
	cmdline_parser( argc, argv, &args);

	double v = args.linearVelocity_arg;

	NodeHandle n;
	Publisher geom_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel_mux/input/navi", 1000);
	Subscriber odom_sub = n.subscribe( "/odom", 100, callB);
	Publisher empty_pub = n.advertise<std_msgs::Empty>("/mobile_base/commands/reset_odometry", 1000);


	//kobuki_msgs::Sound msg;
	geometry_msgs::Twist msg;

	//Publisher pub = n.advertise<msg_folder::msg_type>("topic name",1);


	msg.linear.x =.01;

	double t = 10.0;

	ros::Rate loop_rate(10);

	int count = 0;
	int max_count = t*10;

	while( ros::ok() && count < max_count )
	{

		msg.linear.x = .5;		
		msg.angular.z = 1*sin(count/10.0);
		geom_pub.publish(msg);
		ros:spinOnce();
		loop_rate.sleep() ;
		count++;

	}

}











