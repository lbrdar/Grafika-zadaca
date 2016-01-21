#include <GL/gl.h>
#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <Eigen/Dense>
using namespace Eigen;
using namespace std;

Vector4d pt_x1, pt_y1, pt_z1;
Vector4d pt_x2, pt_y2, pt_z2;
Vector4d pt_x3, pt_y3, pt_z3;
VectorXd t_x1, t_y1, t_z1;
VectorXd t_x2, t_y2, t_z2;
VectorXd t_x3, t_y3, t_z3;
VectorXd ts;
Vector3d sphere_coo;
Vector3d cube_coo;
float counter1=0;
float counter2=0;
float counter3 = 0;
bool start = false;
bool stop = false;
float angle_0=-90;
float angle_n=0;
float angle1 = -90;
float angle2 = 0;
float angle3 = 0;

long binomials ( long n, long k ){
    // n && k >=0
    long i;
    long b;

    if ( 0 == k || n == k )
    {
        return 1;
    }

    if ( k > n )
    {
        return 0;
    }

    if ( k > ( n - k ) )
    {
        k = n - k;
    }

    if ( 1 == k )
    {
        return n;
    }

    b = 1;

    for ( i = 1; i <= k; ++i )
    {
        b *= ( n - ( k - i ) );

        if ( b < 0 ) return -1; /* OVERFLOW */

        b /= i;
    }

    return b;
}

double polyterm ( const int &n, const int &k, const double &t ){
    return pow ( ( 1.-t ),n-k ) *pow ( t,k );
}

double getValue ( const double &t, const VectorXd &v ){
    int order = v.size()-1;
    double value = 0;

    for ( int n=order, k=0; k<=n; k++ )
    {
        if ( v ( k ) ==0 ) continue;

        value += binomials ( n,k ) * polyterm ( n,k,t ) * v ( k );
    }

    return value;
}

void changeSize ( int w, int h ){
    if ( h == 0 )
    {
        h = 1;
    }

    float ratio = 1.0* w / h;

    glMatrixMode ( GL_PROJECTION );
    glLoadIdentity();
    glViewport ( 0, 0, w, h );
    gluPerspective ( 45,ratio,1,1000 ); // view angle u y, aspect, near, far
}

void keyboard (unsigned char key, int x, int y) {
	  switch(key){
	    case 27: exit(0); break;
	    case 's': start = true; break;
	    case 'x': stop = true; break;
	    default: break;
	  }
}

void generateBezierCurve1(){
	ts = VectorXd::LinSpaced ( 21,0,1. );
	t_x1.resize ( ts.size() );
	t_y1.resize ( ts.size() );
	t_z1.resize ( ts.size() );

	for ( int idx=0; idx< ts.size(); ++idx )
	{
		t_x1 ( idx ) = getValue ( ts ( idx ), pt_x1 );
		t_y1 ( idx ) = getValue ( ts ( idx ), pt_y1 );
		t_z1 ( idx ) = getValue ( ts ( idx ), pt_z1 );
        }
}

void generateBezierCurve2(){
	ts = VectorXd::LinSpaced ( 21,0,1. );
	t_x2.resize ( ts.size() );
	t_y2.resize ( ts.size() );
	t_z2.resize ( ts.size() );

	for ( int idx=0; idx< ts.size(); ++idx )
	{
		t_x2 ( idx ) = getValue ( ts ( idx ), pt_x2 );
		t_y2 ( idx ) = getValue ( ts ( idx ), pt_y2 );
		t_z2 ( idx ) = getValue ( ts ( idx ), pt_z2 );
        }
}

void generateBezierCurve3(){
	ts = VectorXd::LinSpaced ( 21,0,1. );
	t_x3.resize ( ts.size() );
	t_y3.resize ( ts.size() );
	t_z3.resize ( ts.size() );

	for ( int idx=0; idx< ts.size(); ++idx )
	{
		t_x3 ( idx ) = getValue ( ts ( idx ), pt_x3 );
		t_y3 ( idx ) = getValue ( ts ( idx ), pt_y3 );
		t_z3 ( idx ) = getValue ( ts ( idx ), pt_z3 );
        }
}

void drawBezierCurve1(){
    glPushMatrix();
	glColor3f ( 1.0f, 0.0f, 0.0f );
	glBegin ( GL_LINE_STRIP );
	for ( int i=0; i< (int)counter1; ++i ){
	    glVertex3f ( t_x1 ( i ), t_y1 ( i ), t_z1 ( i ) );
	}
	glEnd();
    glPopMatrix();
}

void drawBezierCurve2(){
    glPushMatrix();
	glColor3f ( 1.0f, 0.0f, 0.0f );
	glBegin ( GL_LINE_STRIP );
	for ( int i=0; i< (int)counter2; ++i ){
	    glVertex3f ( t_x2 ( i ), t_y2 ( i ), t_z2 ( i ) );
	}
	glEnd();
    glPopMatrix();
}

void drawBezierCurve3(){
    glPushMatrix();
	glColor3f ( 1.0f, 0.0f, 0.0f );
	glBegin ( GL_LINE_STRIP );
	for ( int i=0; i< (int)counter3; ++i ){
	    glVertex3f ( t_x3 ( i ), t_y3 ( i ), t_z3 ( i ) );
	}
	glEnd();
    glPopMatrix();
}

void drawSphere(){
    glPushMatrix();
    glTranslatef(sphere_coo(0), sphere_coo(1), sphere_coo(2));
    glColor3f(0,0,1);
    glutSolidSphere(0.2, 16,16);
    glPopMatrix();
    
}

void drawScene(){
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glMatrixMode ( GL_MODELVIEW ); // idemo u perspektivu
    
    glLoadIdentity(); // resetiranje
    gluLookAt ( 0.0,0.0,10.0, // camera
                0.0,0.0,-1.0, // where
                0.0f,1.0f,0.0f ); // up vector

	glPushMatrix();
		glTranslatef(-5.4, 3.9, 0);
		glPushMatrix();
			glTranslatef(-pt_x1(0), -pt_y1(0), -pt_z1(0));
			drawBezierCurve1();
			drawBezierCurve2();
			drawBezierCurve3();
			drawSphere();
		glPopMatrix();
	glPopMatrix();
	
    glutSwapBuffers();
}

void update ( int  ){
  if( (start) && !(stop) ){
	glutPostRedisplay();
	    if(angle1 <0)
	    {
		    counter1 += 0.238;
		    double t = (angle1 - angle_0)/(angle_n - angle_0);
		    sphere_coo(0) = getValue(t, pt_x1);
		    sphere_coo(1) = getValue(t, pt_y1);
		    sphere_coo(2) = getValue(t, pt_z1);
		    angle1 +=1;
		    if (angle1 == 0){angle2=-90;}
	    } else if(angle2 < 0){
		    counter2 += 0.238;
		    double t = (angle2 - angle_0)/(angle_n - angle_0);
		    sphere_coo(0) = getValue(t, pt_x2);
		    sphere_coo(1) = getValue(t, pt_y2);
		    sphere_coo(2) = getValue(t, pt_z2);
		    angle2 +=1;
		    if (angle2 == 0){angle3=-90;}
	    }else if(angle3 < 0){
		    counter3 += 0.238;
		    double t = (angle3 - angle_0)/(angle_n - angle_0);
		    sphere_coo(0) = getValue(t, pt_x3);
		    sphere_coo(1) = getValue(t, pt_y3);
		    sphere_coo(2) = getValue(t, pt_z3);
		    angle3 +=1;
	    }

	    
	    if ( (int)counter1 > ts.size() ) counter1= ts.size();
	    if ( (int)counter2 > ts.size() ) counter2= ts.size();
	    if ( (int)counter3 > ts.size() ) counter3= ts.size();
   }
   glutTimerFunc ( 25, update, 0 );
}

int main ( int argc, char **argv ){
    
    pt_x1 << -0.9, 0.4, 1.7, 2;
    pt_y1 << 5, 4.8, 3, -3;
    pt_z1 << 0, 0, 0, 0;
    
    pt_x2 << 2, 2.3, 6.2, 6.5;
    pt_y2 << -3, 3, 3, -3;
    pt_z2 << 0, 0, 0, 0;
    
    pt_x3 << 6.5, 7.67, 8.84, 10;
    pt_y3 << -3, 1, 1, -3;
    pt_z3 << 0, 0, 0, 0;
    
    sphere_coo << pt_x1(0), pt_y1(0), pt_z1(0);
    
    generateBezierCurve1();
    generateBezierCurve2();
    generateBezierCurve3();
    glutInit ( &argc, argv );
    glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize ( 800, 600 );

    glutCreateWindow ( "Zadaca - zadatak1" );
    glutReshapeFunc ( changeSize );
    glutDisplayFunc ( drawScene );
    glutKeyboardFunc(keyboard);
    glutTimerFunc ( 25, update, 0 );
    glutMainLoop();

    return 0;
}
