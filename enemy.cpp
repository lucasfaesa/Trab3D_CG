#include "enemy.h"
#include "enemyTiro.h"
#include "tinyxml2.h"
#include <cmath>
#include <iostream>
#include <cstdlib>

typedef struct
{
    //Vertex coordinate
    double X;
    double Y;
    double Z;

    //Vertex normal
    double nX;
    double nY;
    double nZ;

    //Vertex texture coordinate
    double U;
    double V;
} VERTICES;

typedef struct
{
    VERTICES * vtx;
    int numVtx;
    double radius;
} OBJ;

OBJ* objEnemyHead;

float enemyCabecaRadius = 1;
float enemyTroncoHeight  = 3.8;
float enemyTroncoWidth  = 2;
float enemyTroncoExtrusion = 1.5;
float enemyBracoHeight  = 3;
float enemyBracoWidth  = 0.5;
float enemyBracoExtrusion = 0.5;
float enemyPernaHeight  = 1.8;
float enemyPernaWidth  = 0.5;
float enemyPernaExtrusion = 0.5;

int contEnemies = 0;
bool enemyRotatePernaE1Backwards[7] = {};
bool enemyRotatePernaD1Backwards[7] = {};

OBJ *CreateEnemySphere(double R, double space);

void Enemy::DesenhaRect(GLfloat height, GLfloat width, GLfloat extrusion, GLfloat R, GLfloat G, GLfloat B, GLfloat A, GLuint texture)
{
    /*glBegin(GL_QUADS);
    glColor4f(R,G,B,A);// Face posterior
    glVertex3f(width/2, height, 1.0);
    glVertex3f(-width/2, height, 1.0);
    glVertex3f(-width/2, 0.0, 1.0);
    glVertex3f(width/2, 0.0, 1.0);
    glEnd();*/

    GLfloat materialEmission[] = { 0.10, 0.10, 0.10, 1};
    GLfloat materialColorA[] = { 0.2, 0.2, 0.2, 1};
    GLfloat materialColorD[] = { 1.0, 1.0, 1.0, 1};
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1};
    GLfloat mat_shininess[] = { 100.0 };

    glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialColorA);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColorD);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT  );//X
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );//Y
    glBindTexture (GL_TEXTURE_2D, texture);

    double textureS = 1;

    glBegin(GL_QUADS);
    //glColor3f(R,G,B);// Face posterior
        glNormal3f(0.0, 0.0, 1.0);	// Normal da face
        glTexCoord2f (0, textureS);
        glVertex3f(width/2, height, extrusion/2);
        glTexCoord2f (textureS, textureS);
        glVertex3f(-width/2, height, extrusion/2);
        glTexCoord2f (textureS, 0);
        glVertex3f(-width/2, 0.0, extrusion/2);
        glTexCoord2f (0, 0);
        glVertex3f(width/2, 0.0, extrusion/2);
    glEnd();

    glBegin(GL_QUADS);
    //glColor3f(R,G,B);// Face anterior
        glNormal3f(0.0, 0.0, -1.0);	// Normal da face
        glTexCoord2f (textureS, textureS);
        glVertex3f(width/2, height, -extrusion/2);
        glTexCoord2f (textureS, 0);
        glVertex3f(width/2, 0, -extrusion/2);
        glTexCoord2f (0, 0);
        glVertex3f(-width/2, 0.0, -extrusion/2);
        glTexCoord2f (0, textureS);
        glVertex3f(-width/2, height, -extrusion/2);
    glEnd();

    glBegin(GL_QUADS);
    //glColor3f(R,G,B);// Face lateral esquerda
        glNormal3f(-1.0, 0.0, 0.0);	// Normal da face
        glTexCoord2f (0, textureS);
        glVertex3f(-width/2, height, extrusion/2);
        glTexCoord2f (textureS, textureS);
        glVertex3f(-width/2, height, -extrusion/2);
        glTexCoord2f (textureS, 0);
        glVertex3f(-width/2, 0.0, -extrusion/2);
        glTexCoord2f (0, 0);
        glVertex3f(-width/2, 0, extrusion/2);
    glEnd();

    glBegin(GL_QUADS);
    //glColor3f(R,G,B);// Face lateral direita
        glNormal3f(1.0, 0.0, 0.0);	// Normal da face
        glTexCoord2f (textureS, textureS);
        glVertex3f(width/2, height, extrusion/2);
        glTexCoord2f (textureS, 0);
        glVertex3f(width/2, 0, extrusion/2);
        glTexCoord2f (0, 0);
        glVertex3f(width/2, 0, -extrusion/2);
        glTexCoord2f (0, textureS);
        glVertex3f(width/2, height, -extrusion/2);
    glEnd();

    glBegin(GL_QUADS);
    //glColor3f(R,G,B);// Face superior
        glNormal3f(0.0, 1.0, 0.0);	// Normal da face
        glTexCoord2f (0, textureS);
        glVertex3f(-width/2, height, -extrusion/2);
        glTexCoord2f (0, 0);
        glTexCoord2f (0, textureS);
        glVertex3f(-width/2, height, extrusion/2);
        glTexCoord2f (textureS, 0);
        glVertex3f(width/2, height, extrusion/2);
        glTexCoord2f (textureS, textureS);
        glVertex3f(width/2, height, -extrusion/2);
    glEnd();

    glBegin(GL_QUADS);
    //glColor3f(R,G,B);// Face inferior
        glNormal3f(0.0, -1.0, 0.0);	// Normal da face
        glTexCoord2f (0, textureS);
        glVertex3f(-width/2, 0, -extrusion/2);
        glTexCoord2f (0, textureS);
        glVertex3f(width/2, 0, -extrusion/2);
        glTexCoord2f (textureS, 0);
        glVertex3f(width/2, 0, extrusion/2);
        glTexCoord2f (textureS, textureS);
        glVertex3f(-width/2, 0, extrusion/2);
    glEnd();

}
void Enemy::DesenhaCabeca(GLint index, GLfloat x, GLfloat y, GLfloat radius, GLfloat R,GLfloat G, GLfloat B,GLuint textureHead)
{
    glPushMatrix();
    glTranslatef(x,y,0);
    glRotatef(90,1,0,0);
    glRotatef(45,0,0,1);

    if(enemyFacingRight[index]){
        glScalef(-1,1,1);
        glRotatef(-90,0,0,1);
    }

    GLfloat materialEmission[] = { 0.00, 0.00, 0.00, 1};
    GLfloat materialColorA[] = { 0.2, 0.2, 0.2, 1};
    GLfloat materialColorD[] = { 1.0, 1.0, 1.0, 1};
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1};
    GLfloat mat_shininess[] = { 100.0 };
    glColor3f(1,1,1);

    glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialColorA);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColorD);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glBindTexture (GL_TEXTURE_2D, textureHead);
    glBegin (GL_TRIANGLE_STRIP);
    for ( int i = 0; i <objEnemyHead->numVtx; i++)
    {
        glNormal3f(objEnemyHead->vtx[i].nX, objEnemyHead->vtx[i].nY, objEnemyHead->vtx[i].nZ);
        glTexCoord2f (objEnemyHead->vtx[i].U, objEnemyHead->vtx[i].V);
        glVertex3f (objEnemyHead->vtx[i].X, objEnemyHead->vtx[i].Y, objEnemyHead->vtx[i].Z);
    }
    glEnd();

    glPopMatrix();
}

void Enemy::DesenhaBraco(GLint index, GLfloat x, GLfloat y, GLfloat theta1, GLuint textureArm)
{
    glPushMatrix();
    if(enemyFacingRight[index])
        glScalef(1,1,1);
    else
        glScalef(-1,1,1);
    glTranslatef(x,y,0);
    glRotatef(theta1,0,0,1);
    DesenhaRect(enemyBracoHeight,enemyBracoWidth,enemyBracoExtrusion,0,0,0,1,textureArm);
    glPopMatrix();
}

void Enemy::DesenhaPerna(GLint index, GLfloat x, GLfloat y, GLfloat pEtheta1, GLfloat pEtheta2, GLfloat pDtheta1, GLfloat pDtheta2, GLuint textureLeg)
{
    if(enemyFacingRight[index]){ //TODO TROCAR PARA LOOP FOR
        glPushMatrix();
            glTranslatef(x,y,0.3);
            glScalef(-1,1,1);
            glRotatef(pEtheta1,0,0,1);
            DesenhaRect(-enemyPernaHeight,enemyPernaWidth,enemyPernaExtrusion,0,1,0,1,textureLeg); //desenhando primeira perna esquerda
            glTranslatef(0,-enemyPernaHeight,0);
            glRotatef(pEtheta2,0,0,1);
            DesenhaRect(-enemyPernaHeight,enemyPernaWidth,enemyPernaExtrusion,1,1,0,1,textureLeg); //desenhando segunda perna esquerda
        glPopMatrix();

        glPushMatrix();
            glTranslatef(x,y,-0.3);
            glScalef(-1,1,1);
            glRotatef(pDtheta1,0,0,1);
            DesenhaRect(-enemyPernaHeight,enemyPernaWidth,enemyPernaExtrusion,1,0,0,1,textureLeg); //desenhando primeira perna direita
            glTranslatef(0,-enemyPernaHeight,0);
            glRotatef(pDtheta2,0,0,1);
            DesenhaRect(-enemyPernaHeight,enemyPernaWidth,enemyPernaExtrusion,1,1,1,1,textureLeg); //desenhando segunda perna direita*/
        glPopMatrix();
    }else{
        glPushMatrix();
            glTranslatef(x,y,0.3);
            glRotatef(pDtheta1,0,0,1);
            DesenhaRect(-enemyPernaHeight,enemyPernaWidth,enemyPernaExtrusion,1,0,0,1,textureLeg); //desenhando primeira perna direita
            glTranslatef(0,-enemyPernaHeight,0);
            glRotatef(pDtheta2,0,0,1);
            DesenhaRect(-enemyPernaHeight,enemyPernaWidth,enemyPernaExtrusion,1,1,1,1,textureLeg); //desenhando segunda perna direita*/
        glPopMatrix();

        glPushMatrix();
            glTranslatef(x,y,-0.3);
            glRotatef(pEtheta1,0,0,1);
            DesenhaRect(-enemyPernaHeight,enemyPernaWidth,enemyPernaExtrusion,0,1,0,1,textureLeg); //desenhando primeira perna esquerda
            glTranslatef(0,-enemyPernaHeight,0);
            glRotatef(pEtheta2,0,0,1);
            DesenhaRect(-enemyPernaHeight,enemyPernaWidth,enemyPernaExtrusion,1,1,0,1,textureLeg); //desenhando segunda perna esquerda
        glPopMatrix();
    }
}

void Enemy::DesenhaEnemy(GLint index ,GLfloat x, GLfloat y, GLfloat bTheta, GLfloat pETheta1, GLfloat pETheta2, GLfloat pDTheta1, GLfloat pDTheta2,
                         GLuint textureChest, GLuint textureArm, GLuint textureLeg, GLuint textureHead)
{
   glPushMatrix();
        glTranslatef(x,y,25);
        if(enemyFacingRight[index])
            glScalef(1,1,1);
        else
            glScalef(-1,1,1);

        glPushMatrix();
            if(!enemyFacingRight[index]) glScalef(-1,1,1);
            DesenhaRect(enemyTroncoHeight,enemyTroncoWidth, enemyTroncoExtrusion, 0,1,0.3,1,textureChest); //desenhando base
        glPopMatrix();
        DesenhaCabeca(index, 0, enemyTroncoHeight + enemyCabecaRadius, enemyCabecaRadius, 0, 1, 0.3,textureHead);
        DesenhaBraco(index, 0,enemyTroncoHeight/2,bTheta,textureArm);
        DesenhaPerna(index, 0, 0,pETheta1,pETheta2, pDTheta1, pDTheta2,textureLeg);

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
void Enemy::DesenhaTodos(GLuint textureChest, GLuint textureArm, GLuint textureLeg, GLuint textureHead) {

    for(int i =0; i<sizeof(enemiesObj)/sizeof(enemiesObj[0]); i++){
        if(enemiesObj[i].canBeDrawn)
            DesenhaEnemy(i,enemiesObj[i].gX,enemiesObj[i].gY,enemiesObj[i].bTheta,enemiesObj[i].pETheta1,enemiesObj[i].pETheta2,enemiesObj[i].pDTheta1,enemiesObj[i].pDTheta2,
                         textureChest, textureArm, textureLeg, textureHead);
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

void Enemy::CreateEnemyHead(){
    objEnemyHead = CreateEnemySphere(enemyCabecaRadius, 10);
}

OBJ *CreateEnemySphere(double R, double space) {
    OBJ *obj = new OBJ;

    obj->numVtx = (180 / space) *
                  (2 + 360 / (2*space)) * 4;
    obj->vtx = new VERTICES[ obj->numVtx ];
    obj->radius = R;

    int n;
    double vR, lVR;
    double hR, lHR;
    double norm;
    n = 0;
    for( vR = 0; vR <= 180-space; vR+=space){
        for(hR = 0; hR <= 360+2*space; hR+=2*space)
        {
            lVR = vR;
            lHR = hR;
            obj->vtx[n].X = R *
                            sin(lHR / 180 * M_PI) *
                            sin(lVR / 180 * M_PI);
            obj->vtx[n].Y = R *
                            cos(lHR / 180 * M_PI) *
                            sin(lVR / 180 * M_PI);
            obj->vtx[n].Z = R *
                            cos(lVR / 180 * M_PI);
            obj->vtx[n].V = lVR / 180;
            obj->vtx[n].U = lHR / 360;
            norm = sqrt(
                    obj->vtx[n].X*obj->vtx[n].X+
                    obj->vtx[n].Y*obj->vtx[n].Y+
                    obj->vtx[n].Z*obj->vtx[n].Z);
            obj->vtx[n].nX = obj->vtx[n].X/norm;
            obj->vtx[n].nY = obj->vtx[n].Y/norm;
            obj->vtx[n].nZ = obj->vtx[n].Z/norm;
            n++;

            lVR = vR + space;
            lHR = hR;
            obj->vtx[n].X = R * sin(lHR / 180 * M_PI) * sin(lVR / 180 * M_PI);
            obj->vtx[n].Y = R * cos(lHR / 180 * M_PI) * sin(lVR / 180 * M_PI);
            obj->vtx[n].Z = R * cos(lVR / 180 * M_PI);
            obj->vtx[n].V = lVR / 180;
            obj->vtx[n].U = lHR / 360;
            norm = sqrt(obj->vtx[n].X*obj->vtx[n].X+obj->vtx[n].Y*obj->vtx[n].Y+obj->vtx[n].Z*obj->vtx[n].Z);
            obj->vtx[n].nX = obj->vtx[n].X/norm;
            obj->vtx[n].nY = obj->vtx[n].Y/norm;
            obj->vtx[n].nZ = obj->vtx[n].Z/norm;
            n++;

            lVR = vR;
            lHR = hR + space;
            obj->vtx[n].X = R * sin(lHR / 180 * M_PI) * sin(lVR / 180 * M_PI);
            obj->vtx[n].Y = R * cos(lHR / 180 * M_PI) * sin(lVR / 180 * M_PI);
            obj->vtx[n].Z = R * cos(lVR / 180 * M_PI);
            obj->vtx[n].V = lVR / 180;
            obj->vtx[n].U = lHR / 360;
            norm = sqrt(obj->vtx[n].X*obj->vtx[n].X+obj->vtx[n].Y*obj->vtx[n].Y+obj->vtx[n].Z*obj->vtx[n].Z);
            obj->vtx[n].nX = obj->vtx[n].X/norm;
            obj->vtx[n].nY = obj->vtx[n].Y/norm;
            obj->vtx[n].nZ = obj->vtx[n].Z/norm;
            n++;

            lVR = vR + space;
            lHR = hR + space;
            obj->vtx[n].X = R * sin(lHR / 180 * M_PI) * sin(lVR / 180 * M_PI);
            obj->vtx[n].Y = R * cos(lHR / 180 * M_PI) * sin(lVR / 180 * M_PI);
            obj->vtx[n].Z = R * cos(lVR / 180 * M_PI);
            obj->vtx[n].V = lVR / 180;
            obj->vtx[n].U = lHR / 360;
            norm = sqrt(obj->vtx[n].X*obj->vtx[n].X+obj->vtx[n].Y*obj->vtx[n].Y+obj->vtx[n].Z*obj->vtx[n].Z);
            obj->vtx[n].nX = obj->vtx[n].X/norm;
            obj->vtx[n].nY = obj->vtx[n].Y/norm;
            obj->vtx[n].nZ = obj->vtx[n].Z/norm;
            n++;
        }
    }
    return obj;
}

