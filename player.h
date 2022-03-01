#ifndef MAIN_CPP_PLAYER_H
#define MAIN_CPP_PLAYER_H
#include <GL/gl.h>
#include <GL/glu.h>
#include "tiro.h"
#include "enemyTiro.h"

static bool facingRight = true;

class Player {
    GLfloat gX;
    GLfloat gY;
    GLfloat initialBTheta = 0;
    GLfloat bTheta = 0;
    GLfloat pETheta1 = 0;
    GLfloat pETheta2 = 0;
    GLfloat pDTheta1 = 0;
    GLfloat pDTheta2 = 0;


private:
void DesenhaRect(GLfloat height, GLfloat width, GLfloat R, GLfloat G, GLfloat B, GLfloat A);
void DesenhaCabeca(GLfloat x, GLfloat y, GLfloat radius, GLfloat R,GLfloat G, GLfloat B);
void DesenhaPerna(GLfloat x, GLfloat y, GLfloat theta1, GLfloat theta2, GLfloat theta3, GLfloat theta4);
void DesenhaBraco(GLfloat x, GLfloat y, GLfloat theta1);
void DesenhaCollider();
void DesenhaPlayer(GLfloat x, GLfloat y, GLfloat bTheta, GLfloat pETheta1, GLfloat pETheta2, GLfloat pDTheta1, GLfloat pDTheta2);

public:

    Player(){
        gX = -157;
        gY = -180;
        initialBTheta = 90;
        bTheta = -90;
        pETheta1 = 30;
        pETheta2 = 0;
        pDTheta1 = -30;
        pDTheta2 = 0;
    };
    void Desenha(){
        DesenhaPlayer(gX, gY, bTheta, pETheta1, pETheta2, pDTheta1, pDTheta2);
    };

    void RodaBraco(GLfloat inc);
    void RodaPernaE1(GLfloat inc);
    void RodaPernaE2(GLfloat inc);
    void RodaPernaD1(GLfloat inc);
    void RodaPernaD2(GLfloat inc);
    Tiro* Atira();
    bool Atingido(int index, enemyTiro *enemyTiro);
    void MoveEmX(GLfloat dx, GLfloat timeDifference);
    void MoveEmY(GLfloat dy, bool &isJumping);
    void FreeFall (GLfloat dy);
    void ResetJumpDistance();
    void ResetPlayerPos();
    void MoveEmMenosY(GLfloat dy, bool jumping);
    void GetPos(GLfloat &xOut, GLfloat &yOut){
        xOut = gX;
        yOut = gY;
    };
};
#endif //MAIN_CPP_PLAYER_H
