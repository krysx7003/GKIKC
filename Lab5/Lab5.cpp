#include <windows.h>
#include <iostream>
#include <GL/glu.h>
#include <vector>
#include <math.h>
#define FREEGLUT_STATIC
#include <GL/freeglut.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Egg.hpp"
#include "Light.hpp"
using namespace std;
HWND consoleWindow;     
HWND glutWindow;
const int TEXT_HEIGHT = 13;
void *font = GLUT_BITMAP_8_BY_13;
u_int textureIDs[4];
int currentTex = 0;
GLfloat deg = 0;       
int sx =0,sy = 0,sz = 0; 
bool drawTeapot = true;
int moveMode = 0;
float totalRotationX = 0.0f,totalRotationY = 0.0f,totalRotationZ = 0.0f;
float pix2angle,theta = 0.0f,phi = 0.0f;
int radius = 6,lastX = 0,lastY = 0;
float cameraRotationX = radius * cosf((theta*(M_PI/180))) * cosf((phi*(M_PI/180)));
float cameraRotationY = radius * sinf((phi*(M_PI/180)));
float cameraRotationZ = radius * sinf((theta*(M_PI/180))) * cosf((phi*(M_PI/180)));
Light light1(GL_LIGHT0);
GLfloat light1Radius = 10;
string movementMode[2] = {"Obiekt","Kamera"};
string textureName[4] = {"0 - Brak","1 - Kulki","2 - Granit","3 - Siatka"};
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
void reset_rotation(){
	theta = 0.0f;
	phi = 0.0f;
	lastX = 0;
	lastY = 0;
	cameraRotationX = radius * cosf((theta*(M_PI/180.0f))) * cosf((phi*(M_PI/180.0f)));
	cameraRotationY = radius * sinf((phi*(M_PI/180.0f)));
	cameraRotationZ = radius * sinf((theta*(M_PI/180.0f))) * cosf((phi*(M_PI/180.0f)));
}
void drawString(const char *str, int x, int y, float color[4], void *font){
    glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT); 
    glDisable(GL_LIGHTING); 
    glDisable(GL_TEXTURE_2D);
    glDepthFunc(GL_ALWAYS);
    glColor4fv(color);
    glRasterPos2i(x, y); 
    while(*str){
        glutBitmapCharacter(font, *str);
        ++str;
    }
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glDepthFunc(GL_LEQUAL);
    glPopAttrib();	 
}
void showInfo(){
	glPushMatrix();                     
    glLoadIdentity();                   
    glMatrixMode(GL_PROJECTION);        
    glPushMatrix();                     
    glLoadIdentity();                   
    gluOrtho2D(0, 800, 0, 800);
	float color[4] = {1, 1, 1, 1};
	string s = "Tryb obrotu: "+movementMode[moveMode];
	drawString(s.c_str(),2,800-TEXT_HEIGHT,color,font);
	s = "F1 - Obiekt || F2 - Kamera";
	drawString(s.c_str(),2,800-(2*TEXT_HEIGHT),color,font);
	s = "Tekstura: "+textureName[currentTex];
	drawString(s.c_str(),2,800-(3*TEXT_HEIGHT),color,font);
	s = "F3 - Nastepna || F4 - Poprzednia";
	drawString(s.c_str(),2,800-(4*TEXT_HEIGHT),color,font);
	glPopMatrix();                   
    glMatrixMode(GL_MODELVIEW);      
    glPopMatrix(); 
}
void printControls(){
	cout<<"==============================\n";
	cout<<"F1 - Tryb obrotu obiektu\n";
	cout<<"F2 - Tryb obrotu kamery\n";
	cout<<"F3 - Nastepna tekstura\n";
	cout<<"F4 - Poprzednia tekstura\n";
	cout<<"ESC - Powrot do menu (okno konsolowe)\n";
	cout<<"Nalezy nacisnac i przytrzymac PPM\n";
	cout<<"Ruch myszy w osi X - Obrot osi X\n";
	cout<<"Ruch myszy w osi Y - Obrot osi Y\n";
	cout<<"Scroll up - Przybilizenie obiektu\n";
	cout<<"Scroll down - Oddalenie obiektu\n";
	cout<<"Nacisnij Enter zeby kontynuowac\n"<<flush;
	cin.get();
	cin.get();
}
void axis(){
	glDisable(GL_LIGHTING);
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
	glEnable(GL_LIGHTING);
}
void printOptions();
void menu();
void printOptions(){
	int density = egg.getDensity();    
    bool color = egg.getColor(); 
    float scale = egg.getScale();
	cout<<"==============================\n";
	cout<<"1.Skala obiektow: "<<scale<<"\n";
	cout<<"2.Ilosc punktow: "<<density<<"\n";
	cout<<"3.Promien kamery: "<<radius<<"\n";
	cout<<"4.Wroc do menu"<<"\n";
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
		cout<<"Nowy promien kamery\n";
		cout<<"> ";
		cin>>radius;
		printOptions();
		break;
	case 4:
		menu();
		break;
	}
}
void menu(){
	toggleFocusToConsole();
	reset_rotation();
	cout<<"==============================\n";
	cout<<"1. Narysuj czajnik\n";
	cout<<"2. Narysuj jajko\n";
	cout<<"3. Opcje\n";
	cout<<"4. Kontrola\n";
	cout<<"5. Zakoncz program\n";
	cout<<"> ";
	int x;
	cin>>x;
	switch (x){
	case 1:
		drawTeapot = true; 
		break;
	case 2:
		drawTeapot = false;
		break;
	case 3:
		printOptions();
		break;
	case 4:
		printControls();
		menu();
		break;
	case 5:
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
	//F3 - Następna tekstura
	case GLUT_KEY_F3:
		currentTex++;
		if(currentTex>3){
			currentTex = 0;
		}
		break;
	//F3 - Poprzednia tekstura
	case GLUT_KEY_F4:
		currentTex--;
		if(currentTex<0){
			currentTex = 3;
		}
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
	float dY = y - lastY;
	lastY = y;
	float dX = x - lastX;
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
	if(radius<=1){
		radius=1;
	}
	glutPostRedisplay();
}
void display() {
	GLfloat lPos1[] = {0,0,10,1};//x,y,z,czy światło jest odległe
	GLfloat col[] = {1,0,0,1};
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	gluLookAt(cameraRotationX,cameraRotationY,cameraRotationZ,0,0,0,0,1,0);//Ustawienie kamery
	light1.setPosition(lPos1);
	glEnable(GL_COLOR_MATERIAL);
	axis();
	glRotatef(totalRotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(totalRotationY, 0.0f, 1.0f, 0.0f); 
    glRotatef(totalRotationZ, 0.0f, 0.0f, 1.0f);
	
	glBindTexture(GL_TEXTURE_2D, textureIDs[currentTex]);
	if(drawTeapot){
		glColor3f(1.0, 1.0, 1.0); 
		glutSolidTeapot(1);
	}else{
		egg.draw();
	}
	showInfo();
	glutSwapBuffers();
}
void loadTexture(const char* fileName,int texID){
	glGenTextures(1, &textureIDs[texID]);	
    glBindTexture(GL_TEXTURE_2D, textureIDs[texID]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	int width,height,nrChannels;
	unsigned char *data = stbi_load(fileName, &width, &height, &nrChannels, 0);
	if (data){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else{
		cout << "Failed to load texture!" <<stbi_failure_reason()<< endl;
		system("pause");
		exit(1);
	}
	stbi_image_free(data);
	egg.setTextureSize(height,width);
}	
void Init() {
	pix2angle = 360.0/800;
	glEnable(GL_DEPTH_TEST); //bez tego frontalna sciana nadpisuje tylnią
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glFrustum(-1,1,-1,1,2,20);
	glMatrixMode(GL_MODELVIEW);
    glFrontFace(GL_CW);// Ustawia kierunek frontowych ścianek jako przeciwny do ruchu wskazówek zegara
	glEnable(GL_CULL_FACE);// Włącza culling, czyli pomijanie tylnych ścianek  
    glCullFace(GL_BACK);// Ustawia pomijanie tylnych ścianek
	// Kolor stały
	light1.setColor(1.0,1.0,1.0);
	light1.initLight();
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING); //Włączenie oświetlenia
	glEnable(GL_LIGHT0); //Dodanie źródła światła
	glEnable(GL_TEXTURE_2D); //Włącza teksturowanie
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	loadTexture("../tekstura1.tga",1);
	loadTexture("../tekstura2.tga",2);
	loadTexture("../tekstura3.tga",3);
}
int main(int argc, char** argv){
	consoleWindow = GetConsoleWindow();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800,800);
	glutCreateWindow("Lab 5 - Tekstury");
	glutWindow = FindWindowW(NULL,L"Lab 5 - Tekstury");
	Init();
	glutDisplayFunc(display);
	glutIdleFunc(nullptr);
	glutKeyboardFunc(normalKey);
	glutSpecialFunc(specialKey);
	glutMotionFunc(mouse);
	glutMouseWheelFunc(mouseWheel);
	menu();
	glutMainLoop();
	return 0;
}