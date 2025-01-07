#include "Sun.h"
void Sun::initLight(){
    
}
void Sun::draw(u_int texture){
    glDisable(GL_LIGHTING);
    Planet::draw(texture);
    glEnable(GL_LIGHTING);
}