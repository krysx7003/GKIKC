#include <windows.h>
#include <iostream>
#include <GL/glu.h>
#include <vector>
#include <math.h>
#define FREEGLUT_STATIC
#include <GL/freeglut.h>
using namespace std;
HWND consoleWindow;     
HWND glutWindow;

GLfloat deg = 0;       
int sx =0,sy = 0,sz = 0; 
bool spin = false;
bool drawTeapot = true;
bool color = false;
int eggMode = 0;
float totalRotationX = 0.0f;
float totalRotationY = 0.0f;
float totalRotationZ = 0.0f;
struct pointsRgb{
    //Pozycja
    float x = 0.0;
    float y = 0.0;
    float z = 0.0;
    //Kolor
    float r = 0.0;
    float g = 0.0;
    float b = 0.0;
}typedef pointsRgb;

class Egg{
	private:
	int density;
    vector<vector<pointsRgb>> pointsMatrix;
	float randFloat(){
		return (float)rand()/(float)(RAND_MAX);
	}
	public:
	Egg(int density ) : density(density){
		pointsMatrix.resize(density,vector<pointsRgb>(density*2));
	}
	vector<vector<pointsRgb>> getPointsMatrix(){
		return pointsMatrix;
	}
	void generateMatrix(float scale){
		
		for(int u=0;u<(density/2);u++){
			float _u = u/((float)density-1);
			for(int v=0;v<density;v++){
				float _v = v/((float)density-1);
				_v *= 2.0f * M_PI;
				pointsMatrix[u][v].x = scale*((-90*pow(_u,5)) + (255*pow(_u,4)) - (270*pow(_u,3)) + (180*pow(_u,2)) - (45*_u)) * cos(M_PI*_v);
				pointsMatrix[u][v].y = scale*((160*pow(_u,4)) - (320*pow(_u,3)) + (160 * pow(_u,2)) - 5);
				pointsMatrix[u][v].z = scale*((-90*pow(_u,5)) + (255*pow(_u,4)) - (270*pow(_u,3)) + (180*pow(_u,2)) - (45*_u)) * sin(M_PI*_v);
				if(color){
					pointsMatrix[u][v].r = randFloat();
					pointsMatrix[u][v].g = randFloat();
					pointsMatrix[u][v].b = randFloat();
				}else{
					pointsMatrix[u][v].r = 0.0f;
					pointsMatrix[u][v].g = 0.0f;
					pointsMatrix[u][v].b = 0.0f;
				}
				
			}
		}
	}
	void draw(int model){
		switch (model)
		{
		case 1:
			glBegin(GL_POINTS);
			for(int u=0;u<(density/2)-6;u++){
				for(int v=0;v<density;v++){
					glColor3f(pointsMatrix[u][v].r,pointsMatrix[u][v].g,pointsMatrix[u][v].b);
					glVertex3f(pointsMatrix[u][v].x,pointsMatrix[u][v].y,pointsMatrix[u][v].z);
				}
			}
			glEnd();
			break;
		case 2:
			glBegin(GL_LINES);
			for(int u=0;u<(density/2)-7;u++){
				for(int v=0;v<density-1;v++){
					glColor3f(pointsMatrix[u][v].r,pointsMatrix[u][v].g,pointsMatrix[u][v].b);
					glVertex3f(pointsMatrix[u][v].x,pointsMatrix[u][v].y,pointsMatrix[u][v].z);
					glColor3f(pointsMatrix[u+1][v].r, pointsMatrix[u+1][v].g, pointsMatrix[u+1][v].b);
        			glVertex3f(pointsMatrix[u+1][v].x, pointsMatrix[u+1][v].y, pointsMatrix[u+1][v].z);
					
					glColor3f(pointsMatrix[u][v].r,pointsMatrix[u][v].g,pointsMatrix[u][v].b);
					glVertex3f(pointsMatrix[u][v].x,pointsMatrix[u][v].y,pointsMatrix[u][v].z);
					glColor3f(pointsMatrix[u][v+1].r, pointsMatrix[u][v+1].g, pointsMatrix[u][v+1].b);
        			glVertex3f(pointsMatrix[u][v+1].x, pointsMatrix[u][v+1].y, pointsMatrix[u][v+1].z);
					
				}
			}
			glEnd();
			break;
		case 3:
			glBegin(GL_TRIANGLES);
			for(int u=0;u<(density/2)-7;u++){
				for(int v=1;v<density/3;v++){
					glColor3f(pointsMatrix[u][v].r,pointsMatrix[u][v].g,pointsMatrix[u][v].b);
					glVertex3f(pointsMatrix[u][v].x,pointsMatrix[u][v].y,pointsMatrix[u][v].z);
					glVertex3f(pointsMatrix[u+1][v].x, pointsMatrix[u+1][v].y, pointsMatrix[u+1][v].z);
					glVertex3f(pointsMatrix[u+1][v-1].x, pointsMatrix[u+1][v-1].y, pointsMatrix[u+1][v-1].z);
					
					glColor3f(pointsMatrix[u+1][v].r,pointsMatrix[u+1][v].g,pointsMatrix[u+1][v].b);
					glVertex3f(pointsMatrix[u][v].x,pointsMatrix[u][v].y,pointsMatrix[u][v].z);
					glVertex3f(pointsMatrix[u+1][v].x, pointsMatrix[u+1][v].y, pointsMatrix[u+1][v].z);
					glVertex3f(pointsMatrix[u][v+1].x, pointsMatrix[u][v+1].y, pointsMatrix[u][v+1].z);	
					
				}
			}
			glEnd();
			break;
		}

	}
	~Egg(){

	}
};
Egg egg(100);
void toggleFocusToConsole() {
	ShowWindow(glutWindow, SW_HIDE);  
    ShowWindow(consoleWindow, SW_SHOWNORMAL);  
    SetForegroundWindow(consoleWindow);        
}

void toggleFocusToGLUT() {
	ShowWindow(consoleWindow, SW_HIDE);
	ShowWindow(glutWindow, SW_SHOWNORMAL);   
    SetForegroundWindow(glutWindow);        
}
void animate(){
	float rotationSpeed = 0.5f;
    totalRotationX += rotationSpeed * sx;
    totalRotationY += rotationSpeed * sy;
    totalRotationZ += rotationSpeed * sz;
	glutPostRedisplay();
}
void reset_rotation(){
	totalRotationX = 0.0f;
	totalRotationY = 0.0f;
	totalRotationZ = 0.0f;
}
string bool_to_string(bool convert){
    if(convert){
        return "true";
    }else{
        return "false";
    }
}
void menu(){
	toggleFocusToConsole();
	reset_rotation();
	cout<<"==============================\n";
	cout<<"1. Narysuj czajnik\n";
	cout<<"2. Narysuj jajko (punkty)\n";
	cout<<"3. Narysuj jajko (linie)\n";
	cout<<"4. Narysuj jajko (trojkaty) \n";
	cout<<"5. Rysowanie w kolorze: "<<bool_to_string(color)<<"\n";
	cout<<"6. Zakoncz program\n";
	cout<<"> ";
	int x;
	cin>> x;
	switch (x)
	{
	case 1:
		drawTeapot = true; 
		break;
	case 2:
		drawTeapot = false;
		eggMode = 1;
		break;
	case 3:
		drawTeapot = false;
		eggMode = 2;
		break;
	case 4:
		drawTeapot = false;
		eggMode = 3;
		break;
	case 5:
		color=!color;
		egg.generateMatrix(0.5f);
		menu();
		break;
	case 6:
		exit(0);
		break;
	default:
		cout<<"Podano nieporawny znak\n";
		menu();
		break;
	}
	toggleFocusToGLUT();
	glutPostRedisplay();
}
void keyDown(u_char key,int x,int y){
	switch (key)
	{
	case 'Q':
	case 'q':
		sz=1;
		glutIdleFunc(animate);
		break;
	case 'E':
	case 'e':
		sz=-1;
		glutIdleFunc(animate);
		break;
	case 'W':
	case 'w':
		sx=-1;
		glutIdleFunc(animate);
		break;
	case 'S':
	case 's':
		sx=1;
		glutIdleFunc(animate);
		break;
	case 'A':
	case 'a':
		sy=-1;
		glutIdleFunc(animate);
		break;
	case 'D':
	case 'd':
		sy=1;
		glutIdleFunc(animate);
		break;
	default:
		break;
	}
}
void keyUp(u_char key,int x,int y){
	switch (key)
	{
	case 'E':
	case 'Q':
	case 'e':
	case 'q':
		sz=0;
		break;
	case 'W':
	case 'S':
	case 'w':
	case 's':
		sx=0;
		break;
	case 'A':
	case 'D':
	case 'd':
	case 'a':
		sy=0;
		break;
	case 27:
		menu();
		break;
	default:
		break;
	}
	if (sx == 0 && sy == 0 && sz == 0) {
        glutIdleFunc(nullptr); 
    }
}
void display() {
	GLfloat lPos[] = {0,4,0,1};//x,y,z,czy światło jest odległe
	GLfloat col[] = {1,0,0,1};
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glLightfv(GL_LIGHT0,GL_POSITION,lPos);
	gluLookAt(0,0,6,0,0,0,0,1,0);//Ustawienie kamery
	//Pierwsze trzy lokalizacja
	//Gdzie patrzy
	//Tilt kamery
	glRotatef(totalRotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(totalRotationY, 0.0f, 1.0f, 0.0f); 
    glRotatef(totalRotationZ, 0.0f, 0.0f, 1.0f);
	if(drawTeapot){
		glutWireTeapot(1);
	}else{
		glPushMatrix();
			//glTranslatef(0.0f, -1.5f, -5.0f); // Adjust to bring the egg into view
			//glScalef(0.5f, 0.5f, 0.5f);
			egg.draw(eggMode);
		glPopMatrix();
	}
	glutSwapBuffers();
}	
void Init() {
	egg.generateMatrix(0.5f);
	glEnable(GL_DEPTH_TEST); //bez tego frontalna sciana nadpisuje tylnią
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1,1,-1,1,2,10);
	glMatrixMode(GL_MODELVIEW);
	//glEnable(GL_LIGHTING); //Włączenie oświetlenia
	//glEnable(GL_LIGHT0); //Dodanie źródła światła
}
int main(int argc, char** argv){
	consoleWindow = GetConsoleWindow();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800,800);
	glutCreateWindow("Lab 2 - Czajnik i Jajko");
	glutWindow = FindWindowW(NULL,L"Lab 2 - Czajnik i Jajko");
	Init();
	glutDisplayFunc(display);
	glutIdleFunc(nullptr);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	menu();
	
	glutMainLoop();
	system("pause");
	return 0;
}