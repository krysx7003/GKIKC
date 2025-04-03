#include <GL/glu.h>
#include <math.h>
#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#include "Light.hpp"
using namespace std;

void Light::initLight(){
    glLightfv(lightID, GL_AMBIENT, light_ambient);
	glLightfv(lightID, GL_DIFFUSE, light_diffuse);
	glLightfv(lightID, GL_SPECULAR, light_specular);
	glLightf(lightID, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(lightID, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(lightID, GL_QUADRATIC_ATTENUATION, att_quadratic);
}
Light::Light(GLenum newLightID){
    lightID = newLightID;
}
void Light::setPosition(GLfloat lPos[]){
    glLightfv(lightID,GL_POSITION,lPos);
}
void Light::setColor(float r,float g,float b){
    light_ambient[0] = r;
    light_ambient[1] = g;
    light_ambient[2] = b;
    light_diffuse[0] = r;
    light_diffuse[1] = g;
    light_diffuse[2] = b;
    light_specular[0] = r;
    light_specular[1] = g;
    light_specular[2] = b;
}