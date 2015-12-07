#include "Statistics.h"
#include "StatisticsConfig.h"
#include "crypto/CCCrypto.h"
#include "CommonFunction.h"
#include "Common/pystring.h"
#include "Model/OnlineUserModel.h"
#include "SessionManager.h"
#include "U8sdkFunction.h"

static Statistics* __instance = nullptr;

Statistics::Statistics()
	:eStatisticsType(S_INSTALL)
{
}
Statistics::~Statistics() {
}

Statistics* Statistics::shareInstance()
{
	if (__instance==nullptr)
	{
		__instance = new Statistics();
		if (!__instance->init()) {
			CC_SAFE_RELEASE_NULL(__instance);
		};
	}
	return __instance;
}

void Statistics::purgeInstance()
{
	CC_SAFE_RELEASE_NULL(__instance);
}

//发送统计数据
void Statistics::sendStatisticsData(int type){
	setStatisticsType((StatisticsType)type);

	//安装统计
	std::string sessionid = CU8sdkFunction::GetInstance().channelID;
	HttpRequest *request = new HttpRequest();
	std::string url = k_url;
	request->setRequestType(HttpRequest::Type::POST);
	//request->setResponseCallback(this, callfuncND_selector(Statistics::onHttpRequestCompleted));//回调函数，处理接收到的信息
	request->setResponseCallback(this, SEL_HttpResponse(&Statistics::onHttpRequestCompleted));
	std::string postData ;
	switch (eStatisticsType)
	{
	case Statistics::S_INSTALL:
	{

		url += "GameInstall";

		postData = "sessionID=";
		postData.append(sessionid);
		postData.append("&code=");
		std::string code = "server";
		code.append(sessionid);
		code.append(k_session_verion);
		code.append("lmyspread");

		std::string md5Code = Crypto::MD5String((void*)code.c_str(), code.length());
		postData.append(md5Code.c_str());

		postData.append("&machineCode=");

		std::string uuid = CommonFunction::getUUID();
		std::string Bit32UUid = Crypto::MD5String((void*)uuid.c_str(), strlen(uuid.c_str()));

		Bit32UUid = "mobile" + pystring::slice(Bit32UUid,0,26);
		postData.append(Bit32UUid.c_str());
	}
		break;
	case Statistics::S_REGISTER:
	{
		url += "AccountsRegister";
		//////////////////////////////////////////////////////////////////////////
		postData = "gameID=";
		postData.append(__String::createWithFormat("%ld", SessionManager::shareInstance()->getUser()->getGameId())->getCString());
		postData.append("&sessionID=");
		postData.append(sessionid);
		postData.append("&code=");
		std::string code = "server";
		code.append(sessionid);
		code.append(k_session_verion);
		code.append("lmyspread");

		std::string md5Code = Crypto::MD5String((void*)code.c_str(), code.length());
		postData.append(md5Code.c_str());

		postData.append("&machineCode=");

		std::string uuid = CommonFunction::getUUID();
		std::string Bit32UUid = Crypto::MD5String((void*)uuid.c_str(), strlen(uuid.c_str()));

		Bit32UUid = "mobile" + pystring::slice(Bit32UUid,0,26);
		postData.append(Bit32UUid.c_str());
	}
		break;
	case Statistics::S_ACCOUNT_LOGON:
	{
		url += "AccountsLogon";
		//////////////////////////////////////////////////////////////////////////
		postData = "gameID=";
		postData.append(__String::createWithFormat("%ld", SessionManager::shareInstance()->getUser()->getGameId())->getCString());
		postData.append("&sessionID=");
		postData.append(sessionid);
		postData.append("&code=");
		std::string code = "server";
		code.append(sessionid);
		code.append(k_session_verion);
		code.append("lmyspread");

		std::string md5Code = Crypto::MD5String((void*)code.c_str(), code.length());
		postData.append(md5Code.c_str());

		postData.append("&machineCode=");

		std::string uuid = CommonFunction::getUUID();
		std::string Bit32UUid = Crypto::MD5String((void*)uuid.c_str(), strlen(uuid.c_str()));

		Bit32UUid = "mobile" + pystring::slice(Bit32UUid,0,26);
		postData.append(Bit32UUid.c_str());
	}
		break;
	default:
		break;
	}
	
	
	request->setUrl(url.c_str());
	request->setRequestData(postData.c_str(), strlen(postData.c_str()));
	HttpClient::getInstance()->send(request);
	request->release();
}

void Statistics::onHttpRequestCompleted(HttpClient* client , HttpResponse* data)
{
	HttpResponse* response = (HttpResponse*)data;
	if (!response) { log("Log:response =null,plase check it."); return; }
	//请求失败
	if (!response->isSucceed())
	{
		return;
	}
	switch (eStatisticsType)
	{
	case Statistics::S_INSTALL:
	{
		LoginUserModel* pLoginModel = SessionManager::shareInstance()->getLoginModel();
		pLoginModel->setFirstLogin(true);
		pLoginModel->flush();
	}
		break;
	case Statistics::S_REGISTER:
		break;
	case Statistics::S_ACCOUNT_LOGON:
		break;
	default:
		break;
	}
}