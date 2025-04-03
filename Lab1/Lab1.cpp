#include <windows.h>
#include <iostream>
#define FREEGLUT_STATIC
#include <GL/freeglut.h>

HWND consoleWindow;     
HWND glutWindow;        

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
void draw_square(float x, float y, float a) {
	glColor3d(1, 1, 1);
	glBegin(GL_TRIANGLES);
	glVertex2f(x, y);
	glVertex2f(x, y + a);
	glVertex2f(x + a, y);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex2f(x + a, y + a);
	glVertex2f(x, y + a);
	glVertex2f(x + a, y);
	glEnd();
}
int desired_depth;
int sleep_time = 0;
void sierpinski(float size,int depth,float startx,float starty) {
	float edge = size / 3;
	float y = starty;
	for (int i = 0; i < 3; i++) {
		float x = startx;
		for (int j = 0; j < 3; j++) {
			if (j == 1 && i == 1) {
				draw_square(x, y, edge);
				Sleep(sleep_time);
				glFlush();
			}
			else if (depth < desired_depth) {
				sierpinski(edge, depth + 1, x, y);
			}
			x += edge;
		}
		y += edge;
	}

}
bool disappear = false;
void menu(){
	toggleFocusToConsole();
	int x;
	std::cout<<"===================================\n";
	std::cout<<"1. Dywan o podanej glebokosci\n";
	std::cout<<"2. Wizualizacja znikania dywanu\n";
	std::cout<<"3. Zakoncz program\n";
	std::cout<<"===================================\n";
	std::cout<<"> ";
	std::cin>>x;
	switch (x)
	{
	case 1:
		std::cout<<"Podaj glebokosc dywanu: \n";
		std::cout<<"> ";
		std::cin>>desired_depth;
		break;
	case 2:
		disappear = true;
		sleep_time=16;
		break;
	case 3:
		exit(0);
		break;
	default:
		break;
	}
	toggleFocusToGLUT();
}
void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	menu();
	if(disappear){
		for(int i = 0;i<6;i++){
			desired_depth=i;
			sleep_time /=2;
			sierpinski(2,0,-1,-1);
			Sleep(40);
		}
		disappear = false; 
	}else{
		sierpinski(2,0,-1,-1);
	}
	Sleep(4000);
	glutPostRedisplay();
}	
void Init() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glColor3f(0, 0, 0);
	glLoadIdentity();
	gluOrtho2D(-1.0, 1.0, - 1.0, 1.0);
}
int main(int argc, char** argv){
	consoleWindow = GetConsoleWindow();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowSize(243, 243);
	glutCreateWindow("Lab 1 - Dywan Sierpińskiego");
	glutWindow = FindWindowW(NULL,L"Lab 1 - Dywan Sierpinskiego");
	glutDisplayFunc(display);
	Init();
	glutMainLoop();
	system("pause");
	return 0;
}

