
#include "SplashScene.h"
#include "LoginScene.h"
#include "CommonFunction.h"
#include "json/stringbuffer.h"
#include "CommonFunction.h"
#include "Common/Statistics.h"
#include "Model/LoginUserModel.h"
#include "SessionManager.h"
#include "Common/StatisticsConfig.h"
#include "Common/pystring.h"

using namespace rapidjson;
USING_NS_CC_EXT;
USING_NS_CC;

SplashScene::SplashScene()
{
	isCheckFinsh = false;
	mUpdateInfo = __Dictionary::create();
	mUpdateInfo->retain();
}

SplashScene::~SplashScene()
{
	CC_SAFE_RELEASE_NULL(mUpdateInfo);
}

bool SplashScene::init()
{
	if (!GameBaseScene::init()) {
		return false;
	}

	return true;
}

void SplashScene::onEnter()
{
	GameBaseScene::onEnter();
	loadUI();
	//std::string url = "http://download.qicainiu.com/download/Mobile/GameCatchFish/updateConfigQCN.txt";
	std::string url  = __String::createWithFormat("http://download.qicainiu.com/download/Mobile/GameCatchFish/updateConfigQCN_%s.txt",k_session_id)->getCString();
	HttpRequest* request = new HttpRequest();
	request->setUrl(url.c_str());
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(this, SEL_HttpResponse(&SplashScene::HttpRequestComplete));
	request->setTag("GET UPDATE CONFIG");
	HttpClient::getInstance()->send(request);
	request->release();

	LoginUserModel* pLoginModel = SessionManager::shareInstance()->getLoginModel();
	//安装统计
	if (!pLoginModel->getFirstLogin())
	{
		Statistics::shareInstance()->sendStatisticsData(Statistics::S_INSTALL);
	}

	Director::getInstance()->getScheduler()->schedule(schedule_selector(SplashScene::update), this,1.0f, false);

}

void SplashScene::HttpRequestComplete(HttpClient* client , HttpResponse* response) 
{
	if (!response)
	{
		return;
	}

	// You can get original request type from: response->request->reqType
	if (0 != strlen(response->getHttpRequest()->getTag()))
	{
		CCLOG("%s completed", response->getHttpRequest()->getTag());
	}

	int statusCode = response->getResponseCode();
	char statusString[64] = {};
	sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());
	CCLOG("response code: %d", statusCode);

	if (!response->isSucceed())
	{
		isCheckFinsh = true;
		mUpdateInfo->setObject(__Bool::create(false),"needUpdate");
		return;
	}
	// dump data
	std::vector<char> *buffer = response->getResponseData();
	std::string bufffff(buffer->begin(), buffer->end());
	checkUpdate(bufffff.c_str());
}

void SplashScene::checkUpdate(const char* buf){

	std::string sCurVersion = CommonFunction::getVersion();

	std::string sServiceVersion;

	std::string urlAndroid;

	std::string urlIphone;

	std::string strAndroidAction;

	std::string strIphoneAction;

	int iUpdateType;

	std::string strUpdateContent;

	rapidjson::Document readdoc;
	readdoc.Parse<0>(buf);	

	if(readdoc.HasParseError())
	{
		log("GetParseError%s\n", readdoc.GetParseError());
	}

	rapidjson::Value& root = readdoc["updateConfig"];

	
	if (root.HasMember("var"))
	{
		sServiceVersion = root["var"].GetString();
	}

	if (root.HasMember("androidAction"))
	{
		strAndroidAction = root["androidAction"].GetString();
	}

	if (root.HasMember("iphoneAction"))
	{
		strIphoneAction = root["iphoneAction"].GetString();
	}

	if (root.HasMember("updateType"))
	{
		iUpdateType = root["updateType"].GetInt();
		mUpdateInfo->setObject(__Integer::create(iUpdateType),"updateType");
	}

	if (root.HasMember("updateContent"))
	{
		strUpdateContent= root["updateContent"].GetString();
		mUpdateInfo->setObject(__String::create(strUpdateContent),"updateDesc");
	}


	if (root.HasMember("url_android"))
	{
		urlAndroid = root["url_android"].GetString();
	}


	if (root.HasMember("url_iphone"))
	{
		urlIphone = root["url_iphone"].GetString();
	}

	//登录点数组
	if (root.HasMember("url_logon_list"))
	{
		__Array* pUrlArray = SessionManager::shareInstance()->getLoginUrls();
		rapidjson::Value& logonArray = root["url_logon_list"];
		if (logonArray.IsArray())
		{
			for (int i = 0; i < logonArray.Capacity(); i++)
			{
				rapidjson::Value& urlItem = logonArray[i];
				if (urlItem.HasMember("logonAddr"))
				{
					log("urlItem %s", urlItem["logonAddr"].GetString());
					pUrlArray->addObject(__String::create(urlItem["logonAddr"].GetString()));
				}
				
			}
		}
	}
	

	std::vector< std::string > VcurVersion;
	std::vector< std::string > VserviceVersion;
	pystring::split(sCurVersion,VcurVersion,".");
	pystring::split(sServiceVersion,VserviceVersion,".");

	//大版本号变动则强制升级
	if (VcurVersion[0]==VserviceVersion[0] && VcurVersion[1]==VserviceVersion[1]  )
	{
		//不用强制更新也能继续玩
		mUpdateInfo->setObject(__Integer::create(0),"updateType");
	}

	if (strcmp(sCurVersion.c_str(),sServiceVersion.c_str()) !=0)
	{
		mUpdateInfo->setObject(__Bool::create(true),"needUpdate");
	}
	else
	{
		mUpdateInfo->setObject(__Bool::create(false),"needUpdate");
	}

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	mUpdateInfo->setObject(__Bool::create(false),"needUpdate");
#endif

#if CC_TARGET_PLATFORM != CC_PLATFORM_IOS
	mUpdateInfo->setObject(__String::create(urlAndroid),"updateUrl");
	mUpdateInfo->setObject(__String::create(strAndroidAction),"updateAction");
#else
	mUpdateInfo->setObject(__String::create(urlIphone),"updateUrl");
	mUpdateInfo->setObject(__String::create(strIphoneAction),"updateUrl");
#endif
	isCheckFinsh = true;
}


void SplashScene::onExit()
{
	Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
	GameBaseScene::onExit();	
}

void SplashScene::loadUI()
{
	LayerColor*  pLayer = LayerColor::create(ccc4(0,0,0,255));
	pLayer->setContentSize(Size(kScreenWidth,kScreenHeight));
	addChild(pLayer);
	/*ArmatureDataManager::getInstance()->addArmatureFileInfo("Splash/AnimationCompanyLogo.ExportJson");  
	Armature* armature = Armature::create("AnimationCompanyLogo");
	armature->setAnchorPoint(ccp(0.5, 0.5));
	armature->setPosition(kScreenWidth/2 - 220,kScreenHeight/ 2);
	armature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(SplashScene::onAnimationEventOver));
	armature->getAnimation()->setFrameEventCallFunc(this, frameEvent_selector(SplashScene::onAnimationEventFrame));
	armature->getAnimation()->play("Animation1");
	this->addChild(armature);*/
}


void SplashScene::onAnimationEventOver(Armature *pArmature, MovementEventType movementType, const std::string& movementID){
	switch (movementType)
	{
	case MovementEventType::COMPLETE:
	case MovementEventType::LOOP_COMPLETE:
		{
			if (strcmp(movementID.c_str(), "Animation1") == 0)
			{
				if(!isCheckFinsh)
				{
					Director::getInstance()->getScheduler()->schedule(schedule_selector(SplashScene::update), this,1.0f, false);
				}
				else
				{
					Director::sharedDirector()->replaceScene(LoginScene::create(mUpdateInfo));
				}
			}
		}
		break;
	default:
		break;
	}
}

void SplashScene::update(float dt)
{
	if (isCheckFinsh)
	{
		Director::sharedDirector()->pushScene(LoginScene::create(mUpdateInfo));
		isCheckFinsh =false;
	}
}

void SplashScene::onAnimationEventFrame(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex){
	if (strcmp(evt.c_str(), "userEnter") == 0)
	{

	}
}



