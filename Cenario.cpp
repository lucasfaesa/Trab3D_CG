#include "Cenario.h"
#include "tinyxml2.h"
#include <string>
#include <cmath>
using namespace std;

int cont = 0;

void Cenario::DesenhaRect(GLfloat x, GLfloat y, GLfloat height, GLfloat width, string color)
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

    glPushMatrix();
    glTranslatef(x+width/2,y,0);
    glBegin(GL_QUADS);

    glColor3f(R,G,B);// Face posterior
    //glNormal3f(0.0, 0.0, 1.0);	// Normal da face
    glVertex2f(width/2, height);
    glVertex2f(-width/2,height);
    glVertex2f(-width/2, 0);
    glVertex2f(width/2, 0);
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

void Cenario::DesenhaCenario() {

    for(int i =0; i<sizeof(boxesObj); i++){
        if(boxesObj[i].height == 0) break;

        DesenhaRect(boxesObj[i].xPos,boxesObj[i].yPos,boxesObj[i].height,boxesObj[i].width,boxesObj[i].color);
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


