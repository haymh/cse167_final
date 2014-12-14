#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "Vector4d.h"
#include "Group.h"
#include "MatrixTransform.h"
#include "Turtle3D.h"


class Window	  // OpenGL output window related routines
{
  public:
    static int width, height; 	            // window size
	static int old_x, old_y;

	static void init();
    static void idleCallback(void);
    static void reshapeCallback(int, int);
    static void displayCallback(void);
	static void keyboardProcess(unsigned char key, int x, int y);
	static void processSpecialKeys(int k, int x, int y);
	static void mouseMotionProcess(int, int);
	static void mouseProcess(int, int, int, int);

private :
	static MatrixTransform* root; 
	static MatrixTransform * scaling_mt;
	static MatrixTransform * rotate_mt;
	static Vector3d trackBallMapping(int x, int y);
	static Turtle3D *turtle;
	
};

#endif

