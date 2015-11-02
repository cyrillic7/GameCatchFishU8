
#ifndef __SPLASH_SCENE_H__
#define __SPLASH_SCENE_H__

#include "cocos2d.h"
#include "GameBaseScene.h"
#include "MsgDefine/CMD_LogonServer.h"

#include "cocostudio/CCArmatureDataManager.h"
#include "cocostudio/CCArmatureAnimation.h"
#include "cocostudio/CocoStudio.h"

#include "network/HttpRequest.h"
#include "network/HttpClient.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocostudio;
using namespace network;

//更新信息
struct updateInfo
{
	//是否显示更新
	bool isShowUpdateTip;
	//更新说明
	std::string strUpdateContent;
	//更新类型（1强制更新）
	int iUpdateType;
	//更新动作(update:程序内更新 toUrl:跳转url更新)
	std::string strUpdateAction;
	//url地址
	std::string strUpdateUrl;
};



class SplashScene : public GameBaseScene {
public:
	SplashScene();
	~SplashScene();
	CREATE_FUNC(SplashScene);
	virtual bool init(void);
	virtual void onEnter();
	virtual void onExit();
	void loadUI();

	void onAnimationEventOver(Armature *pArmature, MovementEventType movementType, const std::string& movementID);
	void onAnimationEventFrame(Bone *bone, const  std::string& evt, int originFrameIndex, int currentFrameIndex);

	void HttpRequestComplete(HttpClient* client , HttpResponse* response) ;
	void checkUpdate(const char* buf);
	void update(float dt);
protected:
	updateInfo uInfo;
	bool isCheckFinsh;
	__Dictionary* mUpdateInfo;
};

#endif
