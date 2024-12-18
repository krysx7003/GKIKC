#pragma once
#include <GL/glu.h>
struct point{
    float x;
    float y;
    float z;
};
class Planet{
    protected:
    int points = 50;
    float size  = 1;
    float distance = 0.0f;
    float tilt = 0.0f;
    point positon;
    point center = {0,0,0};
    void axis();
    public:
    void draw(u_int texture);
    void setTilt(float degree);
    void rotateYAxis(float speed);
    void orbitE();
    void drawOrbit();
    Planet(float newDistance,float newSize);
    Planet();
    void setDistance(float newDistance);
    void setCentre(point newCentre);
    point getPosition();
};