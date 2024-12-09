#ifndef __GAME_SETTINGS_H__
#define __GAME_SETTINGS_H__

class GameSettings
{
public:
    // Volumen de la musica (valor entre 0.0f y 1.0f)
    static float musicVolume;

   
    static void setMusicVolume(float volume) { musicVolume = volume; }
    static float getMusicVolume() { return musicVolume; }
};

#endif // __GAME_SETTINGS_H__
