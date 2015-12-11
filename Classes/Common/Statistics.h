//统计
#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"

#include "network/HttpRequest.h"
#include "network/HttpClient.h"

USING_NS_CC;
using namespace network;



class Statistics:public CCNode {
public:
	enum StatisticsType
	{
		S_INSTALL=0,			//安装
		S_REGISTER,				//注册
		S_ACCOUNT_LOGON,		//帐号登录
	};
	CC_SYNTHESIZE(StatisticsType, eStatisticsType, StatisticsType);
public:
	Statistics();
	~Statistics();
	static Statistics* shareInstance();
	virtual void purgeInstance();
	bool init() {return true;}
public:
	//发送统计数据
	void sendStatisticsData(int type);
private:
	//网络请求回调
	void onHttpRequestCompleted(HttpClient* client , HttpResponse* data);
};
