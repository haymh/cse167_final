#include <stdlib.h>
#include "Window.h"
#include <iostream>
#include "Matrix4d.h"
#include "Light.h"

#include "Const.h"
#include "Material.h"
#include "MatrixTransform.h"
#include "Sphere.h"
#include "Line.h"
#include "PiecewiseBezierCurve.h"
#include "Camera.h"





#define ROTSCALE 50.0;
#define ZOOMSCALE 2.0;
#define DELTA 0.1
#define ANGLE 1.0


using namespace std;

GLfloat _ambient[4] = { 0.82, 0.41, 0.12 };    // ambient reflection
GLfloat _diffuse[4] = { 0.82, 0.41, 0.12 };   // diffuse reflection 

// spotlight sources parameters
f4 ambient(0.1, 0.1, 0.1, 1.0);
f4 diffuse(0.2, 0.2, 0.2, 1.0);
f4 specular(1, 1, 1, 1.0);
f4 position(3.0, 10.0, 0.0, 1.0);
f3 spot_direction(-3.0, -10.0, 0.0);

// point light source parameters
f4 p_ambient(0.4, 0.4, 0.4, 1.0);
f4 p_diffuse(1, 1, 1, 1.0);
f4 position2(0.0, 50.0, -4.0, 0.0);

Light spotLight(ambient, diffuse, specular, position);
Light pointLight(p_ambient, p_diffuse, specular, position2);

int Window::width  = 512;   // set window width in pixels here
int Window::height = 512;   // set window height in pixels here
int Window::old_x = width / 2;
int Window::old_y = height / 2;

int g = 1;
bool selected = false;

MatrixTransform * Window::root = new MatrixTransform(Matrix4d());
MatrixTransform * tt;
MatrixTransform * mt;
Turtle3D * Window::turtle;
PiecewiseBezierCurve * pbc;


bool useShader;

//trackball variables
control::MOVEMENT movement = control::NONE;
Vector3d lastPoint;
Matrix4d rotation;
Matrix4d scaling;
MatrixTransform* Window::scaling_mt = new MatrixTransform(scaling);
MatrixTransform* Window::rotate_mt = new MatrixTransform(rotation);

// camera parameters
Vector3d eye(0, 2, -8);
Vector3d lookat(0, 2, 0);
Vector3d up(0, 1, 0);

Camera cam(eye, lookat, up);

KEY key = F1;

void Window::init(){
	/*
	turtle = new Turtle3D("ABC-D+E", "ABCDE", 20, 20, 0.8);
	turtle->addRule('A', "FFFFFFF[-\AE]F[+&+AE]FFF", 1.0);
	turtle->addRule('B', "^^^FF", 0.3);
	turtle->addRule('B', "FF&&&FF", 0.3);
	turtle->addRule('B', "+F\\FF", 0.4);
	turtle->addRule('C', "F[/+CE]F[^AE]F", 1.0);
	turtle->addRule('D', "FF&F",1.0);
	turtle->addRule('E', "[-/F][^-FL][\FFL][+F][&FL][+FFL]FFL",1.0);
	mt = turtle->generate(g);
	Matrix4d t;
	t.makeTranslate(0, -1, 0);
	tt = new MatrixTransform(t);
	root->addChild(rotate_mt);
	rotate_mt->addChild(scaling_mt);
	scaling_mt->addChild(tt);
	tt->addChild(mt);
	*/
	Vector3d * v = new Vector3d[5];
	v[0] = Vector3d(0, 1, 0);
	v[1] = Vector3d(1, 2, 1);
	v[2] = Vector3d(0.5, 1, 2);
	v[3] = Vector3d(-0.2, 2, 1.5);
	v[4] = Vector3d(-0.8, 1, 0);
	pbc = new PiecewiseBezierCurve(4, v, 1000);
}


//----------------------------------------------------------------------------
// Callback method called when system is idle.
void Window::idleCallback()
{
	displayCallback();         // call display routine to show the cube
}

//----------------------------------------------------------------------------
// Callback method called by GLUT when graphics window is resized by the user
void Window::reshapeCallback(int w, int h)
{
	cerr << "Window::reshapeCallback called" << endl;
	width = w;
	height = h;
	glViewport(0, 0, w, h);  // set new viewport size
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, double(width) / (double)height, 1.0, 1000.0); // set perspective projection viewing frustum
	//gluLookAt(eye[0], eye[1], eye[2], lookat[0], lookat[1], lookat[2], up[0], up[1], up[2]);
	//pointLight.apply();
}

//----------------------------------------------------------------------------
// Callback method called by GLUT when window readraw is necessary or when glutPostRedisplay() was called.
void Window::displayCallback()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear color and depth buffers
	glMatrixMode(GL_MODELVIEW);
	//glLoadMatrixd(cam.getMatrix().getPointer());
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, _ambient);
	// set the diffuse reflection for the object
	//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, _diffuse);
	//root->draw(cam.getMatrix());
	//root->draw(Matrix4d());
	glLoadMatrixd((cam.getMatrix() * rotation * scaling).getPointer());
	pbc->render(rotation * scaling);
	
	glFlush();
	glutSwapBuffers(); 
}


void Window::keyboardProcess(unsigned char key, int x, int y){
	switch (key){
	case 'g':
		g++;
		tt->removeChild(mt);
		delete mt;
		mt = turtle->generate(g);
		tt->addChild(mt);
		break;
	case 'f':
		if (g == 0)
			return;
		g--;
		tt->removeChild(mt);
		delete mt;
		mt = turtle->generate(g);
		tt->addChild(mt);
		break;
	case 'd':
		selected = false;
		pbc->deSelect();
		break;
	case 'x':
		if (selected){
			int index = pbc->getSelected();
			Vector3d v = pbc->getCp(index);
			v.set(0, v[0] + DELTA);
			pbc->setCp(index,v);
			cerr << "sdf";
		}
		break;
	case 'X':
		if (selected){
			int index = pbc->getSelected();
			Vector3d v = pbc->getCp(index);
			v.set(0, v[0] - DELTA);
			pbc->setCp(index, v);
		}
		break;
	case 'y':
		if (selected){
			int index = pbc->getSelected();
			Vector3d v = pbc->getCp(index);
			v.set(1, v[1] + DELTA);
			pbc->setCp(index, v);
		}
		break;
	case 'Y':
		if (selected){
			int index = pbc->getSelected();
			Vector3d v = pbc->getCp(index);
			v.set(1, v[1] - DELTA);
			pbc->setCp(index, v);
		}
		break;
	case 'z':
		if (selected){
			int index = pbc->getSelected();
			Vector3d v = pbc->getCp(index);
			v.set(2, v[2] + DELTA);
			pbc->setCp(index, v);
		}
		break;
	case 'Z':
		if (selected){
			int index = pbc->getSelected();
			Vector3d v = pbc->getCp(index);
			v.set(2, v[2] - DELTA);
			pbc->setCp(index, v);
		}
		break;

	case 27:
		exit(0);
	}
}
void Window::processSpecialKeys(int k, int x, int y){
	switch (k){
	case GLUT_KEY_LEFT:
		pbc->selectNext();
		selected = true;
		break;
	case GLUT_KEY_RIGHT:
		pbc->selectPrev();
		selected = true;
		break;
	case GLUT_KEY_F1:
		cam.move(FORWARD, DELTA);
		break;
	case GLUT_KEY_F2:
		cam.move(BACKWARD, DELTA);
		break;
	case GLUT_KEY_F3:
		cam.move(LEFTWARD, DELTA);
		break;
	case GLUT_KEY_F4:
		cam.move(RIGHTWARD, DELTA);
		break;
	case GLUT_KEY_F5:
		cam.move(UPWARD, DELTA);
		break;
	case GLUT_KEY_F6:
		cam.move(DOWNWARD, DELTA);
		break;
	case GLUT_KEY_F7:
		cam.rotate(Vector3d(0, 1, 0), ANGLE);
		break;
	}
}
void Window::mouseMotionProcess(int x, int y){
	Vector3d direction;
	double pixel_diff;
	double rot_angle, zoom_factor;
	Vector3d curPoint;
	curPoint = trackBallMapping(x, y);
	switch (movement){
	case control::ROTATION:
	{
		direction = curPoint - lastPoint;
		double velocity = direction.magnitude();
		if (velocity > 0.0001){
			Vector3d rotAxis = lastPoint * curPoint;
			rotAxis.normalize();
			rot_angle = velocity * ROTSCALE;
			/*
			Matrix4d r;
			r.makeRotate(rot_angle, rotAxis);
			rotation = r * rotation;
			rotate_mt->setMatrix(rotation);
			*/
			cam.rotate(rotAxis, rot_angle);
		}
	}
		break;
	case control::SCALING:
	{
		pixel_diff = curPoint[1] - lastPoint[1];
		zoom_factor = 1.0 + pixel_diff * ZOOMSCALE;
		Matrix4d s;
		s.makeScale(zoom_factor, zoom_factor, zoom_factor);
		scaling = scaling * s;
		scaling_mt->setMatrix(scaling);
		displayCallback();

	}
		break;
	}
	lastPoint = curPoint;
}


void Window::mouseProcess(int button, int state, int x, int y){
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		movement = control::ROTATION;
		old_x = x;
		old_y = y;
		lastPoint = trackBallMapping(x, y);
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		movement = control::SCALING;
		old_x = x;
		old_y = y;
		lastPoint = trackBallMapping(x, y);
	}
	else
		movement = control::NONE;
}


Vector3d Window::trackBallMapping(int x, int y){
	Vector3d v(double (2*x - width) / double (width), double (height - 2*y) / double (height), 0);
	double d = v.magnitude();
	d = d < 1.0 ? d : 1.0;
	v.set(2, sqrt(1.001 - d*d));
	v.normalize();
	return v;
}