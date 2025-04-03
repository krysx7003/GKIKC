#pragma once
#include "Planet.h"
class Sun : public Planet{
    GLenum light;
    void initLight();
    public:
    void draw(u_int texture);
    Sun(GLenum newLight,float newDistance,float newSize):Planet(newDistance,newSize){
        light = newLight;
        initLight();
    };
    Sun():Planet(){}
};