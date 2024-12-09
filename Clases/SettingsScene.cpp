#include "SettingsScene.h"
#include "C:\Users\Danie\Desktop\AstroPilot\proj.win32\GameSettings.h"
#include "HelloWorldScene.h"


USING_NS_CC;

Scene* SettingsScene::createScene()
{
    return SettingsScene::create();
}
// detectar errores con la carga de recursos
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool SettingsScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto SettingsFondo = Sprite::create("Settings .png");
    if (SettingsFondo == nullptr)
    {
        problemLoading("'Settings .png'");
    }
    else
    {
        SettingsFondo->setAnchorPoint(Vec2(0, 0));
        SettingsFondo->setPosition(Vec2(0, 0));

       
        this->addChild(SettingsFondo, 0);
    }

    auto volumeLabel = Label::createWithTTF("Volumen", "fonts/Marker Felt.ttf", 24);
    if (volumeLabel == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
        return false;
    }
    else
    {
       
        volumeLabel->setPosition(Vec2(visibleSize.width / 2, ((visibleSize.height / 2)+20)));
        this->addChild(volumeLabel, 1); 
    }

    
    auto nivelLabel = Label::createWithTTF("Nivel: " + std::to_string(static_cast<int>(GameSettings::getMusicVolume() * 100)) + "%",
        "fonts/Marker Felt.ttf", 20);
    nivelLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 100));
    this->addChild(nivelLabel, 1);

   
    auto increaseVolumeItem = MenuItemImage::create(
        "Increase.png",
        "IncreaseS.png",
        [=](Ref* sender) {
            float currentVolume = GameSettings::getMusicVolume();
            if (currentVolume < 1.0f)
            {
                currentVolume += 0.1f;
                if (currentVolume > 1.0f) currentVolume = 1.0f; 
                GameSettings::setMusicVolume(currentVolume);
                nivelLabel->setString("Nivel: " + std::to_string(static_cast<int>(currentVolume * 100)) + "%");
            }
        });

    if (increaseVolumeItem == nullptr ||
        increaseVolumeItem->getContentSize().width <= 0 ||
        increaseVolumeItem->getContentSize().height <= 0)
    {
        problemLoading("'Increase.png' and 'IncreaseS.png'");
    }
    else
    {
        increaseVolumeItem->setPosition(Vec2(visibleSize.width / 2 + 50, visibleSize.height / 2 - 40));
       
    }

    
    auto decreaseVolumeItem = MenuItemImage::create(
        "Decrease.png",
        "DecreaseS.png",
        [=](Ref* sender) {
            float currentVolume = GameSettings::getMusicVolume();
            if (currentVolume > 0.0f)
            {
                currentVolume -= 0.1f;
                if (currentVolume < 0.0f) currentVolume = 0.0f; 
                GameSettings::setMusicVolume(currentVolume);
                nivelLabel->setString("Nivel: " + std::to_string(static_cast<int>(currentVolume * 100)) + "%");
            }
        });

    if (decreaseVolumeItem == nullptr ||
        decreaseVolumeItem->getContentSize().width <= 0 ||
        decreaseVolumeItem->getContentSize().height <= 0)
    {
        problemLoading("'Decrease.png' and 'DecreaseS.png'");
    }
    else
    {
        decreaseVolumeItem->setPosition(Vec2(visibleSize.width / 2 - 50, visibleSize.height / 2 - 40));
    }

    
    auto buttonItem = MenuItemImage::create(
        "btnRegresar1.png",  // Imagen del estado normal
        "btnRegresar1S.png", // Imagen del estado seleccionado
        CC_CALLBACK_1(SettingsScene::Regresar, this)); 

 
    if (buttonItem == nullptr ||
        buttonItem->getContentSize().width <= 0 ||
        buttonItem->getContentSize().height <= 0)
    {
        printf("Error while loading 'btnRegresar1.png' and 'btnRegresar1S.png'\n");
    }
    else {
        buttonItem->setAnchorPoint(Vec2::ZERO);
    }

    auto menu = Menu::create(increaseVolumeItem, decreaseVolumeItem, buttonItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
   

    
    

    return true;
}

void SettingsScene::Regresar(Ref* pSender)
{
    auto MenuRegreso = HelloWorld::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, MenuRegreso, Color3B(0, 0, 0)));
}