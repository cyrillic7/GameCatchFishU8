#include "SoundManager.h"
#include "SimpleAudioEngine.h"
#include "SessionManager.h"
#include "GameGlobalSetting.h"
#include "audio/include/AudioEngine.h"
#include "CommonFunction.h"


#define  kFireVolume 0.3f

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#import <AudioToolbox/AudioToolbox.h>
#endif

using namespace CocosDenshion;
using namespace cocos2d::experimental;

static __Dictionary* soundIdSaver = NULL;

void SoundManager::playMusicBackground(char const *musicName, bool loop) {

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    if (NULL != musicName) {
		SessionManager::shareInstance()->setMusicName(musicName);
        if (SessionManager::shareInstance()->getMusicOn()) {
            SimpleAudioEngine::sharedEngine()->playBackgroundMusic(musicName, loop);
        }
    }
#endif
}

void SoundManager::pauseMusicBackground() {
    //SessionManager::sharedManager()->setPauseMusic(true);
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

void SoundManager::playMusicEffect(char const *musicName, bool loop, bool saveSoundId) {

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    if (NULL != musicName) {
        if (SessionManager::shareInstance()->getEffectOn()) {
            int soundId = SimpleAudioEngine::sharedEngine()->playEffect(musicName, loop);
            if (saveSoundId) {
                if (NULL == soundIdSaver) {
                    soundIdSaver = __Dictionary::create();
                    soundIdSaver->retain();
                }
                soundIdSaver->setObject(__Integer::create(soundId), musicName);
            }
        }
    }
#endif

}

void SoundManager::stopAllMusicEffect() {
    SimpleAudioEngine::sharedEngine()->stopAllEffects();
}

void SoundManager::stopMusicEffect(char const *name) {
    if (NULL != name && NULL != soundIdSaver) {
        __Integer* soundId = (__Integer*)soundIdSaver->objectForKey(name);
        if (NULL != soundId) {
            SimpleAudioEngine::sharedEngine()->stopEffect(soundId->getValue());
            soundIdSaver->removeObjectForKey(name);
        }
    }
}

void SoundManager::resumeMusicBackground() {
	log("resumeMusicBackground %s",SessionManager::shareInstance()->getMusicName().c_str());
  SimpleAudioEngine::sharedEngine()->playBackgroundMusic(SessionManager::shareInstance()->getMusicName().c_str(), true);
}

void SoundManager::resumeAllEffects() {
    SessionManager::shareInstance()->setEffectOn(1);
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}

void SoundManager::stopMusicBackground() {
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}

void SoundManager::pauseAllEffects() {
    SessionManager::shareInstance()->setEffectOn(0);
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

bool SoundManager::isBackgroundMusicPlaying() {
    return SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying();
}

void SoundManager::setBackgroundMusicMute(bool b)
{
	static int musicVolume = 0;
	if (b == true)
	{
		int tempVol = SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume();
		if (tempVol > 0)
		{
			musicVolume = tempVol;
			SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0);
		}
	}
	else
	{
		int tempVol = SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume();
		if (tempVol == 0)
		{
			SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(musicVolume);
		}
	}
}

void SoundManager::setSoundEffectMute(bool b)
{
	static int soundVolume = 0;
	if (b == true)
	{
		int tempVol = SimpleAudioEngine::sharedEngine()->getEffectsVolume();
		if (tempVol > 0)
		{
			soundVolume = tempVol;
			SimpleAudioEngine::sharedEngine()->setEffectsVolume(0);
		}
	}
	else
	{
		int tempVol = SimpleAudioEngine::sharedEngine()->getEffectsVolume();
		if (tempVol == 0)
		{
			SimpleAudioEngine::sharedEngine()->setEffectsVolume(soundVolume);
		}
	}
}

bool SoundManager::isBackgroundMusicMuteOn()
{
	return (SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume() == 0);
}

bool SoundManager::isSoundEffectMuteOn()
{
	return (SimpleAudioEngine::sharedEngine()->getEffectsVolume() == 0);
}

void SoundManager::vibrate() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //TODO
#endif
}

void SoundManager::preloadEffect(const char* name) {
    SimpleAudioEngine::sharedEngine()->preloadEffect(name);
}

void SoundManager::unloadEffect(const char* name) {
    SimpleAudioEngine::sharedEngine()->unloadEffect(name);
}

void SoundManager::preloadBackgroundMusic(const char* name) {
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(name);
}


void SoundManager::bagualu(int tag)
{
	if (tag >= 3)
	{
		return;
	}
	switch (tag)
	{
	case Bglu_Type_Ta:
		playMusicEffect("audios/jgtzi.mp3");
		break;
	case Bglu_Type_Lua:
		playMusicEffect("audios/fhlzi.mp3");
		break;
	case Bglu_Type_Bang:
		playMusicEffect("audios/ryjgbzi.mp3");
		break;
	}
}

void SoundManager::highScore()
{
	int index =  rand()%5 ;
	//playMusicEffect(__String::createWithFormat("audios/highscore%02d.mp3",index+1)->getCString());
	playFishEffect(__String::createWithFormat("highscore%02d.mp3",index+1)->getCString());
}

void SoundManager::fish(FishKind kind)
{
	std::string name = "";
	int index = rand()%2 + 1;
	int gameKindId = SessionManager::shareInstance()->getGameKind();
	if(gameKindId == GameKindLkpy)
	{
		if(kind >= FISH_KIND_10 && kind <= FISH_KIND_17)
		{
			//name = __String::createWithFormat("audios_lk/fish%d_%d.mp3",kind - FISH_KIND_1,index)->getCString();
			//playMusicEffect(name.c_str());
			playFishEffect(__String::createWithFormat("fish%d_%d.mp3",kind - FISH_KIND_1,index)->getCString());
		}
	}
	else if(gameKindId == GameKindJcpy)
	{
		if(kind >= FISH_KIND_JC10 && kind <= FISH_KIND_JC17)
		{
			//name = __String::createWithFormat("audios_lk/fish%d_%d.mp3",kind - FISH_KIND_JC1,index)->getCString();
			//playMusicEffect(name.c_str());
			playFishEffect(__String::createWithFormat("fish%d_%d.mp3",kind - FISH_KIND_JC1,index)->getCString());
		}
	}
}

void SoundManager::playFishEffect(const char* name,float volume)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	if (NULL != name) {
		if (SessionManager::shareInstance()->getEffectOn()) {
			AudioEngine::play2d(CommonFunction::getEffectName(name).c_str(),false,volume);
		}
	}
#endif
}