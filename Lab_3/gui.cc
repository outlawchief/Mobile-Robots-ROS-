#include "gui.h"
#include <math.h>
#include <random>
#include <fstream>

//Laser Info
int resolution = 64;
std::vector<float> distances(resolution);
int max_range = 20;
std::uniform_real_distribution<float> distribution(-1.0,1.0);
std::default_random_engine generator;

float robotx = 0;
float roboty = 0;
float robotradius = .15;
float robotorient[4];
float robotyaw = 0;
float span;
//cone points
float cone_centerx = 0;
float cone_centery = 1;
float cone_radius = .1;
float perimpoints[360][2];


GUI::GUI( QWidget * parent ) : QGLWidget( parent ), timer() {
	timer = new QTimer(NULL);
	setMinimumSize( 600, 600 );
	connect( timer, SIGNAL( timeout() ), this, SLOT( timer_callback() ) );
	timer->start( 100 ); // call timer_callback at 0.1 Hz (period is 100 ms) 
}
GUI::~GUI(){
}

void GUI::handle_laserscan( const sensor_msgs::LaserScan::ConstPtr& msg ){
 	// implement storing of laserscan message here
	
	for( int i = 0; i<msg->ranges.size(); i++)
	{
		distances[i] = msg->ranges[i];
	}
	
	span = msg->angle_min - msg->angle_max;

	return; 
}
void GUI::handle_odom( const nav_msgs::Odometry::ConstPtr& msg ){

	// implement storing of robot pose here

	std::cout << "Pose x" << msg->pose.pose.position.x << std::endl;
	robotx = msg->pose.pose.position.y;
	roboty = msg->pose.pose.position.x;
	robotorient[0] = msg->pose.pose.orientation.x;
	robotorient[1] = msg->pose.pose.orientation.y;
	robotorient[2] = msg->pose.pose.orientation.z;
	robotorient[3] = msg->pose.pose.orientation.w;

	return; 
}

double GUI::q2rpy(double* quat)
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



float GUI::gaussiansample()
{
	
	float result = 0;
	int i;
	//std::cout << "just outside for loop" << endl;
	for(i = 0;i<12;i++);
	{
		//std::cout << "not yet fault" << endl;
		//sample[i] = rand()%(range-(-1*range) + 1.0) + (-1.0*range);
		//result += (float(rand())/RAND_MAX)*max_range;
		result += distribution(generator);
		
	}
	
	result = result/2;

	return result;
}




void GUI::timer_callback( void ) {
        this->updateGL();
	ros::spinOnce(); // Process the messages in here 
	return;

}

void GUI::initializeGL(){
	glClearColor( 1.0, 1.0, 1.0, 1.0 ); gluOrtho2D(-100,100,-100,100);
	return;
}

void GUI::drawCircle(float centerx, float centery, float radius)
{
	for(int i = 0; i<360; i++)
	{
		
		perimpoints[i][0] = radius*cos(i) + centerx;
		perimpoints[i][1] = radius*sin(i) + centery;
		//perimangles[i] = atan(perimpoints[i][0] - robotx + perimpoints[i][1] + roboty);
			
	}
	
	glBegin( GL_LINES );
	for(int i =0; i<360; i++)
	{

		glVertex3f(perimpoints[i][0],perimpoints[i][1],0.0);
	}
	glEnd();

}

void GUI::drawLasers()
{
	std::ofstream stream ("simdata.txt", std::ofstream::app);
	
	
	
	float angle = M_PI/4;
	for(int i =0;i<distances.size();i++)
	{
		glBegin( GL_LINES );	

		glVertex3f(robotx,roboty,0);
		float noise = .1 * gaussiansample();
		std::cout << "drawing lasers" << std::endl;
		stream << distances[i] + noise << ",";
		glVertex3f(robotx+ (distances[i]+noise)*cos(angle), roboty + (distances[i]+noise)*sin(angle),0);
		
		glEnd();
		angle += (M_PI/2.0)/64.0f;
	}
	stream << "\n";	
	std::cout << "firing lasers" << std::endl;
	
	
}


void GUI::paintGL(){
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
	glLoadIdentity();
	// draw a coordinate system at the origin
	glBegin( GL_LINES );
	glColor4f( 1.0, 0.0, 0.0, 1.0 );
	glVertex3f( 0.0, 0.0, 0.0 );
	glVertex3f( 1.0, 0.0, 0.0 );
	glColor4f( 0.0, 1.0, 0.0, 1.0 );
	glVertex3f( 0.0, 0.0, 0.0 );
	glVertex3f( 0.0, 1.0, 0.0 );
	glColor4f( 0.0, 0.0, 1.0, 1.0 );
	glVertex3f( 0.0, 0.0, 0.0 );
	glVertex3f( 0.0, 0.0, 1.0 );
	glEnd();
	//std::cout << "drawing" << std::endl << std::flush;
	drawCircle(cone_centerx,cone_centery,cone_radius);
	drawCircle(robotx,roboty,robotradius);
	drawLasers();
	

	// implement drawing of laserscan and robot pose here
	
	return; 
}
