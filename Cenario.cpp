#include "Cenario.h"
#include "tinyxml2.h"
#include <string>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <iostream>

using namespace std;

int cont = 0;
int zzzz = 0;

void Cenario::DesenhaRect(GLfloat x, GLfloat y, GLfloat height, GLfloat width, string color, GLuint textureTop,GLuint textureSide)
{
    float R = 0;
    float G = 0;
    float B = 0;
    if(color == "blue"){
        R = 0;
        G = 0;
        B = 1;
    }if(color == "green"){
        R = 0;
        G = 1;
        B = 0;
    }

    if(color == "blue"){ //TODO REMOVER ISSO
        return;
    }

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
    glBindTexture (GL_TEXTURE_2D, textureSide);

    double tilingSidesX = 1;
    double tilingSidesY = 1;
    double tilingFrontBackX = 1;
    double tilingFrontBackY = 1;
    double tilingTopDownX = 1;
    double tilingTopDownY = 1;

    if(abs((int)height) <= 3){
        tilingSidesX = 1;
        tilingSidesY = 6;
    }
    if(abs((int)height) > 3 && abs((int)height) <= 6) {
        tilingSidesX = 1;
        tilingSidesY = 5;
    }
    if(abs((int)height) > 6 && abs((int)height) <= 11) {
        tilingSidesX = 2;
        tilingSidesY = 4;
    }
    if(abs((int)height) > 11 && abs((int)height) <= 30) {
        tilingSidesX = 4;
        tilingSidesY = 6;
    }
    if(abs((int)height) > 30){
        tilingSidesX = 8;
        tilingSidesY = 8;
    }

    if(abs((int)width) <= 6){
        tilingFrontBackX = 1;
        tilingFrontBackY = 1;

        tilingTopDownX = 1;
        tilingTopDownY = 5;

        if(abs((int)height) >= 30){
            tilingFrontBackX = 6;
            tilingFrontBackY = 1;
        }
    }
    if(abs((int)width) > 6 && abs((int)width) <= 12){
        tilingFrontBackX = 2;
        tilingFrontBackY = 2;

        tilingTopDownX = 2;
        tilingTopDownY = 5;
    }
    if(abs((int)width) > 12 && abs((int)width) <= 23){
        tilingFrontBackX = 1;
        tilingFrontBackY = 4;

        tilingTopDownX = 3;
        tilingTopDownY = 6;
    }
    if(abs((int)width) > 23 && abs((int)width) <= 31){
        tilingFrontBackX = 1;
        tilingFrontBackY = 6;

        tilingTopDownX = 3;
        tilingTopDownY = 7;
    }
    if(abs((int)width) > 31){
        tilingFrontBackX = 1;
        tilingFrontBackY = 12;

        tilingTopDownX = 20;
        tilingTopDownY = 6;
    }

    glPushMatrix();

    glTranslatef(x+width/2,y,0);

    glBegin(GL_QUADS);
        //glColor3f(1,0,0.3);// Face posterior
        glNormal3f(0.0, 0.0, 1.0);	// Normal da face
        glTexCoord2f (0, tilingFrontBackX);
        glVertex3f(width/2, 0, 45.8);
        glTexCoord2f (tilingFrontBackY, tilingFrontBackX);
        glVertex3f(-width/2, 0, 45.8);
        glTexCoord2f (tilingFrontBackY, 0);
        glVertex3f(-width/2,height, 45.8);
        glTexCoord2f (0, 0);
        glVertex3f(width/2, height, 45.8); //z metade da altura do fundo azul

    glEnd();

    glBegin(GL_QUADS);
        //glColor3f(1,0,0);// Face  traseira
        glNormal3f(0.0, 0.0, -1.0);	// Normal da face
        glTexCoord2f (0, 0);
        glVertex3f(-width/2, height, -0);
        glTexCoord2f (0, tilingFrontBackX);
        glVertex3f(-width/2, 0, -0);
        glTexCoord2f (tilingFrontBackY, tilingFrontBackX);
        glVertex3f(width/2,0, -0);
        glTexCoord2f (tilingFrontBackY, 0);
        glVertex3f(width/2, height, -0); //z metade da altura do fundo azul

    glEnd();

    glBegin(GL_QUADS);
        //glColor3f(0,1,0);// Face lateral esquerda
        glNormal3f(-1.0, 0.0, 0.0);	// Normal da face
        glTexCoord2f (0, tilingSidesX);
        glVertex3f(-width/2, 0, 45.8);
        glTexCoord2f ( tilingSidesY,tilingSidesX);
        glVertex3f(-width/2, 0, -0);
        glTexCoord2f (tilingSidesY, 0);
        glVertex3f(-width/2,height, -0);
        glTexCoord2f (0, 0);
        glVertex3f(-width/2, height, 45.8); //z metade da altura do fundo azul
    glEnd();

    glBegin(GL_QUADS);
        //glColor3f(1.0,0,0);// Face lateral direita
        glNormal3f(1.0, 0.0, 0.0);	// Normal da face
        glTexCoord2f (0, 0);
        glVertex3f(width/2, height, -0);
        glTexCoord2f (0, tilingSidesX);
        glVertex3f(width/2, 0, -0);
        glTexCoord2f (tilingSidesY, tilingSidesX);
        glVertex3f(width/2,0, 45.8);
        glTexCoord2f (tilingSidesY, 0);
        glVertex3f(width/2, height, 45.8); //z metade da altura do fundo azul
    glEnd();

    glBindTexture (GL_TEXTURE_2D, textureTop);
    glBegin(GL_QUADS);
        //glColor3f(1,1,0);// Face superior
        glNormal3f(0.0, 1.0, 0.0);	// Normal da face
        glTexCoord2f (0, 0);
        glVertex3f(-width/2, 0, 45.8);
        glTexCoord2f (0, tilingTopDownX);
        glVertex3f(width/2, 0, 45.8);
        glTexCoord2f (tilingTopDownY, tilingTopDownX);
        glVertex3f(width/2,0, -0);
        glTexCoord2f (tilingTopDownY, 0);
        glVertex3f(-width/2, 0, -0); //z metade da altura do fundo azul
    glEnd();

    glBegin(GL_QUADS);
        //glColor3f(1,1,1);// Face inferior
        glNormal3f(0.0, -1.0, 0.0);	// Normal da face
        glTexCoord2f (0, 0);
        glVertex3f(width/2, height, -0);
        glTexCoord2f (0, tilingTopDownY);
        glVertex3f(width/2, height, 45.8);
        glTexCoord2f (tilingTopDownX, tilingTopDownY);
        glVertex3f(-width/2,height, 45.8);
        glTexCoord2f (tilingTopDownX, 0);
        glVertex3f(-width/2, height, -0); //z metade da altura do fundo azul
    glEnd();


    /*glVertex2f(width/2, height); //z metade da altura do fundo azul
    glVertex2f(-width/2,height);
    glVertex2f(-width/2, 0);
    glVertex2f(width/2, 0);*/



    glPopMatrix();
}

void Cenario::DesenhaBg(GLfloat x, GLfloat y, GLfloat height, GLfloat width, string color, GLuint textureSky)
{
    GLfloat materialEmission[] = { 1.00, 1.00, 1.00, 1};
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
    glBindTexture (GL_TEXTURE_2D, textureSky);

    double tilingSidesX = 3;
    double tilingSidesY = 2;
    double tilingFrontBackX = 3;
    double tilingFrontBackY = 8;
    double tilingTopDownX = 8;
    double tilingTopDownY = 2;

    glPushMatrix();

        glTranslatef(x+width/2,y,0);

        glBegin(GL_QUADS);
            //glColor3f(1,0,0.3);// Face posterior
            glNormal3f(0.0, 0.0, 1.0);	// Normal da face
            glTexCoord2f (0, tilingFrontBackX);
            glVertex3f(width/2, height, 45.8); //z metade da altura do fundo azul
            glTexCoord2f (tilingFrontBackY, tilingFrontBackX);
            glVertex3f(-width/2,height, 45.8);
            glTexCoord2f (tilingFrontBackY, 0);
            glVertex3f(-width/2, 0, 45.8);
            glTexCoord2f (0, 0);
            glVertex3f(width/2, 0, 45.8);
        glEnd();

        glBegin(GL_QUADS);
            //glColor3f(1,0,0);// Face  traseira
            glNormal3f(0.0, 0.0, -1.0);	// Normal da face
            glTexCoord2f (0, 0);
            glVertex3f(width/2, height, -0);
            glTexCoord2f (0, tilingFrontBackX);
            glVertex3f(width/2,0, -0);
            glTexCoord2f (tilingFrontBackY, tilingFrontBackX);
            glVertex3f(-width/2, 0, -0);
            glTexCoord2f (tilingFrontBackY, 0);
            glVertex3f(-width/2, height, -0);
             //z metade da altura do fundo azul
        glEnd();

        glBegin(GL_QUADS);
            //glColor3f(0,1,0);// Face lateral esquerda
            glNormal3f(1.0, 0.0, 0.0);	// Normal da face
            glTexCoord2f (0, tilingSidesX);
            glVertex3f(-width/2, 0, -0);
            glTexCoord2f ( tilingSidesY,tilingSidesX);
            glVertex3f(-width/2, 0, 45.8);
            glTexCoord2f (tilingSidesY, 0);
            glVertex3f(-width/2, height, 45.8); //z metade da altura do fundo azul
            glTexCoord2f (0, 0);
            glVertex3f(-width/2,height, -0);

        glEnd();

        glBegin(GL_QUADS);
            //glColor3f(1.0,0,0);// Face lateral direita
            glNormal3f(1.0, 0.0, 0.0);	// Normal da face
            glTexCoord2f (0, 0);
            glVertex3f(width/2, height, 45.8); //z metade da altura do fundo azul
            glTexCoord2f (0, tilingSidesX);
            glVertex3f(width/2,0, 45.8);
            glTexCoord2f (tilingSidesY, tilingSidesX);
            glVertex3f(width/2, 0, -0);
            glTexCoord2f (tilingSidesY, 0);
            glVertex3f(width/2, height, -0);
        glEnd();

        glBindTexture (GL_TEXTURE_2D, textureSky);
            glBegin(GL_QUADS);
            //glColor3f(1,1,0);// Face superior
            glNormal3f(0.0, 1.0, 0.0);	// Normal da face
            glTexCoord2f (0, 0);
            glVertex3f(-width/2, 0, -0); //z metade da altura do fundo azul
            glTexCoord2f (0, tilingTopDownX);
            glVertex3f(width/2,0, -0);
            glTexCoord2f (tilingTopDownY, tilingTopDownX);
            glVertex3f(width/2, 0, 45.8);
            glTexCoord2f (tilingTopDownY, 0);
            glVertex3f(-width/2, 0, 45.8);
        glEnd();

        glBegin(GL_QUADS);
            //glColor3f(1,1,1);// Face inferior
            glNormal3f(0.0, -1.0, 0.0);	// Normal da face
            glTexCoord2f (0, 0);
            glVertex3f(width/2, height, -0);
            glTexCoord2f (0, tilingTopDownY);
            glVertex3f(width/2, height, 45.8);
            glTexCoord2f (tilingTopDownX, tilingTopDownY);
            glVertex3f(-width/2,height, 45.8);
            glTexCoord2f (tilingTopDownX, 0);
            glVertex3f(-width/2, height, -0); //z metade da altura do fundo azul
        glEnd();

    glPopMatrix();
}

void Cenario::DesenhaCirc(GLfloat x, GLfloat y, GLfloat radius, string color)
{
    float R,G,B;
    if(color == "green"){
        R = 0;
        G = 1;
        B = 0;
    }
    if(color == "red"){
        R = 1;
        G = 0;
        B = 0;
    }

    glPushMatrix();
    glScalef(1,-1,1); //invertendo
    glTranslatef(x,y,0);
    glColor3f(R,G,B);
    glBegin(GL_POLYGON);
    for(int i=0; i<20; i++){
        double angle = i *(2.0 * M_PI/20);
        glVertex2f(radius*cos(angle), radius*sin(angle));
    }

    glEnd();
    glPopMatrix();
}

void Cenario::DesenhaCenario(GLuint textureGrassTop, GLuint textureGrassSide, GLuint textureSky) {

    for(int i =0; i<sizeof(boxesObj); i++){
        if(boxesObj[i].height == 0) break;

        if(boxesObj[i].color == "blue")
            DesenhaBg(boxesObj[i].xPos,boxesObj[i].yPos,boxesObj[i].height,boxesObj[i].width,boxesObj[i].color, textureSky);
        else
            DesenhaRect(boxesObj[i].xPos,boxesObj[i].yPos,boxesObj[i].height,boxesObj[i].width,boxesObj[i].color, textureGrassTop,textureGrassSide);
    }

   // DesenhaRect(-163.5, -187.2, -10,364.1373,"black");
}

void Cenario::GetCenarioFromSvg() {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(imagePath);

    tinyxml2::XMLElement *levelElement = doc.FirstChildElement("svg");
    for (tinyxml2::XMLElement *child = levelElement->FirstChildElement("rect");
         child != NULL; child = child->NextSiblingElement("rect")) {
        const char *pAttrW = child->Attribute("width");
        const char *pAttrH = child->Attribute("height");
        const char *pAttrX = child->Attribute("x");
        const char *pAttrY = child->Attribute("y");
        const char *pAttrFill = child->Attribute("fill");

        float iWidth = strtof(pAttrW, NULL);
        float iHeight = strtof(pAttrH, NULL);
        float iX = strtof(pAttrX, NULL);
        float iY = strtof(pAttrY, NULL);

        /*if((int)iWidth == 364){
            continue;
        }*/

        Cenario::AddBoxesToArray(cont,iX,-iY,-iHeight,iWidth, child->Attribute("fill")); //invetendo Y e altura para desenho correto
        cont++;
    }

    for (tinyxml2::XMLElement *child = levelElement->FirstChildElement("circle");
         child != NULL; child = child->NextSiblingElement("circle")) {
        const char *pAttrX = child->Attribute("cx");
        const char *pAttrY = child->Attribute("cy");
        const char *pAttrR = child->Attribute("r");
        const char *pAttrFill = child->Attribute("fill");

        float iX = strtof(pAttrX, NULL);
        float iY = strtof(pAttrY, NULL);
        float iR = strtof(pAttrR, NULL);

        DesenhaCirc(iX,iY,iR,pAttrFill);
    }

    //#temp
    Cenario::AddBoxesToArray(cont, -163.5, -187.2, -10,364.1373,"black");
    //#temp
    cont = 0;
}

void Cenario::DesenhaParedeTest(GLuint earth) {
    glPushMatrix();

    glTranslatef(-157,-180,-12);
    glScalef(70,70,1);
    glRotatef(90,1,0,0);

    GLfloat materialEmission[] = { 1.0, 1.0, 1.0, 1};
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

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT  );//X
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );//Y

    glBindTexture (GL_TEXTURE_2D, earth);
    double textureS = 1; // Bigger than 1, repeat
    glBegin (GL_QUADS);
    glNormal3f(0,1,0);
    glTexCoord2f (0, 0);
    glVertex3f (-1, 0, -1);
    glNormal3f(0,1,0);
    glTexCoord2f (0, textureS);
    glVertex3f (-1, 0, +1);
    glNormal3f(0,1,0);
    glTexCoord2f (textureS, textureS);
    glVertex3f (+1, 0, +1);
    glNormal3f(0,1,0);
    glTexCoord2f (textureS, 0);
    glVertex3f (+1, 0, -1);
    glEnd();
    glPopMatrix();
    //glDisable(GL_TEXTURE_2D);
}

