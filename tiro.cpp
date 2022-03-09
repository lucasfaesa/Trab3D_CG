#include "tiro.h"
#include "player.h"
#include <math.h>
#include <iostream>
#include <GL/glut.h>

#define DISTANCIA_MAX 50

void Tiro::DesenhaCirc(GLfloat radius, GLfloat R, GLfloat G, GLfloat B)
{
    glColor3f(R,G,B);

    /*glBegin(GL_POLYGON);
    for(int i=0; i<20; i++){
        double angle = i *(2.0 * M_PI/20);
        glVertex2f(radius*cos(angle), radius*sin(angle));
    }
    glEnd();*/
    //glColor3f(0,0,0);
    glutSolidSphere(radius,20,10);
}

void Tiro::DesenhaTiro(GLfloat x, GLfloat y)
{
    glPushMatrix();
   // std::cout << "gX= " << x << " " << "gY= " << y << std::endl;
    glTranslatef(x,y,25); //TODO MODIFICAR PARA ESTAR SEMPRE NO Z DO BRAÇO
    DesenhaCirc(0.5,1,1,1);
    glPopMatrix();
}

void Tiro::Move(double timeDifference)
{
    gX += gVel * timeDifference * cos(gDirectionAng*M_PI/180);
    if(gFireRightDirection){
        gY += gVel * timeDifference * sin(gDirectionAng*M_PI/180);
    }else{
        gY -= gVel * timeDifference * sin(gDirectionAng*M_PI/180);
    }


}

bool Tiro::Valido()
{
    if(abs(gX - gXInit) >= DISTANCIA_MAX || abs(gY - gYInit) >= DISTANCIA_MAX)
        return false;
    else
        return true;
}
