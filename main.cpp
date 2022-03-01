#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include "player.h"
#include "tinyxml2.h"
#include "Cenario.h"
#include "tiro.h"
#include "enemyTiro.h"
#include "enemy.h"
#include <algorithm>
#include <cmath>

#define INC_KEY 1
#define INC_KEYIDLE 0.02 //player X Speed
//#define INC_KEYIDLE 0.16 //player X Speed

//Key status
int keyStatus[256];

Player Player;
Cenario Cenario;
Enemy Enemy;

bool pressingJumpKey = false;
bool isJumping = false;
bool test = false;

Boxes boxesArray[50] = {};
Enemies enemiesArray[7] = {};

bool playerCollidingOnRightSide;
bool playerCollidingOnLeftSide;
bool playerCollidingBottom;
bool playerCollidingTop;

bool enemyCollidingRightSide[7] = {};
bool enemyCollidingLeftSide[7] = {};
bool enemyCollidingBottom[7] = {};
bool enemyCollidingTop[7] = {};

bool enemyChangedDirectionNoGround[7] = {};
bool enemyReachedGround[7] = {};

float previousPlayerLeft = 0;
float previousPlayerRight = 0;
float previousPlayerTop = 0;
float previousPlayerBottom = 0;

float previousEnemyLeft[7] = {};
float previousEnemyRight[7] = {};
float previousEnemyTop[7] = {};
float previousEnemyBottom[7] = {};

float currentPlayerBottom = 0;
float currentPlayerRight =0;
float currentPlayerLeft = 0;
float currentPlayerTop = 0;

float currentEnemyBottom = 0;
float currentEnemyRight =0;
float currentEnemyLeft = 0;
float currentEnemyTop = 0;

float yVel = 0.01; //player Y Speed

float enemyTiroTimer = 0;
float enemyTiroDelay = 1000; //1 segundos

float camMove = 0;

bool canShowText;
bool gameWon;
bool drawPlayer = true;
bool canEnemiesMove = true;
bool canEnemiesShoot = true;
bool canPlayerShoot = true;
bool canPlayerMove = true;
bool canUseGravity = true;

Tiro * tiro = NULL; //Um tiro por vez
enemyTiro * enemyTiroArray[7] = {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};


void CheckKeyPress(GLdouble diference);
void CheckPlayerCollision();
void MoveCamera();
void CheckPlayerTiro(GLdouble diference);
void MoveEnemies(GLdouble diference);
void CheckEnemiesCollision();
void CheckEnemyTiro(GLdouble diference);
void CheckPlayerGameWon();
void ResetGame();

void RandomEnemyShoot(GLdouble diference);

// Window dimensions
const GLint Width = 500;
const GLint Height = 500;

static char str[1000];
void * font = GLUT_BITMAP_9_BY_15;
void ExibirTexto()
{
    if(!canShowText) return;
    float x;
    float y;
    Player.GetPos(x,y);

    glColor3f(1.0, 1.0, 1.0);
    //Cria a string a ser impressa
    char *tmpStr;
    if(gameWon){
        sprintf(str, "Parabens, voce venceu! Aperte 'R' para reiniciar");
        canPlayerShoot = false;
        canPlayerMove = false;
        canEnemiesShoot = false;
        canEnemiesMove = false;
    }else{
        sprintf(str, "Tente novamente! Aperte 'R' para reiniciar");
        canPlayerShoot = false;
        canPlayerMove = false;
        canEnemiesShoot = false;
        canEnemiesMove = false;
        drawPlayer = false;
    }

    //Define a posicao onde vai comecar a imprimir
    glRasterPos2f(x - 35, y + 20);
    //Imprime um caractere por vez
    tmpStr = str;
    while( *tmpStr ){
        glutBitmapCharacter(font, *tmpStr);
        tmpStr++;
    }
}

void renderScene(void)
{
    // Clear the screen.
    glClear(GL_COLOR_BUFFER_BIT);

    Cenario.Desenha();

    if(drawPlayer)
        Player.Desenha();

    for(int i=0; i<sizeof(enemiesArray)/sizeof(enemiesArray[0]); i++){
        if(enemiesArray[i].canBeDrawn)
            Enemy.Desenha(i, enemiesArray[i]);
    }

    ExibirTexto();

    if (tiro) tiro->Desenha();

    if(canEnemiesShoot){
        for(int i=0; i<sizeof(enemyTiroArray)/sizeof(enemyTiroArray[0]); i++){
            if (enemyTiroArray[i])
                enemyTiroArray[i]->Desenha();
        }
    }

    glutSwapBuffers(); // Desenha the new frame of the game.
}

void keyPress(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'a':
        case 'A':
            keyStatus[(int)('a')] = 1; //Using keyStatus trick
            break;
        case 'd':
        case 'D':
            keyStatus[(int)('d')] = 1; //Using keyStatus trick
            break;
        case 't':
        case 'T':
            canEnemiesMove = !canEnemiesMove;
            break;
        case 'y':
        case 'Y':
            canEnemiesShoot = !canEnemiesShoot;
            break;
        case 'r':
        case 'R':
            ResetGame();
            break;


        case 27 :
            exit(0);
    }
    glutPostRedisplay();
}

void keyup(unsigned char key, int x, int y)
{
    keyStatus[(int)(key)] = 0;
    glutPostRedisplay();
}

void ResetKeyStatus()
{
    int i;
    //Initialize keyStatus
    for(i = 0; i < 256; i++)
        keyStatus[i] = 0;
}

void init(int w, int h)
{
    srand (time(NULL));

    Enemy.GetFromSvg();
    Cenario.GetFromSvg();

    Cenario.GetBoxesArray(boxesArray);
    Enemy.GetEnemiesArray(enemiesArray);

    //## PLAYER
    float playerPosX;
    float playerPosY;
    Player.GetPos(playerPosX,playerPosY);
    previousPlayerBottom = playerPosY  - 3.8;
    previousPlayerRight = playerPosX + 2 / 2;
    previousPlayerLeft = playerPosX - 2 / 2;
    previousPlayerTop = playerPosY + 5.6;

    //## ENEMY
    float enemyPosX[7] = {};
    float enemyPosY[7] = {};
    for(int i=0;i<sizeof(enemiesArray)/sizeof(enemiesArray[0]); i++) {
        Enemy.GetPos(i, enemyPosX[i], enemyPosY[i]);

        previousEnemyBottom[i] = enemyPosY[i] - 3.6; //perna height x 2
        previousEnemyRight[i] = enemyPosX[i] + 2 / 2; //metade da largura do tronco
        previousEnemyLeft[i] = enemyPosX[i] - 2 / 2; //metade da largura do tronco
        previousEnemyTop[i] = enemyPosY[i] + 5.6; //cabeca x 2 + altura do tronco
    }

    ResetKeyStatus();
    // The color the windows will redraw. Its done to erase the previous frame.
    int R  = 1;
    int G = 0.2;
    int B = 1;
    glClearColor(R,G,B, 1);
    glMatrixMode(GL_PROJECTION); // Select the projection matrix

    glEnable(GL_BLEND); //Enable blending.
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set blending function.

    gluOrtho2D(-45.9,45.9,-45.9,45.9);
    glMatrixMode(GL_MODELVIEW); // Select the projection matrix
    glLoadIdentity();
}

void idle(void)
{
    static GLdouble previousTime = glutGet(GLUT_ELAPSED_TIME);
    GLdouble currentTime, timeDiference;
    //Pega o tempo que passou do inicio da aplicacao
    currentTime = glutGet(GLUT_ELAPSED_TIME);
    // Calcula o tempo decorrido desde de a ultima frame.
    timeDiference = currentTime - previousTime;
    //Atualiza o tempo do ultimo frame ocorrido
    previousTime = currentTime;

    RandomEnemyShoot(timeDiference);

    CheckKeyPress(timeDiference);
    CheckPlayerCollision();
    CheckPlayerTiro(timeDiference);
    CheckEnemyTiro(timeDiference);
    //Enemy.MoveEmX(0,enemiesArray[0].speed, timeDiference);
    //Enemy.MoveEmX(1,enemiesArray[1].speed, timeDiference);
    CheckEnemiesCollision();
    MoveEnemies(timeDiference);
    CheckPlayerGameWon();
    MoveCamera();

    if(isJumping && pressingJumpKey){
        Player.MoveEmY(yVel, isJumping);
    }

    /*if(test){
        Player.MoveEmMenosY(yVel, true);
    }*/

    glutPostRedisplay();

}

void RandomEnemyShoot(GLdouble diference) {

    enemyTiroTimer += diference;

    if(enemyTiroTimer > enemyTiroDelay){

        int randomNumber = rand() % 7;
        while(!enemiesArray[randomNumber].canBeDrawn){
            randomNumber = rand() % 7;
        }

        if (!enemyTiroArray[randomNumber])
            enemyTiroArray[randomNumber] = Enemy.Atira(randomNumber);

        enemyTiroTimer = 0;
    }

    //braco do inimigo sempre apontar pro player
    float x;
    float y;
    Player.GetPos(x,y);
    for(int i =0; i<sizeof(enemiesArray)/sizeof(enemiesArray[0]); i++){
        Enemy.RodaBraco(i,x,y);
    }
}

void ResetGame() {
    canEnemiesMove = true;
    canEnemiesShoot = true;
    canPlayerMove = true;
    canPlayerShoot = true;

    for(int i =0; i<sizeof(enemiesArray)/sizeof(enemiesArray[0]); i++){
        Enemy.SetEnemyVisibility(i,true);
        enemiesArray[i].canBeDrawn = true;
    }
    camMove = 0;
    Player.ResetPlayerPos();
    canShowText = false;
    playerCollidingOnLeftSide = false;
    playerCollidingOnRightSide = false;
    drawPlayer = true;
    canUseGravity = true;
    gameWon = false;

    float playerPosX;
    float playerPosY;
    Player.GetPos(playerPosX,playerPosY);

    currentPlayerBottom = playerPosY - 3.6; //perna height x 2
    currentPlayerRight = playerPosX + 2 / 2; //metade da largura do tronco
    currentPlayerLeft = playerPosX - 2 / 2; //metade da largura do tronco
    currentPlayerTop = playerPosY + 5.6;

    previousPlayerLeft = currentPlayerLeft;
    previousPlayerRight = currentPlayerRight;
    previousPlayerTop = currentPlayerTop;
    previousPlayerBottom = currentPlayerBottom;
}

void CheckPlayerGameWon() {
    float x;
    float y;
    Player.GetPos(x,y);

    if(x >= 190 && y<= -170 && !gameWon){
        gameWon = true;
        canShowText = true;
    }

}

void CheckEnemyTiro(GLdouble diference) {

    if(!canEnemiesShoot) return;

    for(int i=0; i<sizeof(enemyTiroArray)/sizeof(enemyTiroArray[0]); i++){
        if(enemyTiroArray[i]){
            enemyTiroArray[i]->Move(diference);


            if(Player.Atingido(i,enemyTiroArray[i])){
                gameWon = false;
                canShowText = true;
            }


            if (!enemyTiroArray[i]->Valido()){
                delete enemyTiroArray[i];
                enemyTiroArray[i] = NULL;
            }
        }
    }
}

void CheckEnemiesCollision() {
    float enemyPosX[7] = {};
    float enemyPosY[7] = {};

    for(int i=0;i<sizeof(enemiesArray)/sizeof(enemiesArray[0]); i++) {
    //for (int i = 0; i < 1; i++) {
        //i = 3;
        Enemy.GetPos(i, enemyPosX[i], enemyPosY[i]);

        currentEnemyBottom = enemyPosY[i] - 3.6; //perna height x 2
        currentEnemyRight = enemyPosX[i] + 2 / 2; //metade da largura do tronco
        currentEnemyLeft = enemyPosX[i] - 2 / 2; //metade da largura do tronco
        currentEnemyTop = enemyPosY[i] + 5.6; //cabeca x 2 + altura do tronco

        int contCollisionBottom = 0;
        int contCollisionRight = 0;
        int contCollisionTop = 0;
        int contCollisionLeft = 0;

        for (Boxes box: boxesArray) {
            if (box.height == 0) break;

            if((int)box.width == 364 && (int)box.height == -91){
                continue;
            }

            float boxLeft = box.xPos;
            float boxRight = box.xPos + box.width;
            float boxTop = box.yPos;
            float boxBottom = box.yPos + box.height;

            //devido a contagem de colisões com varias caixas, apenas uma irá ter uma colisao no topo, se não houver um cont de colisões
            //a proxima verificação de colisão dará false, pois naquela caixa especifica não há o player em cima;


            if (currentEnemyBottom <= boxTop && currentEnemyTop > boxBottom && currentEnemyRight > boxLeft && currentEnemyLeft < boxRight) {
                contCollisionBottom++;
            }

            if (previousEnemyRight[i] < boxLeft && currentEnemyRight >= boxLeft && currentEnemyBottom + 0.32 < boxTop &&
                currentEnemyTop > boxBottom) {
                contCollisionRight++;
            }

            if (previousEnemyLeft[i] > boxRight && currentEnemyLeft <= boxRight && currentEnemyBottom + 0.32 < boxTop &&
                currentEnemyTop > boxBottom) {
                contCollisionLeft++;

            }

            if (previousEnemyTop[i] < boxBottom && currentEnemyTop >= boxBottom && currentEnemyRight > boxLeft &&
                currentEnemyLeft < boxRight) {
                contCollisionTop++;
            }
        }

        if (contCollisionBottom > 0) {
            enemyCollidingBottom[i] = true;
            enemyChangedDirectionNoGround[i] = false;
            enemyReachedGround[i] = true;
        } else {
            enemyCollidingBottom[i] = false;
            previousEnemyBottom[i] = currentEnemyBottom;
        }

        if (contCollisionRight > 0 )
            enemyCollidingRightSide[i] = true;

        else {
            enemyCollidingRightSide[i] = false;
            previousEnemyRight[i] = currentEnemyRight;
        }

        if (contCollisionLeft > 0)
            enemyCollidingLeftSide[i] = true;
        else {
            enemyCollidingLeftSide[i] = false;
            previousEnemyLeft[i] = currentEnemyLeft;
        }

        if (contCollisionTop > 0)
            enemyCollidingTop[i] = true;
        else {
            enemyCollidingTop[i] = false;
            previousEnemyTop[i] = currentEnemyTop;
        }


        if (!enemyCollidingBottom[i]) {

            if (!enemyReachedGround[i]) {
                Enemy.FreeFall(i, yVel);
            } else {
                if (!enemyChangedDirectionNoGround[i]) {
                    enemyChangedDirectionNoGround[i] = true;
                    enemiesArray[i].speed *= -1;
                }

            }
        }
        if (enemyCollidingLeftSide[i] || enemyCollidingRightSide[i]) {
            enemiesArray[i].speed *= -1;
        }
    }
}

void MoveEnemies(GLdouble diference) {

    if(!canEnemiesMove) return;

    for(int i=0; i<sizeof(enemiesArray)/sizeof(enemiesArray[0]); i++) {
        Enemy.MoveEmX(i, enemiesArray[i].speed, diference);
        Enemy.RodaPernaD1(i, enemiesArray[i].speed);
        Enemy.RodaPernaE1(i, enemiesArray[i].speed);
    }
}

void CheckPlayerTiro(GLdouble diference) {

    if(tiro){
        tiro->Move(diference);

        for(int i =0; i<sizeof(enemiesArray)/sizeof(enemiesArray[0]); i++){
            if(Enemy.Atingido(i,tiro)){
                Enemy.SetEnemyVisibility(i,false);
                enemiesArray[i].canBeDrawn = false;
            }
        }

        if (!tiro->Valido()){
            delete tiro;
            tiro = NULL;
        }
    }
}

void MoveCamera() {
    glMatrixMode(GL_PROJECTION); // Select the projection matrix
    glLoadIdentity();

    glTranslatef(camMove/45.9 + 3.427/*offset*/,3.08/*offset*/,0);
    //glTranslatef((camMove/45.9) + 3.427/*offset*/,4/*offset*/,0);
    //glTranslatef(-0.1 /*offset*/,1/*offset*/,0);

    gluOrtho2D(-45.9,45.9,-45.9,45.9);
    //gluOrtho2D(-200,200,-200,200);
    //gluOrtho2D(-100,100,-100,100);

    glMatrixMode(GL_MODELVIEW); // Select the projection matrix
    glLoadIdentity();
}

void CheckPlayerCollision() {
    float playerPosX;
    float playerPosY;
    Player.GetPos(playerPosX,playerPosY);

    currentPlayerBottom = playerPosY - 3.6; //perna height x 2
    currentPlayerRight = playerPosX + 2 / 2; //metade da largura do tronco
    currentPlayerLeft = playerPosX - 2 / 2; //metade da largura do tronco
    currentPlayerTop = playerPosY + 5.6; //cabeca x 2 + altura do tronco

    int contCollisionBottom = 0;
    int contCollisionRight = 0;
    int contCollisionTop = 0;
    int contCollisionLeft = 0;

    for (Boxes box : boxesArray){
        if(box.height == 0) break;

        float boxLeft = box.xPos;
        float boxRight = box.xPos + box.width;
        float boxTop = box.yPos;
        float boxBottom = box.yPos + box.height;

        //devido a contagem de colisões com varias caixas, apenas uma irá ter uma colisao no topo, se não houver um cont de colisões
        //a proxima verificação de colisão dará false, pois naquela caixa especifica não há o player em cima;

        if(previousPlayerBottom > boxTop && currentPlayerBottom <= boxTop && currentPlayerRight > boxLeft && currentPlayerLeft < boxRight){
            Player.ResetJumpDistance();
            contCollisionBottom++;
        }

        if(previousPlayerRight < boxLeft && currentPlayerRight >= boxLeft &&  currentPlayerBottom < boxTop && currentPlayerTop > boxBottom){
            contCollisionRight++;
        }

        if(previousPlayerLeft > boxRight && currentPlayerLeft <= boxRight && currentPlayerBottom < boxTop && currentPlayerTop > boxBottom){
            contCollisionLeft++;
        }

        if(previousPlayerTop < boxBottom && currentPlayerTop >= boxBottom && currentPlayerRight > boxLeft && currentPlayerLeft < boxRight){
            contCollisionTop++;
        }
    }

    for (Enemies enemy : enemiesArray){

        float x;
        float y;
        for(int i=0;i<sizeof(enemiesArray)/sizeof(enemiesArray[0]); i++){
            Enemy.GetPos(i, x,y);
            enemiesArray[i].gX = x;
            enemiesArray[i].gY = y;
        }
        if(!enemy.canBeDrawn) continue;

        float boxLeft = enemy.gX - 2 / 2;  //metade da largura do tronco
        float boxRight = enemy.gX +  2 / 2; //metade da largura do tronco
        float boxTop = enemy.gY + 5.6;;
        float boxBottom = enemy.gY - 3.6; //perna height x 2

        //devido a contagem de colisões com varias caixas, apenas uma irá ter uma colisao no topo, se não houver um cont de colisões
        //a proxima verificação de colisão dará false, pois naquela caixa especifica não há o player em cima;

        if(previousPlayerBottom > boxTop && currentPlayerBottom <= boxTop && currentPlayerRight > boxLeft && currentPlayerLeft < boxRight){
            Player.ResetJumpDistance();
            contCollisionBottom++;
        }

        if(previousPlayerRight < boxLeft && currentPlayerRight >= boxLeft &&  currentPlayerBottom < boxTop && currentPlayerTop > boxBottom){
            contCollisionRight++;
        }

        if(previousPlayerLeft > boxRight && currentPlayerLeft <= boxRight && currentPlayerBottom < boxTop && currentPlayerTop > boxBottom){
            contCollisionLeft++;
        }

        if(previousPlayerTop < boxBottom && currentPlayerTop >= boxBottom && currentPlayerRight > boxLeft && currentPlayerLeft < boxRight){
            contCollisionTop++;
        }
    }

    if(contCollisionBottom > 0)
        playerCollidingBottom = true;
    else{
        playerCollidingBottom = false;
        previousPlayerBottom = currentPlayerBottom;
    }

    if(contCollisionRight > 0)
        playerCollidingOnRightSide = true;

    else{
        playerCollidingOnRightSide = false;
        previousPlayerRight = currentPlayerRight;
    }


    if(contCollisionLeft > 0)
        playerCollidingOnLeftSide = true;
    else{
        playerCollidingOnLeftSide = false;
        previousPlayerLeft = currentPlayerLeft;
    }


    if(contCollisionTop > 0)
        playerCollidingTop = true;
    else{
        playerCollidingTop = false;
        previousPlayerTop = currentPlayerTop;
    }

    if(playerCollidingTop){
        isJumping = false;
    }

    if(!playerCollidingBottom && previousPlayerBottom == currentPlayerBottom && !isJumping){ //gravity?
        if(canUseGravity)
            Player.FreeFall(yVel);
    }
}

void CheckKeyPress(GLdouble diference) {

    double inc = INC_KEYIDLE;
    //Treat keyPress
    if(keyStatus[(int)('a')])
    {
        if(!canPlayerMove) return;

        if(!playerCollidingOnLeftSide){
            camMove += inc * diference;
            Player.MoveEmX(-inc, diference);
            if(playerCollidingBottom){
                Player.RodaPernaD1(-inc);
                Player.RodaPernaE1(-inc);
            }
        }
    }
    if(keyStatus[(int)('d')])
    {
        if(!canPlayerMove) return;

        if(!playerCollidingOnRightSide) {
            camMove -= inc * diference;
            Player.MoveEmX(inc, diference);
            if(playerCollidingBottom){
                Player.RodaPernaD1(inc);
                Player.RodaPernaE1(inc);
            }
        }
    }
}

void passive(int x1,int y1) {

    int centerY = 250;
    int deltaY = y1 - centerY;
    deltaY = deltaY / 3 /* sense inversa*/;
    if(deltaY < -45){
        deltaY = -45;
    }
    if(deltaY > 45){
        deltaY = 45;
    }
    Player.RodaBraco(-deltaY);

}

void MyMouse(int button, int state, int x, int y)
{
    switch (button)
    {
        case GLUT_LEFT_BUTTON:

            if(state == GLUT_UP)
            {
                if(!canPlayerShoot) return;
                if (!tiro)
                    tiro = Player.Atira();
            }
            break;

        case GLUT_RIGHT_BUTTON:
            if(state == GLUT_DOWN){
                if(!canPlayerMove) return;
                pressingJumpKey = true;
                if(playerCollidingBottom){
                    isJumping = true;
                }
            }

            if(state == GLUT_UP){
                if(!canPlayerMove) return;
                pressingJumpKey = false;
                isJumping = false;
            }
            break;

        case GLUT_MIDDLE_BUTTON:
            if(state == GLUT_DOWN){
                if(!playerCollidingBottom)
                    test = true;
            }

            if(state == GLUT_UP){
                test = false;
            }
            break;
    }
}

int main(int argc, char *argv[])
{
    string x;
    cout << "Caminho do SVG:"; // Type a number and press enter
    getline(cin, x);
    Cenario.SetImagePath(x.c_str());
    Enemy.SetImagePath(x.c_str());

    // Initialize openGL with Double buffer and RGB color without transparency.
    // Its interesting to try GLUT_SINGLE instead of GLUT_DOUBLE.
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    // Create the window.
    glutInitWindowSize(Width, Height);
    glutInitWindowPosition(150,150);
    glutCreateWindow("Trab 2D - Lucas Rios");

    // Define callbacks.
    glutDisplayFunc(renderScene);
    glutKeyboardFunc(keyPress);
    glutPassiveMotionFunc(passive);
    glutIdleFunc(idle);
    glutMouseFunc(MyMouse);
    glutKeyboardUpFunc(keyup);

    glutReshapeFunc(init);

    glutMainLoop();

    return 0;
}

