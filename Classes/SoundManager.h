/**
 * @file SoundManager
 * @brief 声音管理类
 * @author tjl
 * @date
 */

#ifndef __SoundManager_H_
#define __SoundManager_H_

#include "cocos2d.h"
#include "GameGlobalSetting.h"

USING_NS_CC;

class SoundManager {
public:
    // obviously you know.
    static void playMusicBackground(const char* musicName, bool loop = false);
    static void pauseMusicBackground();
    static void playMusicEffect(const char* musicName, bool loop = false, bool saveSoundId = false);
    static void stopAllMusicEffect();
    static void stopMusicEffect(const char* name);
    static void stopMusicBackground();
    static void resumeMusicBackground();
    static void resumeAllEffects();
    static void pauseAllEffects();
    static bool isBackgroundMusicPlaying();
    
    static void preloadEffect(const char* name);
    static void unloadEffect(const char* name);
    static void preloadBackgroundMusic(const char* name);
    static void unloadBackgroundMusic(const char* name);

	//b:true 开启BGM静音
	static void setBackgroundMusicMute(bool b);
	//b：true 开启音效静音
	static void setSoundEffectMute(bool b);

	static bool isBackgroundMusicMuteOn();

	static bool isSoundEffectMuteOn();
    
    //震动
    static void vibrate();
    //八卦炉
	static void bagualu(int tag);
	//highscore
	static void highScore();


	static void fish(FishKind kind);

	static void playFishEffect(const char* name,float volume =1.0f);
private:
    static std::string getMusicFileName(const char* musicName);
};

#endif //__SoundManager_H_
