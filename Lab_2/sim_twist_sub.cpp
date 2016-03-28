#include "ros/ros.h"
#include "nav_msgs/Odometry.h"
#include "sim_twist_sub_cmdline.h"
#include "ros/time.h"
#include <fstream>
#include "geometry_msgs/Twist.h"
#include <cmath>
#include <ctime>

double x = 0;
double y = 0;
double z = 0;

double alpha[6] = {.01,.01,.1,.1,.01,.01};

using namespace ros;
void TwistCallback(const geometry_msgs::Twist::ConstPtr& msg)
{

	std::ofstream stream;
	
	/*stream.open("data.txt",std::ios::app);
	stream << ros::Time::now() << "\n";
	stream << msg->pose.pose;
	stream.close();
	std::cout << "pose = " << msg->pose.pose << std::endl;
	*/
	std::cout << "pose = " << msg->linear << std::endl;

}


int main( int argc, char* argv[])
{
	init( argc, argv, "listener");
	NodeHandle n;
	Subscriber sub = n.subscribe<geometry_msgs::Twist>("/cmd_vel_mux/input/navi",100, TwistCallback);


	ros::spin();

}


template <typename T> int sgn(T val) 
{
	return ((T(0) < val) - (val < T(0)));
}


double sample(double lincoef, double angcoef, double velocity, double angular)
{
	double range = lincoef*velocity + angcoef*angular;

	double sample[12];
	double result = 0;
	for( int i = 0;i<12;i++);
	{

		//sample[i] = rand()%(range-(-1*range) + 1.0) + (-1.0*range);
		double test = (double(rand())/RAND_MAX)*range;
	}
	
	result = result/2;

	return result;
}


double* rpy2q(double yaw)
{
	
	double R[3][3];
	
	R[1][1] = cos(yaw);
	R[1][2] = -1 * sin(yaw);
	R[2][1] = sin(yaw);
	R[2][2] = -1 * cos(yaw);
	R[3][3] = 1;
	R[1][3] = 0;
	R[2][3] = 0;
	R[3][1] = 0;	
	R[3][2] = 0;

	double scalar = (1/2)*sqrt(R[1][1] +R[2][2] + R[3][3] +1);
	
	double vector[3];
	vector[0] = sgn(R[3][2] - R[2][3]) * sqrt(R[1][1] -R[2][2] - R[3][3] +1);
	vector[1] = sgn(R[1][3] - R[3][1]) * sqrt(R[2][2] -R[3][3] - R[1][1] +1);
	vector[2] = sgn(R[2][1] - R[1][2]) * sqrt(R[3][3] -R[1][1] - R[2][2] +1);

	double quat[4] = {vector[0],vector[1],vector[2],scalar};
	
	return quat;

}

// Velocity motion model planning.
double * calculatePosition(double v, double w, double theta, double gamma, double deltat)
{
	double newx = x + (((-v/w) * sin(theta)) + (v/w)*sin(theta + w*deltat));
	double newy = y + (((v/w) * cos(theta)) - (v/w)*cos(theta + w*deltat));
	double newtheta = theta + w*deltat + gamma*deltat;
}













