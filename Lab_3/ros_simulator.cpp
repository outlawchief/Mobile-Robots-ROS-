#include "ros/ros.h"
#include "ros/time.h"

#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"
#include "std_msgs/Empty.h"
#include "sensor_msgs/LaserScan.h" 
#include "ros_simulator_cmdline.h"

#include <fstream>
#include <cmath>
#include <math.h>
#include <ctime>
#include <random>



#include "gui.h"


double x = 0;
double y = 0;
double z = 0;
double linx = 0;
double liny = 0;

double alpha[6] = {.01,.01,.01,.01,.01,.01};

// ENVIRONMENT GEOMETRY
float cone_radius = 0.1f;
int cone_centerx = 0;
int cone_centery = 1;
float perimpoints[360][2];
std::vector<float> perimangles(360);
std::vector<float> wallsegment(100);

//Robot Position
double robotx = 0;
double roboty = 0;
float yaw = 0;

std::uniform_real_distribution<double> distribution(-1.0,1.0);
std::default_random_engine generator;

//prev message timestamps

double prevnavtime;


using namespace ros;
#define _USE_MATH_DEFINES


template <typename T> int sgn(T val) 
{
	return ((T(0) < val) - (val < T(0)));
}


double distributionSample(double lincoef, double angcoef, double velocity, double angular)
{
	double result = 0;
	int i;

	for(i = 0;i<12;i++);
	{
		
		result += distribution(generator);
		
	}
	
	result = result/2;

	return result;
}




void rpy2q(double yaw, double (&quat)[4])
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

	quat[0] = vector[0];
	quat[1] = vector[1];
	quat[2] = vector[2];
	quat[3] = scalar;
	
	//return *quat;

}

// Velocity motion model planning.
// v is linear velocity
// w is angular velocity
// theta is 
std::vector<double> calculatePosition(double v, double w, double theta, double msgtime)
{	

	double deltat = (msgtime - prevnavtime);
	std::cout << "Time:" << ros::Time::now().toSec() << std::endl;
	prevnavtime = msgtime;
	double newx = x + (((-v/w) * sin(theta)) + (v/w)*sin(theta + w*deltat));
	std::cout << "newx:" << newx << std::endl;
	double newy = y + (((v/w) * cos(theta)) - (v/w)*cos(theta + w*deltat));
	double gamma = alpha[4] * v + alpha[5]*w;
	double newtheta = theta + w*deltat + gamma*deltat;
	std::vector<double> pose(3);
	pose[0] = newx;
	pose[1] = newy;
	pose[2] = newtheta;

	robotx = newx;
	roboty = newy;
	yaw = newtheta;


	return pose;
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


void TwistCallback(const geometry_msgs::Twist::ConstPtr& msg)
{

	std::ofstream stream ("simdata.txt", std::ofstream::out);
	
	//stream << 
	
	/*stream.open("data.txt",std::ios::app);
	stream << ros::Time::now() << "\n";
	stream << msg->pose.pose;
	stream.close();
	std::cout << "pose = " << msg->pose.pose << std::endl;
	*/
	calculatePosition(msg->linear.x,msg->angular.z,yaw,ros::Time::now().toSec());
	//std::cout << "pose = " << msg->linear << std::endl;

}

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

	robotx = 0;
	roboty = 0;

}

float distance(double targetx, double targety, double originx, double originy)
{
	return float(sqrt(pow(targetx - originx,2) + pow(targety - originy,2)));
}

int len(float *p)
{
	return (sizeof(p)/sizeof(*p));
}
/*
void circlePoints(float centerx, float centery, float radius)
{
	for(int i = 0; i<360; i++)
	{
		
		perimpoints[i][0] = cos(i);
		perimpoints[i][1] = sin(i);
		perimangles[i] = atan(perimpoints[i][0] - robotx / perimpoints[i][1] - roboty);
			
	}
}
*/

int main( int argc, char* argv[])
{
	init( argc, argv, "listener");  

	//QWidget* widget = new QWidget();
	//GUI* gui  = new GUI(widget);
	

	NodeHandle n;
	prevnavtime = ros::Time::now().toSec();
	Subscriber geom_sub = n.subscribe<geometry_msgs::Twist>("/cmd_vel_mux/input/navi",100, TwistCallback);
	Subscriber empty_sub = n.subscribe<std_msgs::Empty>("/mobile_base/commands/reset_odometry",100, wipeCallback);
	

	Publisher laser_pub = n.advertise<sensor_msgs::LaserScan>("/scan",100);
	Publisher odom_pub = n.advertise<nav_msgs::Odometry>("/odom", 1000);


	nav_msgs::Odometry odom_msg;




	sensor_msgs::LaserScan laser_msg;
	laser_msg.angle_min = -M_PI/4.0f;
	laser_msg.angle_max = M_PI/4.0f;
	laser_msg.angle_increment = (M_PI/2.0f)/64.0f;
	

	double t = 45.0;

	ros::Rate loop_rate(10);

	int count = 0;
	int max_count = t*10;
	float resolution = 64.0f;
	float range = M_PI/2;
	std::vector<float> distances(resolution);
	laser_msg.ranges.resize(resolution);

	while( ros::ok() && count < max_count )
	{
		odom_msg.pose.pose.position.x = robotx;
		odom_msg.pose.pose.position.y = roboty;
		//odom_msg.pose.pose.orientation
		odom_pub.publish(odom_msg);
		ros:spinOnce();
		loop_rate.sleep() ;


		for(int i = 0; i<360; i++){
		
			perimpoints[i][0] = cone_radius*cos(i)+cone_centerx;
			perimpoints[i][1] = cone_radius*sin(i)+cone_centery;
			perimangles[i] = atan(perimpoints[i][0] - robotx / perimpoints[i][1] - roboty);
			
		}
		for(int i =0; i <(int)distances.size();i++)
		{
			distances[i] = 20;
		}

		
		int count = 0;
		for(double i = -M_PI/4.0f ; i<M_PI/4.0f ; i+=(range/resolution) )
		{
			//check angle against environement
			//J is for indexing, I is a double
			//std::cout<<"looping: "<< i << std::endl;
			float initdist = 1000;
			for(int j = 0  ; j < perimangles.size(); j++)
			{
				if(i == perimangles[j])
				{
					distances[count] = distance(perimpoints[j][0], perimpoints[j][1], robotx, roboty);
					std::cout << "direct hit" << std::endl;
				}
				else
				{	
					//std::cout << "miss" << std::endl;
					//if there is room to go up by one (and if its going to hit the cone)
					if(j < (perimangles.size() - 1))
					{
						//std::cout << "well sized" << std::endl;
						if(i > perimangles[j] && i< perimangles[j+1])
						{
							//std::cout<<"in the middle"<<std::endl;
							if( fabs(i- perimangles[j]) < fabs(i - perimangles[j+1]))
								
									distances[count] =  distance(perimpoints[j][0], perimpoints[j][1], robotx, roboty);
								
							else
							{
								
									distances[count] =  distance(perimpoints[j+1][0], perimpoints[j+1][1], robotx, roboty);
								
							}
						}
						else
						{	
							//std::cout << i << "VS" << perimangles[j] << " VS " << perimangles[j+1]<<std::endl;
						}	

					}
				}

			}
		count++;
		}

		//std::cout << distances.size() << std::endl;
		
		for(int i = 0; i<360; i++)
		{
			//std::cout << perimangles[i] << std::endl;
		}

		for(int x = 0; x<distances.size() ; x++)
		{
			//segfault for some reason
			laser_msg.ranges[x] = distances[x];
			//std::cout << "working" << std::endl;
			//std::cout << distances[x] << ",";
		}

		count++;
		laser_pub.publish(laser_msg);

		


	}

}






