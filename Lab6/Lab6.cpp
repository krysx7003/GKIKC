#include <windows.h>
#include <iostream>
#include <GL/glu.h>
#include <vector>
#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//My .h files
#include "Planet.h"
#include "Sun.h"

using namespace std;
//Constants
const int TEXT_HEIGHT = 13;
const int PLANET_NUM = 10;//Moon is a planet
const int CAMERA_NUM = 2;
//Global variables
HWND consoleWindow;     
HWND glutWindow;
u_int textureIDs[PLANET_NUM];
void *font = GLUT_BITMAP_8_BY_13;
int currentPlanet = 3;
Planet planets[PLANET_NUM];
string planetNames[PLANET_NUM] = {"Mercury","Venus","Earth","Moon","Mars","Jupiter","Saturn","Uranus","Neptune","Pluto"};
float planetSizes[PLANET_NUM] = {0.4,0.9,1.0,0.27,0.5,5.0,4.0,2.0,1.9,0.2};
float planetDistances[PLANET_NUM] = {15.0,25.0,35.f,3.8,50.0f,120.0f,200.0f,300.0f,400.0f,500.0f};
float planetAxialTilts[PLANET_NUM] = {0.03f,177.4f,23.5f,6.7,25.2f,3.1f,26.7f,97.8f,28.3f,122.5f};
float planetOrbitalTilts[PLANET_NUM] = {7.0,3.39,0.0,5.1,1.85,1.31,2.49,0.77,1.77,17.16} ;
int currentCamera = 0; 
string cameraNames[CAMERA_NUM] = {"Wolna","Na planecie"};
float pix2angle = 360.0/800,theta = 0.0f,phi = 0.0f;
int radius = 35,lastX = 0,lastY = 0;
float cameraRotationX = radius * cosf((theta*(M_PI/180))) * cosf((phi*(M_PI/180)));
float cameraRotationY = radius * sinf((phi*(M_PI/180)));
float cameraRotationZ = radius * sinf((theta*(M_PI/180))) * cosf((phi*(M_PI/180)));
Sun sun;
//Window controls
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
//UI
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
	string s = "Kamera: "+cameraNames[currentCamera];
	drawString(s.c_str(),2,800-TEXT_HEIGHT,color,font);
    s = "F1 - Zmien kamere";
    drawString(s.c_str(),2,800-(2*TEXT_HEIGHT),color,font);
    s = "Planeta: "+planetNames[currentPlanet];
    drawString(s.c_str(),2,800-(3*TEXT_HEIGHT),color,font);
    s = "F2 - Poprzednia planeta || F3 Nastepna planeta || F4 - Lista Planet";
	drawString(s.c_str(),2,800-(4*TEXT_HEIGHT),color,font);
	s = "ESC - Wyjdz z programu";
	drawString(s.c_str(),2,800-(5*TEXT_HEIGHT),color,font);
	glPopMatrix();                   
    glMatrixMode(GL_MODELVIEW);      
    glPopMatrix(); 
}
//Keyboard control
void normalKey(u_char key,int x,int y){
	switch (key)
	{
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}
void specialKey(int key,int x,int y){
	switch (key){
	//F1 - Change camera
	case GLUT_KEY_F1:
		if(currentCamera==0){
			currentCamera = 1;
		}else{
			currentCamera = 0;
		}
		break;
	//F2 - Prevoius planet
	case GLUT_KEY_F2:
        currentPlanet--;
        if(currentPlanet == 0){
            currentPlanet = PLANET_NUM-1;
        }
		break;
	//F3 - Next planet
	case GLUT_KEY_F3:
        currentPlanet++;
        if(currentPlanet == PLANET_NUM){
            currentPlanet = 1;
        }
		break;
	//F4 - Print plantets
	case GLUT_KEY_F4:
        for(int i=0;i<PLANET_NUM;i++){
            cout<<i<<". "<<planetNames[i]<<"\n";
        }
        toggleFocusToConsole();
        cout<<"Nacisnij Enter zeby kontynuowac\n"<<flush;
        cin.get();
        cin.get();
        toggleFocusToGLUT();
		break;
	default:
		break;
	}
	glutPostRedisplay();
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
	switch(currentCamera){
		case 0:
			cameraRotationX = radius * cosf((theta*(M_PI/180.0f))) * cosf((phi*(M_PI/180.0f)));
			cameraRotationY = radius * sinf((phi*(M_PI/180.0f)));
			cameraRotationZ = radius * sinf((theta*(M_PI/180.0f))) * cosf((phi*(M_PI/180.0f)));
			break;
		case 1:
			
			break;
	}
	lastX = x;
	lastY = y;
	glutPostRedisplay();
}
void mouseWheel(int button, int dir, int x, int y){
	if (dir > 0){
        radius -= 10;
    }else{
        radius += 10;
    }
	if(radius<20){
		radius=20;
	}
	glutPostRedisplay();
}
void animate(){
	for(int i = 0;i<PLANET_NUM;i++){
		planets[i].animateSpin();
	}
	glutPostRedisplay();
}
void display(){
	GLfloat lPos[] = {0,0,0,1};
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(cameraRotationX,cameraRotationY,cameraRotationZ,0,0,0,0,1,0);
	glLightfv(GL_LIGHT0,GL_POSITION,lPos);
	// glRotatef(90.0f, 1.0f, 0.0f, 0.0f); 
	sun.draw(0.0f);
	for(int i = 0;i<PLANET_NUM;i++){
		glPushMatrix();
		glRotatef(planetOrbitalTilts[i], 1.0f, 0.0f, 0.0f);
		planets[i].drawOrbit();
		planets[i].draw(i);
		glPopMatrix();
	}
    showInfo();
	glutSwapBuffers();
}
void loadTexture(const char* fileName,int texID){		
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
		cout << "Failed to load texture! " <<stbi_failure_reason()<< endl;
		system("pause");
		exit(1);
	}
	stbi_image_free(data);
}	
void init(){
	sun = Sun(GL_LIGHT0,0,10);
	float pos = 10;
	for(int i = 0;i<PLANET_NUM;i++){
		if(i!=3){
			planets[i] = Planet(i,planetSizes[i]);
			pos += planetDistances[i];
			planets[i].setDistance(pos);
			planets[i].setTilt(planetAxialTilts[i]);
		}else{
			planets[i] = Planet(i,planetSizes[i]);
			planets[i].setCentre(planets[2].getPosition());
			planets[i].setDistance(planetDistances[i]);
			planets[i].setTilt(planetAxialTilts[i]);
		}

	}
    glEnable(GL_DEPTH_TEST); //bez tego frontalna sciana nadpisuje tylnią
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(45,1,0.01,5000);
	glMatrixMode(GL_MODELVIEW);
    glFrontFace(GL_CCW);// Ustawia kierunek frontowych ścianek jako przeciwny do ruchu wskazówek zegara
	glEnable(GL_CULL_FACE);// Włącza culling, czyli pomijanie tylnych ścianek  
    glCullFace(GL_BACK);// Ustawia pomijanie tylnych ścianek

    glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING); //Włączenie oświetlenia
	glEnable(GL_LIGHT0); //Dodanie źródła światła
	glEnable(GL_TEXTURE_2D); //Włącza teksturowanie
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glGenTextures(10,textureIDs);
}
//Na 7 stycznia
//Kule oteksturowane
//Jedna z nich jest lampą (oświetlenie kierunkowe ale 4pi steradianów)
//Planety wirują wokół osi Y (delikatnie pochylone)
//Słońce wiruje wokół osi 
//Planety latają wokół słońca na początku po okręgu potem elipsa
//W jednym z ognisk elipsy słońce
//Trecie prawo Keplera (zmienna prędkość)
//Jeżeli się uda przypisać ziemi księżyc
//2 Kamery sterowane przy użyciu myszy
//Swobodna na sferze (biegunowa,azymut)
//Umiejscowiona na planecie
int main(int argc, char** argv){
    consoleWindow = GetConsoleWindow();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800,800);
	glutCreateWindow("Lab 6 - Uklad sloneczny");
	glutWindow = FindWindowW(NULL,L"Lab 6 - Uklad sloneczny");
	init();
	glutDisplayFunc(display);
    glutKeyboardFunc(normalKey);
    glutSpecialFunc(specialKey);
	glutMotionFunc(mouse);
	glutMouseWheelFunc(mouseWheel);
	glutIdleFunc(animate);
	glutMainLoop();
	return 0;
}
