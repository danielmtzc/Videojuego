#include "leaderboard.h"
#include "HelloWorldScene.h"
#include <fstream>
#include <algorithm>

USING_NS_CC;

Scene* Leaderboard::createScene()
{
    return Leaderboard::create();
}

bool Leaderboard::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // Establecer el fondo
    background = Sprite::create("Settings .png");
    background->setAnchorPoint(Vec2(0, 0));
    background->setPosition(Vec2(0, 0));
    this->addChild(background, 0);

    
    auto titleLabel = Label::createWithTTF("Leaderboard", "fonts/Marker Felt.ttf", 36);
    titleLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 90));
    titleLabel->setColor(Color3B::WHITE); 
    this->addChild(titleLabel, 1);

    // Cargar puntajes desde el archivo
    loadScoresFromFile();

    // Mostrar los puntajes en la pantalla
    displayScores();

   
    auto backButton = MenuItemLabel::create(
        Label::createWithTTF("Back", "fonts/Marker Felt.ttf", 24),
        [](Ref* sender) {
            Director::getInstance()->replaceScene(HelloWorld::createScene());
        }
    );
    backButton->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 50));

    auto menu = Menu::create(backButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    return true;
}

void Leaderboard::loadScoresFromFile()
{
    std::ifstream file("C:/Users/Danie/Desktop/AstroPilot/Resources/leader.txt");

    scores.clear();

    if (file.is_open())
    {
        int score;
        while (file >> score)
        {
            scores.push_back(score);
        }
        file.close();
    }

   
    std::sort(scores.begin(), scores.end(), std::greater<int>());

    
    if (scores.size() > 10)
    {
        scores.resize(10);
    }
}

void Leaderboard::saveScoresToFile()
{
    std::ofstream file("C:/Users/Danie/Desktop/AstroPilot/Resources/leader.txt");

    if (file.is_open())
    {
        for (int score : scores)
        {
            file << score << "\n";
        }
        file.close();
    }
}

void Leaderboard::displayScores()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    for (size_t i = 0; i < 10; ++i)
    {
        std::string text;

        if (i < scores.size())
        {
            
            text = std::to_string(i + 1) + ". " + std::to_string(scores[i]);
        }
        else
        {
            
            text = std::to_string(i + 1) + ". Vacio";
        }

        auto scoreLabel = Label::createWithTTF(
            text,
            "fonts/Marker Felt.ttf",
            24
        );
        scoreLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 180 - i * 50));
        this->addChild(scoreLabel, 1);
    }
}

void Leaderboard::addScore(int newScore)
{

    loadScoresFromFile();
    
    if (scores.size() < 10 || newScore > scores.back())
    {
        scores.push_back(newScore);

        // Ordenar nuevamente
        std::sort(scores.begin(), scores.end(), std::greater<int>());

        // Mantener solo los 10 puntajes mas altos
        if (scores.size() > 10)
        {
            scores.resize(10);
        }

       
        saveScoresToFile();
    }
}
