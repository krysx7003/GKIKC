#include <windows.h>
#include <iostream>
#include <GL/glu.h>
#include <vector>
#include <math.h>
#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//My .h files

using namespace std;
//Constants
const int TEXT_HEIGHT = 13;
const int PLANET_NUM = 10;//Sun is a planet
const int CAMERA_NUM = 2;
//Global variables
HWND consoleWindow;     
HWND glutWindow;
u_int *textureIDs;
void *font = GLUT_BITMAP_8_BY_13;
int currentPlanet = 3;
string planetNames[PLANET_NUM];
int currentCamera = 0; 
string cameraNames[CAMERA_NUM];
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
	string s = "Kamera: ";
	drawString(s.c_str(),2,800-TEXT_HEIGHT,color,font);
    s = "F1 - Zmien kamere";
    drawString(s.c_str(),2,800-(2*TEXT_HEIGHT),color,font);
    s = "Planeta: "+planetNames[currentPlanet];
    drawString(s.c_str(),2,800-(3*TEXT_HEIGHT),color,font);
    s = "F2 - Poprzednia planeta || F3 Nastepna planeta || F4 - Lista Planet";
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
}
void display(){
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
		cout << "Failed to load texture! " <<stbi_failure_reason()<< endl;
		system("pause");
		exit(1);
	}
	stbi_image_free(data);
}	
void init(){
    glEnable(GL_DEPTH_TEST); //bez tego frontalna sciana nadpisuje tylnią
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glFrustum(-1,1,-1,1,2,20);
	glMatrixMode(GL_MODELVIEW);
    glFrontFace(GL_CW);// Ustawia kierunek frontowych ścianek jako przeciwny do ruchu wskazówek zegara
	glEnable(GL_CULL_FACE);// Włącza culling, czyli pomijanie tylnych ścianek  
    glCullFace(GL_BACK);// Ustawia pomijanie tylnych ścianek

    glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING); //Włączenie oświetlenia
	glEnable(GL_LIGHT0); //Dodanie źródła światła
	glEnable(GL_TEXTURE_2D); //Włącza teksturowanie
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
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
	glutIdleFunc(nullptr);
	glutMainLoop();
    
    
	return 0;
}
