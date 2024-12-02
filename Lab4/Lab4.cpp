#include <windows.h>
#include <iostream>
#include <GL/glu.h>
#include <vector>
#include <math.h>
#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#include "Egg.hpp"
using namespace std;
HWND consoleWindow;     
HWND glutWindow;

GLfloat deg = 0;       
int sx =0,sy = 0,sz = 0; 
bool spin = false;
bool drawTeapot = true;
int eggMode = 0;
float rotationSpeed = 0.5f,sensitivity =0.75f;
float totalRotationX = 0.0f,totalRotationY = 0.0f,totalRotationZ = 0.0f;
int radius = 6,lastX = 0,lastY = 0;
float cameraRotationX = 0.0f,cameraRotationY = 0.0f,cameraRotationZ = radius;
float phi = 0.0f;       
float theta = 0.0f; 

Egg egg(20);
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
    totalRotationX += rotationSpeed * sx;
    totalRotationY += rotationSpeed * sy;
    totalRotationZ += rotationSpeed * sz;
	glutPostRedisplay();
}
void reset_rotation(){
	totalRotationX = 0.0f;
	totalRotationY = 0.0f;
	totalRotationZ = 0.0f;
	radius = 6;
	cameraRotationX = 0.0f;
	cameraRotationY = 0.0f;
	cameraRotationZ = radius;
	lastX = 0;
	lastY = 0;
}
string bool_to_string(bool convert){
    if(convert){
        return "true";
    }else{
        return "false";
    }
}
void printControls(){
	cout<<"==============================\n";
	cout<<"A D - obrot po osi Y\n";
	cout<<"W S - obrot po osi X\n";
	cout<<"Q E - obrot po osi Z\n";
	cout<<"ESC - Powrot do menu (okno konsolowe)\n";
	cout<<"Nalezy nacisnac i przytrzymac PPM\n";
	cout<<"Ruch myszy w osi X - Obrot kamery w osi X\n";
	cout<<"Ruch myszy w osi Y - Obrot kamery w osi Y\n";
	cout<<"Scroll up - Przybilizenie obiektu\n";
	cout<<"Scroll down - Oddalenie obiektu\n";
	cout<<"Nacisnij Enter zeby kontynuowac\n"<<flush;
	cin.get();
	cin.get();
}
void printOptions();
void menu();
void printOptions(){
	int density = egg.getDensity();    
    bool color = egg.getColor(); 
    float scale = egg.getScale(); 
    float pointSize = egg.getPointSize();
	cout<<"==============================\n";
	cout<<"1.Skala obiektow: "<<scale<<"\n";
	cout<<"2.Ilosc punktow: "<<density<<"\n";
	cout<<"3.Rysowanie w kolorze: "<<bool_to_string(color)<<"\n";
	cout<<"4.Promien kamery: "<<radius<<"\n";
	cout<<"5.Predkosc kamery: "<<sensitivity<<"\n";
	cout<<"6.Predkosc obiektu: "<<rotationSpeed<<"\n";
	cout<<"7.Rozmiar punktow: "<<pointSize<<"\n";
	cout<<"8.Wroc do menu"<<"\n";
	cout<<"> ";
	int x;
	cin>>x;
	switch (x){
	case 1:
		cout<<"Nowa skala\n";
		cout<<"> ";
		cin>>scale;
		egg.setScale(scale);
		break;
	case 2:
		cout<<"Nowa gestosc\n";
		cout<<"> ";
		cin>>density;
		egg.setDensity(density);
		break;
	case 3:
		color =! color;
		egg.setColor(color);
		egg.generateMatrix();
		break;
	case 4:
		cout<<"Nowy promien kamery\n";
		cout<<"> ";
		cin>>radius;
		break;
	case 5:
		cout<<"Nowa predkosc kamery\n";
		cout<<"> ";
		cin>>sensitivity;
		break;
	case 6:
		cout<<"Nowa predkosc obiektu\n";
		cout<<"> ";
		cin>>rotationSpeed;
		break;
	case 7:
		cout<<"Nowy rozmiar punktow\n";
		cout<<"> ";
		cin>>pointSize;
		egg.setPointSize(pointSize);
		break;
	case 8:
		menu();
		break;
	}
	printOptions();
}
void menu(){
	toggleFocusToConsole();
	reset_rotation();
	cout<<"==============================\n";
	cout<<"1. Narysuj czajnik\n";
	cout<<"2. Narysuj jajko (punkty)\n";
	cout<<"3. Narysuj jajko (linie)\n";
	cout<<"4. Narysuj jajko (trojkaty) \n";
	cout<<"5. Opcje\n";
	cout<<"6. Kontrola\n";
	cout<<"7. Zakoncz program\n";
	cout<<"> ";
	int x;
	cin>>x;
	switch (x){
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
		printOptions();
		break;
	case 6:
		printControls();
		menu();
		break;
	case 7:
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
void mouse(int x, int y){
	float phi = sensitivity*((2.0f * y / 400) - 1.0f);           
	float theta = sensitivity*((2.0f * (400 - x) / 400) - 1.0f);
	float maxPhi = 1.75f;
    if (phi > maxPhi){
		phi = maxPhi;
	} 
    if (phi < -maxPhi){
		phi = -maxPhi;
	} 
	cameraRotationX = radius*cos(theta)*cos(phi);
	cameraRotationY = radius*sin(phi);
	cameraRotationZ = radius*sin(theta)*cos(phi);
	lastX = x;
	lastY = y;
	glutPostRedisplay();
}
void mouseWheel(int button, int dir, int x, int y){
	if (dir > 0){
        radius -= 1;
    }else{
        radius += 1;
    }
	if(radius>=10){
		radius=10;
	}
	if(radius<=1){
		radius=1;
	}
	glutPostRedisplay();
}
void display() {
	GLfloat lPos[] = {0,4,0,1};//x,y,z,czy światło jest odległe
	GLfloat col[] = {1,0,0,1};
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glLightfv(GL_LIGHT0,GL_POSITION,lPos);
	gluLookAt(cameraRotationX,cameraRotationY,cameraRotationZ,0,0,0,0,1,0);//Ustawienie kamery
	glRotatef(totalRotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(totalRotationY, 0.0f, 1.0f, 0.0f); 
    glRotatef(totalRotationZ, 0.0f, 0.0f, 1.0f);
	if(drawTeapot){ 
		glutWireTeapot(1);
	}else{
		//glShadeModel(GL_FLAT);
		egg.draw(eggMode);
	}
	glutSwapBuffers();
}	
void Init() {
	egg.generateMatrix();
	glEnable(GL_DEPTH_TEST); //bez tego frontalna sciana nadpisuje tylnią
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1,1,-1,1,2,20);
	glMatrixMode(GL_MODELVIEW);
	// // Włącza culling, czyli pomijanie tylnych ścianek
	// glEnable(GL_CULL_FACE);
    // // Ustawia kierunek frontowych ścianek jako przeciwny do ruchu wskazówek zegara
    // glFrontFace(GL_CW);
    // // Ustawia pomijanie tylnych ścianek
    glCullFace(GL_BACK);
	//glEnable(GL_LIGHTING); //Włączenie oświetlenia
	//glEnable(GL_LIGHT0); //Dodanie źródła światła
}
int main(int argc, char** argv){
	consoleWindow = GetConsoleWindow();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800,800);
	glutCreateWindow("Lab 3 - Czajnik i Jajko");
	glutWindow = FindWindowW(NULL,L"Lab 3 - Czajnik i Jajko");
	Init();
	glutDisplayFunc(display);
	glutIdleFunc(nullptr);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutMotionFunc(mouse);
	glutMouseWheelFunc(mouseWheel);
	menu();
	
	glutMainLoop();
	system("pause");
	return 0;
}