#include <math.h>
#include <GL/glu.h>
#include <iostream>
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
void Egg::generateMatrix(){	
    for(int u=0;u<(density);u++){
        float _u = 0.5/((float)density-1);
        _u *= u;
        for(int v=0;v<density;v++){
            float _v = v/((float)density);
            _v *= 2.0f;
            pointsMatrix[u][v].x = scale*((-90*pow(_u,5) + 225*pow(_u,4) - 270*pow(_u,3) + 180*pow(_u,2) - 45*_u) * cos(M_PI*_v));
            pointsMatrix[u][v].y = scale*(160*pow(_u,4) - 320*pow(_u,3) + 160 * pow(_u,2) - 5);
            pointsMatrix[u][v].z = scale*((-90*pow(_u,5) + 225*pow(_u,4) - 270*pow(_u,3) + 180*pow(_u,2) - 45*_u) * sin(M_PI*_v));
            //Białe jajko
            pointsMatrix[u][v].r = 1.0f;
            pointsMatrix[u][v].g = 1.0f;
            pointsMatrix[u][v].b = 1.0f;
            pointsMatrix[u][v].u = ((float)u/(density));
            pointsMatrix[u][v].v = 2.0f * abs(((float)v / density) - 0.5f);
        }
    }
}
void Egg::draw(){
    glBegin(GL_TRIANGLES);
    for(int u=0;u<density-1;u++){
        //Obecnie trójkąty są CCW
        if(u==0){
            for(int v=0;v<density;v++){
                int nextV = (v + 1) % density;
                glTexCoord2f(pointsMatrix[u+1][nextV].u,pointsMatrix[u+1][nextV].v);
                glColor3f(pointsMatrix[u+1][nextV].r,pointsMatrix[u+1][nextV].g,pointsMatrix[u+1][nextV].b);
                glVertex3f(pointsMatrix[u+1][nextV].x,pointsMatrix[u+1][nextV].y,pointsMatrix[u+1][nextV].z);
                
                glTexCoord2f(pointsMatrix[u+1][v].u,pointsMatrix[u+1][v].v);
                glColor3f(pointsMatrix[u+1][v].r,pointsMatrix[u+1][v].g,pointsMatrix[u+1][v].b);
                glVertex3f(pointsMatrix[u+1][v].x,pointsMatrix[u+1][v].y,pointsMatrix[u+1][v].z);
                
                glTexCoord2f(pointsMatrix[u][nextV].u,pointsMatrix[u][nextV].v);
                glColor3f(pointsMatrix[u][0].r,pointsMatrix[u][0].g,pointsMatrix[u][0].b);
                glVertex3f(pointsMatrix[u][0].x,pointsMatrix[u][0].y,pointsMatrix[u][0].z);
                
            }
            continue;
        }
        if(u==density-2){
            for(int v=0;v<density;v++){
                int nextV = (v + 1) % density;
                glTexCoord2f(pointsMatrix[u+1][nextV].u,pointsMatrix[u+1][nextV].v);
                glColor3f(pointsMatrix[u+1][0].r,pointsMatrix[u+1][0].g,pointsMatrix[u+1][0].b);
                glVertex3f(pointsMatrix[u+1][0].x,pointsMatrix[u+1][0].y,pointsMatrix[u+1][0].z);
                
                glTexCoord2f(pointsMatrix[u][v].u,pointsMatrix[u][v].v);
                glColor3f(pointsMatrix[u][v].r,pointsMatrix[u][v].g,pointsMatrix[u][v].b);
                glVertex3f(pointsMatrix[u][v].x,pointsMatrix[u][v].y,pointsMatrix[u][v].z);
                
                glTexCoord2f(pointsMatrix[u][nextV].u,pointsMatrix[u][nextV].v);
                glColor3f(pointsMatrix[u][nextV].r,pointsMatrix[u][nextV].g,pointsMatrix[u][nextV].b);
                glVertex3f(pointsMatrix[u][nextV].x,pointsMatrix[u][nextV].y,pointsMatrix[u][nextV].z);
            }
            break;
        }
        for(int v=0;v<density;v++){
            int nextV = (v + 1) % density;
            //Pierwszy trójkąt
            glTexCoord2f(pointsMatrix[u][v].u,pointsMatrix[u][v].v);
            glColor3f(pointsMatrix[u][v].r,pointsMatrix[u][v].g,pointsMatrix[u][v].b);
            glVertex3f(pointsMatrix[u][v].x,pointsMatrix[u][v].y,pointsMatrix[u][v].z);
            if(nextV!=0){
                glTexCoord2f(pointsMatrix[u+1][nextV].u,pointsMatrix[u+1][nextV].v);
            }else{
                glTexCoord2f(pointsMatrix[u+1][nextV].u,1);
            }
            glColor3f(pointsMatrix[u+1][nextV].r,pointsMatrix[u+1][nextV].g,pointsMatrix[u+1][nextV].b);
            glVertex3f(pointsMatrix[u+1][nextV].x, pointsMatrix[u+1][nextV].y, pointsMatrix[u+1][nextV].z);
            glTexCoord2f(pointsMatrix[u+1][v].u,pointsMatrix[u+1][v].v);
            glColor3f(pointsMatrix[u+1][v].r,pointsMatrix[u+1][v].g,pointsMatrix[u+1][v].b);
            glVertex3f(pointsMatrix[u+1][v].x, pointsMatrix[u+1][v].y, pointsMatrix[u+1][v].z);
            //Drugi trójkąt
            if(nextV!=0){
                glTexCoord2f(pointsMatrix[u+1][nextV].u,pointsMatrix[u+1][nextV].v);
            }else{
                glTexCoord2f(pointsMatrix[u+1][nextV].u,1);
            }
            glColor3f(pointsMatrix[u+1][nextV].r,pointsMatrix[u+1][nextV].g,pointsMatrix[u+1][nextV].b);
            glVertex3f(pointsMatrix[u+1][nextV].x, pointsMatrix[u+1][nextV].y, pointsMatrix[u+1][nextV].z);	
            glTexCoord2f(pointsMatrix[u][v].u,pointsMatrix[u][v].v);
            glColor3f(pointsMatrix[u][v].r,pointsMatrix[u][v].g,pointsMatrix[u][v].b);
            glVertex3f(pointsMatrix[u][v].x,pointsMatrix[u][v].y,pointsMatrix[u][v].z);
            if(nextV!=0){
                glTexCoord2f(pointsMatrix[u][nextV].u,pointsMatrix[u][nextV].v);
            }else{
                glTexCoord2f(pointsMatrix[u][nextV].u,1);
            }
            glColor3f(pointsMatrix[u][nextV].r,pointsMatrix[u][nextV].g,pointsMatrix[u][nextV].b);
            glVertex3f(pointsMatrix[u][nextV].x, pointsMatrix[u][nextV].y, pointsMatrix[u][nextV].z);         
        }
    }
    glEnd();
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