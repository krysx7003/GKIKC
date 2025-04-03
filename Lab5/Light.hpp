#pragma once
#include <GL/freeglut.h>
class Light{
    GLenum lightID;
    float light_ambient[4] = {1.0,1.0,1.0,1.0} ;
    float light_diffuse[4] = {0.0,0.0,0.0,1.0} ;
    float light_specular[4] = {0.0,0.0,0.0,1.0} ;
    float att_constant = 1.0;
    float att_linear = 0.05;
    float att_quadratic = 0.001;
    public:
    void initLight();
    void normalize(GLfloat* v);
    void setPosition(GLfloat lPos[]);
    void setColor(float r,float g,float b);
    Light(GLenum newLightID);
};