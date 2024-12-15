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
void Light::normalize(GLfloat* v) {
    GLfloat length = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (length > 0.0f) {
        v[0] /= length;
        v[1] /= length;
        v[2] /= length;
    }
}
void Light::setPosition(GLfloat lPos[]){
    GLfloat light_direction[3];
    light_direction[0] = -lPos[0];
    light_direction[1] = -lPos[1];
    light_direction[2] = -lPos[3];
    normalize(light_direction);
    glLightfv(lightID,GL_POSITION,lPos);
    glLightfv(lightID,GL_SPOT_DIRECTION,light_direction);
    glLightf(lightID, GL_SPOT_CUTOFF, 25.0f);
    glLightf(lightID, GL_SPOT_EXPONENT, 2.0f);
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