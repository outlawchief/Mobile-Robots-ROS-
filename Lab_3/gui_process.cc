#include <QtGui/QApplication> 
#include "gui.h"
#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h" 

#include "gui_process_cmdline.h"

using namespace std;


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

int main( int argc,char* argv[] ){
	std::cout << "starting" << std::endl << std::flush;
	gengetopt_args_info args;
	cmdline_parser( argc, argv, &args );
	QApplication app( argc, argv );
	
	ros::init( argc, argv, "gui" );

	ros::NodeHandle node_handle;

	GUI gui;

	ros::Subscriber odom_sub = node_handle.subscribe( "/odom", 1, &GUI::handle_odom, &gui );
	ros::Subscriber laser_sub = node_handle.subscribe("/scan",100, &GUI::handle_laserscan, &gui );

	ros::Subscriber subscriber_odom = node_handle.subscribe( "/odom", 1, &GUI::handle_odom, &gui );
	gui.show();

	return app.exec(); 
}
