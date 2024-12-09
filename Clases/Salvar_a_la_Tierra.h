#ifndef SALVAR_A_LA_TIERRA_H
#define SALVAR_A_LA_TIERRA_H

#include "cocos2d.h"
#include <map>

class Salvar_a_la_Tierra : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    Salvar_a_la_Tierra(); 
    ~Salvar_a_la_Tierra(); 

    virtual bool init(); 

   
    CREATE_FUNC(Salvar_a_la_Tierra);

private:
   
    void lanzarMisil();
    void inicializarMovimiento();
    void update(float delta) override;
    void lanzarMisilEnemigo(cocos2d::Sprite* enemigo);
    void crearEnemigos(float dt);
    void verificarColisiones();
    void verificarColisionesEntreEnemigos();
    void inicializarVidas();
    void perderVida(); 
    void gameOver(); 
    void verificarColisionConNave();
    void actualizarScore(int puntos);
    void Pausa(cocos2d::Ref* pSender);
    void GoToMenu(cocos2d::Ref* pSender);
    void escape_tecla();
    void crearBoss(float dt);
    void dispararMisilBoss(float dt);
    void verificarColisionBoss1();
    void crearBoss2(float dt);
    void dispararMisilBoss2(float dt);
    void verificarColisionBoss2();
    void crearBoss3(float dt);
    void dispararMisilBoss3(float dt);
    void verificarColisionBoss3();
    void juego(float dt);
    
    
    int audioFondo;

   
    cocos2d::Sprite* _nave; // Sprite para la nave
    std::map<cocos2d::EventKeyboard::KeyCode, bool> teclasPresionadas; // Control de teclas
    std::vector<cocos2d::Sprite*> enemigos;
    std::vector<cocos2d::Sprite*> vidasSprites;
    int score; // Puntuacion del jugador
    cocos2d::Label* scoreLabel; // Etiqueta para mostrar el puntaje
    cocos2d::Sprite* fondoActual;


    
    
    cocos2d::Sprite* boss;                       
    int bossVida;                               
    cocos2d::ProgressTimer* bossBar;             
};

#endif // SALVAR_A_LA_TIERRA_H

