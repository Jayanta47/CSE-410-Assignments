#include "vector.h"


// global variables initialization
Point pos, U, R, L;
// pos indicates the position of the camera
// U -> upper vector
// R -> right indicating vector
// L -> Look vector
// Notice: U, R, L are all unit vectors

int sq_side, radius_curv;


void rotate_vector(Point *vec, Point axis, int angle_dir) // + for anticlockwise, - for clockwise
{
    Point vec_cross_axis = axis.crossProduct(vec->returnDeepCopy());

    double ang = angle_dir * deg2Rad(rotation_unit);

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


void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

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
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,0);
		glVertex3f( a,-a,0);
		glVertex3f(-a,-a,0);
		glVertex3f(-a, a,0);
	}glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    Point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    Point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawSphere(double radius,int slices,int stacks, int parts = 1, bool lower_sphere = true)
{
	Point points[100][100];
	int i,j;
	double h,r;
	double total_angle = (2.0*pi)/parts;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*total_angle);
			points[i][j].y=r*sin(((double)j/(double)slices)*total_angle);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{

        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
        glColor3f(1, 1, 1);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                if (lower_sphere) {
                    glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
                    glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
                    glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
                    glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
                }

			}glEnd();
		}
	}
}

void drawCylinder(double radius, double height, int segments, int parts = 1, bool lower_part = true)
{
    Point points[200];
    int h, r;
    double total_angle = (2.0*pi)/parts;
    //generate points
    for(int i = 0; i <= segments; i++)
    {
        points[i].x = radius * cos(((double) i / (double) segments) * total_angle);
        points[i].y = radius * sin(((double) i / (double) segments) * total_angle);
    }

    for (int i = 0; i < segments; i++)
    {
        glColor3f(0, 1, 0);
        glBegin(GL_QUADS);
        {
            glVertex3f(points[i].x, points[i].y, 0);
            glVertex3f(points[i].x, points[i].y, height/2.0);
            glVertex3f(points[i + 1].x, points[i + 1].y, height/2.0);
            glVertex3f(points[i + 1].x, points[i + 1].y, 0);
            if (lower_part)
            {
                glVertex3f(points[i].x, points[i].y, 0);
                glVertex3f(points[i].x, points[i].y, -height/2.0);
                glVertex3f(points[i + 1].x, points[i + 1].y, -height/2.0);
                glVertex3f(points[i + 1].x, points[i + 1].y, 0);
            }
        }
        glEnd();
    }
}

void drawSphereSegment()
{
    // total 8 segments needs to be built
    // the segment locations are described assuming
    // camera position at above +z axis and looking
    // towards x-y plane

    // segment 1: +x, +y, +z
    glPushMatrix();
    {
        glTranslated(sq_side, sq_side, sq_side);
        drawSphere(radius_curv, 50, 30, 4, false);
    }
    glPopMatrix();

    // segment 1: -x, +y, +z
    glPushMatrix();
    {
        glTranslated(-sq_side, sq_side, sq_side);
        glRotated(90, 0,0,1);
        drawSphere(radius_curv, 50, 30, 4, false);
    }
    glPopMatrix();

    // segment 1: +x, -y, +z
    glPushMatrix();
    {
        glTranslated(sq_side, -sq_side, sq_side);
        glRotated(-90, 0,0,1);
        drawSphere(radius_curv, 50, 30, 4, false);
    }
    glPopMatrix();

    // segment 1: -x, -y, +z
    glPushMatrix();
    {
        glTranslated(-sq_side, -sq_side, sq_side);
        glRotated(180, 0,0,1);
        drawSphere(radius_curv, 50, 30, 4, false);
    }
    glPopMatrix();

    // segment 1: +x, +y, -z
    glPushMatrix();
    {
        glTranslated(sq_side, sq_side, -sq_side);
        glRotated(180, 1,1,0);
        drawSphere(radius_curv, 50, 30, 4, false);
    }
    glPopMatrix();

    // segment 1: -x, +y, -z
    glPushMatrix();
    {
        glTranslated(-sq_side, sq_side, -sq_side);
        glRotated(90, 0,0,1);
        glRotated(180, 1, 1, 0); // mirror to x-y plane
        drawSphere(radius_curv, 50, 30, 4, false);
    }
    glPopMatrix();

    // segment 1: +x, -y, -z
    glPushMatrix();
    {
        glTranslated(sq_side, -sq_side, -sq_side);
        glRotated(-90, 0,0,1);
        glRotated(180, 1, 1, 0); // mirror to x-y plane
        drawSphere(radius_curv, 50, 30, 4, false);
    }
    glPopMatrix();

    // segment 1: -x, -y, -z
    glPushMatrix();
    {
        glTranslated(-sq_side, -sq_side, -sq_side);
        glRotated(180, 0,0,1);
        glRotated(180, 1, 1, 0); // mirror to x-y plane
        drawSphere(radius_curv, 50, 30, 4, false);
    }
    glPopMatrix();
}

void drawCyliderSegment()
{
    // total 12 segments needs to be built
    // each segment refers to an edge
    // the segment locations are described assuming
    // camera position at above +z axis and looking
    // towards x-y plane

    // segment 1: (+,+,+) to (-,+,+)
    glPushMatrix();
    {
        glTranslatef(0, sq_side, sq_side);
        glRotated(-90, 0, 1, 0);
        drawCylinder(radius_curv, 2*sq_side, 50, 4);
    }
    glPopMatrix();

    // segment 2: (+,+,+) to (+,-,+)
    glPushMatrix();
    {
        glTranslatef(sq_side, 0, sq_side);
        glRotated(90, 1, 0, 0);
        drawCylinder(radius_curv, 2*sq_side, 50, 4);
    }
    glPopMatrix();

    // segment 3: (+,-,+) to (-,-,+)
    glPushMatrix();
    {
        glTranslatef(0, -sq_side, sq_side);
        glRotated(90, 1, 0, 0);
        glRotated(-90, 0, 1, 0);
        drawCylinder(radius_curv, 2*sq_side, 50, 4);
    }
    glPopMatrix();

    // segment 4: (-,+,+) to (-,-,+)
    glPushMatrix();
    {
        glTranslatef(-sq_side, sq_side, 0);
        glRotated(-90, 1, 0, 0);
        drawCylinder(radius_curv, 2*sq_side, 50, 4);
    }
    glPopMatrix();

    // segment 5: (+,+,-) to (-,+,-)
    glPushMatrix();
    {

    }
    glPopMatrix();

    // segment 6: (+,+,-) to (+,-,-)
    glPushMatrix();
    {

    }
    glPopMatrix();

    // segment 7: (+,-,-) to (-,-,-)
    glPushMatrix();
    {

    }
    glPopMatrix();

    // segment 8: (-,+,-) to (-,-,-)
    glPushMatrix();
    {

    }
    glPopMatrix();

    // segment 9: (+,+,+) to (+,+,-)
    glPushMatrix();
    {
        glTranslatef(sq_side, sq_side, 0);
        drawCylinder(radius_curv, 2*sq_side, 50, 4);
    }
    glPopMatrix();

    // segment 10: (-,+,+) to (-,+,-)
    glPushMatrix();
    {
        glTranslatef(-sq_side, sq_side, 0);
        glRotated(90, 0,0,1);
        drawCylinder(radius_curv, 2*sq_side, 50, 4);
    }
    glPopMatrix();

    // segment 11: (+,-,+) to (+,-,-)
    glPushMatrix();
    {
        glTranslatef(sq_side, -sq_side, 0);
        glRotated(-90, 0,0,1);
        drawCylinder(radius_curv, 2*sq_side, 50, 4);
    }
    glPopMatrix();

    // segment 12: (-,-,+) to (-,-,-)
    glPushMatrix();
    {
        glTranslatef(-sq_side, -sq_side, 0);
        glRotated(180, 0,0,1);
        drawCylinder(radius_curv, 2*sq_side, 50, 4);
    }
    glPopMatrix();
}

void drawSS()
{
    glColor3f(1,0,0);
    drawSquare(20);

//    drawSphere(30, 25, 50, 4, false);
//    drawCylinder(radius_curv, 2*sq_side, 50, 4);
    drawSphereSegment();
    drawCyliderSegment();

    glRotatef(angle,0,0,1);
    glTranslatef(110,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    drawSquare(15);
//
//    glPushMatrix();
//    {
//        glRotatef(angle,0,0,1);
//        glTranslatef(60,0,0);
//        glRotatef(2*angle,0,0,1);
//        glColor3f(0,0,1);
//        drawSquare(10);
//    }
//    glPopMatrix();
//
//    glRotatef(3*angle,0,0,1);
//    glTranslatef(40,0,0);
//    glRotatef(4*angle,0,0,1);
//    glColor3f(1,1,0);
//    drawSquare(5);
}


void keyboardListener(unsigned char key, int x,int y){
//	``1 - rotate/look left
//    2 - rotate/look right
//    3 - look up
//    4 - look down
//    5 - tilt clockwise
//    6 - tilt counterclockwise

	switch(key){

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

    //glColor3f(1,0,0);
    //drawSquare(10);

    drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);

	//drawSphere(30,24,20);




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
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;
	sq_side = 10;
	radius_curv = 10;

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

    pos = Point(100, 100, 0);
    U = Point(0, 0, 1);
    R = Point(-1/sqrt(2.0), 1/sqrt(2.0), 0);
    L = Point(-1/sqrt(2.0), -1/sqrt(2.0), 0);
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

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
