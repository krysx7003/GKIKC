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
bool drawTeapot = true,smooth = true;
int eggMode = 0,moveMode = 0;
float sensitivity = 0.75f;
float totalRotationX = 0.0f,totalRotationY = 0.0f,totalRotationZ = 0.0f;
int radius = 6,lastX = 0,lastY = 0;
float cameraRotationX = 0.0f,cameraRotationY = 0.0f,cameraRotationZ = radius;
int light1Radius = 10;
float light1RotationX = 0.0f,light1RotationY = 0.0f,light1RotationZ = light1Radius;
int light2Radius = 10;
float light2RotationX = 0.0f,light2RotationY = 0.0f,light2RotationZ = light2Radius;
float phi = 0.0f;       
float theta = 0.0f;

struct light{
	float mat_ambient[4] = {1.0, 1.0, 1.0, 1.0};
	float mat_diffuse[4] = {1.0, 1.0, 1.0, 1.0};
	float mat_specular[4] = {1.0, 1.0, 1.0, 1.0};

	float mat_shininess = 20.0;

	//Kolor ustawić raz
	float light_ambient[4] = {0.1, 0.1, 0.0, 1.0};
	float light_diffuse[4] = {0.8, 0.8, 0.0, 1.0};
	float light_specular[4] = {1.0, 1.0, 1.0, 1.0};
	float light_position[4] = {1.0, 0.0, 10.0, 1.0};
	float att_constant = 1.0;
	float att_linear = 0.05;
	float att_quadratic = 0.001;
};
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
	cout<<"5.Czulosc myszki: "<<sensitivity<<"\n";
	cout<<"6.Rozmiar punktow: "<<pointSize<<"\n";
	cout<<"7.Niewidoczne trojkaty: "<<bool_to_string(smooth)<<"\n";
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
		smooth =! smooth;
		printOptions();
		break;
	case 8:
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
	float phi,theta,maxPhi;
	int dX,dY;
	phi = sensitivity*((2.0f * y / 400) - 1.0f);           
	theta = sensitivity*((2.0f * (400 - x) / 400) - 1.0f);
	switch(moveMode){
		case 0:
			dX = x - lastX;
			dY = y - lastY;
			totalRotationX += sensitivity * dY;
			totalRotationY += sensitivity * dX;
			//TODO - Naprawić to gówno
			// totalRotationZ += sensitivity * (dX - dY);
			break;
		case 1:
			cameraRotationX = radius*cos(theta)*cos(phi);
			cameraRotationY = radius*sin(phi);
			cameraRotationZ = radius*sin(theta)*cos(phi);
			break;
		case 2:
			light1RotationX = radius*cos(theta)*cos(phi);
			light1RotationY = radius*sin(phi);
			light1RotationZ = radius*sin(theta)*cos(phi);
			break;
		case 3:
			light2RotationX = radius*cos(theta)*cos(phi);
			light2RotationY = radius*sin(phi);
			light2RotationZ = radius*sin(theta)*cos(phi);
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
	GLfloat lPos2[] = {light1RotationX,light1RotationY,light1RotationZ,1};
	GLfloat col[] = {1,0,0,1};
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLightfv(GL_LIGHT0,GL_POSITION,lPos1);
	gluLookAt(cameraRotationX,cameraRotationY,cameraRotationZ,0,0,0,0,1,0);//Ustawienie kamery
	glRotatef(totalRotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(totalRotationY, 0.0f, 1.0f, 0.0f); 
    glRotatef(totalRotationZ, 0.0f, 0.0f, 1.0f);
	if(drawTeapot){ 
		glutWireTeapot(1);
	}else{
		if(smooth){
			glShadeModel(GL_SMOOTH);
		}else{
			glShadeModel(GL_FLAT);
		}
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
	// Włącza culling, czyli pomijanie tylnych ścianek
	glEnable(GL_CULL_FACE);
    // Ustawia kierunek frontowych ścianek jako przeciwny do ruchu wskazówek zegara
    glFrontFace(GL_CW);
    // Ustawia pomijanie tylnych ścianek
    glCullFace(GL_BACK);
	// Właściwości do punktu stałe
	// glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	// glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	// glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	// glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
	light light1;
	// Kolor stały
	glLightfv(GL_LIGHT0, GL_AMBIENT, light1.light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light1.light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light1.light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light1.light_position);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, light1.att_constant);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, light1.att_linear);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, light1.att_quadratic);
	//Drugie światło
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING); //Włączenie oświetlenia
	glEnable(GL_LIGHT0); //Dodanie źródła światła
	glEnable(GL_LIGHT1);
}
// Sprawko do 15 w pon
// W sprawku Phong,Gouraud i wektor normalny
// TODO - Kąty przestzenne dla lamp radiany określają stożek świecenia światła
// Stożek za duży zmniejszyć
// ADS - (Nie odpowiada fizyce) światło nie jest jednorodne 
// Ambient - ogólnie wszędzie bezkierunkowe
// Diffuse - kąt padania = kąt odbicia
// Specular - odbicia lustrzane
// TODO - 2 Reflektory światła kontrastowe B i R
// TODO - Każdemu punktowi dodać ADS składowa to sposób w jaki obiekt odbija ads
// Tylko jednokrotne odbicie
// TODO - Światło z reflektora ma drogę reflektor/obiekt(Tłumienie) obiekt/kamera
// TODO - cieniowanie Phonga i Gourauda
// Różnią się liczenie wektora normalnego
// Jakjko jeden kolor najlepiej biały tło czarne
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