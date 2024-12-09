#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include "cocos2d.h"
#include <vector>
#include <string>

class Leaderboard : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();                  
    void loadScoresFromFile();         // Cargar los puntajes desde el archivo
    void saveScoresToFile();           // Guardar los puntajes en el archivo
    void displayScores();              // Mostrar los puntajes en la pantalla
    void addScore(int newScore);       // Agregar un nuevo puntaje al leaderboard

    CREATE_FUNC(Leaderboard);

private:
    std::vector<int> scores; // Vector para almacenar los puntajes
    cocos2d::Sprite* background; // Fondo de la pantalla


};

#endif // LEADERBOARD_H
