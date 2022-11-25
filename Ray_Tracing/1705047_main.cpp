#include "1705047_Vector.h"
#include "1705047_Camera.h"
#include "1705047_classes.h"
#include "bitmap_image.hpp"


using namespace std;

// global variables initialization
extern Point pos;
Point U, R, L;
// pos indicates the position of the camera
// U -> upper vector
// R -> right indicating vector
// L -> Look vector
// Notice: U, R, L are all unit vectors

extern int recursionLevel;
int imgPxDim = 0;
int objectsCount = 0;
int pointLightsCount = 0;
int spotLightsCount = 0;
int imageCount = 10;

int windowWidth = 500;
int windowHeight = 500;
double fovY;

void capture();

extern std::vector<Object*> objects;
extern std::vector<PointLight> pointLights;
extern std::vector<SpotLight> spotLights;


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

		case '0':
			capture();
			break;
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

	for(int i=0; i<objects.size(); i++) {
        objects[i]->draw();
	}

	/* adding lights */
	for(int i=0; i<pointLights.size(); i++) {
        pointLights[i].draw();
	}
	// cout<<pointLights.size()<<endl;

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
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;
	fovY = 80.0;
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
	gluPerspective(fovY,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance

    pos = Point(100, 100, 20);
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
    input >> recursionLevel >> imgPxDim;

    /* extracting objects information from input file */
    input >> objectsCount;

    string objectShape;
    bool bInvalidObjectShapeFound = false;

    Object *object = NULL;

    for(int i=0; i<objectsCount; i++) {
        input >> objectShape;
		// cout<<objectShape<<endl;
        if(objectShape.compare("sphere") == 0) {
            Point center;
            double radius;

            input >> center.x>>center.y>>center.z;
            input >> radius;

			// cout<< center.x<<" "<<center.y<<" "<<center.z<< " rad:"<<radius<<endl;

            object  = new Sphere(center, radius, 72, 24);
        } else if(objectShape.compare("triangle") == 0) {
            Point a, b, c;

            input >> a.x >>a.y>>a.z;
            input >> b.x>>b.y>>b.z;
            input >> c.x>>c.y>>c.z;

			// cout<<a.x<<" "<<a.y<<" "<<a.z<<endl;
			// cout<<b.x<<" "<<b.y<<" "<<b.z<<endl;
			// cout<<c.x<<" "<<c.y<<" "<<c.z<<endl;
			// cout<<endl;

            object = new Triangle(a, b, c);
            // object = t;
        } 
        else if(objectShape.compare("general") == 0) {
            gQuadCoeff gQCoeff;
            Point cubeReferencePoint;
            double length, width, height;

            input >> gQCoeff.a >> gQCoeff.b >> gQCoeff.c>>gQCoeff.d>>gQCoeff.e>>gQCoeff.f>>gQCoeff.g>>gQCoeff.h>>gQCoeff.i>>gQCoeff.j;
            input >> cubeReferencePoint.x >>cubeReferencePoint.y>>cubeReferencePoint.z;
            input >> length >> width >> height;

			// cout<<gQCoeff.a <<" "<< gQCoeff.b <<" "<<  gQCoeff.c<<" "<<gQCoeff.d<<" "<<gQCoeff.e<<" "<<gQCoeff.f<<" "<<gQCoeff.g<<" "<<gQCoeff.h<<" "<<gQCoeff.i<<" "<<gQCoeff.j<<endl;
			// cout<<cubeReferencePoint.x <<" "<< cubeReferencePoint.y<<" "<< cubeReferencePoint.z<<endl;;
			// cout<<length <<" "<< width <<" "<< height<<endl;;
            object = new GeneralQuadSurface(gQCoeff, length, width, height, cubeReferencePoint);
        }
        else if (objectShape.compare("")==0) {
            continue;
        } 
        else {
            cout << objectShape << ": invalid object shape found" << endl;
            bInvalidObjectShapeFound = true;
            break;
        }

        Color color;
        ReflectionCoeff refco;
        int shininess;

        input >> color.red >> color.green>> color.blue;
        input >> refco.ambientCoeff >> refco.diffuseCoeff >> refco.specularCoeff>> refco.recursiveCoeff;
        input >> shininess;

        object->setColor(color);
        object->setReflectionCoefficient(refco);
        object->setShininess(shininess);

        objects.push_back(object);
    }
    object = NULL;

    // if(bInvalidObjectShapeFound) {
    //     clearObjects();
    //     input.close();
    //     exit(EXIT_FAILURE);
    // }

    /* extracting light sources information from input file */
    input >> pointLightsCount;

    for(int i=0; i<pointLightsCount; i++) {
        Point position;
        Color color;

        input >> position.x>>position.y>>position.z;
        input >> color.red>>color.green>>color.blue;

        pointLights.push_back(PointLight(position, color));
    }

	input >> spotLightsCount;
	for (int i=0; i< spotLightsCount; i++)
	{
		Point position;
		Color color;
		int cutOffAngle; // in degrees
		Point direction;

		input >> position.x >> position.y >> position.z;
		input >> color.red >> color.green >> color.blue;
		input >> direction.x >> direction.y >> direction.z;
		input >> cutOffAngle;

		PointLight p(position, color);
		spotLights.push_back(SpotLight(p, direction, cutOffAngle));
	}

    input.close();

    /* creating a floor object and pushing it to objects vector */

    object = new Floor(1000.0, 20.0);
    object->setColor(Color(1.0, 1.0, 1.0));  // color = white
    object->setReflectionCoefficient(ReflectionCoeff(0.25, 0.25, 0.25, 0.25));
    object->setShininess(15);

    objects.push_back(object);
	object = NULL;
	// cout<<"lights:"<<pointLights.size()<<endl;
}


void capture()
{
	bitmap_image image(imgPxDim, imgPxDim);

	// initialize bitmap image and set background color
	for(int j=0; j<imgPxDim; j++) {
        for(int i=0; i<imgPxDim; i++) {
            image.set_pixel(j, i, 0, 0, 0);  // color = black
        }
    }

	double planeDistance = (windowHeight/2.0)/(tan((fovY*pi)/(2.0*180)));

	// topleft = eye + l*planeDistance - r*windowWidth/2 + u*windowHeight/2
	
	Point l = L.returnCopy();
	Point r = R.returnCopy();
	Point u = U.returnCopy();

	l.scalarMult(planeDistance);
	r.scalarMult(windowWidth/2.0);
	u.scalarMult(windowHeight/2.0);

	l.addVector(r, -1);
	l.addVector(u);

	Point topLeft = pos.addVectorCopy(l);

	double du = ((double) windowWidth/imgPxDim);
    double dv = ((double) windowHeight/imgPxDim);

	// topleft = topleft + r*(0.5*du) - u*(0.5*dv)

	r = R.returnCopy();
	r.scalarMult(0.5*du);

	u = U.returnCopy();
	u.scalarMult(0.5*dv);

	r.addVector(u, -1);
	topLeft.addVector(r);

	int ctr = 0;

	for (int j = 0; j < imgPxDim; j++)
	{
		for (int i=0; i< imgPxDim; i++)
		{
			// r = R.returnCopy();
			// r.scalarMult(j*du);

			// u = U.returnCopy();
			// u.scalarMult(i*dv);

			// r.addVector(u, -1);

			// Point curPixel = topLeft.returnCopy();
			// curPixel.addVector(r);

			Point curPixel = topLeft.addVectorCopy(R.scalarMultCopy(j*du)).addVectorCopy(U.scalarMultCopy(i*du), -1);

			Point curPixelDiffWithPos = curPixel.addVectorCopy(pos, -1);
			Ray ray(pos, curPixelDiffWithPos);

			int nearest = INT32_MAX; 
			double t, tMin=INF;

			for(int i=0; i<objects.size(); i++) {
                Color color;  // color = black
                t = objects[i]->intersect(ray, color, 0);

				// if (objects[i]->get_type() == "Floor") ctr++;

                if(t>0.0 && t<tMin) {
                    tMin = t;
                    nearest = i;
                }
            }

			if(nearest != INT32_MAX) {
                Color color;  // color = black
                tMin = objects[nearest]->intersect(ray, color, 1);
				if (objects[nearest]->get_type() == "Floor") {
					ctr++;
					// image.set_pixel(j, i, 255.0, 255.0, 255.0);
					// continue;
				}
                image.set_pixel(j, i, (int) round(color.red*255.0), (int) round(color.green*255.0), (int) round(color.blue*255.0));
            }
		}
	}

	stringstream oss;
	imageCount++;
	oss<<imageCount;
	string image_name = "output_"+ oss.str()+".bmp";
	image.save_image("./output/"+image_name);

	cout<<"image saved "<<endl;

}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(windowWidth, windowHeight);
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

	loadData();

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}

