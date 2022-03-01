#ifndef MAIN_CPP_ENEMY_H
#define MAIN_CPP_ENEMY_H
#include <GL/gl.h>
#include <GL/glu.h>
#include <cstring>
#include <cstdlib>
#include "enemyTiro.h"
#include "tiro.h"
#include <time.h>

static bool enemyFacingRight[7] = {};

class Enemies{
public:
    float gX;
    float gY;
    float bTheta = 0;
    float pETheta1 = 0;
    float pETheta2 = 0;
    float pDTheta1 = 0;
    float pDTheta2 = 0;
    bool canBeDrawn;
    float speed = 0;
};

class Enemy {
    GLfloat gX;
    GLfloat gY;
    GLfloat bTheta = 0;
    GLfloat pETheta1 = 0;
    GLfloat pETheta2 = 0;
    GLfloat pDTheta1 = 0;
    GLfloat pDTheta2 = 0;

private:
    void DesenhaRect(GLfloat height, GLfloat width, GLfloat R, GLfloat G, GLfloat B, GLfloat A);
    void DesenhaCabeca(GLint index, GLfloat x, GLfloat y, GLfloat radius, GLfloat R,GLfloat G, GLfloat B);
    void DesenhaPerna(GLint index, GLfloat x, GLfloat y, GLfloat theta1, GLfloat theta2, GLfloat theta3, GLfloat theta4);
    void DesenhaBraco(GLint index, GLfloat x, GLfloat y, GLfloat theta1);
    void GetEnemiesFromSvg();
    void DesenhaEnemy(GLint index, GLfloat x, GLfloat y, GLfloat bTheta, GLfloat pETheta1, GLfloat pETheta2, GLfloat pDTheta1, GLfloat pDTheta2);
    void AddEnemiesToArray(int index, GLfloat x, GLfloat y, GLfloat bTheta, GLfloat pETheta1, GLfloat pETheta2, GLfloat pDTheta1, GLfloat pDTheta2){
        enemiesObj[index].gX = x;
        enemiesObj[index].gY = y;
        enemiesObj[index].bTheta = bTheta;
        enemiesObj[index].pETheta1 = pETheta1;
        enemiesObj[index].pETheta2 = pETheta2;
        enemiesObj[index].pDTheta1 = pDTheta1;
        enemiesObj[index].pDTheta2 = pDTheta2;
        enemiesObj[index].canBeDrawn = true;
      //  srand (time(NULL));

        //enemiesObj[index].speed = 0.01;

        int randomNumber = rand() % 3;
        switch (randomNumber) {
            case 0:
                enemiesObj[index].speed = 0.01;
                break;
            case 1:
                enemiesObj[index].speed = 0.008;
                break;
            case 2:
                enemiesObj[index].speed = 0.006;
                break;
        }
    }
    void DesenhaTodos();

public:
    Enemies enemiesObj[7] = {};
    const char *enemyImagePath = "";

    void SetImagePath(const char* path){
        enemyImagePath = path;
    }

    void Desenha(int index, Enemies enemy){
        DesenhaTodos();
    };
    void GetFromSvg(){
        GetEnemiesFromSvg();
    }
    void GetEnemiesArray(Enemies (&x)[7]){
        memcpy(x, enemiesObj, sizeof(x));
    };

    void RodaBraco(int index, GLfloat playerX, GLfloat playerY);
    void RodaPernaE1(int index, GLfloat inc);
    void RodaPernaE2(GLfloat inc);
    void RodaPernaD1(int index, GLfloat inc);
    void RodaPernaD2(GLfloat inc);
    enemyTiro* Atira(int index);
    bool Atingido(int index, Tiro *tiro);
    void MoveEmX(int index, GLfloat dx, GLfloat timeDifference);
    //void MoveEmY(GLfloat dy, bool &isJumping);
    void FreeFall (GLint index, GLfloat dy);
    void GetPos(int index, GLfloat &xOut, GLfloat &yOut){
        xOut = enemiesObj[index].gX;
        yOut = enemiesObj[index].gY;
    };
    void SetEnemyVisibility(int index, bool status);


};


#endif //MAIN_CPP_ENEMY_H
