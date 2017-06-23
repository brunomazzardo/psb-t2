#include <math.h>
#include <chipmunk.h>
#include <SOIL.h>

// Rotinas para acesso da OpenGL
#include "opengl.h"
#include <time.h>



// Funções para movimentação de objetos
void chutaoA(cpBody* body);
void chutaoB(cpBody* body);
void chutaoAtacanteA(cpBody* body);
void chutaoAtacanteB(cpBody* body);
void moveRobo(cpBody* body, void* data);
void moveBola(cpBody* body, void* data);
void moveGoleiroB(cpBody* body, void* data);
void moveGoleiroA(cpBody* body, void* data);
void moveZagueiroA1(cpBody* body, void* data);
void moveZagueiroA2(cpBody* body, void* data);
void moveZagueiroA3(cpBody* body, void* data);
void moveZagueiroB1(cpBody* body, void* data);
void moveZagueiroB2(cpBody* body, void* data);
void moveZagueiroB3(cpBody* body, void* data);
void moveAtacanteA1(cpBody* body, void* data);
void moveAtacanteA2(cpBody* body, void* data);
void moveAtacanteB1(cpBody* body, void* data);
void moveAtacanteB2(cpBody* body, void* data);
int taPerto(int vrau,int target);
int taPertoGoleiro(int vrau,int target);


// Prototipos
void initCM();
void freeCM();
void restartCM();
cpShape* newLine(cpVect inicio, cpVect fim, cpFloat fric, cpFloat elast);
cpBody* newCircle(cpVect pos, cpFloat radius, cpFloat mass, char* img, bodyMotionFunc func, cpFloat fric, cpFloat elast);

// Score do jogo
int score1 = 0;
int score2 = 0;

// Flag de controle: 1 se o jogo tiver acabado
int gameOver = 0;

// cpVect e' um vetor 2D e cpv() e' uma forma rapida de inicializar ele.
cpVect gravity;
cpVect golA;
cpVect golB;
// O ambiente
cpSpace* space;

// Paredes "invisíveis" do ambiente
cpShape* leftWall, *rightWall, *topWall, *bottomWall;

// A bola
cpBody* ballBody;

// Um robô
cpBody* robotBody;

// Cada passo de simulação é 1/60 seg.
cpFloat timeStep = 1.0/120.0;

cpBody* goleiroA;
cpBody* ZagueiroA1;
cpBody* ZagueiroA2;
cpBody* ZagueiroA3;
cpBody* atacanteA1;
cpBody* atacanteA2;
//cpBody* atacanteEsquerdaA;

//Robos do time B
cpBody* goleiroB;
cpBody* ZagueiroB1;
cpBody* ZagueiroB2;
cpBody* ZagueiroB3;
cpBody* atacanteB1;
cpBody* atacanteB2;
//cpBody* atacanteDireitaB;
//cpBody* atacanteEsquerdaB;

// Inicializa o ambiente: é chamada por init() em opengl.c, pois necessita do contexto
// OpenGL para a leitura das imagens
void initCM()
{
    srand(time(NULL));
    gravity = cpv(0, 100);
    golB=cpv(980,355);
    golA=cpv(50,355);

    space = cpSpaceNew();

    cpSpaceSetDamping(space, 0.8);

    leftWall   = newLine(cpv(0,0), cpv(0,ALTURA_JAN), 0, 1.0);
    rightWall  = newLine(cpv(LARGURA_JAN,0), cpv(LARGURA_JAN,ALTURA_JAN), 0, 1.0);
    bottomWall = newLine(cpv(0,0), cpv(LARGURA_JAN,0), 0, 1.0);
    topWall    = newLine(cpv(0,ALTURA_JAN), cpv(LARGURA_JAN,ALTURA_JAN), 0, 1.0);

    // Agora criamos a bola...
    // Os parâmetros são:
    //   - posição: cpVect (vetor: x e y)
    //   - raio
    //   - massa
    //   - imagem a ser carregada
    //   - ponteiro para a função de movimentação (chamada a cada passo, pode ser NULL)
    //   - coeficiente de fricção
    //   - coeficiente de elasticidade
    ballBody = newCircle(cpv(512,360), 8, 1, "small_football.png", moveBola, 0.2, 1);

    // ... e um robô de exemplo

    goleiroA   = newCircle(cpv(60,350), 20, 5, "taffarel.png", moveGoleiroA, 0.2, 0.5);
    goleiroB = newCircle(cpv(800,350), 20, 5, "topete.png", moveGoleiroB, 0.2, 0.5);
    ZagueiroA1= newCircle(cpv(290,200), 20, 5, "kaneman.png", moveZagueiroA1, 0.2, 0.5);
    ZagueiroA2= newCircle(cpv(290,600), 20, 5, "kaneman.png", moveZagueiroA2, 0.2, 0.5);
    ZagueiroA3= newCircle(cpv(350,450), 20, 5, "kaneman.png", moveZagueiroA3, 0.2, 0.5);
    ZagueiroB1= newCircle(cpv(750,200), 20, 5, "geromel.png", moveZagueiroB1, 0.2, 0.5);
    ZagueiroB2= newCircle(cpv(750,600), 20, 5, "geromel.png", moveZagueiroB2, 0.2, 0.5);
    ZagueiroB3= newCircle(cpv(700,450), 20, 5, "geromel.png", moveZagueiroB3, 0.2, 0.5);
    atacanteA1=newCircle(cpv(400,450), 20, 5, "luan.png", moveAtacanteA1, 0.2, 0.5);
    atacanteA2=newCircle(cpv(400,150), 20, 5, "luan.png", moveAtacanteA2, 0.2, 0.5);
    atacanteB1=newCircle(cpv(624,450), 20, 5, "balaka.png", moveAtacanteB1, 0.2, 0.5);
    atacanteB2=newCircle(cpv(622,250), 20, 5, "balaka.png", moveAtacanteB2, 0.2, 0.5);
}
void updateScore(int lado){
    if(lado == 1){
        score1++;
    }else{
        score2++;
    }
}
void chutaoA(cpBody* body){
    cpVect robotPos = cpBodyGetPosition(body);
    cpVect  tempGol=golB;
    tempGol.y=rand()%750;
    cpVect delta = cpvadd(tempGol,robotPos);
    delta = cpvmult(cpvnormalize(delta),38);
    cpBodyApplyImpulseAtWorldPoint(body, delta, cpBodyGetPosition(body));
}
void chutaoB(cpBody* body){
    cpVect ball = cpBodyGetPosition(body);
    ball.x=-ball.x;
    ball.y=-ball.y;
    cpVect tempGol=golA;
    tempGol.y=rand()%750;
    cpVect delta = cpvadd(tempGol,ball);
    delta = cpvmult(cpvnormalize(delta),38);
    cpBodyApplyImpulseAtWorldPoint(body, delta, cpBodyGetPosition(body));
}

void chutaoAtacanteA(cpBody* body){

     cpVect ball = cpBodyGetPosition(body);
         ball.x=-ball.x;
     ball.y=-ball.y;
     cpVect delta = cpvadd(ball,golB);
     delta = cpvmult(cpvnormalize(delta),30);
     cpBodyApplyImpulseAtWorldPoint(body, delta, cpBodyGetPosition(body));
}
void chutaoAtacanteB(cpBody* body){
     cpVect ball = cpBodyGetPosition(body);
     ball.x=-ball.x;
     ball.y=-ball.y;
     cpVect delta = cpvadd(ball,golA);
     delta = cpvmult(cpvnormalize(delta),30);
     cpBodyApplyImpulseAtWorldPoint(body, delta, cpBodyGetPosition(body));
}

void moveGoleiroA(cpBody* body, void* data)
{
    cpVect vel = cpBodyGetVelocity(body);
    vel = cpvclamp(vel, 50);
    cpBodySetVelocity(body, vel);
    cpVect robotPos = cpBodyGetPosition(body);
    cpVect ballPos  = cpBodyGetPosition(ballBody);
    //chutao(robotBody,1);
      if(taPertoGoleiro(robotPos.x,ballPos.x)==1&& taPertoGoleiro(robotPos.y,ballPos.y)==1){
        chutaoA(ballBody);

    }
    if(robotPos.x>=155){

        cpVect pos = robotPos;
        pos.x = -robotPos.x;
        pos.y = -robotPos.y;
        ballPos.x = -ballPos.x;
        cpVect delta = cpvadd(ballPos,pos);


        delta = cpvmult(cpvnormalize(delta),20);

        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);

    }else if(robotPos.y <= 215)
    {
        cpVect pos = robotPos;
        pos.x = robotPos.x;
        pos.y = robotPos.y;
        cpVect delta = cpvadd(ballPos,pos);

        delta = cpvmult(cpvnormalize(delta),20);

        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);
    }
    else if(robotPos.y >= 500)
    {
        cpVect pos = robotPos;
        pos.x = -robotPos.x;
        pos.y = -robotPos.y;
        ballPos.y = -ballPos.y;
        cpVect delta = cpvadd(ballPos,pos);

        delta = cpvmult(cpvnormalize(delta),20);

        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);


    }else{

        cpVect pos = robotPos;
        pos.x = -robotPos.x;
        pos.y = -robotPos.y;
        cpVect delta = cpvadd(ballPos,pos);
        delta = cpvmult(cpvnormalize(delta),50);
        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);
    }



}

void moveGoleiroB(cpBody* body, void* data)
{
    cpVect vel = cpBodyGetVelocity(body);
    vel = cpvclamp(vel, 50);
    cpBodySetVelocity(body, vel);
    cpVect robotPos = cpBodyGetPosition(body);
    cpVect ballPos  = cpBodyGetPosition(ballBody);
      if(taPertoGoleiro(robotPos.x,ballPos.x)==1&& taPertoGoleiro(robotPos.y,ballPos.y)==1)
        chutaoB(ballBody);
      //chutao(robotBody,2);
    if(robotPos.x<875){

        cpVect pos = robotPos;
        pos.x = robotPos.x;
        pos.y = robotPos.y;
        ballPos.x = ballPos.x;
        cpVect delta = cpvadd(ballPos,pos);


        delta = cpvmult(cpvnormalize(delta),20);

        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);

    }else if(robotPos.y <= 215)
    {
        cpVect pos = robotPos;
        pos.x = robotPos.x;
        pos.y = robotPos.y;
        cpVect delta = cpvadd(ballPos,pos);

        delta = cpvmult(cpvnormalize(delta),20);

        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);
    }
    else if(robotPos.y >= 500)
    {
        cpVect pos = robotPos;
        pos.x = -robotPos.x;
        pos.y = -robotPos.y;
        ballPos.y = -ballPos.y;
        cpVect delta = cpvadd(ballPos,pos);

        delta = cpvmult(cpvnormalize(delta),20);

        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);


    }else{

        cpVect pos = robotPos;
        pos.x = -robotPos.x;
        pos.y = -robotPos.y;
        cpVect delta = cpvadd(ballPos,pos);
        delta = cpvmult(cpvnormalize(delta),50);
        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);
    }



}
void moveZagueiroA1(cpBody* body, void* data){
    cpVect vel = cpBodyGetVelocity(body);
    vel = cpvclamp(vel, 50);
    cpBodySetVelocity(body, vel);
    cpVect robotPos = cpBodyGetPosition(body);
    cpVect ballPos  = cpBodyGetPosition(ballBody);
        cpVect ata=robotPos;

    if(taPerto(robotPos.x,ballPos.x)==1&& taPerto(robotPos.y,ballPos.y)==1){
        chutaoA(ballBody);
    }

    if(robotPos.x>=512){
        cpVect pos = robotPos;
        pos.x = -robotPos.x;
        pos.y = -robotPos.y;
        ballPos.x = -ballPos.x;
        cpVect delta = cpvadd(ballPos,pos);
        delta = cpvmult(cpvnormalize(delta),40);
        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);

    }else if(robotPos.y >= 280){
        cpVect pos = robotPos;
        pos.x = -robotPos.x;
        pos.y = -robotPos.y;
        ballPos.y = -ballPos.y;
        cpVect delta = cpvadd(ballPos,pos);
        delta = cpvmult(cpvnormalize(delta),40);
        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);
    }else{

        cpVect pos = robotPos;
        pos.x = -robotPos.x;
        pos.y = -robotPos.y;
        cpVect delta = cpvadd(ballPos,pos);
        delta = cpvmult(cpvnormalize(delta),50);
        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);
    }

}
void moveZagueiroB1(cpBody* body, void* data){
    cpVect vel = cpBodyGetVelocity(body);
    vel = cpvclamp(vel, 50);
    cpBodySetVelocity(body, vel);
    cpVect robotPos = cpBodyGetPosition(body);
    cpVect ballPos  = cpBodyGetPosition(ballBody);
    if(taPerto(robotPos.x,ballPos.x)==1&& taPerto(robotPos.y,ballPos.y)==1)
        chutaoB(ballBody);

    if(robotPos.x<=512){
        cpVect pos = robotPos;
        pos.x = robotPos.x;
        pos.y = robotPos.y;
        ballPos.x = ballPos.x;
        cpVect delta = cpvadd(ballPos,pos);
        delta = cpvmult(cpvnormalize(delta),40);
        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);

    }else if(robotPos.y >= 280){
        cpVect pos = robotPos;
        pos.x = -robotPos.x;
        pos.y = -robotPos.y;
        ballPos.y =- ballPos.y;
        cpVect delta = cpvadd(ballPos,pos);
        delta = cpvmult(cpvnormalize(delta),40);
        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);
    }else{

        cpVect pos = robotPos;
        pos.x = -robotPos.x;
        pos.y = -robotPos.y;
        cpVect delta = cpvadd(ballPos,pos);
        delta = cpvmult(cpvnormalize(delta),20);
        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);
    }

}
void moveZagueiroA2(cpBody* body, void* data){
     cpVect vel = cpBodyGetVelocity(body);
    vel = cpvclamp(vel, 50);
    cpBodySetVelocity(body, vel);
    cpVect robotPos = cpBodyGetPosition(body);
    cpVect ballPos  = cpBodyGetPosition(ballBody);
    if(taPerto(robotPos.x,ballPos.x)==1&& taPerto(robotPos.y,ballPos.y)==1)
        chutaoA(ballBody);
    if(robotPos.x>=512){
        cpVect pos = robotPos;
        pos.x = -robotPos.x;
        pos.y = -robotPos.y;
        ballPos.x = -ballPos.x;
        cpVect delta = cpvadd(ballPos,pos);
        delta = cpvmult(cpvnormalize(delta),40);
        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);

    }else if(robotPos.y < 500){
        cpVect pos = robotPos;
        pos.x = robotPos.x;
        pos.y = robotPos.y;
        ballPos.y = ballPos.y;
        cpVect delta = cpvadd(ballPos,pos);
        delta = cpvmult(cpvnormalize(delta),40);
        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);
    }else{

        cpVect pos = robotPos;
        pos.x = -robotPos.x;
        pos.y = -robotPos.y;
        cpVect delta = cpvadd(ballPos,pos);
        delta = cpvmult(cpvnormalize(delta),20);
        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);
    }
}
void moveZagueiroB2(cpBody* body, void* data){
     cpVect vel = cpBodyGetVelocity(body);
    vel = cpvclamp(vel, 50);
    cpBodySetVelocity(body, vel);
    cpVect robotPos = cpBodyGetPosition(body);
    cpVect ballPos  = cpBodyGetPosition(ballBody);
    if(taPerto(robotPos.x,ballPos.x)==1&& taPerto(robotPos.y,ballPos.y)==1)
        chutaoB(ballBody);
    if(robotPos.x<=512){
        cpVect pos = robotPos;
        pos.x = robotPos.x;
        pos.y = robotPos.y;
        ballPos.x = ballPos.x;
        cpVect delta = cpvadd(ballPos,pos);
        delta = cpvmult(cpvnormalize(delta),40);
        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);

    }else if(robotPos.y < 200){
        cpVect pos = robotPos;
        pos.x = robotPos.x;
        pos.y = robotPos.y;
        ballPos.y = ballPos.y;
        cpVect delta = cpvadd(ballPos,pos);
        delta = cpvmult(cpvnormalize(delta),40);
        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);
    }else{

        cpVect pos = robotPos;
        pos.x = -robotPos.x;
        pos.y = -robotPos.y;
        cpVect delta = cpvadd(ballPos,pos);
        delta = cpvmult(cpvnormalize(delta),50);
        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);
    }
}
void moveZagueiroA3(cpBody* body, void* data){
       cpVect vel = cpBodyGetVelocity(body);
    vel = cpvclamp(vel, 50);
    cpBodySetVelocity(body, vel);
    cpVect robotPos = cpBodyGetPosition(body);
    cpVect ballPos  = cpBodyGetPosition(ballBody);
    if(taPerto(robotPos.x,ballPos.x)==1&& taPerto(robotPos.y,ballPos.y)==1)
        chutaoA(ballBody);
    if(robotPos.x>=512){
        cpVect pos = robotPos;
        pos.x = -robotPos.x;
        pos.y = -robotPos.y;
        ballPos.x = -ballPos.x;
        cpVect delta = cpvadd(ballPos,pos);
        delta = cpvmult(cpvnormalize(delta),40);
        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);

    }else if(robotPos.y < 300){
        cpVect pos = robotPos;
        pos.x = robotPos.x;
        pos.y = robotPos.y;
        ballPos.y = ballPos.y;
        cpVect delta = cpvadd(ballPos,pos);
        delta = cpvmult(cpvnormalize(delta),40);
        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);
        }else if(robotPos.y > 600){
             cpVect pos = robotPos;
        pos.x = -robotPos.x;
        pos.y = -robotPos.y;
        ballPos.y = -ballPos.y;
        cpVect delta = cpvadd(ballPos,pos);
        delta = cpvmult(cpvnormalize(delta),40);
        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);


  }else{

        cpVect pos = robotPos;
        pos.x = -robotPos.x;
        pos.y = -robotPos.y;
        cpVect delta = cpvadd(ballPos,pos);
        delta = cpvmult(cpvnormalize(delta),20);
        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);
    }
}
void moveZagueiroB3(cpBody* body, void* data){
       cpVect vel = cpBodyGetVelocity(body);
    vel = cpvclamp(vel, 50);
    cpBodySetVelocity(body, vel);
    cpVect robotPos = cpBodyGetPosition(body);
    cpVect ballPos  = cpBodyGetPosition(ballBody);
    if(taPerto(robotPos.x,ballPos.x)==1&& taPerto(robotPos.y,ballPos.y)==1)
        chutaoB(ballBody);
    if(robotPos.x<=512){
        cpVect pos = robotPos;
        pos.x = robotPos.x;
        pos.y = robotPos.y;
        ballPos.x = ballPos.x;
        cpVect delta = cpvadd(ballPos,pos);
        delta = cpvmult(cpvnormalize(delta),60);
        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);

    }else if(robotPos.y < 360){
        cpVect pos = robotPos;
        pos.x = robotPos.x;
        pos.y = robotPos.y;
        ballPos.y = ballPos.y;
        cpVect delta = cpvadd(ballPos,pos);
        delta = cpvmult(cpvnormalize(delta),60);
        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);
        }else if(robotPos.y > 480){
             cpVect pos = robotPos;
        pos.x = -robotPos.x;
        pos.y =- robotPos.y;
        ballPos.y = -ballPos.y;
        cpVect delta = cpvadd(ballPos,pos);
        delta = cpvmult(cpvnormalize(delta),60);
        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);


  }else{

        cpVect pos = robotPos;
        pos.x = -robotPos.x;
        pos.y = -robotPos.y;
        cpVect delta = cpvadd(ballPos,pos);
        delta = cpvmult(cpvnormalize(delta),20);
        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);
    }
}
void moveAtacanteA1(cpBody* body, void* data){
  cpVect vel = cpBodyGetVelocity(body);
    vel = cpvclamp(vel, 50);
    cpBodySetVelocity(body, vel);
    cpVect robotPos = cpBodyGetPosition(body);
    cpVect ballPos  = cpBodyGetPosition(ballBody);
    if(robotPos.x<=700){
        cpVect pos = robotPos;
        pos.x = robotPos.x;
        pos.y = robotPos.y;
        ballPos.x = ballPos.x;
        cpVect delta = cpvadd(ballPos,pos);
        delta = cpvmult(cpvnormalize(delta),40);
        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);

    }else if(robotPos.y < 300){
             cpVect pos = robotPos;
        pos.x = robotPos.x;
        pos.y = robotPos.y;
        ballPos.y = ballPos.y;
        cpVect delta = cpvadd(ballPos,pos);
        delta = cpvmult(cpvnormalize(delta),40);
        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);


  }else{
        cpVect pos = robotPos;
        pos.x = -robotPos.x;
        pos.y = -robotPos.y;
        cpVect delta = cpvadd(ballPos,pos);
        delta = cpvmult(cpvnormalize(delta),20);
        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);
    }
    if(taPerto(robotPos.x,ballPos.x)==1&& taPerto(robotPos.y,ballPos.y)==1)
        chutaoAtacanteA(ballBody);
}
void moveAtacanteA2(cpBody* body, void* data){
  cpVect vel = cpBodyGetVelocity(body);
    vel = cpvclamp(vel, 50);
    cpBodySetVelocity(body, vel);
    cpVect robotPos = cpBodyGetPosition(body);
    cpVect ballPos  = cpBodyGetPosition(ballBody);
    if(robotPos.x<=700){
        cpVect pos = robotPos;
        pos.x = robotPos.x;
        pos.y = robotPos.y;
        ballPos.x = ballPos.x;
        cpVect delta = cpvadd(ballPos,pos);
        delta = cpvmult(cpvnormalize(delta),40);
        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);

    }else if(robotPos.y > 350){
             cpVect pos = robotPos;
        pos.x = -robotPos.x;
        pos.y =-robotPos.y;
        ballPos.y = -ballPos.y;
        cpVect delta = cpvadd(ballPos,pos);
        delta = cpvmult(cpvnormalize(delta),40);
        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);


  }else{
        cpVect pos = robotPos;
        pos.x = -robotPos.x;
        pos.y = -robotPos.y;
        cpVect delta = cpvadd(ballPos,pos);
        delta = cpvmult(cpvnormalize(delta),20);
        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);
    }
    if(taPerto(robotPos.x,ballPos.x)==1&& taPerto(robotPos.y,ballPos.y)==1)
        chutaoAtacanteA(ballBody);
}
void moveAtacanteB1(cpBody* body, void* data){
  cpVect vel = cpBodyGetVelocity(body);
    vel = cpvclamp(vel, 50);
    cpBodySetVelocity(body, vel);
    cpVect robotPos = cpBodyGetPosition(body);
    cpVect ballPos  = cpBodyGetPosition(ballBody);
    if(robotPos.x>=300){
        cpVect pos = robotPos;
        pos.x = -robotPos.x;
        pos.y = -robotPos.y;
        ballPos.x = -ballPos.x;
        cpVect delta = cpvadd(ballPos,pos);
        delta = cpvmult(cpvnormalize(delta),40);
        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);

    }else if(robotPos.y < 300){
             cpVect pos = robotPos;
        pos.x = robotPos.x;
        pos.y =robotPos.y;
        ballPos.y = ballPos.y;
        cpVect delta = cpvadd(ballPos,pos);
        delta = cpvmult(cpvnormalize(delta),40);
        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);


  }else{
        cpVect pos = robotPos;
        pos.x = -robotPos.x;
        pos.y = -robotPos.y;
        cpVect delta = cpvadd(ballPos,pos);
        delta = cpvmult(cpvnormalize(delta),20);
        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);
    }
    if(taPerto(robotPos.x,ballPos.x)==1&& taPerto(robotPos.y,ballPos.y)==1)
        chutaoAtacanteB(ballBody);
}
void moveAtacanteB2(cpBody* body, void* data){
  cpVect vel = cpBodyGetVelocity(body);
    vel = cpvclamp(vel, 50);
    cpBodySetVelocity(body, vel);
    cpVect robotPos = cpBodyGetPosition(body);
    cpVect ballPos  = cpBodyGetPosition(ballBody);
    if(robotPos.x>=300){
        cpVect pos = robotPos;
        pos.x = -robotPos.x;
        pos.y = -robotPos.y;
        ballPos.x = -ballPos.x;
        cpVect delta = cpvadd(ballPos,pos);
        delta = cpvmult(cpvnormalize(delta),40);
        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);

    }else if(robotPos.y > 350){
             cpVect pos = robotPos;
        pos.x = -robotPos.x;
        pos.y =-robotPos.y;
        ballPos.y =- ballPos.y;
        cpVect delta = cpvadd(ballPos,pos);
        delta = cpvmult(cpvnormalize(delta),40);
        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);


  }else{
        cpVect pos = robotPos;
        pos.x = -robotPos.x;
        pos.y = -robotPos.y;
        cpVect delta = cpvadd(ballPos,pos);
        delta = cpvmult(cpvnormalize(delta),20);
        cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);
    }
    if(taPerto(robotPos.x,ballPos.x)==1&& taPerto(robotPos.y,ballPos.y)==1)
        chutaoAtacanteB(ballBody);
}


int taPerto(int vrau,int target){
   for(int i=-30;i<30;i++){
    if(vrau+i==target){

        return 1;
      }
   }
    return 0;
}
int taPertoGoleiro(int vrau,int target){
   for(int i=-30;i<30;i++){
    if(vrau+i==target){

        return 1;
      }
   }
    return 0;
}



// Exemplo de função de movimentação: move o robô em direção à bola
void moveRobo(cpBody* body, void* data)
{
    // Veja como obter e limitar a velocidade do robô...
    cpVect vel = cpBodyGetVelocity(body);
//    printf("vel: %f %f", vel.x,vel.y);

    // Limita o vetor em 50 unidades
    vel = cpvclamp(vel, 50);
    // E seta novamente a velocidade do corpo
    cpBodySetVelocity(body, vel);

    // Obtém a posição do robô e da bola...
    cpVect robotPos = cpBodyGetPosition(body);
    cpVect ballPos  = cpBodyGetPosition(ballBody);

    // Calcula um vetor do robô à bola (DELTA = B - R)
    cpVect pos = robotPos;
    pos.x = -robotPos.x;
    pos.y = -robotPos.y;
    cpVect delta = cpvadd(ballPos,pos);

    // Limita o impulso em 20 unidades
    delta = cpvmult(cpvnormalize(delta),20);
    // Finalmente, aplica impulso no robô
    cpBodyApplyImpulseAtWorldPoint(body, delta, robotPos);
}

// Exemplo: move a bola aleatoriamente
void moveBola(cpBody* body, void* data)
{
    // Sorteia um impulso entre -10 e 10, para x e y
if(score1>2|| score2>2)
    gameOver=1;



     cpVect ballPos  = cpBodyGetPosition(ballBody);
     if(ballPos.x <= 50 && ballPos.y >= 325 && ballPos.y <= 385){
        updateScore(2);
        resetPositions();
    }else if(ballPos.x >= 975 && ballPos.y >= 325 && ballPos.y <= 385){
        updateScore(1);
        resetPositions();
    }
}

// Libera memória ocupada por cada corpo, forma e ambiente
// Acrescente mais linhas caso necessário
void freeCM()
{
    printf("Cleaning up!\n");
    UserData* ud = cpBodyGetUserData(ballBody);
    cpShapeFree(ud->shape);
    cpBodyFree(ballBody);

    ud = cpBodyGetUserData(robotBody);
    cpShapeFree(ud->shape);
    cpBodyFree(robotBody);

    cpShapeFree(leftWall);
    cpShapeFree(rightWall);
    cpShapeFree(bottomWall);
    cpShapeFree(topWall);

    cpSpaceFree(space);
}

void resetPositions(){
     cpBodySetVelocity(ballBody,cpv(0,0));
     cpBodySetVelocity(goleiroA,cpv(0,0));
     cpBodySetVelocity(ZagueiroA1,cpv(0,0));
     cpBodySetVelocity(ZagueiroA2,cpv(0,0));
     cpBodySetVelocity(ZagueiroA3,cpv(0,0));
     cpBodySetVelocity(atacanteA1,cpv(0,0));
     cpBodySetVelocity(atacanteA2,cpv(0,0));
     cpBodySetVelocity(goleiroB,cpv(0,0));
     cpBodySetVelocity(ZagueiroB1,cpv(0,0));
     cpBodySetVelocity(ZagueiroB2,cpv(0,0));
     cpBodySetVelocity(ZagueiroB3,cpv(0,0));
     cpBodySetVelocity(atacanteB1,cpv(0,0));
     cpBodySetVelocity(atacanteB2,cpv(0,0));
     cpVect pos = cpBodyGetPosition(ballBody);
     pos.x = 512;
     pos.y = 350;
     cpBodySetPosition(ballBody, pos);

     pos = cpBodyGetPosition(goleiroA);
     pos.x = 60;
     pos.y = 350;
     cpBodySetPosition(goleiroA, pos);

     pos = cpBodyGetPosition(goleiroB);
     pos.x = 900;
     pos.y = 350;
     cpBodySetPosition(goleiroB, pos);

     pos = cpBodyGetPosition(atacanteA1);
     pos.x = 400;
     pos.y = 450;
     cpBodySetPosition(atacanteA1, pos);

     pos = cpBodyGetPosition(ZagueiroA1);
     pos.x = 290;
     pos.y = 200;
     cpBodySetPosition(ZagueiroA1, pos);


     pos = cpBodyGetPosition(ZagueiroA2);
     pos.x = 290;
     pos.y = 600;
     cpBodySetPosition(ZagueiroA2, pos);

     pos = cpBodyGetPosition(ZagueiroA3);
     pos.x = 350;
     pos.y = 450;
     cpBodySetPosition(ZagueiroA3, pos);
       pos = cpBodyGetPosition(ZagueiroB1);
     pos.x = 750;
     pos.y = 200;
     cpBodySetPosition(ZagueiroB1, pos);


     pos = cpBodyGetPosition(ZagueiroB2);
     pos.x = 750;
     pos.y = 600;
     cpBodySetPosition(ZagueiroB2, pos);

     pos = cpBodyGetPosition(ZagueiroB3);
     pos.x = 700;
     pos.y = 450;
     cpBodySetPosition(ZagueiroB3, pos);

      pos = cpBodyGetPosition(atacanteA2);
     pos.x = 400;
     pos.y = 150;
     cpBodySetPosition(atacanteA2, pos);

       pos = cpBodyGetPosition(atacanteB1);
     pos.x = 624;
     pos.y = 450;
     cpBodySetPosition(atacanteB1, pos);

     pos = cpBodyGetPosition(atacanteB2);
     pos.x = 622;
     pos.y = 250;
     cpBodySetPosition(atacanteB2, pos);

 }
// Função chamada para reiniciar a simulação
void restartCM()
{
    resetPositions();
    score1=0;
    score2=0;
    // Escreva o código para reposicionar os jogadores, ressetar o score, etc.

    // Não esqueça de ressetar a variável gameOver!
    gameOver = 0;
}

// ************************************************************
//
// A PARTIR DESTE PONTO, O PROGRAMA NÃO DEVE SER ALTERADO
//
// A NÃO SER QUE VOCÊ SAIBA ***EXATAMENTE*** O QUE ESTÁ FAZENDO
//
// ************************************************************

int main(int argc, char** argv)
{
    // Inicialização da janela gráfica
    init(argc,argv);

    // Não retorna... a partir daqui, interação via teclado e mouse apenas, na janela gráfica
    glutMainLoop();
    return 0;
}

// Cria e adiciona uma nova linha estática (segmento) ao ambiente
cpShape* newLine(cpVect inicio, cpVect fim, cpFloat fric, cpFloat elast)
{
   cpShape* aux = cpSegmentShapeNew(cpSpaceGetStaticBody(space), inicio, fim, 0);
   cpShapeSetFriction(aux, fric);
   cpShapeSetElasticity(aux, elast);
   cpSpaceAddShape(space, aux);
   return aux;
}

// Cria e adiciona um novo corpo dinâmico, com formato circular
cpBody* newCircle(cpVect pos, cpFloat radius, cpFloat mass, char* img, bodyMotionFunc func, cpFloat fric, cpFloat elast)
{
    // Primeiro criamos um cpBody para armazenar as propriedades fisicas do objeto
    // Estas incluem: massa, posicao, velocidade, angulo, etc do objeto
    // A seguir, adicionamos formas de colisao ao cpBody para informar o seu formato e tamanho

    // O momento de inercia e' como a massa, mas para rotacao
    // Use as funcoes cpMomentFor*() para calcular a aproximacao dele
    cpFloat moment = cpMomentForCircle(mass, 0, radius, cpvzero);

    // As funcoes cpSpaceAdd*() retornam o que voce esta' adicionando
    // E' conveniente criar e adicionar um objeto na mesma linha
    cpBody* newBody = cpSpaceAddBody(space, cpBodyNew(mass, moment));

    // Por fim, ajustamos a posicao inicial do objeto
    cpBodySetPosition(newBody, pos);

    // Agora criamos a forma de colisao do objeto
    // Voce pode criar multiplas formas de colisao, que apontam ao mesmo objeto (mas nao e' necessario para o trabalho)
    // Todas serao conectadas a ele, e se moverao juntamente com ele
    cpShape* newShape = cpSpaceAddShape(space, cpCircleShapeNew(newBody, radius, cpvzero));
    cpShapeSetFriction(newShape, fric);
    cpShapeSetElasticity(newShape, elast);

    UserData* newUserData = malloc(sizeof(UserData));
    newUserData->tex = loadImage(img);
    newUserData->radius = radius;
    newUserData->shape= newShape;
    newUserData->func = func;
    cpBodySetUserData(newBody, newUserData);
    printf("newCircle: loaded img %s\n", img);
    return newBody;
}
