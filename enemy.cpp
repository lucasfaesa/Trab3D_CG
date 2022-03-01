#include "enemy.h"
#include "enemyTiro.h"
#include "tinyxml2.h"
#include <cmath>
#include <iostream>
#include <cstdlib>

float enemyCabecaRadius = 1;
float enemyTroncoHeight  = 3.8;
float enemyTroncoWidth  = 2;
float enemyBracoHeight  = 3;
float enemyBracoWidth  = 0.5;
float enemyPernaHeight  = 1.8;
float enemyPernaWidth  = 0.5;
float testzz;
int contEnemies = 0;
bool enemyRotatePernaE1Backwards[7] = {};
bool enemyRotatePernaD1Backwards[7] = {};

void Enemy::DesenhaRect(GLfloat height, GLfloat width, GLfloat R, GLfloat G, GLfloat B,GLfloat A)
{
    glBegin(GL_QUADS);
    glColor4f(R,G,B,A);// Face posterior
    glVertex3f(width/2, height, 1.0);
    glVertex3f(-width/2, height, 1.0);
    glVertex3f(-width/2, 0.0, 1.0);
    glVertex3f(width/2, 0.0, 1.0);
    glEnd();

}
void Enemy::DesenhaCabeca(GLint index, GLfloat x, GLfloat y, GLfloat radius, GLfloat R, GLfloat G, GLfloat B)
{
    glPushMatrix();
    glTranslatef(x,y,0);
    if(enemyFacingRight[index])
        glScalef(1,1,1);
    else
        glScalef(-1,1,1);

    glColor3f(R,G,B);
    glBegin(GL_POLYGON);
    for(int i=0; i<20; i++){
        double angle = i *(2.0 * M_PI/20);
        glVertex2f(radius*cos(angle), radius*sin(angle));
    }

    glEnd();
    glPopMatrix();
}

void Enemy::DesenhaBraco(GLint index, GLfloat x, GLfloat y, GLfloat theta1)
{
    glPushMatrix();
    glTranslatef(x,y,0);
    glRotatef(theta1,0,0,1);
    /*if(enemyFacingRight[index])
        glScalef(1,1,1);
    else
        glScalef(-1,1,1);*/

    DesenhaRect(enemyBracoHeight,enemyBracoWidth,1,1,0,1);
    glPopMatrix();
}

void Enemy::DesenhaPerna(GLint index, GLfloat x, GLfloat y, GLfloat pEtheta1, GLfloat pEtheta2, GLfloat pDtheta1, GLfloat pDtheta2)
{
    glPushMatrix();
    glTranslatef(x,y,0);

    if(enemyFacingRight[index]){ //TODO TROCAR PARA LOOP FOR
        if(enemyFacingRight[index])
            glScalef(1,1,1);
        else
            glScalef(-1,1,1);
        glRotatef(pEtheta1,0,0,1);
        DesenhaRect(-enemyPernaHeight,enemyPernaWidth,0,1,0,1); //desenhando primeira perna esquerda
        glTranslatef(0,-enemyPernaHeight,0);
        glRotatef(pEtheta2,0,0,1);
        DesenhaRect(-enemyPernaHeight,enemyPernaWidth,1,1,0,1); //desenhando segunda perna esquerda
        glPopMatrix();

        glPushMatrix();
        glTranslatef(x,y,0);
        if(enemyFacingRight[index])
            glScalef(1,1,1);
        else
            glScalef(-1,1,1);
        glRotatef(pDtheta1,0,0,1);
        DesenhaRect(-enemyPernaHeight,enemyPernaWidth,1,0,0,1); //desenhando primeira perna direita
        glTranslatef(0,-enemyPernaHeight,0);
        glRotatef(pDtheta2,0,0,1);
        DesenhaRect(-enemyPernaHeight,enemyPernaWidth,1,1,1,1); //desenhando segunda perna direita*/
        glPopMatrix();
    }else{
        glPushMatrix();
        glTranslatef(x,y,0);
        if(enemyFacingRight[index])
            glScalef(1,1,1);
        else
            glScalef(-1,1,1);
        glRotatef(pDtheta1,0,0,1);
        DesenhaRect(-enemyPernaHeight,enemyPernaWidth,1,0,0,1); //desenhando primeira perna direita
        glTranslatef(0,-enemyPernaHeight,0);
        glRotatef(pDtheta2,0,0,1);
        DesenhaRect(-enemyPernaHeight,enemyPernaWidth,1,1,1,1); //desenhando segunda perna direita*/
        glPopMatrix();

        if(enemyFacingRight[index])
            glScalef(1,1,1);
        else
            glScalef(-1,1,1);
        glRotatef(pEtheta1,0,0,1);
        DesenhaRect(-enemyPernaHeight,enemyPernaWidth,0,1,0,1); //desenhando primeira perna esquerda
        glTranslatef(0,-enemyPernaHeight,0);
        glRotatef(pEtheta2,0,0,1);
        DesenhaRect(-enemyPernaHeight,enemyPernaWidth,1,1,0,1); //desenhando segunda perna esquerda
        glPopMatrix();
    }

}

void Enemy::DesenhaEnemy(GLint index ,GLfloat x, GLfloat y, GLfloat bTheta, GLfloat pETheta1, GLfloat pETheta2, GLfloat pDTheta1, GLfloat pDTheta2)
{
    glLoadIdentity();
    glFlush();

    glPushMatrix();
    glTranslatef(x,y,0);
    /*if(enemyFacingRight[index])
        glScalef(1,1,1);
    else
        glScalef(-1,1,1);*/
    DesenhaRect(enemyTroncoHeight,enemyTroncoWidth,1,0,0,1); //desenhando base
    DesenhaCabeca(index, 0, enemyTroncoHeight + enemyCabecaRadius /*offset*/, enemyCabecaRadius, 1, 0, 0);
    DesenhaBraco(index, 0,enemyTroncoHeight/2, bTheta);
    DesenhaPerna(index, 0, 0,pETheta1,pETheta2, pDTheta1, pDTheta2);

    glPopMatrix();
}

enemyTiro* Enemy::Atira(int index) {

    float angleSumTheta1 = (enemiesObj[index].bTheta) * M_PI / 180;

     //if(enemyFacingRight[0]) {
        float bulletX = enemiesObj[index].gX - enemyBracoHeight * sin(angleSumTheta1);
        float bulletY = enemiesObj[index].gY + enemyTroncoHeight / 2 + enemyBracoHeight * cos(angleSumTheta1);
        return new enemyTiro(bulletX, bulletY, 90 + enemiesObj[index].bTheta, enemyFacingRight[index]);
    /*}
    else{
        float bulletX = enemiesObj[index].gX + enemyBracoHeight * sin(angleSumTheta1) ;
        float bulletY = enemiesObj[index].gY + enemyTroncoHeight/2 + enemyBracoHeight * cos(angleSumTheta1);
        return new enemyTiro(bulletX, bulletY, -90 + enemiesObj[index].bTheta, enemyFacingRight[index]);
    }*/

}

void Enemy::GetEnemiesFromSvg() {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(enemyImagePath);

    tinyxml2::XMLElement *levelElement = doc.FirstChildElement("svg");

    for (tinyxml2::XMLElement *child = levelElement->FirstChildElement("circle");
         child != NULL; child = child->NextSiblingElement("circle")) {
        const char *pAttrX = child->Attribute("cx");
        const char *pAttrY = child->Attribute("cy");
        const char *pAttrR = child->Attribute("r");
        const char *pAttrFill = child->Attribute("fill");

        float iX = strtof(pAttrX, NULL);
        float iY = strtof(pAttrY, NULL);
        float iR = strtof(pAttrR, NULL);
        std::string color = child->Attribute("fill");

        if(color == "green")
            continue;

        //DesenhaEnemy(contEnemies, iX, -iY - 1.5 /*offset*/, -90, 30, 0, -30, 0);

        Enemy::AddEnemiesToArray(contEnemies, iX, -iY, -90, 30, 0, -30, 0);

        contEnemies++;
    }
    contEnemies = 0;
}
void Enemy::DesenhaTodos() {

    for(int i =0; i<sizeof(enemiesObj)/sizeof(enemiesObj[0]); i++){
        if(enemiesObj[i].canBeDrawn)
            DesenhaEnemy(i,enemiesObj[i].gX,enemiesObj[i].gY,enemiesObj[i].bTheta,enemiesObj[i].pETheta1,enemiesObj[i].pETheta2,enemiesObj[i].pDTheta1,enemiesObj[i].pDTheta2);
    }
}

void Enemy::SetEnemyVisibility(int index, bool status) {
    enemiesObj[index].canBeDrawn = status;
}

void Enemy::RodaBraco(int index, GLfloat playerX, GLfloat playerY)
{
    double dx = playerX - enemiesObj[index].gX;
    double dy = playerY - enemiesObj[index].gY;
    double ang = atan2(dy, dx) * (180 / M_PI);
    float armRot = ang;

    if(enemyFacingRight[index]){
        if(armRot > 45)
            armRot = 45;
        if(armRot < -45)
            armRot = -45;
        enemiesObj[index].bTheta = armRot - 90;
    }else{
        if(armRot < 135 && armRot > 0)
            armRot = 135;
        if(armRot > -135 && armRot < 0)
            armRot = -135;
        enemiesObj[index].bTheta = armRot - 90;
    }
}

void Enemy::RodaPernaE1(int index, GLfloat inc)
{
    float maxRot = 30;
    float minRot = -30;

    float speed = 100;
    if(enemiesObj[index].pETheta1 >= maxRot){
        if(enemyFacingRight[index])
            enemyRotatePernaE1Backwards[index] = false;
        else
            enemyRotatePernaE1Backwards[index] = true;
    }
    if(enemiesObj[index].pETheta1 <= minRot){
        if(enemyFacingRight[index])
            enemyRotatePernaE1Backwards[index] = true;
        else
            enemyRotatePernaE1Backwards[index] = false;
    }

    if(enemyRotatePernaE1Backwards[index]){
        enemiesObj[index].pETheta1 += inc * speed;
    }else{
        enemiesObj[index].pETheta1 -= inc * speed;
    }

}

void Enemy::RodaPernaE2(GLfloat inc)
{
}

void Enemy::RodaPernaD1(int index, GLfloat inc)
{
    float maxRot = 30;
    float minRot = -30;

    float speed = 100;
    if(enemiesObj[index].pDTheta1 >= maxRot){
        if(enemyFacingRight[index])
            enemyRotatePernaD1Backwards[index] = true;
        else
            enemyRotatePernaD1Backwards[index] = false;
    }
    if(enemiesObj[index].pDTheta1 <= minRot){
        if(enemyFacingRight[index])
            enemyRotatePernaD1Backwards[index] = false;
        else
            enemyRotatePernaD1Backwards[index] = true;
    }

    if(enemyRotatePernaD1Backwards[index]){
        enemiesObj[index].pDTheta1 -= inc * speed;
    }else{
        enemiesObj[index].pDTheta1 += inc * speed;
    }
}

void Enemy::RodaPernaD2(GLfloat inc)
{
}

void Enemy::MoveEmX(int index, GLfloat dx, GLfloat timeDifference)
{
    if(dx > 0){
        enemyFacingRight[index] = true;
    }
    if(dx < 0){
        enemyFacingRight[index] = false;
    }
    enemiesObj[index].gX += dx * timeDifference;
    //gX += dx * timeDifference;
}

/*void Enemy::MoveEmY(GLfloat dy, bool &isJumping)
{
    float speed = 25;
    enemymaxJumpHeight = ((CabecaRadius * 2 + troncoHeight + pernaHeight * 2) * 3);

    if(fabs(jumpUpDistanceTraveled) <= maxJumpHeight){
        gY += dy * speed;
        jumpUpDistanceTraveled += previousPosY - gY;
        isJumping = true;
    }else{
        isJumping = false;
    }
    previousPosY = gY;
}*/

void Enemy::FreeFall(int index, GLfloat dy)
{
    float speed = 25;
    enemiesObj[index].gY -= dy * speed;
}

bool Enemy::Atingido(int index, Tiro *tiro) {
    float tiroXPos = 0;
    float tiroYPos = 0;
    float tiroRadius = 0.5;

    tiro->GetPos(tiroXPos,tiroYPos);

    float enemySizeY = (enemyPernaHeight * 2) + (enemyCabecaRadius * 2) + enemyTroncoHeight;

    bool collisionX = enemiesObj[index].gX + enemyTroncoWidth/2 >= tiroXPos &&
                        tiroXPos + (tiroRadius * 2) >= enemiesObj[index].gX - enemyTroncoWidth/2;
                        //topo do inimigo
    bool collisionY = enemiesObj[index].gY + (enemyTroncoHeight + enemyCabecaRadius*2) >= tiroYPos &&
                      tiroYPos + (tiroRadius * 2) >= enemiesObj[index].gY - (enemyPernaHeight * 2);

    return collisionX && collisionY;
}

