#include "vector.h"
#include <assert.h>

#define WHEEL_AXLE_HEIGHT 4.0
#define WHEEL_RADIUS 20.0
#define SLICES 50


// global variables initialization
Point pos, U, R, L;
// pos indicates the position of the camera
// U -> upper vector
// R -> right indicating vector
// L -> Look vector
// Notice: U, R, L are all unit vectors

Point wheelCenter; // vector pointing to the center of the wheel
Point wheelForward; // vector indicating the front direction of the wheel

/// wForwardRotateChange: angular change at each step when wheel moves forward
double wForwardRotateChange = 5; /// unit: degrees;

/// wXaxisAngleChange: rotation angle with respect to x-axis when rotating left or right
double wXaxisAngleChange = 3; /// unit: degrees;


/// wAxleRotationAngle: how much rotation of the axle there will be
/// at a certain point of relocation
double wAxleRotationAngle = 0; /// unit: degrees

/// wAngleWithXaxis: The current angle with respect to the x-axis at any point of time
double wAngleWithXaxis = 0;

double distChange = WHEEL_RADIUS * deg2Rad(wForwardRotateChange);

void rotate_vector(Point *vec, Point axis, int angle_dir) // angle dir-> + for anticlockwise, - for clockwise
{
    Point vec_cross_axis = axis.crossProduct(vec->returnDeepCopy());

    double ang = angle_dir * deg2Rad(wXaxisAngleChange);

    vec->x = vec->x * cos(ang) + vec_cross_axis.x * sin(ang);
    vec->y = vec->y * cos(ang) + vec_cross_axis.y * sin(ang);
    vec->z = vec->z * cos(ang) + vec_cross_axis.z * sin(ang);
}

void move_camera_forward ()
{
    pos.addVector(L);
}
void move_camera_backward ()
{
    pos.addVector(L, -1);
}
void move_camera_right ()
{
    pos.addVector(R);
}
void move_camera_left ()
{
    pos.addVector(R, -1);
}
void move_camera_up ()
{
    pos.addVector(U);
}
void move_camera_down ()
{
    pos.addVector(U, -1);
}

void rotate_left()
{
    // rotate L anticlockwise w.r.t U
    // rotate R anticlockwise w.r.t U
    rotate_vector(&L, U, ANTICLOCKWISE);
    rotate_vector(&R, U, ANTICLOCKWISE);
}
void rotate_right()
{
    // rotate L clockwise w.r.t U
    // rotate R clockwise w.r.t U
    rotate_vector(&L, U, CLOCKWISE);
    rotate_vector(&R, U, CLOCKWISE);
}
void look_up()
{
    // rotate L anticlockwise w.r.t R
    // rotate U anticlockwise w.r.t R
    rotate_vector(&L, R, ANTICLOCKWISE);
    rotate_vector(&U, R, ANTICLOCKWISE);
}
void look_down()
{
    // rotate L clockwise w.r.t R
    // rotate U clockwise w.r.t R
    rotate_vector(&L, R, CLOCKWISE);
    rotate_vector(&U, R, CLOCKWISE);
}
void tilt_clockwise()
{
    // rotate U anticlockwise w.r.t L
    // rotate R anticlockwise w.r.t L
    rotate_vector(&U, L, ANTICLOCKWISE);
    rotate_vector(&R, L, ANTICLOCKWISE);
}
void tilt_counter_clockwise()
{
    // rotate U clockwise w.r.t L
    // rotate R clockwise w.r.t L
    rotate_vector(&U, L, CLOCKWISE);
    rotate_vector(&R, L, CLOCKWISE);
}

void moveWheelForward()
{
    wheelCenter.x += wheelForward.x * distChange;
    wheelCenter.y += wheelForward.y * distChange;
    wAxleRotationAngle += wForwardRotateChange;

}

void moveWheelBackward()
{
    wheelCenter.x -= wheelForward.x * distChange;
    wheelCenter.y -= wheelForward.y * distChange;
    wAxleRotationAngle -= wForwardRotateChange;
}

void rotateWheelLeft()
{
    wAngleWithXaxis += wXaxisAngleChange;
    Point z_axis(0,0,1);
    rotate_vector(&wheelForward, z_axis, ANTICLOCKWISE);
}

void rotateWheelRight()
{
    wAngleWithXaxis -= wXaxisAngleChange;
    Point z_axis(0,0,1);
    rotate_vector(&wheelForward, z_axis, CLOCKWISE);
}

void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 210,0,0);
			glVertex3f(-210,0,0);

			glVertex3f(0,-210,0);
			glVertex3f(0, 210,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-20;i<=20;i++){

//				if(i==0)
//					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -210, 0);
				glVertex3f(i*10,  210, 0);

				//lines parallel to X-axis
				glVertex3f(-210, i*10, 0);
				glVertex3f( 210, i*10, 0);
			}
		}glEnd();
	}
}


void drawWheelAxle(double axleBeamHeight, double wheelRadius)
{
    assert(axleBeamHeight < wheelRadius && axleBeamHeight > 0.0 && wheelRadius > 0.0);
    glColor3f(1,1,1);

    // along x-axis
    glBegin(GL_QUADS);
    {
        glVertex3f(wheelRadius, 0, axleBeamHeight/2.0);
        glVertex3f(wheelRadius, 0, -axleBeamHeight/2.0);
        glVertex3f(-wheelRadius, 0, -axleBeamHeight/2.0);
        glVertex3f(-wheelRadius, 0, axleBeamHeight/2.0);
    }
    glEnd();

    // along y-axis
    glBegin(GL_QUADS);
    {
        glVertex3f(0, wheelRadius, axleBeamHeight/2.0);
        glVertex3f(0, wheelRadius, -axleBeamHeight/2.0);
        glVertex3f(0, -wheelRadius, -axleBeamHeight/2.0);
        glVertex3f(0, -wheelRadius, axleBeamHeight/2.0);
    }
    glEnd();

    // along +x, +y
    glBegin(GL_QUADS);
    {
        glVertex3f(wheelRadius/sqrt(2.0), wheelRadius/sqrt(2.0), axleBeamHeight/2.0);
        glVertex3f(wheelRadius/sqrt(2.0), wheelRadius/sqrt(2.0), -axleBeamHeight/2.0);
        glVertex3f(-wheelRadius/sqrt(2.0), -wheelRadius/sqrt(2.0), -axleBeamHeight/2.0);
        glVertex3f(-wheelRadius/sqrt(2.0), -wheelRadius/sqrt(2.0), axleBeamHeight/2.0);
    }
    glEnd();

    // along +x, -y
    glBegin(GL_QUADS);
    {
        glVertex3f(-wheelRadius/sqrt(2.0), wheelRadius/sqrt(2.0), axleBeamHeight/2.0);
        glVertex3f(-wheelRadius/sqrt(2.0), wheelRadius/sqrt(2.0), -axleBeamHeight/2.0);
        glVertex3f(wheelRadius/sqrt(2.0), -wheelRadius/sqrt(2.0), -axleBeamHeight/2.0);
        glVertex3f(wheelRadius/sqrt(2.0), -wheelRadius/sqrt(2.0), axleBeamHeight/2.0);
    }
    glEnd();
}


void drawWheelPerimeter(double wheelRadius, double wheelPmWidth, int slices, int parts = 1, bool doubleHeight = true)
{
    assert(slices < 100);
    Point points[100];

    double total_angle = (2.0*pi)/parts;
    //generate points
    for(int i=0;i<=slices;i++){
        points[i].x=wheelRadius*cos(((double)i/(double)slices) * total_angle);
        points[i].y=wheelRadius*sin(((double)i/(double)slices) * total_angle);
    }

    for (int i=0; i<slices; i++)
    {
//        glColor3f(0.7,0.5,0.3);
        glColor3f((double)(slices-i)/(double)slices,
                  (double)(slices-i)/(double)slices,
                  (double)(slices-i)/(double)slices);
        glBegin(GL_QUADS); {
            glVertex3f(points[i].x, points[i].y, 0);
            glVertex3f(points[i+1].x, points[i+1].y, 0);
            glVertex3f(points[i+1].x, points[i+1].y, wheelPmWidth/2);
            glVertex3f(points[i].x, points[i].y, wheelPmWidth/2);

            if (doubleHeight)
            {
                glVertex3f(points[i].x, points[i].y, 0);
                glVertex3f(points[i+1].x, points[i+1].y, 0);
                glVertex3f(points[i+1].x, points[i+1].y, -wheelPmWidth/2);
                glVertex3f(points[i].x, points[i].y, -wheelPmWidth/2);
            }
        } glEnd();
    }

}



void drawWheel()
{

    glPushMatrix();
    {
        glTranslatef(wheelCenter.x, wheelCenter.y, wheelCenter.z);
        glRotatef(wAngleWithXaxis, 0, 0, 1);
        glRotatef(wAxleRotationAngle, 0, 1, 0);
        glRotatef(90, 1, 0, 0);
        drawWheelAxle(WHEEL_AXLE_HEIGHT, WHEEL_RADIUS);

        drawWheelPerimeter(WHEEL_RADIUS, WHEEL_AXLE_HEIGHT+5.0, SLICES);
    }
    glPopMatrix();

}

void keyboardListener(unsigned char key, int x,int y){
//	``1 - rotate/look left
//    2 - rotate/look right
//    3 - look up
//    4 - look down
//    5 - tilt clockwise
//    6 - tilt counterclockwise

	switch(key){

		case 'w':
            moveWheelForward();
			break;

        case 's':
            moveWheelBackward();
			break;

        case 'a':
            rotateWheelLeft();
			break;

        case 'd':
            rotateWheelRight();
			break;

        case '1':
            rotate_left();
			break;

        case '2':
            rotate_right();
			break;

        case '3':
            look_up();
			break;

        case '4':
            look_down();
			break;

        case '5':
            tilt_clockwise();
			break;

        case '6':
            tilt_counter_clockwise();
			break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			move_camera_backward();
			break;

		case GLUT_KEY_UP:		// up arrow key
			move_camera_forward();
			break;

		case GLUT_KEY_RIGHT:
			move_camera_right();
			break;

		case GLUT_KEY_LEFT:
			move_camera_left();
			break;

		case GLUT_KEY_PAGE_UP:
		    move_camera_up();
			break;

		case GLUT_KEY_PAGE_DOWN:
		    move_camera_down();
			break;

		case GLUT_KEY_INSERT:

			break;

		case GLUT_KEY_HOME:

			break;

		case GLUT_KEY_END:

			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}


void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(pos.x, pos.y, pos.z,
           pos.x + L.x, pos.y + L.y, pos.z + L.z,
           U.x, U.y, U.z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();


    drawWheel();

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=1;
	drawaxes=0;
	cameraHeight=150.0;
	cameraAngle=1.0;

    wheelCenter = Point(10,10, WHEEL_RADIUS);
    wheelForward = Point(1, 0, 0);

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance

    pos = Point(100, 100, 50);
    U = Point(0, 0, 1);
    R = Point(-1/sqrt(2.0), 1/sqrt(2.0), 0);
    L = Point(-1/sqrt(2.0), -1/sqrt(2.0), 0);
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Wheel Demo");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
