#include "Vector.h"
#include "Camera.h"
#include "1705047_classes.h"
using namespace std;

// global variables initialization
Point pos, U, R, L;
// pos indicates the position of the camera
// U -> upper vector
// R -> right indicating vector
// L -> Look vector
// Notice: U, R, L are all unit vectors

int imagePixelDimension = 0;
int objectsCount = 0;

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

void keyboardListener(unsigned char key, int x,int y){
//	``1 - rotate/look left
//    2 - rotate/look right
//    3 - look up
//    4 - look down
//    5 - tilt clockwise
//    6 - tilt counterclockwise

	switch(key){

		case '1':
            change_camera_orientation(L, R, U, "rotate_left");
			break;

        case '2':
            change_camera_orientation(L, R, U, "rotate_right");
			break;

        case '3':
            change_camera_orientation(L, R, U, "look_up");
			break;

        case '4':
            change_camera_orientation(L, R, U, "look_down");
			break;

        case '5':
            change_camera_orientation(L, R, U, "tilt_clockwise");

			break;

        case '6':
            change_camera_orientation(L, R, U, "tilt_counter_clockwise");

			break;

		default:
			break;
	}
}

void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			change_camera_position(pos, L, R, U, "backward");
			break;

		case GLUT_KEY_UP:		// up arrow key
			change_camera_position(pos, L, R, U, "forward");
			break;

		case GLUT_KEY_RIGHT:
			change_camera_position(pos, L, R, U, "right");
			break;

		case GLUT_KEY_LEFT:
			change_camera_position(pos, L, R, U, "left");
			break;

		case GLUT_KEY_PAGE_UP:
		    change_camera_position(pos, L, R, U, "up");
			break;

		case GLUT_KEY_PAGE_DOWN:
		    change_camera_position(pos, L, R, U, "down");
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


    // drawSS();

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=1;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;
	// sq_side = 20;
	// radius_curv = 20;

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

void loadData() {
    ifstream input;

    /* preparing input for extracting values from input file */
    input.open("./scene.txt");
    if(!input.is_open()) {
        cout << "input.is_open(): failed to open input file" << endl;
        exit(EXIT_FAILURE);
    }

    /* extracting recursion level & image pixel dimension from input file */
    input >> recursionLevel >> imagePixelDimension;

    /* extracting objects information from input file */
    input >> objectsCount;

    string objectShape;
    bool bInvalidObjectShapeFound = false;

    Object* object = NULL;

    for(int i=0; i<objectsCount; i++) {
        input >> objectShape;

        if(objectShape.compare("sphere") == 0) {
            Point center;
            double radius;

            input >> center.x>>center.y>>center.z;
            input >> radius;

            object = new Sphere(center, radius, 72, 24);
        } else if(objectShape.compare("triangle") == 0) {
            Point a, b, c;

            input >> a.x >>a.y>>a.z;
            input >> b.x>>b.y>>b.z;
            input >> c.x>>c.y>>c.z;

            object = new Triangle(a, b, c);
        } 
        // else if(objectShape.compare("general") == 0) {
        //     GeneralQuadricSurfaceCoefficient coefficient;
        //     Point cubeReferencePoint;
        //     double length, width, height;

        //     input >> coefficient;
        //     input >> cubeReferencePoint;
        //     input >> length >> width >> height;

        //     object = new GeneralQuadricSurface(coefficient, cubeReferencePoint, length, width, height);
        // }
        else if (objectShape.compare("")==0) {
            continue;
        } 
        else {
            cout << objectShape << ": invalid object shape found" << endl;
            bInvalidObjectShapeFound = true;
            break;
        }

        Color color;
        ReflectionCoeff reflectionCoefficient;
        int shininess;

        input >> color;
        input >> reflectionCoefficient;
        input >> shininess;

        object->setColor(color);
        object->setReflectionCoefficient(reflectionCoefficient);
        object->setShininess(shininess);

        objects.push_back(object);
    }
    object = NULL;

    if(bInvalidObjectShapeFound) {
        clearObjects();
        input.close();
        exit(EXIT_FAILURE);
    }

    /* extracting light sources information from input file */
    input >> lightsCount;

    for(int i=0; i<lightsCount; i++) {
        Vector position;
        Color color;

        input >> position;
        input >> color;

        lights.push_back(Light(position, color, 1.0, 12, 4));
    }
    input.close();

    /* creating a floor object and pushing it to objects vector */
    object = new Floor(1000.0, 20.0, Color());  // color = black

    object->setColor(Color(1.0, 1.0, 1.0));  // color = white
    object->setReflectionCoefficient(ReflectionCoeff(0.25, 0.25, 0.25, 0.25));
    object->setShininess(15);

    objects.push_back(object);
    object = NULL;
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


// void loadData() {
//     ifstream input;

//     /* preparing input for extracting values from input file */
//     input.open("D:\\Academic 4-1\\CSE410 (Computer Graphics sessional)\\offline-3\\offline3-src\\inputs\\scene.txt");
//     if(!input.is_open()) {
//         cout << "input.is_open(): failed to open input file" << endl;
//         exit(EXIT_FAILURE);
//     }

//     /* extracting recursion level & image pixel dimension from input file */
//     input >> recursionLevel >> imagePixelDimension;

//     /* extracting objects information from input file */
//     input >> objectsCount;

//     string objectShape;
//     bool bInvalidObjectShapeFound = false;

//     Object* object = NULL;

//     for(int i=0; i<objectsCount; i++) {
//         input >> objectShape;

//         if(objectShape.compare("sphere") == 0) {
//             Vector center;
//             double radius;

//             input >> center;
//             input >> radius;

//             object = new Sphere(center, radius, 72, 24);
//         } else if(objectShape.compare("triangle") == 0) {
//             Vector a, b, c;

//             input >> a;
//             input >> b;
//             input >> c;

//             object = new Triangle(a, b, c);
//         } else if(objectShape.compare("general") == 0) {
//             GeneralQuadricSurfaceCoefficient coefficient;
//             Vector cubeReferencePoint;
//             double length, width, height;

//             input >> coefficient;
//             input >> cubeReferencePoint;
//             input >> length >> width >> height;

//             object = new GeneralQuadricSurface(coefficient, cubeReferencePoint, length, width, height);
//         } else {
//             cout << objectShape << ": invalid object shape found" << endl;
//             bInvalidObjectShapeFound = true;
//             break;
//         }

//         Color color;
//         ReflectionCoefficient reflectionCoefficient;
//         int shininess;

//         input >> color;
//         input >> reflectionCoefficient;
//         input >> shininess;

//         object->setColor(color);
//         object->setReflectionCoefficient(reflectionCoefficient);
//         object->setShininess(shininess);

//         objects.push_back(object);
//     }
//     object = NULL;

//     if(bInvalidObjectShapeFound) {
//         clearObjects();
//         input.close();
//         exit(EXIT_FAILURE);
//     }

//     /* extracting light sources information from input file */
//     input >> lightsCount;

//     for(int i=0; i<lightsCount; i++) {
//         Vector position;
//         Color color;

//         input >> position;
//         input >> color;

//         lights.push_back(Light(position, color, 1.0, 12, 4));
//     }
//     input.close();

//     /* creating a floor object and pushing it to objects vector */
//     object = new Floor(1000.0, 20.0, Color());  // color = black

//     object->setColor(Color(1.0, 1.0, 1.0));  // color = white
//     object->setReflectionCoefficient(ReflectionCoefficient(0.25, 0.25, 0.25, 0.25));
//     object->setShininess(15);

//     objects.push_back(object);
//     object = NULL;
// }
