#include "CommonFunction.h"
#include "U8sdkFunction.h"
#include "cocostudio/CCSGUIReader.h"
#include "Message.h"
#include "GameServiceClientManager.h"
#include "SessionManager.h"
#include "LoginScene.h"

CU8sdkFunction::CU8sdkFunction()
{
}


CU8sdkFunction::~CU8sdkFunction()
{
}


void CU8sdkFunction::OnInitSuc()
{
	//一般不需要做什么处理
}

void CU8sdkFunction::OnLoginSuc(U8LoginResult* result)
{
	log("SDK登录并认证成功...%s", result->userId.c_str());
	//this->labelUsername->setString(result->sdkUsername);

	if (result->isSwitchAccount)
	{
		//TODO:这里退回到登录界面，并且SDK是已经登录成功状态，不用再弹出SDK的登录界面
		log("U8----result->isSwitchAccount");
	}
	else
	{
		//TODO:登录认证成功，获取服务器列表，进入游戏服...
		log("U8----loginresult");
		std::string umid = result->sdkUserId;
		std::string username = result->sdkUsername;
		std::string token = result->token;
		std::string userid = result->userId;
		channelID = result->channelID.c_str();

		log("channelID = %s", channelID.c_str());
		log("umid = %s", umid.c_str());
		log("username = %s", username.c_str());
		log("token = %s", token.c_str());

		__Dictionary*  dic = __Dictionary::create();
		dic->setObject(__String::create(umid), "umid");
		dic->setObject(__String::create(username), "username");
		dic->setObject(__String::create(token), "token");
		dic->setObject(__String::create(userid), "userid");
		Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(u8LoginMsg, dic);
	}
}

void CU8sdkFunction::OnSwitchLogin()
{
	std::string name = GameServiceClientManager::sharedInstance()->getCurrentServiceClientName();
	GameServiceClientManager::sharedInstance()->removeServiceClient(name.c_str());
	SessionManager::shareInstance()->clearMsgArray();
	SessionManager::shareInstance()->clearHornMsg();
	//断开大厅SORKET
	GameServiceClient* c = GameServiceClientManager::sharedInstance()->serviceClientForName(taskClient);
	c->closeSoket();

	LoginUserModel *userModel = SessionManager::shareInstance()->getLoginModel();
	//上次是QQ 登录，切换帐号后清空缓存的帐号的信息
	if (userModel->getAccountType() == accountType::accountQQ)
	{
		userModel->setAccount("");
		userModel->setPassword("");
	}
	//removeFromParent();
	Director::sharedDirector()->replaceScene(LoginScene::create());

	U8SDKInterface::getInstance()->login();
}

void CU8sdkFunction::OnLogout()
{
	CCLOG("SDK帐号退出成功...");
	//this->labelUsername->setString("U8SDK");
	//TODO:退回到登录界面，并弹出SDK登录界面(调用U8SDKInterface::getInstance()->login())
	std::string name = GameServiceClientManager::sharedInstance()->getCurrentServiceClientName();
	GameServiceClientManager::sharedInstance()->removeServiceClient(name.c_str());
	SessionManager::shareInstance()->clearMsgArray();
	SessionManager::shareInstance()->clearHornMsg();
	//断开大厅SORKET
	GameServiceClient* c = GameServiceClientManager::sharedInstance()->serviceClientForName(taskClient);
	c->closeSoket();

	LoginUserModel *userModel = SessionManager::shareInstance()->getLoginModel();
	//上次是QQ 登录，切换帐号后清空缓存的帐号的信息
	if (userModel->getAccountType() == accountType::accountQQ)
	{
		userModel->setAccount("");
		userModel->setPassword("");
	}
	//removeFromParent();
	Director::sharedDirector()->replaceScene(LoginScene::create());

	U8SDKInterface::getInstance()->login();
}

void CU8sdkFunction::OnU8sdkInit()
{
	log("SDKinit成功...111");
	U8SDKInterface::getInstance()->initSDK(this);
}

void CU8sdkFunction::OnU8sdkLogin()
{
	log("OnU8sdkLogin");
	U8SDKInterface::getInstance()->login();
}

std::string CU8sdkFunction::OnGetChannelid()
{
	std::string channelid = U8SDKInterface::getInstance()->getchannelId();
	channelID = channelid;
	log("OnU8sdkLogin%s", channelid.c_str());

	return channelid;
}

void CU8sdkFunction::OnU8sdkPay(std::string strName, std::string strDesc, int price, std::string tradeNo)
{
	U8PayParams* params = U8PayParams::create();

	params->productId = "1";
	params->productName = strName;
	params->productDesc = strDesc;
	params->price = price;
	params->buyNum = 1;
	params->coinNum = 0;
	params->serverId = "1";
	params->serverName = "";
	params->roleId = tradeNo;
	params->roleName = "";
	params->roleLevel = 0;
	params->vip = "";

	//去服务器获取游戏中商品的订单号以及扩展数据,这里测试
	params->orderID = tradeNo;
	params->extension = "";

	log("CU8sdkFunction::OnU8sdkPay productName==%s,productDesc==%s,price==%d,orderID==%s",params->productName.c_str()
		, params->productDesc.c_str()
		,params->price
		,params->orderID.c_str());

	U8SDKInterface::getInstance()->pay(params);
}