#include "GameAnimation.h"
#include "CommonFunction.h"
#include "SessionManager.h"
#include "Message.h"

GameAnimation::GameAnimation()
{

	mSpeed  =0;
	mLastFrameTick = -1;
	mIsplaying = false;
	mFrameCount =0;
	mFps =0;
	mFrameWidth =0;
	mFrameHeight =0;
	mCurFrame =0;
}

GameAnimation::~GameAnimation()
{

}

GameAnimation* GameAnimation::create(AnimationInfo* info)
{
	GameAnimation* pAnima = new GameAnimation();
	if(pAnima && pAnima->init(info))
	{
		pAnima->autorelease();
		return pAnima;
	}
	CC_SAFE_DELETE(pAnima);
	return nullptr;
}

bool GameAnimation::init(AnimationInfo*info)
{
	if (!Sprite::init())
		return false;
	mKindId = info->getKind();
	mFps = info->getFps();
	mSpeed = 1.0f/mFps;
	mFrameCount = info->getFrames();
	mFrameWidth = info->getFrameWidth();
	mFrameHeight = info->getFrameHeight();
	mIsloop = info->isLoop();

	
	//载入动画TEXTURE
	String* pathStr =nullptr;
	int gameKindInDex = SessionManager::shareInstance()->getGameKind();

	switch (gameKindInDex)
	{
	case GameKindDntg:
		{
			if (info->getResId() == 0 )
			{
				if(mKindId <= FISH_BGLU )
				{
					if (info->isWalk())
					{
						pathStr = String::createWithFormat("dntg2/images/fish/%s.png",CommonFunction::fish_name_with_status(info->getKind(),"walking"));
					} 
					else
					{
						pathStr = String::createWithFormat("dntg2/images/fish/%s.png",CommonFunction::fish_name_with_status(info->getKind(),"dying"));
					}
				}
				else
				{
					pathStr = String::createWithFormat("dntg2/images/fish/%s.png",info->getImageName().c_str());
				}
			}
			else if(info->getResId() == 1)
			{
				pathStr = String::createWithFormat("common/players/%s.png",info->getImageName().c_str());
			}
		}
		break;
	case  GameKindLkpy:
		{
			if(mKindId >= FISH_KIND_1 && mKindId < FISH_KIND_25_27)
			{
				if (info->isWalk())
				{
					pathStr = String::createWithFormat("%s/fish/%s.png",CommonFunction::getResDir().c_str(),CommonFunction::fish_name_with_status(info->getKind(),"walking"));
				} 
				else
				{
					
					pathStr = String::createWithFormat("%s/fish/%s.png",CommonFunction::getResDir().c_str(),CommonFunction::fish_name_with_status(info->getKind(),"dying"));
				}
			}
		}
		break;
	case  GameKindJcpy:
		{
			if(mKindId >= FISH_KIND_JC1 && mKindId < FISH_KIND_JCKING)
			{
				if (info->isWalk())
				{
					pathStr = String::createWithFormat("%s/fish/%s.png",CommonFunction::getResDir().c_str(),__String::createWithFormat("%s_walking",info->getImageName().c_str())->getCString());
				} 
				else
				{
					pathStr = String::createWithFormat("%s/fish/%s.png",CommonFunction::getResDir().c_str(),__String::createWithFormat("%s_dying",info->getImageName().c_str())->getCString());
				}
			}
		}
		break;
	}
	
	Sprite* fishImg = Sprite::create(pathStr->getCString());
	int row = fishImg->getContentSize().width/mFrameWidth;
	int col = fishImg->getContentSize().height/mFrameHeight;
	if (col == 0)
	{
		col = 1;
	}

	bool isReload = false;
	Texture2D* pTexture =  Director::sharedDirector()->getTextureCache()->getTextureForKey(pathStr->getCString());
	if(pTexture)
		isReload = true;

	SpriteFrame* frame = nullptr;
	for(int i=0;i<col;i++)
	{
		for(int j=0;j<row;j++)
		{
			if(i*row+j < mFrameCount )
			{
				if(isReload)
					frame= SpriteFrame::createWithTexture(pTexture,CCRectMake(j*mFrameWidth,i*mFrameHeight,mFrameWidth,mFrameHeight));
				else
					frame = SpriteFrame::create(pathStr->getCString(),CCRectMake(j*mFrameWidth,i*mFrameHeight,mFrameWidth,mFrameHeight));
				if(i==0 && j==0)
				{
					setSpriteFrame(frame);
				}
				if (NULL != frame) {
					mFrames.pushBack(frame);
				}
			}
		}
	}
	
	if (gameKindInDex == GameKindJcpy)
	{
		if (mKindId == FISH_KIND_JC20)//局部炸弹
		{
			//取第二帧
			frame = SpriteFrame::create(pathStr->getCString(),CCRectMake(0,mFrameHeight,mFrameWidth,mFrameHeight));
			if (NULL != frame) {
				mFrames.pushBack(frame);
			}
			mFrameCount++;


			frame = SpriteFrame::create(pathStr->getCString(),CCRectMake(0,mFrameHeight,mFrameWidth,mFrameHeight));
			if (NULL != frame) {
				mFrames.pushBack(frame);
			}
			mFrameCount++;
		}
	}
	return true;
}

void GameAnimation::play()
{
	mIsplaying = true;
	mLastFrameTick = -1;
	switch (SessionManager::shareInstance()->getGameKind())
	{
	case GameKindDntg:
		if(mKindId <= FISH_BGLU )
			SetCurrentFrame(0);
		break;
	case GameKindLkpy:
		if (mKindId >= FISH_KIND_1 && mKindId < FISH_KIND_25_27)
		{
			SetCurrentFrame(0);
		}
	}
	

}

void GameAnimation::Stop() {
	mIsplaying = false;
}

void GameAnimation::Resume() {
	mIsplaying = true;
}

void GameAnimation::update(float dt)
{
	if (!mIsplaying || getTexture() ==nullptr)
		return;

	//if(mKindId <= FISH_BGLU )
	{
		if (mLastFrameTick == -1.f) {
			mLastFrameTick = 0.f;
		} else {
			mLastFrameTick += dt;
		}

		while (mLastFrameTick >= mSpeed) {
			mLastFrameTick -= mSpeed;
			if (!mIsloop && mCurFrame + 1 == mFrameCount)
				mIsplaying = false;
			if (mIsplaying)
				SetCurrentFrame(mCurFrame + 1);
		}
	}

}

void GameAnimation::render(Vec2 pt,float angele,float scale)
{
	setScale(scale);
	setPosition(pt);
	setRotation(angele);
	
}

void GameAnimation::SetCurrentFrame(int frameCnt)
{
	mCurFrame = frameCnt % mFrameCount;
	if (mCurFrame) {
		this->setDisplayFrame(getFrame(mCurFrame));
	}
	
}

SpriteFrame* GameAnimation::getFrame(int frameIndex)
{
	Vector<SpriteFrame*>::iterator it = mFrames.begin();
	for(int i= 0;it != mFrames.end();it++,i++)
	{
		if(i ==frameIndex)
			return *it;
	}
	return nullptr;
}

void GameAnimation::reset(AnimationInfo* info)
{
	mFrames.clear();

	mKindId = info->getKind();
	mFps = info->getFps();
	mSpeed = 1.0f/mFps;
	mFrameCount = info->getFrames();
	mFrameWidth = info->getFrameWidth();
	mFrameHeight = info->getFrameHeight();
	mIsloop = info->isLoop();


	//载入动画TEXTURE
	String* pathStr =nullptr;
	if (info->getResId() == 0 )
	{
		if(mKindId <= FISH_BGLU )
		{
			if (info->isWalk())
			{
				pathStr = String::createWithFormat("dntg2/images/fish/%s.png",CommonFunction::fish_name_with_status(info->getKind(),"walking"));
			} 
			else
			{
				pathStr = String::createWithFormat("dntg2/images/fish/%s.png",CommonFunction::fish_name_with_status(info->getKind(),"dying"));
			}
		}
		else
		{
			pathStr = String::createWithFormat("dntg2/images/fish/%s.png",info->getImageName().c_str());
		}
	}
	else if(info->getResId() == 1)
	{
		pathStr = String::createWithFormat("common/players/%s.png",info->getImageName().c_str());
	}

	Sprite* fishImg = Sprite::create(pathStr->getCString());
	int row = fishImg->getContentSize().width/mFrameWidth;
	int col = fishImg->getContentSize().height/mFrameHeight;
	if (col == 0)
	{
		col = 1;
	}

	bool isReload = false;
	Texture2D* pTexture =  Director::sharedDirector()->getTextureCache()->getTextureForKey(pathStr->getCString());
	if(pTexture)
		isReload = true;
	for(int i=0;i<col;i++)
	{
		for(int j=0;j<row;j++)
		{
			if(i*row+j < mFrameCount )
			{
				SpriteFrame* frame = nullptr;
				if(isReload)
					frame= SpriteFrame::createWithTexture(pTexture,CCRectMake(j*mFrameWidth,i*mFrameHeight,mFrameWidth,mFrameHeight));
				else
					frame = SpriteFrame::create(pathStr->getCString(),CCRectMake(j*mFrameWidth,i*mFrameHeight,mFrameWidth,mFrameHeight));
				if(i==0 && j==0)
				{
					setSpriteFrame(frame);
				}
				if (NULL != frame) {
					mFrames.pushBack(frame);
				}
			}
		}
	}
}