#include <iostream>
#include "ros/ros.h"
#include "nav_msgs/Odometry.h" 
#include "sensor_msgs/LaserScan.h" 
#include <QtGui/QApplication> 
#include <QtGui/QWidget>
#include <QtOpenGL/QGLWidget> 
#include <QtCore>
#include <QTimer>
#include <GL/glu.h>

class GUI: public QGLWidget { 
	Q_OBJECT 
	public:
		GUI( QWidget * parent = NULL ); 
		virtual ~GUI();
			void handle_laserscan( const sensor_msgs::LaserScan::ConstPtr& msg ); 
			void handle_odom( const nav_msgs::Odometry::ConstPtr& msg );


			QTimer* timer; 

	protected slots:
		void timer_callback( void );
		void drawCircle(float centerx, float centery, float radius);
		void drawLasers();
		double q2rpy(double* quat);
		float gaussiansample();
	protected:
		virtual void initializeGL(); 
		virtual void paintGL();
};
