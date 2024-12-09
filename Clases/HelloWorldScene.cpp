#include "HelloWorldScene.h"
#include "C:\Users\Danie\Desktop\AstroPilot\proj.win32\GameSettings.h"
#include "C:\Users\Danie\Desktop\AstroPilot\proj.win32\Salvar_a_la_Tierra.h"
#include "C:\Users\Danie\Desktop\AstroPilot\proj.win32\SettingsScene.h"
#include "C:\Users\Danie\Desktop\AstroPilot\proj.win32\leaderboard.h"
#include "audio/include/AudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Para detectar errores de carga
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}


bool HelloWorld::init()
{
    //Comprueba que la escena se inicialize correctamente
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

   //Boton para cerrar la ventana
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    //Boton para iniciar juego 
    auto startGameItem = MenuItemImage::create(
        "StartN.png",
        "StartS.png",
        CC_CALLBACK_1(HelloWorld::startGameCallback, this));

    if (startGameItem == nullptr ||
        startGameItem->getContentSize().width <= 0 ||
        startGameItem->getContentSize().height <= 0)
    {
        problemLoading("'StartN .png' and 'StartS .png'");
    }
    else
    {
        float x = origin.x + visibleSize.width / 2;
        float y = origin.y + visibleSize.height / 2 -100;
        startGameItem->setPosition(Vec2(x, y));
    }

    //Boton para Puntuaciones 
    auto puntuacionesItem = MenuItemImage::create(
        "ScoreN.png",
        "ScoreS.png",
        CC_CALLBACK_1(HelloWorld::scoresCallback, this));

    if (puntuacionesItem == nullptr ||
        puntuacionesItem->getContentSize().width <= 0 ||
        puntuacionesItem->getContentSize().height <= 0)
    {
        problemLoading("'ScoreN.png' and 'ScoreS.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width / 2;
        float y = origin.y + visibleSize.height / 2 - 200;
        puntuacionesItem->setPosition(Vec2(x, y));
    }

    //Boton para Configuracion 
    auto configuracionItem = MenuItemImage::create(
        "ConfiN.png",
        "ConfiS.png",
        CC_CALLBACK_1(HelloWorld::configuracionCallback, this));

    if (configuracionItem == nullptr ||
        configuracionItem->getContentSize().width <= 0 ||
        configuracionItem->getContentSize().height <= 0)
    {
        problemLoading("'ConfiN.png' and 'ConfiS.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width / 2;
        float y = origin.y + visibleSize.height / 2 - 300;
        configuracionItem->setPosition(Vec2(x, y));
    }

    auto menu = Menu::create(closeItem, startGameItem, puntuacionesItem, configuracionItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);



    //Label
    auto label = Label::createWithTTF("AstroPilot", "fonts/Marker Felt.ttf", 50);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
       
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));

       
        this->addChild(label, 1);
    }

    //Label Name 
    auto labelname = Label::createWithTTF("by: Daniel Martinez ", "fonts/Marker Felt.ttf", 25);
    if (labelname == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        
        labelname->setAnchorPoint(Vec2(0, 0));
        labelname->setPosition(Vec2(0,0));
        

       
        this->addChild(labelname, 1);
    }

    
    auto MenuFondo = Sprite::create("Fondo_2.jpg");
    if (MenuFondo == nullptr)
    {
        problemLoading("'Fondo_2.jpg'");
    }
    else
    {
        MenuFondo->setAnchorPoint(Vec2(0, 0));
        MenuFondo->setPosition(Vec2(0,0));

        
        this->addChild(MenuFondo, 0);
    }
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}

void HelloWorld::startGameCallback(Ref* pSender)
{
    auto salvarScene = Salvar_a_la_Tierra::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, salvarScene, Color3B(0, 0, 0)));
}

void HelloWorld::scoresCallback(Ref* pSender)
{
    auto leader = Leaderboard::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, leader, Color3B(0, 0, 0)));
}


void HelloWorld::configuracionCallback(Ref* pSender)
{
    auto SettingsScene = SettingsScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, SettingsScene, Color3B(0, 0, 0)));
}