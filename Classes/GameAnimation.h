#ifndef _GAME_ANIMATION_H_H_
#define _GAME_ANIMATION_H_H_

#include "cocos2d.h"
#include <vector>
#include <map>
#include"GameGlobalSetting.h"
#include "GameResourcesManager.h"
USING_NS_CC;


class GameAnimation: public Sprite
{
public:
	GameAnimation();
	~GameAnimation();
	static GameAnimation* create(AnimationInfo* info);
	bool init(AnimationInfo* info);
	void update(float dt);
	void render(Vec2 pt,float angele,float scale = 1.f);
	void play();
	void Stop();
	void Resume();
	bool IsPlaying() const { return mIsplaying; }
	float getSpeed() const { return mSpeed;}
	float getFps() const {return mFps;}
	int   getFrameWidth() const {return mFrameWidth;}
	int   getFrameHeight() const {return mFrameHeight;}
	void  SetCurrentFrame(int frameCnt);
	SpriteFrame* getFrame(int frameIndex);
	//Sprite* getSprite() const {return mSprite;}
	void  reset(AnimationInfo* info);
	void  setLoop(bool value) { mIsloop = value;}
protected:
	//Sprite* mSprite;
	Vector<SpriteFrame*> mFrames;
	int mFrameCount;
	bool mIsloop;// «∑Ò—≠ª∑≤•∑≈
	float mSpeed;
	float mFps;
	bool mIsplaying;
	int  mFrameWidth;
	int  mFrameHeight;
	Vec2 mCentrePt;
	float  mLastFrameTick;
	int  mCurFrame;
	int  mKindId;
};















#endif //_GAME_ANIMATION_H_H_