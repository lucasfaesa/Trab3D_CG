#ifndef MAIN_CPP_ENEMYTIRO_H
#define MAIN_CPP_ENEMYTIRO_H
#include <GL/gl.h>
#include <GL/glu.h>


class enemyTiro {
    GLfloat gXInit;
    GLfloat gYInit;
    GLfloat gX;
    GLfloat gY;
    GLfloat gDirectionAng;
    GLfloat gVel;
    GLboolean gFireRightDirection = true;
private:
    void DesenhaCirc(GLfloat radius, GLfloat R, GLfloat G, GLfloat B);
    void DesenhaTiro(GLfloat x, GLfloat y);
public:
    enemyTiro(GLfloat x, GLfloat y, GLfloat directionAng, GLboolean fireDirection ){
        gXInit = x;
        gYInit = y;
        gX = x;
        gY = y;
        gDirectionAng = directionAng;
        gVel = 0.04; //dobro da velocidade do player
        gFireRightDirection = fireDirection;
    };
    void Desenha(){
        DesenhaTiro(gX, gY);
    };
    void Move(double timeDifference);
    bool Valido();
    void GetPos(GLfloat &xOut, GLfloat &yOut){
        xOut = gX;
        yOut = gY;
    };
};


#endif //MAIN_CPP_ENEMYTIRO_H
