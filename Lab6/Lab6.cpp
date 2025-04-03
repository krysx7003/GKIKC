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
const int PLANET_NUM = 9;
const int ALL = 11;
const int CAMERA_NUM = 2;
//Global variables
HWND consoleWindow;     
HWND glutWindow;
GLuint textureIDs[ALL];
void *font = GLUT_BITMAP_8_BY_13;
int currentPlanet = 3;
Planet planets[PLANET_NUM];
string planetNames[PLANET_NUM+1] = {"Mercury","Venus","Earth","Moon","Mars","Jupiter","Saturn","Uranus","Neptune","Pluto"};
const char* fileNames[ALL] = {"textures\\2k_sun.tga","textures\\2k_mercury.tga","textures\\2k_venus_surface.tga","textures\\2k_earth_daymap.tga",
	"textures\\2k_moon.tga","textures\\2k_mars.tga","textures\\2k_jupiter.tga","textures\\saturnmapthumb.tga","textures\\2k_uranus.tga",
	"textures\\2k_neptune.tga","textures\\plutomapthumb.tga"};
float planetSizes[PLANET_NUM] = {2,6.5,7,6.5,14,10,8,8,5};
float planetDistances[PLANET_NUM] = {15.0,25.0,35,50.0f,70.0f,80.0f,90.0f,100.0f,110.0f};
float planetAxialTilts[PLANET_NUM] = {0.03f,177.4f,23.5f,25.2f,3.1f,26.7f,97.8f,28.3f,122.5f};
float xAxis[PLANET_NUM] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
float yAxis[PLANET_NUM] = {1.3f, 1.3f, 1.2f, 1.21f, 1.25f, 1.15f, 1.25f, 1.2f, 1.4f};
float planetOrbitalTilts[PLANET_NUM] = {7.0,3.39,0.0,1.85,1.31,2.49,0.77,1.77,17.16} ;
int currentCamera = 0; 
string cameraNames[CAMERA_NUM] = {"Wolna","Na planecie"};
float pix2angle = 360.0/800,theta = 0.0f,phi = 0.0f;
int radius = 35,lastX = 0,lastY = 0;
float cameraRotationX = radius * cosf((theta*(M_PI/180))) * cosf((phi*(M_PI/180)));
float cameraRotationY = radius * sinf((phi*(M_PI/180)));
float cameraRotationZ = radius * sinf((theta*(M_PI/180))) * cosf((phi*(M_PI/180)));
float cameraX = 35,cameraY = 35,cameraZ = 35;
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
	float moveSpeed = 10.0f;
	switch (key){
	case 'W':
	case 'w':
		if(currentCamera == 0){
			cameraY -= 10.0f;
		}	
		break;
	case 'A':
	case 'a':
		if(currentCamera == 0){
			cameraX -= 10.0f;
		}
		break;
	case 'S':
	case 's':
		if(currentCamera == 0){
			cameraY += 10.0f;
		}
		break;
	case 'D':
	case 'd':
		if(currentCamera == 0){
			cameraX += 10.0f;
		}
		break;
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
		planets[currentPlanet].setCamera();
        currentPlanet--;
		planets[currentPlanet].setCamera();
        if(currentPlanet == 0){
            currentPlanet = PLANET_NUM-1;
        }
		break;
	//F3 - Next planet
	case GLUT_KEY_F3:
		planets[currentPlanet].setCamera();
        currentPlanet++;
		planets[currentPlanet].setCamera();
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
void timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);  
}
void animate(){
	for(int i = 0;i<PLANET_NUM;i++){
		planets[i].animateSpin();
		planets[i].move(0.1f);
	}
	planets[2].animateSpinMoon();
	planets[2].moveMoon(0.1f);
	glutPostRedisplay();
}
void display(){
	GLfloat lPos[] = {0,0,0,1};
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(-cameraX,0.0f,-cameraY);
	if(currentCamera==0){
		gluLookAt(cameraX + cameraRotationX,cameraY + cameraRotationY,cameraZ + cameraRotationZ , cameraX, cameraY, cameraZ,0,1,0);
	}else{
		planets[currentPlanet].setCamera();
	}
	glLightfv(GL_LIGHT0,GL_POSITION,lPos);
	sun.draw(textureIDs[0]);
	for(int i = 0;i<PLANET_NUM;i++){
		glPushMatrix();
		glRotatef(planetOrbitalTilts[i], 1.0f, 0.0f, 0.0f);
		planets[i].drawOrbit();
		planets[i].draw(textureIDs[i+1]);
		glPopMatrix();
	}
    showInfo();
	glutSwapBuffers();
}
void loadTexture(const char* fileName,GLuint texID){
	glGenTextures(1,&textureIDs[texID]);	
    glBindTexture(GL_TEXTURE_2D, textureIDs[texID]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	int width,height,nrChannels;
	unsigned char *data = stbi_load(fileName, &width, &height, &nrChannels, 0);
	if (data){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}else{
		cout << "Failed to load texture! "<< fileName <<" " <<stbi_failure_reason()<< endl;
		system("pause");
		exit(1);
	}
	stbi_image_free(data);
}	
void init(){
	sun = Sun(GL_LIGHT0,0,16);
	float pos = 10;
	for(int i = 0;i<PLANET_NUM;i++){
		if(i!=3){
			planets[i] = Planet(i,planetSizes[i]);
			pos += planetDistances[i];
			planets[i].setDistance(pos);
			planets[i].setElipse(xAxis[i],yAxis[i]);
			planets[i].setTilt(planetAxialTilts[i]);
		}
	}
	moon newMoon = {
		25,2,0,13,planets[2].getPosition(),6.7
	};
	planets[2].addMoon(newMoon);
    glEnable(GL_DEPTH_TEST); //bez tego frontalna sciana nadpisuje tylnią
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(45,1,0.01,5000);
	glMatrixMode(GL_MODELVIEW);
    glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING); //Włączenie oświetlenia
	glEnable(GL_LIGHT0); //Dodanie źródła światła
	glEnable(GL_TEXTURE_2D); //Włącza teksturowanie
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	for(int i = 0;i<ALL;i++){
		loadTexture(fileNames[i],i);
	}
	planets[2].setMoonTexture(textureIDs[4]);
}
//Na 7 stycznia
//TODO - 2 Kamery sterowane przy użyciu myszy
//TODO - Swobodna na sferze (biegunowa,azymut)
//TODO - Umiejscowiona na planecie
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
	glutTimerFunc(25, timer, 0);
	glutMainLoop();
	return 0;
}
