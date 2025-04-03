#pragma once
#include <GL/glu.h>
struct point{
    float x;
    float y;
    float z;
};
struct moon{
    int points = 25;
    float size  = 1,spin = 0;
    float distance = 0.0f;
    point center = {0,0,0};
    float tilt = 0.0f;
};
class Planet{
    protected:
    int points = 50;
    float size  = 1,spin = 0;
    float distance = 0.0f;
    float tilt = 0.0f;
    point positon;
    float c,ellipseCenterX,ellipseCenterY;
    float xAxis = 1.0f;
    float yAxis = 0.5f;
    float planetTheta = 0.0f;
    u_int moonTexture;
    bool hasMoon = false;
    float moonTheta = 0.0f;
    float planetOrbitSpeed ,planetSpeed = 0.5f;
    float moonOrbitSpeed = 0.24f,moonSpeed = 0.5f;
    bool camera = false;
    moon myMoon; 
    void axis();
    void drawMoonsOrbit();
    public:
    void animateSpin();
    void animateSpinMoon();
    void move(float dTime);
    void moveMoon(float dTime);
    void draw(u_int texture);
    void setTilt(float degree);
    void drawOrbit();
    Planet(float newDistance,float newSize);
    Planet();
    void setDistance(float newDistance);
    void setElipse(float newX,float newY);
    void setCamera();
    void setMoonTexture(u_int newTexture);
    point getPosition();
    void addMoon(moon newMoon);
};