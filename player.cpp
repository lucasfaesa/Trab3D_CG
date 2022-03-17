#include "player.h"
#include "tiro.h"
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <GL/glut.h>

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

OBJ* objHead;

float CabecaRadius = 1;
float troncoHeight  = 3.8;
float troncoWidth  = 2;
float troncoExtrusion = 1.5;
float bracoHeight  = 3;
float bracoWidth  = 0.5;
float bracoExtrusion = 0.5;
float pernaHeight  = 1.8;
float pernaWidth  = 0.5;
float pernaExtrusion = 0.5;

float jumpUpDistanceTraveled = 0;
float maxJumpHeight = 0 ; //TODO tamanho do corpo do boneco
float previousPosY;

bool leftFootTouchingGround = true;
bool rightFootTouchingGround = false;
bool previousPD1FacingRight = true;
bool previousPD2FacingRight = true;
bool previousPE1FacingRight = true;
bool previousPE2FacingRight = true;
bool rotatePernaD1Backwards;
bool rotatePernaD2Backwards;
bool rotatePernaE1Backwards;
bool rotatePernaE2Backwards;

bool pernaD1ReachForwardLimit;
bool pernaD1ReachBackwardLimit;
bool pernaD1BackwardLimitChanged;
bool pernaD1ForwardLimitChanged;
float speedD2 = 0;
float maxRotD2 = 0;
float minRotD2 = 0;

bool pernaE1ReachForwardLimit;
bool pernaE1ReachBackwardLimit;
bool pernaE1BackwardLimitChanged;
bool pernaE1ForwardLimitChanged;
float speedE2 = 0;
float maxRotE2 = 0;
float minRotE2 = 0;
bool reversed;


OBJ *CreatePlayerSphere(double R, double space);

void Player::DesenhaRect(GLfloat height, GLfloat width, GLfloat extrusion, GLfloat R, GLfloat G, GLfloat B, GLfloat A, GLuint texture)
{
    //glPushMatrix();
    /*if(facingRight)
        glScalef(1,1,1);
    else
        glScalef(-1,1,1);*/

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

    //glPopMatrix();
}
void Player::DesenhaCabeca(GLfloat x, GLfloat y, GLfloat radius, GLfloat R, GLfloat G, GLfloat B, GLuint textureHead)
{
    glPushMatrix();
    glTranslatef(x,y,0);
    glRotatef(90,1,0,0);
    glRotatef(45,0,0,1);
    //glRotatef(-90,0,1,0);
    /*if(!facingRight) glScalef(-1,1,1);

    glutSolidSphere(radius,20,10);*/


    GLfloat materialEmission[] = { 0.10, 0.10, 0.10, 1};
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
    for (int i = 0; i < objHead->numVtx; i++)
    {
        glNormal3f(objHead->vtx[i].nX, objHead->vtx[i].nY, objHead->vtx[i].nZ);
        glTexCoord2f (objHead->vtx[i].U, objHead->vtx[i].V);
        glVertex3f (objHead->vtx[i].X, objHead->vtx[i].Y, objHead->vtx[i].Z);
    }
    glEnd();

    glPopMatrix();
}

void Player::DesenhaBraco(GLfloat x, GLfloat y, GLfloat theta1, GLuint textureArm)
{
    glPushMatrix();
        glTranslatef(x,y,0);
        glRotatef(theta1,0,0,1);
        if(!facingRight) glScalef(-1,1,1);
        DesenhaRect(bracoHeight,bracoWidth,bracoExtrusion,0,0,0,1,textureArm);
    glPopMatrix();
}

void Player::DesenhaPerna(GLfloat x, GLfloat y, GLfloat pEtheta1, GLfloat pEtheta2, GLfloat pDtheta1, GLfloat pDtheta2, GLuint textureLeg)
{

    //glTranslatef(x,y,0);

    if(facingRight){ //TODO TROCAR PARA LOOP FOR

        glPushMatrix();
            glTranslatef(x,y,0.3);
            glScalef(-1,1,1);
            glRotatef(pEtheta1,0,0,1);
            DesenhaRect(-pernaHeight,pernaWidth,pernaExtrusion,0,1,0,1,textureLeg); //desenhando primeira perna esquerda
            glTranslatef(0,-pernaHeight,0);
            glRotatef(pEtheta2,0,0,1);
            DesenhaRect(-pernaHeight,pernaWidth,pernaExtrusion,1,1,0,1,textureLeg); //desenhando segunda perna esquerda
        glPopMatrix();

        glPushMatrix();
            glTranslatef(x,y,-0.3);
            glScalef(-1,1,1);
            glRotatef(pDtheta1,0,0,1);
            DesenhaRect(-pernaHeight,pernaWidth,pernaExtrusion,1,0,0,1,textureLeg); //desenhando primeira perna direita
            glTranslatef(0,-pernaHeight,0);
            glRotatef(pDtheta2,0,0,1);
            DesenhaRect(-pernaHeight,pernaWidth,pernaExtrusion,1,1,1,1,textureLeg); //desenhando segunda perna direita*/
        glPopMatrix();
    }else{
        glPushMatrix();
            glTranslatef(x,y,0.3);
            glRotatef(pDtheta1,0,0,1);
            DesenhaRect(-pernaHeight,pernaWidth,pernaExtrusion,1,0,0,1,textureLeg); //desenhando primeira perna direita
            glTranslatef(0,-pernaHeight,0);
            glRotatef(pDtheta2,0,0,1);
            DesenhaRect(-pernaHeight,pernaWidth,pernaExtrusion,1,1,1,1,textureLeg); //desenhando segunda perna direita*/
        glPopMatrix();

        glPushMatrix();
            glTranslatef(x,y,-0.3);
            glRotatef(pEtheta1,0,0,1);
            DesenhaRect(-pernaHeight,pernaWidth,pernaExtrusion,0,1,0,1,textureLeg); //desenhando primeira perna esquerda
            glTranslatef(0,-pernaHeight,0);
            glRotatef(pEtheta2,0,0,1);
            DesenhaRect(-pernaHeight,pernaWidth,pernaExtrusion,1,1,0,1,textureLeg); //desenhando segunda perna esquerda
        glPopMatrix();
    }
}

void Player::DesenhaPlayer(GLfloat x, GLfloat y, GLfloat bTheta, GLfloat pETheta1, GLfloat pETheta2, GLfloat pDTheta1, GLfloat pDTheta2,
                           GLuint textureChest, GLuint textureArm, GLuint textureLeg, GLuint textureHead)
{
    glPushMatrix();
        glTranslatef(x,y,25);
    if(facingRight)
        glScalef(1,1,1);
    else
        glScalef(-1,1,1);

        glPushMatrix();
            if(!facingRight) glScalef(-1,1,1);
            DesenhaRect(troncoHeight,troncoWidth, troncoExtrusion, 0,1,0.3,1,textureChest); //desenhando base
        glPopMatrix();
        DesenhaCabeca(0, troncoHeight + CabecaRadius, CabecaRadius, 0, 1, 0.3,textureHead);
        DesenhaPerna(0, 0,pETheta1,pETheta2, pDTheta1, pDTheta2,textureLeg);
        DesenhaBraco(0,troncoHeight/2,bTheta,textureArm);


    glPopMatrix();
}

Tiro* Player::Atira() {

    float angleSumTheta1 = (bTheta) * M_PI / 180;

    if(facingRight) {
        float bulletX = gX - bracoHeight * sin(angleSumTheta1);
        float bulletY = gY + troncoHeight / 2 + bracoHeight * cos(angleSumTheta1);
        std::cout << "here right" << std::endl;
        return new Tiro(bulletX, bulletY, 90 + bTheta, facingRight);
    }
    else{
        float bulletX = gX + bracoHeight * sin(angleSumTheta1) ;
        float bulletY = gY + troncoHeight/2 + bracoHeight * cos(angleSumTheta1);
        std::cout << "here left" << std::endl;
        return new Tiro(bulletX, bulletY, -90 + bTheta, facingRight);
    }

}

void Player::RodaBraco(GLfloat inc)
{
    bTheta =  inc - Player::initialBTheta;
}

void Player::RodaPernaE1(GLfloat inc)
{
    if(previousPE1FacingRight != facingRight){
        pETheta1 = pETheta1 * -1;
    }

    float maxRot = 30;
    float minRot = -30;

    float speed = 100;
    if(pETheta1 >= maxRot){
        pernaE1ReachForwardLimit = true;
        pernaE1ReachBackwardLimit = false;
        if(facingRight)
            rotatePernaE1Backwards = false;
        else
            rotatePernaE1Backwards = true;
    }
    if(pETheta1 <= minRot){
        pernaE1ReachForwardLimit = false;
        pernaE1ReachBackwardLimit = true;
        if(facingRight)
            rotatePernaE1Backwards = true;
        else
            rotatePernaE1Backwards = false;
    }

    if(rotatePernaE1Backwards){
        pETheta1 += inc * speed;
    }else{
        pETheta1 -= inc * speed;
    }

    previousPE1FacingRight = facingRight;
}

void Player::RodaPernaE2(GLfloat inc)
{
    float speedTouchingGround = 15;
    float speedNotTouchingGround = 60;

    float maxRotTouchingGround = 0;
    float minRotTouchingGround = -15;
    float maxRotNotTouchingGround = 0;
    float minRotNotTouchingGround = -50;

    if(pernaE1ReachForwardLimit && !pernaE1ReachBackwardLimit && !pernaE1ForwardLimitChanged){
        pETheta2 = 0;
        maxRotE2 = maxRotTouchingGround;
        minRotE2 = minRotTouchingGround;
        speedE2 = speedTouchingGround;
        pernaE1ForwardLimitChanged = true;
        pernaE1BackwardLimitChanged = false;
    }

    if(!pernaE1ReachForwardLimit && pernaE1ReachBackwardLimit && !pernaE1BackwardLimitChanged){
        pETheta2 = 0;
        maxRotE2 = maxRotNotTouchingGround;
        minRotE2 = minRotNotTouchingGround;
        speedE2 = speedNotTouchingGround;
        pernaE1ForwardLimitChanged = false;
        pernaE1BackwardLimitChanged = true;
    }
    /*float maxRot;
    float minRot;

    if(previousPD2FacingRight != facingRight){
       //pDTheta2 = pDTheta2 * -1;
    }

    if(facingRight){
        maxRot = 0;
        minRot = -30;
    }else{
        maxRot = -30;
        minRot = 0;
    }*/

    if(pETheta2 >= maxRotE2){
        rotatePernaE2Backwards = true;
    }
    if(pETheta2 < minRotE2){
        rotatePernaE2Backwards = false;
    }

    if(rotatePernaE2Backwards){
        pETheta2 -= inc * speedE2;
    }else{
        pETheta2 += inc * speedE2;
    }

    //if(pDTheta1 > 29){
    //std::cout <<"pD1: " << pDTheta1 << " pD2: " << pDTheta2 <<" speed:" << speedE2 << std::endl;
    //}

    //previousPD2FacingRight = facingRight;
}

void Player::RodaPernaD1(GLfloat inc)
{
    if(previousPD1FacingRight != facingRight){
        pDTheta1 = pDTheta1 * -1;
    }

    float maxRot = 30;
    float minRot = -30;

    float speed = 100;
    if(pDTheta1 >= maxRot){
        pernaD1ReachForwardLimit = true;
        pernaD1ReachBackwardLimit = false;

        if(facingRight)
            rotatePernaD1Backwards = true;
        else
            rotatePernaD1Backwards = false;
    }
    if(pDTheta1 <= minRot){
        pernaD1ReachForwardLimit = false;
        pernaD1ReachBackwardLimit = true;

        if(facingRight)
            rotatePernaD1Backwards = false;
        else
            rotatePernaD1Backwards = true;
    }

    if(rotatePernaD1Backwards){
        pDTheta1 -= inc * speed;
    }else{
        pDTheta1 += inc * speed;
    }

    previousPD1FacingRight = facingRight;

}

void Player::RodaPernaD2(GLfloat inc)
{
    float maxRotTouchingGround;
    float minRotTouchingGround;
    float maxRotNotTouchingGround;
    float minRotNotTouchingGround;
    float speedTouchingGround = 15;
    float speedNotTouchingGround = 60;

    if(previousPD2FacingRight != facingRight){
        pDTheta2 = pDTheta2 * -1;
        reversed = true;
        speedTouchingGround = 15;
        speedNotTouchingGround = 60;
        maxRotTouchingGround = 15;
        minRotTouchingGround = 0;
        maxRotNotTouchingGround = 50;
        minRotNotTouchingGround = 0;
    }
    if(facingRight){
        reversed = false;
        //pDTheta2 = pDTheta2 * -1; ?????????????
        speedTouchingGround = 15;
        speedNotTouchingGround = 60;
        maxRotTouchingGround = 0;
        minRotTouchingGround = -15;
        maxRotNotTouchingGround = 0;
        minRotNotTouchingGround = -50;
    }

    /*float maxRotTouchingGround = 0;
    float minRotTouchingGround = -15;
    float maxRotNotTouchingGround = 0;
    float minRotNotTouchingGround = -50;*/

    if(pernaD1ReachForwardLimit && !pernaD1ReachBackwardLimit && !pernaD1ForwardLimitChanged){
        pDTheta2 = 0;
        maxRotD2 = maxRotTouchingGround;
        minRotD2 = minRotTouchingGround;
        speedD2 = speedTouchingGround;
        pernaD1ForwardLimitChanged = true;
        pernaD1BackwardLimitChanged = false;
    }

    if(!pernaD1ReachForwardLimit && pernaD1ReachBackwardLimit && !pernaD1BackwardLimitChanged){
        pDTheta2 = 0;
        maxRotD2 = maxRotNotTouchingGround;
        minRotD2 = minRotNotTouchingGround;
        speedD2 = speedNotTouchingGround;
        pernaD1ForwardLimitChanged = false;
        pernaD1BackwardLimitChanged = true;
    }

    if(pDTheta2 >= maxRotD2){
        if(reversed)
            rotatePernaD2Backwards = true;
        else
            rotatePernaD2Backwards = false;
    }
    if(pDTheta2 < minRotD2){
        if(reversed)
            rotatePernaD2Backwards = false;
        else
            rotatePernaD2Backwards = true;
    }

    if(rotatePernaD2Backwards){
        if(!reversed)
            pDTheta2 -= inc * speedD2;
        else
            pDTheta2 += inc * speedD2;
    }else{
        if(!reversed)
            pDTheta2 += inc * speedD2;
        else
            pDTheta2 -= inc * speedD2;
    }

    //if(pDTheta1 > 29){
        std::cout << " pD1: " << pDTheta1 <<" pD2: " << pDTheta2 << " speed:" << speedD2 << std::endl;
    //}

    previousPD2FacingRight = facingRight;
}

bool Player::Atingido(int index, enemyTiro *enemyTiro) {
    float tiroXPos = 0;
    float tiroYPos = 0;
    float tiroRadius = 0.5;

    enemyTiro->GetPos(tiroXPos,tiroYPos);

    float playerSizeY = (pernaHeight * 2) + (CabecaRadius * 2) + troncoHeight;

    bool collisionX = gX + troncoWidth/2 >= tiroXPos && tiroXPos + (tiroRadius * 2) >= gX;
                            //base do player
    bool collisionY = (gY - pernaHeight*2) + playerSizeY >= tiroYPos && tiroYPos + (tiroRadius * 2) >= gY;

    if(collisionX && collisionY){
        std::cout << "tiro no player" << std::endl;
    }
    return collisionX && collisionY;
}

void Player::MoveEmX(GLfloat dx, GLfloat timeDifference)
{
    if(dx > 0){
        facingRight = true;
    }
    if(dx < 0){
        facingRight = false;
    }

    gX += dx * timeDifference;

}

void Player::ResetPlayerPos() {
    gX = -157;
    gY = -180;
}

void Player::MoveEmY(GLfloat dy, bool &isJumping)
{
    float speed = 60;
    maxJumpHeight = ((CabecaRadius * 2 + troncoHeight + pernaHeight * 2) * 3);

    if(fabs(jumpUpDistanceTraveled) <= maxJumpHeight){
        gY += dy * speed;
        jumpUpDistanceTraveled += previousPosY - gY;
        isJumping = true;
    }else{
        isJumping = false;
    }
    previousPosY = gY;
}
void Player::FreeFall(GLfloat dy)
{
    float speed = 60;
    gY -= dy * speed;
    previousPosY = gY;
}

void Player::ResetJumpDistance(){
    jumpUpDistanceTraveled = 0;
}

void Player::MoveEmMenosY(GLfloat dy, bool jumping)
{
    if(!jumping) return;

    float speed = dy * 75;
    //float speed = dy * 3;
    jumpUpDistanceTraveled += speed;

    gY -= speed;

}

void Player::CreatePlayerHead(){
    objHead = CreatePlayerSphere(CabecaRadius, 10);
}

OBJ *CreatePlayerSphere(double R, double space) {
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
