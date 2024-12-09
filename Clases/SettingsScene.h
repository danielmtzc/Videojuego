#ifndef __SETTINGS_SCENE_H__
#define __SETTINGS_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h" 

class SettingsScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene(); 
    virtual bool init(); 

    CREATE_FUNC(SettingsScene); 

private:
    void Regresar(cocos2d::Ref* pSender);
};

#endif // __SETTINGS_SCENE_H__