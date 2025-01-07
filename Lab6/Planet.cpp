#include "Planet.h"
#include <math.h>
#define FREEGLUT_STATIC
#include <GL/freeglut.h>
void Planet::axis(){
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-15.0, 0.0, 0.0);
    glVertex3f(15.0, 0.0, 0.0);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, -15.0, 0.0);
    glVertex3f(0.0, 15.0, 0.0);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, -15.0);
    glVertex3f(0.0, 0.0, 15.0);
    glEnd();
	glEnable(GL_LIGHTING);
}
void Planet::animateSpin(){
    spin += planetSpeed;
    if (spin > 360.0f) {
        spin -= 360.0f;
    }
}
void Planet::move(float dTime){
    float orbitalPeriod = sqrtf(powf(distance, 3));
    float orbitalSpeed = 2.0f * 3.1415926f / orbitalPeriod;
    planetTheta += orbitalSpeed*dTime; 
    if (planetTheta >= 2.0f * 3.1415926f) {
        planetTheta -= 2.0f * 3.1415926f;
    }
}
void Planet::moveMoon(float dTime){
    moonTheta += moonOrbitSpeed*dTime;
    if (spin > 360.0f) {
        spin -= 360.0f;
    }
}
void Planet::draw(u_int texture){
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    float planetX = xAxis * distance * cosf(planetTheta) + ellipseCenterX;
    float planetY = yAxis * distance * sinf(planetTheta) - ellipseCenterY;
    glTranslatef(planetX, 0.0f, planetY);
    if(hasMoon){
        float x = myMoon.distance * cosf(moonTheta);
        float y = myMoon.distance * sinf(moonTheta);
        glPushMatrix();
        drawMoonsOrbit();
        glRotatef(moonTheta, 0.0f, 1.0f, 0.0f);
        glTranslatef(x, 0.0f, y);
        glRotatef(tilt, 1.0f, 0.0f, 0.0f);
        glRotatef(myMoon.spin, 0.0f, 1.0f, 0.0f);
        axis();
        glBindTexture(GL_TEXTURE_2D, moonTexture);
        glutSolidSphere(myMoon.size, myMoon.points, myMoon.points);  
        glPopMatrix();
    } 
    positon = {planetX, 0.0f, planetY};
    glRotatef(tilt, 1.0f, 0.0f, 0.0f);
    glRotatef(spin, 0.0f, 1.0f, 0.0f);
    axis();
    if(camera){
        gluLookAt(planetX, 0.0f, planetY,0,0,0,0,1,0);
    }
    glBindTexture(GL_TEXTURE_2D, texture);
    glutSolidSphere(size,points,points);
    glPopMatrix();
}
void Planet::drawOrbit(){
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    glColor3f(1.0, 1.0, 1.0);
    for(int i = 0;i<points *2;i++){
        float theta = 2.0f * 3.1415926f * float(i) / float(points * 2);
        float x = xAxis * distance * cosf(theta);
        float y = yAxis * distance * sinf(theta);
        glVertex3f(x + ellipseCenterX, 0.0f, y - ellipseCenterY);  
    }
    glEnd();
    glEnable(GL_LIGHTING);
    glPopMatrix();
}
void Planet::drawMoonsOrbit(){
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    glColor3f(1.0, 1.0, 1.0);
    for(int i = 0;i<myMoon.points *2;i++){
        float theta = 2.0f * 3.1415926f * float(i) / float(myMoon.points * 2);
        float x = myMoon.distance * cosf(theta) ;
        float y = myMoon.distance * sinf(theta) ;
        glVertex3f(x,0.0f,y);
    }
    glEnd();
    glEnable(GL_LIGHTING);
    glPopMatrix(); 
}
void Planet::animateSpinMoon(){
    myMoon.spin += moonSpeed;
    if (myMoon.spin > 360.0f) {
        myMoon.spin -= 360.0f;
    }
}
void Planet::setTilt(float degree){
    tilt = degree;
}
Planet::Planet(float newDistance,float newSize){
    distance = newDistance;
    size = newSize;
}
Planet::Planet() : points(50), size(1.0f), distance(0.0f) {}
void Planet::setDistance(float newDistance){
    distance = newDistance;
    positon = {distance, 0.0f, 0.0f};
}
void Planet::setElipse(float newX,float newY){
    xAxis = newX;
    yAxis = newY;
    c = sqrtf(yAxis * yAxis - xAxis * xAxis); 
    ellipseCenterX = 0.0f;                  
    ellipseCenterY = c* distance/3;
}
point Planet::getPosition(){
    return positon;
}
void Planet::addMoon(moon newMoon){
    myMoon = newMoon;
    hasMoon = true;
}
void Planet::setCamera(){
    camera = !camera;
}
void Planet::setMoonTexture(u_int newTexture){
    moonTexture = newTexture;
}
