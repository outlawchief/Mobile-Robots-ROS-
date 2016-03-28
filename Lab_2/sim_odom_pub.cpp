#include "ros/ros.h"
#include "kobuki_msgs/Sound.h"
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Twist.h"
#include "sim_odom_pub_cmdline.h"
#include <math.h>


/*  Odometry Message Structure
Header header
    uint32 seq
    time stamp
    string frame_id
string child_frame_id
geometry_msgs/PoseWithCovariance pose
    geometry_msgs/Pose pose
        geometry_msgs/Point position
            float64 x
            float64 y
            float64 z
        geometry_msgs/Quaternion orientation
            float64 x
            float64 y
            float64 z
            float64 w
    float64[36] covariance
geometry_msgs/TwistWithCovariance twist
    geometry_msgs/Twist twist
        geometry_msgs/Vector3 linear
            float64 x
            float64 y
            float64 z
        geometry_msgs/Vector3 angular
            float64 x
            float64 y
            float64 z
    float64[36] covariance

*/



using namespace ros;
double linx
double liny
double posx
double posy
double 

int main( int argc, char* argv[])
{
	init( argc, argv, "talker");

	gengetopt_args_info args;
	cmdline_parser( argc, argv, &args);

	double v = args.linearVelocity_arg;

	NodeHandle n;
	Publisher pub = n.advertise<nav_msgs::Odometry>("/odom", 1000);

	//kobuki_msgs::Sound msg;
	nav_msgs::Odometry msg;

	//velocity
	//msg.twist.twist.linear

	//angular velocity
	//msg.twist.twist.angular
	

	double t = 10.0;

	ros::Rate loop_rate(10);

	int count = 0;
	int max_count = t*10;

	while( ros::ok() && count < max_count )
	{
		//msg.angular.z = 1*sin(count/10.0);
		pub.publish(msg);
		ros:spinOnce();
		loop_rate.sleep() ;
		count++;

	}

}
