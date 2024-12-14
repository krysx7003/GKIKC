#pragma once
#include <GL/glu.h>
using namespace std;
class Light{
    
    //Kolor ustawić raz
	float light_ambient[4] = {0.1f, 0.1f, 0.1f, 1.0f};
	float light_diffuse[4] = {0.7f, 0.7f, 0.7f, 1.0f};
	float light_specular[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	float att_constant = 1.0;
	float att_linear = 0.05;
	float att_quadratic = 0.001;
    float rgb[3] = {1.0,1.0,1.0};
    GLenum lightID;
    void normalize(GLfloat* v);
    public:
    void initLight();
    Light(GLenum newLightID);
    void setColor(float r,float g,float b);
    void setPosition(GLfloat lPos[]);
};