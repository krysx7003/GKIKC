#include <math.h>
#include <GL/glu.h>
#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#include "Egg.hpp"
using namespace std;

float Egg::randFloat(){
    return (float)rand()/(float)(RAND_MAX);
}
Egg::Egg(int density ) : density(density){
    pointsMatrix.resize(density,vector<pointsRgb>(density));
}
vector<vector<pointsRgb>> Egg::getPointsMatrix(){
    return pointsMatrix;
}
void Egg::generateNormalVect(int u,int v){
    float x_u = (-450*pow(u,4) + 900*pow(u,3) - 810*pow(u,2) + 360*u - 45) * cos(M_PI*v);
    float x_v = M_PI * (90*pow(u,5) - 225*pow(u,4) + 270*pow(u,3) - 180*pow(u,2) + 45*u) * sin(M_PI*v);
    float y_u = 640*pow(u,3) - 960*pow(u,2) + 320*u;
    float y_v = 0;
    float z_u = (-450*pow(u,4) + 900*pow(u,3) - 810*pow(u,2) + 360*u - 45) * sin(M_PI*v);
    float z_v = -M_PI * (90*pow(u,5) - 225*pow(u,4) + 270*pow(u,3) - 180*pow(u,2) + 45*u) * cos(M_PI*v);
    y_u * z_v - z_u * y_v;
    z_u * x_v - x_u * z_v;
    x_u * y_v - y_v * x_v;
    //TODO - Trzeba znormalizować
}
void Egg::generateMatrix(){	
    for(int u=0;u<(density);u++){
        float _u = 0.5/((float)density-1);
        _u *= u;
        if(u==density-1){
            pointsMatrix[u][0].y = scale*((160*pow(_u,4)) - (320*pow(_u,3)) + (160 * pow(_u,2)) - 5);
            if(color){
                pointsMatrix[u][0].r = randFloat();
                pointsMatrix[u][0].g = randFloat();
                pointsMatrix[u][0].b = randFloat();
            }else{
                pointsMatrix[u][0].r = 0.0f;
                pointsMatrix[u][0].g = 0.0f;
                pointsMatrix[u][0].b = 0.0f;
            }
            break;
        }
        for(int v=0;v<density;v++){
            float _v = v/((float)density);
            _v *= 2.0f;
            pointsMatrix[u][v].x = scale*((-90*pow(_u,5) + 225*pow(_u,4) - 270*pow(_u,3) + 180*pow(_u,2) - 45*_u) * cos(M_PI*_v));
            pointsMatrix[u][v].y = scale*(160*pow(_u,4) - 320*pow(_u,3) + 160 * pow(_u,2) - 5);
            pointsMatrix[u][v].z = scale*((-90*pow(_u,5) + 225*pow(_u,4) - 270*pow(_u,3) + 180*pow(_u,2) - 45*_u) * sin(M_PI*_v));
            if(color){
                pointsMatrix[u][v].r = randFloat();
                pointsMatrix[u][v].g = randFloat();
                pointsMatrix[u][v].b = randFloat();
            }else{
                pointsMatrix[u][v].r = 0.0f;
                pointsMatrix[u][v].g = 0.0f;
                pointsMatrix[u][v].b = 0.0f;
            }
        }
    }
}
void Egg::draw(int model){
    switch (model)
    {
    case 1:
        glPointSize(pointSize);
        glBegin(GL_POINTS);
        for(int u=0;u<density-1;u++){
            if(u==0){
                glColor3f(pointsMatrix[u][0].r,pointsMatrix[u][0].g,pointsMatrix[u][0].b);
                glVertex3f(pointsMatrix[u][0].x,pointsMatrix[u][0].y,pointsMatrix[u][0].z);
                continue;
            }
            if(u==density-2){
                glColor3f(pointsMatrix[u+1][0].r,pointsMatrix[u+1][0].g,pointsMatrix[u+1][0].b);
                glVertex3f(pointsMatrix[u+1][0].x,pointsMatrix[u+1][0].y,pointsMatrix[u+1][0].z);
                break;
            }
            for(int v=0;v<density;v++){
                glColor3f(pointsMatrix[u][v].r,pointsMatrix[u][v].g,pointsMatrix[u][v].b);
                glVertex3f(pointsMatrix[u][v].x,pointsMatrix[u][v].y,pointsMatrix[u][v].z);
            }
        }
        glEnd();
        break;
    case 2:
        glBegin(GL_LINES);
        for(int u=0;u<density-1;u++){
            if(u==0){
                for(int v=0;v<density;v++){
                    glColor3f(pointsMatrix[u][0].r,pointsMatrix[u][0].g,pointsMatrix[u][0].b);
                    glVertex3f(pointsMatrix[u][0].x,pointsMatrix[u][0].y,pointsMatrix[u][0].z);
                    glColor3f(pointsMatrix[u+1][v].r, pointsMatrix[u+1][v].g, pointsMatrix[u+1][v].b);
                    glVertex3f(pointsMatrix[u+1][v].x, pointsMatrix[u+1][v].y, pointsMatrix[u+1][v].z);
                }
                continue;
            }
            if(u==density-2){
                for(int v=0;v<density;v++){
                    glColor3f(pointsMatrix[u+1][0].r,pointsMatrix[u+1][0].g,pointsMatrix[u+1][0].b);
                    glVertex3f(pointsMatrix[u+1][0].x,pointsMatrix[u+1][0].y,pointsMatrix[u+1][0].z);
                    glColor3f(pointsMatrix[u][v].r, pointsMatrix[u][v].g, pointsMatrix[u][v].b);
                    glVertex3f(pointsMatrix[u][v].x, pointsMatrix[u][v].y, pointsMatrix[u][v].z);
                }
                break;
            }
            for(int v=0;v<density;v++){
                int nextV = (v + 1) % density;
                glColor3f(pointsMatrix[u][v].r,pointsMatrix[u][v].g,pointsMatrix[u][v].b);
                glVertex3f(pointsMatrix[u][v].x,pointsMatrix[u][v].y,pointsMatrix[u][v].z);
                glColor3f(pointsMatrix[u+1][v].r, pointsMatrix[u+1][v].g, pointsMatrix[u+1][v].b);
                glVertex3f(pointsMatrix[u+1][v].x, pointsMatrix[u+1][v].y, pointsMatrix[u+1][v].z);
                
                glColor3f(pointsMatrix[u][v].r,pointsMatrix[u][v].g,pointsMatrix[u][v].b);
                glVertex3f(pointsMatrix[u][v].x,pointsMatrix[u][v].y,pointsMatrix[u][v].z);
                glColor3f(pointsMatrix[u][nextV].r, pointsMatrix[u][nextV].g, pointsMatrix[u][nextV].b);
                glVertex3f(pointsMatrix[u][nextV].x, pointsMatrix[u][nextV].y, pointsMatrix[u][nextV].z);
                
            }
        }
        glEnd();
        break;
    case 3:
        glBegin(GL_TRIANGLES);
        for(int u=0;u<density-1;u++){
            //Obecnie trójkąty są CCW
            if(u==0){
                for(int v=0;v<density;v++){
                    int nextV = (v + 1) % density;
                    glColor3f(pointsMatrix[u][0].r,pointsMatrix[u][0].g,pointsMatrix[u][0].b);
                    glVertex3f(pointsMatrix[u][0].x,pointsMatrix[u][0].y,pointsMatrix[u][0].z);
                    glColor3f(pointsMatrix[u+1][nextV].r,pointsMatrix[u+1][nextV].g,pointsMatrix[u+1][nextV].b);
                    glVertex3f(pointsMatrix[u+1][nextV].x,pointsMatrix[u+1][nextV].y,pointsMatrix[u+1][nextV].z);
                    glColor3f(pointsMatrix[u+1][v].r,pointsMatrix[u+1][v].g,pointsMatrix[u+1][v].b);
                    glVertex3f(pointsMatrix[u+1][v].x,pointsMatrix[u+1][v].y,pointsMatrix[u+1][v].z);
                }
                continue;
            }
            if(u==density-2){
                for(int v=0;v<density;v++){
                    int nextV = (v + 1) % density;
                    glColor3f(pointsMatrix[u+1][0].r,pointsMatrix[u+1][0].g,pointsMatrix[u+1][0].b);
                    glVertex3f(pointsMatrix[u+1][0].x,pointsMatrix[u+1][0].y,pointsMatrix[u+1][0].z);
                    glColor3f(pointsMatrix[u][v].r,pointsMatrix[u][v].g,pointsMatrix[u][v].b);
                    glVertex3f(pointsMatrix[u][v].x,pointsMatrix[u][v].y,pointsMatrix[u][v].z);
                    glColor3f(pointsMatrix[u][nextV].r,pointsMatrix[u][nextV].g,pointsMatrix[u][nextV].b);
                    glVertex3f(pointsMatrix[u][nextV].x,pointsMatrix[u][nextV].y,pointsMatrix[u][nextV].z);
                }
                break;
            }
            for(int v=0;v<density;v++){
                int nextV = (v + 1) % density;
                glColor3f(pointsMatrix[u][v].r,pointsMatrix[u][v].g,pointsMatrix[u][v].b);
                glVertex3f(pointsMatrix[u][v].x,pointsMatrix[u][v].y,pointsMatrix[u][v].z);
                glColor3f(pointsMatrix[u+1][nextV].r,pointsMatrix[u+1][nextV].g,pointsMatrix[u+1][nextV].b);
                glVertex3f(pointsMatrix[u+1][nextV].x, pointsMatrix[u+1][nextV].y, pointsMatrix[u+1][nextV].z);
                glColor3f(pointsMatrix[u+1][v].r,pointsMatrix[u+1][v].g,pointsMatrix[u+1][v].b);
                glVertex3f(pointsMatrix[u+1][v].x, pointsMatrix[u+1][v].y, pointsMatrix[u+1][v].z);
                
                glColor3f(pointsMatrix[u+1][nextV].r,pointsMatrix[u+1][nextV].g,pointsMatrix[u+1][nextV].b);
                glVertex3f(pointsMatrix[u+1][nextV].x, pointsMatrix[u+1][nextV].y, pointsMatrix[u+1][nextV].z);	
                glColor3f(pointsMatrix[u][v].r,pointsMatrix[u][v].g,pointsMatrix[u][v].b);
                glVertex3f(pointsMatrix[u][v].x,pointsMatrix[u][v].y,pointsMatrix[u][v].z);
                glColor3f(pointsMatrix[u][nextV].r,pointsMatrix[u][nextV].g,pointsMatrix[u][nextV].b);
                glVertex3f(pointsMatrix[u][nextV].x, pointsMatrix[u][nextV].y, pointsMatrix[u][nextV].z);
            }
        }
        glEnd();
        break;
    }
}
//Setters
void Egg::setDensity(int newDensity){
    density = newDensity;
    pointsMatrix.resize(density,vector<pointsRgb>(density));
    generateMatrix();
}
void Egg::setColor(float newColor){color = newColor;}
void Egg::setScale(float newScale){scale = newScale;}
void Egg::setPointSize(float newPointSize){pointSize = newPointSize;}
//Getters
int Egg::getDensity(){return density;}
float Egg::getColor(){return color;}
float Egg::getScale(){return scale;}
float Egg::getPointSize(){return pointSize;}
Egg::~Egg(){

}