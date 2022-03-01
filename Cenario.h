#ifndef MAIN_CPP_CENARIO_H
#define MAIN_CPP_CENARIO_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <string>
#include <cstring>

using namespace std;

class Boxes{
public:
    float xPos;
    float yPos;
    float height;
    float width;
    string color;
};

class Cenario {

private:
    void DesenhaRect(GLfloat x, GLfloat y, GLfloat height, GLfloat width, string color);
    void DesenhaCirc(GLfloat x, GLfloat y, GLfloat radius, string color);
    void DesenhaCenario();
    void GetCenarioFromSvg();
    void AddBoxesToArray(int index, float x, float y, float h, float w, string color){
        boxesObj[index].xPos = x;
        boxesObj[index].yPos = y;
        boxesObj[index].height = h;
        boxesObj[index].width = w;
        boxesObj[index].color = color;
    }
public:
    Boxes boxesObj[50] = {};
    const char *imagePath = "";

    void SetImagePath(const char* path){
        imagePath = path;
    }

    const char* ok(){
        return imagePath;
    }
    void Desenha(){
        DesenhaCenario();
    };

    void GetFromSvg(){
        GetCenarioFromSvg();
    }

    void GetBoxesArray(Boxes (&x)[50]){
        memcpy(x, boxesObj, sizeof(x));
    };
};


#endif //MAIN_CPP_CENARIO_H
