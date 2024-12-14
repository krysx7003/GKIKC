#include <windows.h>
#include <iostream>
#include <GL/glu.h>
#include <vector>
#include <math.h>
#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#include "Egg.hpp"
#include "Light.hpp"
using namespace std;
HWND consoleWindow;     
HWND glutWindow;

GLfloat deg = 0;       
int sx =0,sy = 0,sz = 0; 
bool spin = false;
bool drawTeapot = true,smooth = true;
int eggMode = 0,moveMode = 0;
float sensitivity = 0.01f;
float totalRotationX = 0.0f,totalRotationY = 0.0f,totalRotationZ = 0.0f;
float pix2angle,theta = 0.0f,phi = 0.0f;
float dX , dY;
int radius = 6,lastX = 0,lastY = 0, camOrientation = 1;
float cameraRotationX = radius * cosf((theta*(M_PI/180))) * cosf((phi*(M_PI/180)));
float cameraRotationY = radius * sinf((phi*(M_PI/180)));
float cameraRotationZ = radius * sinf((theta*(M_PI/180))) * cosf((phi*(M_PI/180)));
Light light1(GL_LIGHT0);
int light1Radius = 10;
float light1RotationX = radius * cosf((theta*(M_PI/180))) * cosf((phi*(M_PI/180)));
float light1RotationY = radius * sinf((phi*(M_PI/180)));
float light1RotationZ = radius * sinf((theta*(M_PI/180))) * cosf((phi*(M_PI/180)));
Light light2(GL_LIGHT1);
int light2Radius = 10;
float light2RotationX = radius * cosf((theta*(M_PI/180))) * cosf((phi*(M_PI/180)));
float light2RotationY = radius * sinf((phi*(M_PI/180)));
float light2RotationZ = radius * sinf((theta*(M_PI/180))) * cosf((phi*(M_PI/180)));

Egg egg(200);
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
void reset_rotation(){
	theta = 0.0f;
	phi = 0.0f;
	lastX = 0;
	lastY = 0;
	cameraRotationX = radius * cosf((theta*(M_PI/180.0f))) * cosf((phi*(M_PI/180.0f)));
	cameraRotationY = radius * sinf((phi*(M_PI/180.0f)));
	cameraRotationZ = radius * sinf((theta*(M_PI/180.0f))) * cosf((phi*(M_PI/180.0f)));

	light1RotationX = light1Radius * cosf((theta*(M_PI/180))) * cosf((phi*(M_PI/180)));
	light1RotationY = light1Radius * sinf((phi*(M_PI/180)));
	light1RotationZ = light1Radius * sinf((theta*(M_PI/180))) * cosf((phi*(M_PI/180)));

	light2RotationX = light2Radius * cosf((theta*(M_PI/180))) * cosf((phi*(M_PI/180)));
	light2RotationY = light2Radius * sinf((phi*(M_PI/180)));
	light2RotationZ = light2Radius * sinf((theta*(M_PI/180))) * cosf((phi*(M_PI/180)));
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
	cout<<"F1 - tryb obrotu obiektu";
	cout<<"F2 - tryb obrotu kamery";
	cout<<"F3 - tryb obrotu swiatlem 1 (Czerwone)";
	cout<<"F4 - tryb obrotu swiatlem 2 (Zielone)";
	cout<<"ESC - Powrot do menu (okno konsolowe)\n";
	cout<<"Nalezy nacisnac i przytrzymac PPM\n";
	cout<<"Ruch myszy w osi X - Obrot osi X\n";
	cout<<"Ruch myszy w osi Y - Obrot osi Y\n";
	cout<<"Ruch myszy w osi X - Obrot osi X\n";
	cout<<"Ruch myszy w osi Y - Obrot osi Y\n";
	cout<<"Scroll up - Przybilizenie obiektu\n";
	cout<<"Scroll down - Oddalenie obiektu\n";
	cout<<"Nacisnij Enter zeby kontynuowac\n"<<flush;
	cin.get();
	cin.get();
}
void axis(){
	glBegin(GL_LINES);

    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-5.0, 0.0, 0.0);
    glVertex3f(5.0, 0.0, 0.0);

    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, -5.0, 0.0);
    glVertex3f(0.0, 5.0, 0.0);

    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, -5.0);
    glVertex3f(0.0, 0.0, 5.0);

    glEnd();
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
	cout<<"5.Czulosc myszki: "<<sensitivity<<"\n";
	cout<<"6.Rozmiar punktow: "<<pointSize<<"\n";
	cout<<"7.Wroc do menu"<<"\n";
	cout<<"> ";
	int x;
	cin>>x;
	switch (x){
	case 1:
		cout<<"Nowa skala\n";
		cout<<"> ";
		cin>>scale;
		egg.setScale(scale);
		printOptions();
		break;
	case 2:
		cout<<"Nowa gestosc\n";
		cout<<"> ";
		cin>>density;
		egg.setDensity(density);
		printOptions();
		break;
	case 3:
		color =! color;
		egg.setColor(color);
		egg.generateMatrix();
		printOptions();
		break;
	case 4:
		cout<<"Nowy promien kamery\n";
		cout<<"> ";
		cin>>radius;
		printOptions();
		break;
	case 5:
		cout<<"Nowa predkosc kamery\n";
		cout<<"> ";
		cin>>sensitivity;
		printOptions();
		break;
	case 6:
		cout<<"Nowy rozmiar punktow\n";
		cout<<"> ";
		cin>>pointSize;
		egg.setPointSize(pointSize);
		printOptions();
		break;
	case 7:
		menu();
		break;
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
void specialKey(int key,int x,int y){
	switch (key){
	//F1 - Ruch obiektu
	case GLUT_KEY_F1:
		moveMode = 0;
		break;
	//F2 - Ruch kamery
	case GLUT_KEY_F2:
		moveMode = 1;
		break;
	//F3 - Ruch światła 1
	case GLUT_KEY_F3:
		moveMode = 2;
		break;
	//F4 - Ruch światła 2
	case GLUT_KEY_F4:
		moveMode = 3;
		break;
	default:
		break;
	}
}
void normalKey(u_char key,int x,int y){
	switch (key)
	{
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
	dY = y - lastY;
	lastY = y;
	dX = x - lastX;
	lastX = x;
	theta += dX * pix2angle;
	phi += dY * pix2angle;
	if (phi > 89.0f) {phi = 89.0f;}
    if (phi < -89.0f) {phi = -89.0f;}
	switch(moveMode){
		case 0:
			totalRotationX += dY;
			totalRotationY += dX;
			totalRotationZ += atan2f(dY,dX);
			break;
		case 1:
			cameraRotationX = radius * cosf((theta*(M_PI/180.0f))) * cosf((phi*(M_PI/180.0f)));
			cameraRotationY = radius * sinf((phi*(M_PI/180.0f)));
			cameraRotationZ = radius * sinf((theta*(M_PI/180.0f))) * cosf((phi*(M_PI/180.0f)));
			break;
		case 2:
			light1RotationX = light1Radius * cosf((theta*(M_PI/180))) * cosf((phi*(M_PI/180)));
			light1RotationY = light1Radius * sinf((phi*(M_PI/180)));
			light1RotationZ = light1Radius * sinf((theta*(M_PI/180))) * cosf((phi*(M_PI/180)));
			break;
		case 3:
			light2RotationX = light2Radius * cosf((theta*(M_PI/180))) * cosf((phi*(M_PI/180)));
			light2RotationY = light2Radius * sinf((phi*(M_PI/180)));
			light2RotationZ = light2Radius * sinf((theta*(M_PI/180))) * cosf((phi*(M_PI/180)));
			break;
	}
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
	GLfloat lPos1[] = {light1RotationX,light1RotationY,light1RotationZ,1};//x,y,z,czy światło jest odległe
	GLfloat lPos2[] = {light2RotationX,light2RotationY,light2RotationZ,1};
	GLfloat col[] = {1,0,0,1};
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(cameraRotationX,cameraRotationY,cameraRotationZ,0,0,0,0,camOrientation,0);//Ustawienie kamery
	light1.setPosition(lPos1);
	light2.setPosition(lPos2);
	// glEnable(GL_COLOR_MATERIAL);
	glRotatef(totalRotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(totalRotationY, 0.0f, 1.0f, 0.0f); 
    glRotatef(totalRotationZ, 0.0f, 0.0f, 1.0f);
	axis();
	if(drawTeapot){ 
		glutSolidTeapot(1);
	}else{
		egg.initMaterial();
		egg.draw(eggMode);
	}
	glutSwapBuffers();
}	
void Init() {
	pix2angle = 360.0/800;
	pix2angle = 360.0/800;
	egg.generateMatrix();
	glEnable(GL_DEPTH_TEST); //bez tego frontalna sciana nadpisuje tylnią
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1,1,-1,1,2,20);
	glMatrixMode(GL_MODELVIEW);
	// Włącza culling, czyli pomijanie tylnych ścianek
	glEnable(GL_CULL_FACE);
    // Ustawia kierunek frontowych ścianek jako przeciwny do ruchu wskazówek zegara
    glFrontFace(GL_CW);
    // Ustawia pomijanie tylnych ścianek
    glCullFace(GL_BACK);
	// Kolor stały
	light1.setColor(1.0,0.0,0.0);
	light2.setColor(0.0,1.0,0.0);
	light1.initLight();
	light2.initLight();
	//Drugie światło
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING); //Włączenie oświetlenia
	glEnable(GL_LIGHT0); //Dodanie źródła światła
	glEnable(GL_LIGHT1);	
}
// Sprawko do 15 w pon
// W sprawku Phong,Gouraud i wektor normalny
// TODO - Kąty przestzenne dla lamp radiany określają stożek świecenia światła
// ADS - (Nie odpowiada fizyce) światło nie jest jednorodne 
// Ambient - ogólnie wszędzie bezkierunkowe
// Diffuse - kąt padania = kąt odbicia
// Specular - odbicia lustrzane
// TODO - Każdemu punktowi dodać ADS składowa to sposób w jaki obiekt odbija ads
// Tylko jednokrotne odbicie
// TODO - Światło z reflektora ma drogę reflektor/obiekt(Tłumienie) obiekt/kamera
// TODO - cieniowanie Phonga i Gourauda
// Różnią się liczenie wektora normalnego
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
	glutKeyboardFunc(normalKey);
	glutSpecialFunc(specialKey);
	glutMotionFunc(mouse);
	glutMouseWheelFunc(mouseWheel);
	menu();
	
	glutMainLoop();
	system("pause");
	return 0;
}