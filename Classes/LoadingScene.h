#ifndef __LOADING_SCENE_H_H_
#define __LOADING_SCENE_H_H_

#include "cocos2d.h"
USING_NS_CC;

#define  RemoveLoadingMsg "RemoveLoadingMsg"

class LoadingScene : public Scene
{
public:
	LoadingScene();
	~LoadingScene();
	CREATE_FUNC(LoadingScene);
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	void loadUI();
	void setPercent(int persent);//设置进度条的进度
private:
	ProgressTimer* mProgressBar;
	Sprite* mProgressIcon;
};

#endif //__LOADING_SCENE_H_H_