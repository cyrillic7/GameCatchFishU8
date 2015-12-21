
#ifndef __LOGIN_SCENE_H__
#define __LOGIN_SCENE_H__

#include "cocos2d.h"
#include "GameBaseScene.h"
#include "MsgDefine/CMD_LogonServer.h"
USING_NS_CC;

class LoginScene : public GameBaseScene {
public:
	LoginScene();
	~LoginScene();
	static LoginScene* create(__Dictionary* pUpdateInfo = nullptr);
	virtual bool init(__Dictionary* pUpdateInfo);
	virtual void onEnter();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	void loadUI();
	void logonGameByQQ(float dt);
	void logonGameByWx(float dt);
	void netWorkIsValid(EventCustom* evt);
	virtual __Array * getResourceNeedLoad();
protected:
	void LoginSuccessRsp(EventCustom* evt);
	void LoginFaildRsp(EventCustom* evt);
	void sendfastLogin(EventCustom* evt);
	void onFastLogin(EventCustom* evt);
	void QQLogin(EventCustom* evt);
	void U8Login(EventCustom* evt);
	void wxLogin(EventCustom*evt);

	void AccountLogin(EventCustom* evt);
	void FastLoginRsp(EventCustom* evt);
	void AccountRegister(EventCustom* evt);

	void goToUpdate(EventCustom* evt);
	void CancelUpdate(EventCustom* evt);

	void showLoading();
	void removeLoading();

protected:
	std::string m_Account;
	std::string m_Password;
	std::string m_token;
	bool  mbQQLogin;
	bool  mbFastLogin;
	bool  mBRegister;
	__Dictionary* mRegisterInfo;
	CMD_MB_Quick_Logon_Success mQucikAccout;
	__Dictionary* mUpateInfo;
};

#endif
