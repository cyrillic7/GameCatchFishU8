#include "GameServiceClient.h"
#include "MsgDefine/CMD_LogonServer.h"
#include "MsgDefine/CMD_GameServer.h"
#include "Struct.h"
#include "Command/GameResponseHandler.h"
#include "Command/CommonResponseHandler.h"
#include "Command/MatchResponseHandler.h"
#include "GameServiceClientManager.h"

#include "SessionManager.h"
#include "GameTimerManager.h"
#include "Model/GiftModel.h"
#include "Model/OnlineUserModel.h"
#include "CommonFunction.h"
#include "crypto/CCCrypto.h"
#include "Common/pystring.h"
#include "Common/StatisticsConfig.h"


#define  dwOptermianlValue 3

//test server
//#define serverAddress "121.40.31.203" 
//#define  serverAddress  "121.41.77.64"
#define  serverPort    8100

#define  ChargeByInterlbank   2 //网银
#define  ChargeByZfb 7 //支付宝

GameServiceClient::GameServiceClient()
{
	m_needPassToResponseHandler = true;
	m_responseHandlers = __Array::create();
	m_responseHandlers->retain();
	m_isLogin = false;
	m_bConnectFail = false;
}

GameServiceClient::~GameServiceClient()
{
	CC_SAFE_RELEASE_NULL(m_responseHandlers);
	
}

GameServiceClient* GameServiceClient::create(int sorketIndex)
{
	GameServiceClient *client = new GameServiceClient;
	if (client && client->init(sorketIndex)) {
		client->autorelease();
		return client;
	}
	CC_SAFE_DELETE(client);
	return NULL;
}

bool GameServiceClient::init(int sorketIndex)
{
	bool ret =false;
	m_isLookOn = false;
	m_GameSocket.SetGameSocketSink(this);
	//注册事件处理器
	registerHandler();
	//ret = initSocket(sorketIndex);
	m_clientType = sorketIndex;
	Director::getInstance()->getScheduler()->scheduleUpdate(this,0,false);
	return true;
}


void GameServiceClient::registerHandler() {
	m_responseHandlers->addObject(GameResponseHandler::create());
	m_responseHandlers->addObject(CommonResponseHandler::create());
	m_responseHandlers->addObject(MatchResponseHandler::create());
}

bool GameServiceClient::Connect(const char* ip, unsigned short port)
{
	//return m_GameSocket.Connect(ip,port) ;
	LoginUserModel* model = SessionManager::shareInstance()->getLoginModel();
	if (m_GameSocket.Connect(ip,port))
	{
		if (m_clientType == FRAME_SORKET_INDEX)
			model->flush();
		return true;
	}
	else
	{
		if (m_clientType == FRAME_SORKET_INDEX)
		{
			SessionManager::shareInstance()->setLoginAddrNull();
			if (model->getLoginUrlIndex() + 1 >= SessionManager::shareInstance()->getLoginUrls()->count())
			{
				model->setLoginUrlIndex(0);
			}
			else
			{
				model->setLoginUrlIndex(model->getLoginUrlIndex() + 1);
			}
			Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(netWorkValidMsg);
		}
		return false;
	}
	
}

bool GameServiceClient::OnEventTCPSocketLink(bool bConnectSucc)
{
	log("link value %d",bConnectSucc);
	if(!bConnectSucc)
	{
		m_bConnectFail = true;
		
		//waring 多线程问题不能在此传消息
		//Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(netWorkValidMsg);
	}
	else
	{
		if (m_clientType == FRAME_SORKET_INDEX)
		{
			//SessionManager::shareInstance()->getLoginModel()->flush();
		}
		
	}
	return true;
}
//关闭事件
bool GameServiceClient::OnEventTCPSocketShut(unsigned char cbShutReason)
{
	log("PSocket Shut reason = %d socket Index = %d",cbShutReason,m_clientType);
	if (cbShutReason == 0)//主动断开
	{
		return true;
	}
	if(m_clientType == GAMEROOM_SORKET_INDEX)
	{
		if(SessionManager::shareInstance()->isInGame())
			Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(GameSorketCloseMsg);
	}
	else if(m_clientType == TASK_SORKET_INDEX)
	{
		//重连以前清缓存
		SessionManager::shareInstance()->clearMsgArray();
		SessionManager::shareInstance()->clearMatchRooms();
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(TaskSorketCloseMsg);
		//断开重连
		loginTaskServer();
	}
	return true;
}

void GameServiceClient::parseGift(void* pData,WORD wDataSize, ShopItemType type)
{
	CMD_GP_Gift* gifts = static_cast<CMD_GP_Gift*>(pData);
	WORD gift_cnt = wDataSize / sizeof(CMD_GP_Gift);
	__Array* giftArray = __Array::createWithCapacity(gift_cnt);
	
	int len = sizeof(gifts->price);
	for (int i= 0 ; i < gift_cnt;i++)
	{
		GiftModel* pGift = GiftModel::create();
		pGift->setGiftId(gifts->dwID);
		pGift->setGiftName(gifts->szName);
		pGift->setIconUrl(gifts->szImgName);
		pGift->setDiscount(gifts->dwDiscount);
		pGift->setPrice(gifts->price[0].dwCount);
		pGift->setPriceDesc(gifts->price[0].szName);
		pGift->setOpenDesc(gifts->szOpenDespict);
		pGift->setDesc(gifts->szDespict);
		pGift->setGiftType(type);
		giftArray->addObject(pGift);
		gifts++;
	}
	ShopManager::getInstance()->setDataList(type,giftArray);
	__Dictionary* dic = __Dictionary::create();
	dic->setObject(giftArray,"goods");
	dic->setObject(__Integer::create(type),"type");
	/*if (type == ShopTypeGift)
	{
		dic->setObject(__Integer::create(1), "type");
	}
	else if(type == ShopTypeTool)
	{
		dic->setObject(__Integer::create(2), "type");
	}
	else if(type == ShopTypeVip)
	{
		dic->setObject(__Integer::create(type),"type");
	}*/
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(GetShopListFromServerMessage, dic);
	
}

bool GameServiceClient::OnTCPSocketRead(WORD wSocketID, TCP_Command Command, VOID * pData, WORD wDataSize)
{
	if(!m_needPassToResponseHandler)
		return true;
	if(wSocketID == FRAME_SORKET_INDEX)
	{
		switch (Command.wMainCmdID)
		{
		case MDM_UM_LOGIN://登录大厅命令
			{
				return OnTCPNetworkMainMBLogon(Command.wSubCmdID,pData,wDataSize,wDataSize);
			}
		case MDM_UM_MB_DATA:
			{
				return OnTCPNetworkMainMBCount(Command.wSubCmdID,pData,wDataSize,wDataSize);
			}
			break;
		case MDM_GP_USER_SERVICE://用户命令
			{
									
				if (Command.wSubCmdID != SUB_GP_AUCTION_RECORD && Command.wSubCmdID != SUB_GP_MYAUCTION_RECORD &&Command.wSubCmdID != SUB_GP_AUCTION_HISTORY_RECORD)
				{
					if (Command.wSubCmdID != SUB_GP_AUCTION && Command.wSubCmdID != SUB_GP_BUY_AUCTION && Command.wSubCmdID != SUB_GP_CANCEL_AUCTION && Command.wSubCmdID != 413)
						closeSoket();
				}
				
				if(Command.wSubCmdID == SUB_GP_GIFT )//礼包
				{
					parseGift(pData,wDataSize,ShopTypeGift);
				}
				else if (Command.wSubCmdID == SUB_GP_PROPERTY)//道具
				{
					parseGift(pData,wDataSize,ShopTypeTool);
				}else if(Command.wSubCmdID == SUB_GP_VIPSHOP)
				{
					parseGift(pData,wDataSize,ShopTypeVip);
				}else if ( Command.wSubCmdID == SUB_GP_RECHARGE_ORDER)
				{
					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(RechargeOrderRspMsg,pData);
				}
				else if (Command.wSubCmdID == SUB_GP_BUYGIFT)
				{
					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(buyGifRspMsg,pData);
				}
				else if (Command.wSubCmdID == SUB_GP_USE_KNAPSACKLOG)
				{
					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(userKnapsackMsg,pData);
				}
				else if (Command.wSubCmdID == SUB_GP_AUCTION)//上架物品信息
				{
					if (wDataSize % sizeof(CMD_GP_Auction) != 0)
					{
						log("auction list data is error");
					}
					CMD_GP_Auction* info = (CMD_GP_Auction*)pData;
					int cnt = wDataSize / sizeof(CMD_GP_Auction);
					__Array* infoArray = __Array::createWithCapacity(cnt);
					for (int i = 0; i < cnt; i++)
					{
						__Dictionary* dic = __Dictionary::create();
						dic->setObject(__Integer::create(info->dwID),"propId");
						dic->setObject(__Integer::create(info->dwPackType),"propType");
						dic->setObject(__Integer::create(info->dwSortID),"sortId");
						dic->setObject(__Integer::create(info->dwNum),"propCnt");

						dic->setObject(__String::create(info->szName),"propName");
						dic->setObject(__String::create(info->szImgName),"propUrl");
						info++;
						infoArray->addObject(dic);
					}

					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(getAuctionList,infoArray);
				}
				else if (Command.wSubCmdID == SUB_GP_AUCTION_RECORD || Command.wSubCmdID == SUB_GP_QUERY_AUCTION)
				{
					CMD_GP_AuctionRecord<CMD_GP_AuctionRecordItem>* info = (CMD_GP_AuctionRecord<CMD_GP_AuctionRecordItem>*)pData;
					__Dictionary* infoDic = __Dictionary::create();
					int itemCnt = info->dwIndex;
					infoDic->setObject(__Integer::create(itemCnt),"itemCnt");
					infoDic->setObject(__Integer::create(info->dwPageCount),"maxPage");
					infoDic->setObject(__Integer::create( info->dwPageIndex),"curPage");

					__Array* auctionItems = __Array::createWithCapacity(itemCnt);
					for (int i= 0 ; i < itemCnt ; i ++)
					{
						CMD_GP_AuctionRecordItem pRecordItem = info->RecordItem[i];
						__Dictionary* dic = __Dictionary::create();
						dic->setObject(__Integer::create(pRecordItem.dwIndex),"index");//单号
						dic->setObject(__String::create(pRecordItem.szNickName),"nickName");//拍卖者昵称
						dic->setObject(__Integer::create(pRecordItem.dwPropNum),"propNum");//拍卖物品个数
						dic->setObject(__String::create(pRecordItem.szAuctionName),"auctionName");//拍卖物品名称
						dic->setObject(__Integer::create(pRecordItem.lGold),"cost");//拍卖物品价格
						dic->setObject(__Integer::create(pRecordItem.dwGameID),"id");//拍卖者GAMEID
						dic->setObject(__String::create(pRecordItem.szImgName),"imageUrl");
						auctionItems->addObject(dic);
					}

					infoDic->setObject(auctionItems,"items");
					//to do 包存到Session中去
					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(getAuctoinRecord,infoDic);
					


				}else if(Command.wSubCmdID == SUB_GP_MYAUCTION_RECORD)
				{
					CMD_GP_AuctionRecord<CMD_GP_MyAuctionRecordItem>* info = (CMD_GP_AuctionRecord<CMD_GP_MyAuctionRecordItem>*)pData;
					__Dictionary* infoDic = __Dictionary::create();
					int itemCnt = info->dwIndex;
					infoDic->setObject(__Integer::create(itemCnt),"itemCnt");
					infoDic->setObject(__Integer::create(info->dwPageCount),"maxPage");
					infoDic->setObject(__Integer::create( info->dwPageIndex),"curPage");
					__Array* auctionItems = __Array::createWithCapacity(itemCnt);
					for (int i= 0 ; i < itemCnt ; i ++)
					{
						CMD_GP_MyAuctionRecordItem pRecordItem = info->RecordItem[i];
						__Dictionary* dic = __Dictionary::create();
						dic->setObject(__Integer::create(pRecordItem.dwIndex),"index");//单号
						dic->setObject(__Integer::create(pRecordItem.dwPropNum),"propNum");//拍卖物品个数
						dic->setObject(__String::create(pRecordItem.szAuctionName),"auctionName");//拍卖物品名称
						dic->setObject(__Integer::create(pRecordItem.lGold),"cost");//拍卖物品价格
						auctionItems->addObject(dic);
					}

					//to do 包存到Session中去
					infoDic->setObject(auctionItems,"items");
					
					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(getMyAuctionRecord,infoDic);
					
				}else if (Command.wSubCmdID == SUB_GP_AUCTION_HISTORY_RECORD)
				{
					
					CMD_GP_AuctionRecord<CMD_GP_HistoryAuctionRecordItem>* info = (CMD_GP_AuctionRecord<CMD_GP_HistoryAuctionRecordItem>*)pData;
					__Dictionary* infoDic = __Dictionary::create();
					int itemCnt = info->dwIndex;
					infoDic->setObject(__Integer::create(itemCnt),"itemCnt");
					infoDic->setObject(__Integer::create(info->dwPageCount),"maxPage");
					infoDic->setObject(__Integer::create( info->dwPageIndex),"curPage");

					__Array* auctionItems = __Array::createWithCapacity(itemCnt);
					for (int i= 0 ; i < itemCnt ; i ++)
					{
						CMD_GP_HistoryAuctionRecordItem pRecordItem = info->RecordItem[i];
						__Dictionary* dic = __Dictionary::create();
						dic->setObject(__Integer::create(pRecordItem.dwIndex),"index");//单号
						dic->setObject(__String::create(pRecordItem.szNickName),"nickName");//拍卖者昵称
						dic->setObject(__Integer::create(pRecordItem.dwPropNum),"propNum");//拍卖物品个数
						dic->setObject(__String::create(pRecordItem.szAuctionName),"auctionName");//拍卖物品名称
						dic->setObject(__Integer::create(pRecordItem.lGold),"cost");//拍卖物品价格
						dic->setObject(__Integer::create(pRecordItem.dwType),"type");//拍卖类型
						__String* date_str = __String::createWithFormat("%04d-%02d-%02d",pRecordItem.tOpDate.wYear,
																		pRecordItem.tOpDate.wMonth,pRecordItem.tOpDate.wDay);
						dic->setObject(__String::create(date_str->getCString()),"date");
						__String* time_str = __String::createWithFormat("%02d:%02d:%02d",
							pRecordItem.tOpDate.wHour,pRecordItem.tOpDate.wMinute,
							pRecordItem.tOpDate.wSecond);
						dic->setObject(__String::create(time_str->getCString()),"time");
						auctionItems->addObject(dic);
					}

					infoDic->setObject(auctionItems,"items");

					//to do 包存到Session中去
					
					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(getAuctionHistoryRecord,infoDic);
					
				}else if(Command.wSubCmdID == SUB_GP_BUY_AUCTION)
				{
					CMD_GP_Buy_AuctionLog* logInfo = (CMD_GP_Buy_AuctionLog*)pData;
					if (wDataSize != sizeof(CMD_GP_Buy_AuctionLog))
					{
						log("buy auction data is error");
					}
					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(buyAuctionResultMsg,pData);
					
				}else if (Command.wSubCmdID == SUB_GP_SELL_AUCTION)
				{
					CMD_GP_Sell_AuctionLog* selllog = (CMD_GP_Sell_AuctionLog*)pData;
					if (wDataSize != sizeof(CMD_GP_Sell_AuctionLog))
					{
						log("sell auction data is error");
					}
					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(sellAuctionResultMsg,pData);
					
				}else if (Command.wSubCmdID == SUB_GP_CANCEL_AUCTION)
				{
					CMD_GP_Cancel_AuctionLog* cancellog = (CMD_GP_Cancel_AuctionLog*)pData;
					if (wDataSize != sizeof(CMD_GP_Cancel_AuctionLog))
					{
						log("cancel auction data is error");
					}
					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(cancelAuctionResultMsg,pData);
				}else if (Command.wSubCmdID == SUB_GP_KNAPSACK)
				{
					CMD_GP_Knapsack*  bagInfo = (CMD_GP_Knapsack*)pData;
					if (wDataSize % sizeof(CMD_GP_Knapsack) != 0)
					{
						log("bag data is error");
					}
					int cnt = wDataSize / sizeof(CMD_GP_Knapsack);
					__Array* infoArray = __Array::createWithCapacity(cnt);
					for (int i = 0 ; i < cnt;i++)
					{
						
						GiftModel* pGift = GiftModel::create();
						pGift->setGiftId(bagInfo->dwID);
						pGift->setGiftExchageType(bagInfo->dwExchangeType);
						pGift->setGiftUseType(bagInfo->dwUseType);
						pGift->setGiftName(bagInfo->szName);
						pGift->setIconUrl(bagInfo->szImgName);
						pGift->setGiftType(bagInfo->dwPackType);
						pGift->setGiftCount(bagInfo->dwNum);
						pGift->setDesc(bagInfo->szRemark);
						pGift->setOpenDesc(bagInfo->szOpenDespict);
						pGift->setSoureDesc(bagInfo->szDespict);
						infoArray->addObject(pGift);

						bagInfo++;
					}
					if (infoArray->count()>0)
					{
						Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(getBagListMsg,infoArray);
					}
				}else if(Command.wSubCmdID == SUB_GP_KNAPSACKLOG)
				{
					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(receiveKnapsackLogMsg,pData);
				}
				else if (Command.wSubCmdID == SUB_GP_VIP_POWER)
				{
					if (wDataSize % sizeof(CMD_GP_VipPower) != 0 )
					{
						log("vip power data is error");
					}
					else
					{
						Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(getVipPowerInfo,pData);
					}
				}else if (Command.wSubCmdID == SUB_GP_VIP_AWARD)
				{
					if (wDataSize> sizeof(CMD_GP_VipAwardLog))
					{
						log("vip power data is error");
					}
					else
					{
						Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(vipAwardResultMsg,pData);
					}
				}
				else if(Command.wSubCmdID == SUB_GP_USER_TAKE_SCORE)
				{
					if (wDataSize != sizeof(CMD_GP_UserTakeScore))
					{
						log("take score data is error");
					}
					//Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(takeScoreMsg,pData);
				}
				else if(Command.wSubCmdID == SUB_GP_USER_SAVE_SCORE)
				{
					if (wDataSize != sizeof(CMD_GP_UserSaveScore))
					{
						log("save score data is error");
					}
					//Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(saveScoreMsg,pData);
				}//银行
				else if (Command.wSubCmdID == SUB_GP_USER_INSURE_INFO)
				{
					CMD_GP_UserInsureInfo* userInsure = (CMD_GP_UserInsureInfo*)pData;
					int cnt  = sizeof(CMD_GP_UserInsureInfo);
					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(queryInsureInfoMsg,pData);
				}
				else if (Command.wSubCmdID == SUB_GP_USER_INSURE_SUCCESS)
				{
					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(insureSuccessMsg,pData);
				}
				else if(Command.wSubCmdID == SUB_GP_USER_INSURE_FAILURE)
				{
					 Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(insureFailureMsg,pData);
				}
				else if (Command.wSubCmdID == SUB_GP_OPERATE_FAILURE)
				{
					 Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(operateFailureMsg,pData);
				}
				else if (Command.wSubCmdID == SUB_GP_OPERATE_SUCCESS)
				{
					 Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(operateSuccessMsg,pData);
				}
				//签到列表
				else if (Command.wSubCmdID == SUB_GP_GET_SIGNIN_TASK)
				{
					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(getAssignListMsg,pData);
				}//签到
				else if(Command.wSubCmdID == SUB_GP_SIGNIN)
				{
					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(userAssignMsg,pData);
				}//财富
				else if (Command.wSubCmdID == SUB_GP_TREASURE)
				{
					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(treasureInfoMsg,pData);
				}//转化拍卖行所得
				else if(Command.wSubCmdID == SUB_GP_CONVERSION_AUCTIONSCORE)
				{
					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(converAuctionScoreMsg,pData);
				}else if (Command.wSubCmdID == SUB_GP_SEND_CAPTCHA)//绑定手机返回
				{
					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(bindPhoneRspMsg,pData);
				}//找回密码
				else if (Command.wSubCmdID == SUB_GP_CHECK_ACCOUNT)
				{
					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(checkAccountRspMsg,pData);
				}else if (Command.wSubCmdID == SUB_GP_SET_LOGIN_PASS)
				{
					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(setPassRspMsg,pData);
				}else if (Command.wSubCmdID == SUB_GP_SET_INSUREPASS_SEND_CAPTCHA)
				{
					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(setPassRspMsg,pData);
				}//元宝兑换
				else if(Command.wSubCmdID == SUB_GP_EXCHANGE_INGOT)
				{
					 Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(exchangeIngotRspMsg,pData);
				}else if(Command.wSubCmdID == SUB_GP_MORE_GAME)
				{
					if (wDataSize % sizeof(CMD_GP_MoreGame) !=0)
					{
						log("moregame data is error");
						return false;
					}
					int cnt = wDataSize/sizeof(CMD_GP_MoreGame);
					__Array* pArray = __Array::createWithCapacity(cnt);
					CMD_GP_MoreGame* gameInfo = (CMD_GP_MoreGame*)pData;
					for (int i = 0 ; i < cnt ; i++)
					{
						__Dictionary* dic = __Dictionary::create();
						dic->setObject(__String::create(gameInfo->szGameName),"appName");
						dic->setObject(__String::create(gameInfo->szICO),"icon");
						dic->setObject(__String::create(gameInfo->szPackageName),"packageName");
						dic->setObject(__String::create(gameInfo->szDescribeString),"desc");
						dic->setObject(__String::create(gameInfo->szActivityName),"activityName");
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
						dic->setObject(__String::create(gameInfo->szUrlAndroid),"url");
#else
						dic->setObject(__String::create(gameInfo->szUrlIos),"url");
						
#endif
						gameInfo++;
						pArray->addObject(dic);
					}

					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(moreGameMsg,pArray);
				}else if(Command.wSubCmdID == SUB_GP_FEEDBACK)
				{
					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(feedBackMsg,pData);
				}
				else if(Command.wSubCmdID == SUB_GP_DOWNLOAD_RESURL)
				{
					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(downLoadReponseMsg,pData);
				}
				else if(Command.wSubCmdID == SUB_GP_GET_CAPTCHA)//获取验证码返回
				{
					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(getCaptchaRspMsg,pData);
				}else if (Command.wSubCmdID == SUB_GP_GIFT_CHANGE)
				{
					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(giftConvertRspMsg, pData);
				}
				//登录点是短连接，要主动断开 add by tjl 2015 -11 -09
				if (Command.wSubCmdID != SUB_GP_USE_KNAPSACKLOG && Command.wSubCmdID != SUB_GP_SELL_AUCTION &&
					Command.wSubCmdID != SUB_GP_BUY_AUCTION && Command.wSubCmdID != SUB_GP_CANCEL_AUCTION &&
					Command.wSubCmdID != SUB_GP_AUCTION_RECORD && Command.wSubCmdID != SUB_GP_AUCTION &&
					Command.wSubCmdID != SUB_GP_GIFT  && Command.wSubCmdID != SUB_GP_BUYGIFT)
				{
					//log("Command.wSubCmdID =%d", Command.wSubCmdID);
					//closeSoket();
				}
			}
			break;
		case MDM_MB_LOGON ://登录返回
			{
				if(Command.wSubCmdID == SUB_MB_LOGON_SUCCESS )
				{
					CMD_MB_LogonSuccess *userInfo = (CMD_MB_LogonSuccess*)pData;
					CCLOG("login server sucess nickname =%s",userInfo->szNickName);
					SessionManager::shareInstance()->setUserId(userInfo->dwUserID);
					SessionManager::shareInstance()->setBankPwdFlag(userInfo->cbInsurePwd);
					//
					OnlineUserModel* pUser = SessionManager::shareInstance()->getUser();
					pUser->setUserId(userInfo->dwUserID);
					pUser->setVipLevel(userInfo->dwVipLevel);
					pUser->setFaceId(userInfo->wFaceID);
					pUser->setUserSex(userInfo->cbGender);
					pUser->setIsReset(userInfo->cbInsurePwd);
					pUser->setUserIngot(userInfo->lIngotScore);
					pUser->setUserScore(userInfo->lUserScore);
					pUser->setUserInsure(userInfo->lUserInsure);
					pUser->setNickname(userInfo->szNickName);
					pUser->setPhone(userInfo->szPhone);
					pUser->setPassPort(userInfo->szPassPortID);
					pUser->setGameId(userInfo->dwGameID);
					pUser->setFirstCharge(userInfo->dwFirstOnLineOrder);
					//pUser->setUserJiangJuan(userInfo->lIngotScore);

					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(LoginSuccessMsg);
					SessionManager::shareInstance()->setNewTaskTag(userInfo->dwTaskCount>0);
					//SessionManager::shareInstance()->setNewMsgTag(userInfo->dwUnReadCount>0);
					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(updateRedPointMsg);

				}else if(Command.wSubCmdID == SUB_MB_QUICK_LOGIN)
				{
					log("Command.wSubCmdID == SUB_MB_QUICK_LOGIN");
					CMD_MB_Quick_Logon_Success* accountInfo = (CMD_MB_Quick_Logon_Success*)pData;
					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(QucikLoginRspMsg,pData);

				}
				else if(Command.wSubCmdID ==  SUB_MB_LOGON_FAILURE)
				{
					CMD_MB_LogonFailure* errorCode = (CMD_MB_LogonFailure*)pData;
					log("login server failure error code = %s",CommonFunction::GBKToUTF8(errorCode->szDescribeString).c_str());
					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(LoginFaildMsg,errorCode);
				}//保存任务服务器IP 地址
				else if (Command.wSubCmdID == SUB_GP_LOBBY_IP)
				{
					//效验参数
					if (wDataSize < sizeof(CMD_GP_LobbyIp)) 
					{
						log("SUB_GP_LOBBY_IP data is error");
					}
					CMD_GP_LobbyIp *pLobbyIp = (CMD_GP_LobbyIp*)pData;

					SessionManager::shareInstance()->setTaskServer(pLobbyIp->szServerIP);
					SessionManager::shareInstance()->setTaskPort(pLobbyIp->dwServerPort);
					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(loginTaskServerMsg,pData);
				}
			}
			break;
		case MDM_MB_SERVER_LIST://房间信息
			{
				if(Command.wSubCmdID == SUB_MB_LIST_SERVER )
				{

					int gameServerSize = sizeof(tagGameServer);
					int serverCount = wDataSize / gameServerSize;

					BYTE cbDataBuffer[SOCKET_TCP_PACKET + sizeof(TCP_Head)];
					memcpy(cbDataBuffer, pData, wDataSize);

					__Array* roomLevels = __Array::create();
					for (int i = 0; i < serverCount; i++)
					{
						void * pDataBuffer = cbDataBuffer + i*sizeof(tagGameServer);
						tagGameServer *gameServer = (tagGameServer*)pDataBuffer;
						RoomLevelInfo* info = RoomLevelInfo::create();
						info->setRoomMax(gameServer->dwFullCount);
						info->setOnlineCount(gameServer->dwOnLineCount);
						info->setRoomDesc(std::string(gameServer->szDescription));
						info->setRoomLevel(std::string(gameServer->szGameLevel));
						info->setRoomName(std::string(gameServer->szServerName));
						info->setRoomServerAddr(std::string(gameServer->szServerAddr));

						info->setKindIndex(gameServer->wKindID);
						info->setNodeIndex(gameServer->wNodeID);
						info->setSortIndex(gameServer->wSortID);
						info->setRoomId(gameServer->wServerID);
						info->setServerPort(gameServer->wServerPort);
						info->setRoomType((RoomType)gameServer->wServerType);
						roomLevels->addObject(info);
					}
					if(roomLevels->count()>0)
						SessionManager::shareInstance()->setAllRoomLevels(roomLevels);

				}
				else if(Command.wSubCmdID ==  SUB_MB_LIST_FINISH)
				{
					//for test 进入游戏ROOM 
					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(GetRoomListMsg);

				}
				
			}
			break;
		}
		//登录点是短连接，要主动断开 add by tjl 2015 -11 -09
		//closeSoket();
	}
	else if(wSocketID == GAMEROOM_SORKET_INDEX)
	{
		switch (Command.wMainCmdID)
		{
		case MDM_GF_GAME://游戏内的消息
			{
				for (int i=0;i<m_responseHandlers->count();i++)
				{
					BaseResponseHandler* handler = (BaseResponseHandler*)m_responseHandlers->getObjectAtIndex(i);
					handler->handleResponse(Command.wSubCmdID,pData,wDataSize);
				}
			}
			break;
		case MDM_GR_LOGON://房间登录返回
			{
				switch (Command.wSubCmdID)
				{
				case SUB_GR_LOGON_SUCCESS://登录成功
					{
						CCLOG("login realroom suceess");
						CMD_GR_LogonSuccess* ret = (CMD_GR_LogonSuccess*)pData;
						CCLOG("login userRight = %d",ret->dwUserRight);
						
					}
					break;
				case SUB_GR_UPDATE_NOTIFY://升级提示
					{
						CCLOG("need update");
					}
					break;
				case SUB_GR_LOGON_FAILURE://登录失败
					{
						CMD_GR_LogonFailure* errorCode = (CMD_GR_LogonFailure*)pData;
						Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(loginRealRoomFailure,errorCode);
					}
					break;
				case SUB_GR_LOGON_FINISH://登录完成
					{
						log("LOGON FINISH");					
					}
					break;
				}

			}
			break;
		case MDM_GR_CONFIG://配置信息
			{
				if(Command.wSubCmdID == SUB_GR_CONFIG_COLUMN)
				{

				}
				else if (Command.wSubCmdID == SUB_GR_CONFIG_FINISH )
				{
					log("config finish");
					//变量定义
					IPC_GF_MatchClient * pMatchClient=(IPC_GF_MatchClient *)pData;
					SessionManager::shareInstance()->setMatchStatus(pMatchClient->cbMatchStatus);
					
				}
				else if (Command.wSubCmdID == SUB_GR_CONFIG_SERVER)
				{
					CMD_GR_ConfigServer* pServerData = (CMD_GR_ConfigServer*)pData;
					if (pServerData->wServerType == GAME_GENRE_MATCH)
					{
						log("SUB_GR_CONFIG_SERVER");
						sendMatchUserComeRequest();
					}
				}
			}
			break;
		case MDM_GR_STATUS://房间状态信息
			{
				CCLOG("room status");
			};
			break;
		case MDM_GR_USER://用户信息
			{
				if (Command.wSubCmdID == SUB_GR_USER_ENTER)
				{
					tagMobileUserInfoHead* mobieInfo = (tagMobileUserInfoHead*)pData;
					UserModel* pUserInfo = SessionManager::shareInstance()->getUserById(mobieInfo->dwUserID);
					if (pUserInfo)
					{
						pUserInfo->setUserScore(mobieInfo->lScore);//设置用户金币数
					}
					else
					{
						__Array* users = SessionManager::shareInstance()->getAllUsers();
						pUserInfo = UserModel::create();
						pUserInfo->setUserScore(mobieInfo->lScore);//设置用户金币数
						pUserInfo->setUserId(mobieInfo->dwUserID);
						if (mobieInfo->cbUserStatus == US_PLAYING || mobieInfo->cbUserStatus == US_SIT)
						{
							pUserInfo->setChairId(mobieInfo->wChairID);
							pUserInfo->setTableId(mobieInfo->wTableID);
							pUserInfo->setUserStatus(mobieInfo->cbUserStatus);
						}
						users->addObject(pUserInfo);
					}

					
					/*if (pUserInfo->getChairId() >=0 && pUserInfo->getChairId() < GAME_PLAYER)
					{
						if (mobieInfo->dwUserID == SessionManager::shareInstance()->getUserId() && (mobieInfo->wChairID >=0 && mobieInfo->wChairID < GAME_PLAYER))
						{
							SessionManager::shareInstance()->setChairId(mobieInfo->wChairID);
						}
						//用户进入
						if (mobieInfo->cbUserStatus == US_FREE || mobieInfo->cbUserStatus == US_PLAYING )
							Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(GetUserStatusMsg,pUserInfo);
					}*/
					
				}
				else if(Command.wSubCmdID == SUB_GR_USER_STATUS)
				{
					//用户进入
					CMD_GR_UserStatus* userInfo = (CMD_GR_UserStatus*)(pData);
					UserModel* pUserInfo = SessionManager::shareInstance()->getUserById(userInfo->dwUserID);
					if (pUserInfo)
					{
						if(userInfo->UserStatus.cbUserStatus == US_SIT || userInfo->UserStatus.cbUserStatus == US_PLAYING )
						{
							pUserInfo->setChairId(userInfo->UserStatus.wChairID);
							pUserInfo->setTableId(userInfo->UserStatus.wTableID);
							pUserInfo->setUserStatus(userInfo->UserStatus.cbUserStatus);
						}
					}
					else
					{
						__Array* users = SessionManager::shareInstance()->getAllUsers();
						pUserInfo = UserModel::create();
						pUserInfo->setChairId(userInfo->UserStatus.wChairID);
						pUserInfo->setTableId(userInfo->UserStatus.wTableID);
						pUserInfo->setUserStatus(userInfo->UserStatus.cbUserStatus);
						pUserInfo->setUserId(userInfo->dwUserID);
						users->addObject(pUserInfo);
					}
				
					//进入游戏ROOM 
					if ( (userInfo->UserStatus.cbUserStatus == US_SIT || userInfo->UserStatus.cbUserStatus == US_PLAYING) && pUserInfo->getChairId() >= 0 && pUserInfo->getChairId() < GAME_PLAYER )
					{
						if (userInfo->dwUserID == SessionManager::shareInstance()->getUserId())//自已发登录房间成功消息
						{
							if(!SessionManager::shareInstance()->isInGame())
							{
								SessionManager::shareInstance()->setChairId(userInfo->UserStatus.wChairID);
								SessionManager::shareInstance()->addPlayer(pUserInfo);
								Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(loginRealRoomSuccess,pUserInfo);
							}	
						}
						else
						{
							//此处一定要保证自已第一个进入桌子里面
							if (SessionManager::shareInstance()->getHasFinish())
							{
								log("my self is sit other player enter =%d",pUserInfo->getTableId());
								Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(GetUserStatusMsg,pUserInfo);
							}
							else
							{
								SessionManager::shareInstance()->addPlayer(pUserInfo);
							}
						}
					}
					else if (userInfo->UserStatus.cbUserStatus == US_FREE && pUserInfo->getChairId() >= 0 && pUserInfo->getChairId() < GAME_PLAYER)
					{
						//if (pUserInfo->getUserId() == SessionManager::shareInstance()->getUserId())
						//{
						//	if (!SessionManager::shareInstance()->isInGame())
						//	{
						//		//GameServiceClientManager::sharedInstance()->removeServiceClient(XwZoneDntg);
						//	}	
						//}
						//else
						{
							Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(PlayerToSeperaterMsg,pUserInfo);
						}
					}
				}
			}
			break;
		case MDM_GF_FRAME://游戏框架内的消息
			{
				for (int i=0;i<m_responseHandlers->count();i++)
				{
					BaseResponseHandler* handler = (BaseResponseHandler*)m_responseHandlers->getObjectAtIndex(i);
					handler->handleResponse((int)MAKELONG(Command.wMainCmdID,Command.wSubCmdID),pData,wDataSize);
				}
			}
			break;
		}
	}
	else if(wSocketID == TASK_SORKET_INDEX)
	{
		//log("TASK_SORKET_INDEX  mainCmdID %d ,subCmdID = %d",Command.wMainCmdID,Command.wSubCmdID);
		switch (Command.wMainCmdID)
		{
		case MDM_GL_C_DATA:
			{
				if (Command.wSubCmdID == SUB_GL_C_LOGON_ACCOUNTS )
				{

				}else if(Command.wSubCmdID == SUB_GL_LOGON_SUCCESS)
				{
					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(loginTaskServerSuccessMsg);
				}
				else if(Command.wSubCmdID == SUB_GL_C_SYSTEM_MESSAGE)//大厅系统消息
				{
					CMD_GL_SystemMessage* info = (CMD_GL_SystemMessage*)pData;
					if (info->wType & SMT_CLOSE_HALL )//
					{
						Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(loginAtOtherPlace,pData);
					}else if(info->wType & (SMT_CLOSE_ROOM|SMT_CLOSE_LINK))
					{
						//您的游戏次数已达到上限，请等待本场结束后的排名结果！
						Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(MatchPlayCountIsMaxMsg);
					}
				}
				else if(Command.wSubCmdID == SUB_GL_C_TASK_LOAD)//任务列表返回
				{
					if(wDataSize % sizeof(CMD_GL_TaskInfo) != 0 )
					{
						log("task data package is error");
					}
					int taskCnt = wDataSize / sizeof(CMD_GL_TaskInfo);
					SessionManager::shareInstance()->setTaskCnt(taskCnt);
					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(getTaskListMsg,pData);
				}
				else if(Command.wSubCmdID == SUB_GL_C_TASK_REWARD)//任务奖励返回
				{
					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(getTaskAwardMsg,pData);
				}
				else if (Command.wSubCmdID == SUB_GL_C_UPDATE_NOTIFY)
				{
					log("need upgrade client version");
				}
				else if (Command.wSubCmdID == SUB_GL_C_MESSAGE)//消息
				{
					if (wDataSize % sizeof(CMD_GL_MsgNode) !=0 )
					{
						log("msg data is error");
					}
					int cnt = wDataSize / sizeof(CMD_GL_MsgNode);
					CMD_GL_MsgNode* msgNode = (CMD_GL_MsgNode*)pData;
					__Array* pArray = SessionManager::shareInstance()->getMsgArray();
					for ( int i = 0 ; i < cnt ;i++)
					{
						__Dictionary* dic = __Dictionary::create();
						dic->setObject(__Integer::create(msgNode->dwMsgID),"msgId");
						dic->setObject(__Integer::create(msgNode->dwUserID),"userId");
						dic->setObject(__Integer::create(msgNode->dwMsgType),"msgType");
						

						if (msgNode->dwMsgType == Msg_Sell_Success)
						{
							
							dic->setObject(__String::create(msgNode->szMsgcontent),"msgContent");
							
							SessionManager::shareInstance()->setNewAuctionTag(true);
							SessionManager::shareInstance()->setNewMsgTag(true);
							Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(updateRedPointMsg);
						}else if (msgNode->dwMsgType == Msg_Rewards)
						{
							std::vector< std::string > contents;
							// for test std::string szMsgcontent = "恭喜，您已经成功充值10元宝。#30#300#";
							pystring::split(msgNode->szMsgcontent, contents,"#") ;
							dic->setObject(__String::create(contents[0]),"msgContent");
							SessionManager::shareInstance()->setNewTaskTag(true);
							SessionManager::shareInstance()->setNewMsgTag(true);
							Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(updateRedPointMsg);
						}else if (msgNode->dwMsgType == Msg_Notice)
						{
							dic->setObject(__String::create(msgNode->szMsgcontent),"msgContent");
							if (!SessionManager::shareInstance()->hasRead(msgNode->dwMsgID))
							{
								SessionManager::shareInstance()->setNewMsgTag(true);
								Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(updateRedPointMsg);
							}
						}else if (msgNode->dwMsgType == Msg_Delta)
						{
							std::vector< std::string > contents;
							// for test std::string szMsgcontent = "恭喜，您已经成功充值10元宝。#30#300#";
							pystring::split(msgNode->szMsgcontent, contents,"#") ;

							for (int i =0 ; i < contents.capacity();i++)
							{
								std::string content = contents[i];
								if (i == 0)//消息内容
								{
									dic->setObject(__String::create(content),"msgContent");
									SessionManager::shareInstance()->setNewMsgTag(true);
									Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(updateRedPointMsg);
								}else if( i== 1)//当前身上元宝
								{
									__Dictionary* ingotDic = __Dictionary::create();
									ingotDic->setObject(__String::create(content),"ingot");
									Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(RechargeSuccessMsg,ingotDic);
								}
							}
						}
						else if (msgNode->dwMsgType == Msg_Vip)
						{
						   OnlineUserModel* pModel = SessionManager::shareInstance()->getUser();
						   pModel->setVipLevel(atoi(msgNode->szMsgcontent));
						   Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(updateVipLevelMsg);
						   continue;
						}
						pArray->addObject(dic);
						msgNode++;
					}
				}else //大厅比赛相关消息处理
				{
					if (Command.wSubCmdID == SUB_GL_C_MATCH_DELETE)
					{
						int i = 99;
					}
					for (int i=0;i<m_responseHandlers->count();i++)
					{
						BaseResponseHandler* handler = (BaseResponseHandler*)m_responseHandlers->getObjectAtIndex(i);
						handler->handleResponse((int)MAKELONG(Command.wMainCmdID,Command.wSubCmdID),pData,wDataSize);
					}
				}
			}
			break;
		default:
			break;
		}
	}
	return true;
}
void GameServiceClient::update(float dt)  
{  
	retain();
	if (m_GameSocket.IsConnected())
	{
		if (!m_GameSocket.Check()) {
			OnEventTCPSocketShut(1);
			release();
			return;
		}
		
		m_GameSocket.Flush();
		
		while (m_GameSocket.IsConnected())
		{
			char buffer[_MAX_MSGSIZE] = { 0 };
			int nSize = sizeof(buffer);
			char* pbufMsg = buffer;
			if (!m_GameSocket.ReceiveMsg(pbufMsg, nSize)) {
				break;
			}

			TCP_Head* pHead = (TCP_Head*)pbufMsg;
			OnTCPSocketRead(m_clientType, pHead->CommandInfo, pHead + 1, nSize - sizeof(TCP_Head));

		}
	}


	if (m_bConnectFail)
	{

		if(m_clientType == GAMEROOM_SORKET_INDEX)
		{
			Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(connectGameServerFaildMsg);
		}
		else
		{	
			if (m_clientType == FRAME_SORKET_INDEX)
			{
				SessionManager::shareInstance()->setLoginAddrNull();

				LoginUserModel* model = SessionManager::shareInstance()->getLoginModel();
				if (model->getLoginUrlIndex() + 1 >= SessionManager::shareInstance()->getLoginUrls()->count())
				{
					model->setLoginUrlIndex(0);
				}
				else
				{
					model->setLoginUrlIndex(model->getLoginUrlIndex() + 1);
				}
			}
			Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(netWorkValidMsg);
		}
		
		m_bConnectFail = false;
	}
	release();
}

bool GameServiceClient::OnTCPNetworkMainMBLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	CCLOG("login success");
	return true;
}
bool GameServiceClient::OnTCPNetworkMainMBCount(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	tagMDCount  *pMDCount = (tagMDCount *)pData;
	if(pMDCount == NULL)
		return true;
	return true;
}

//登录
void GameServiceClient::onLogin(const char* accout,const char* pwd)
{

	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}
	
	
	CMD_MB_LogonAccounts logonAccounts;
	memset(&logonAccounts,0,sizeof(CMD_MB_LogonAccounts));

	logonAccounts.cbDeviceType = 4;
	logonAccounts.dwPlazaVersion = VERSION_PLAZA;

	strcpy(logonAccounts.szAccounts,accout);

	std::string uuid = CommonFunction::getUUID();
	std::string Bit32UUid = Crypto::MD5String((void*)uuid.c_str(), strlen(uuid.c_str()));

	Bit32UUid = "mobile" + pystring::slice(Bit32UUid,0,26);
	strcpy(logonAccounts.szMachineID,Bit32UUid.c_str());

	strcpy(logonAccounts.szMobilePhone,"");
	strcpy(logonAccounts.szPassPortID,"12");
	strcpy(logonAccounts.szPhoneVerifyID,"1");



	logonAccounts.wModuleID[0] = 260; //DNTG
	logonAccounts.wModuleID[1] = 290; //李逵劈鱼
	logonAccounts.wModuleID[2] = 250; //jcpy

	/*MD5 m;
	m.ComputMd5(pwd, strlen(pwd));
	std::string md5PassWord = m.GetMd5();*/

	std::string md5PassWord = Crypto::MD5String((void*)pwd, strlen(pwd));
	
	strcpy(logonAccounts.szPassword,md5PassWord.c_str());

	m_GameSocket.SendMsg(MDM_MB_LOGON, SUB_MB_LOGON_ACCOUNTS, &logonAccounts, sizeof(logonAccounts));
	
}

//微信登录
void GameServiceClient::onWxLogin(const char* token)
{
	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(), serverPort);
	}

	CMD_MB_AccessToken logonToken;
	logonToken.dwSessionID = 111112;
	strcpy(logonToken.szUMId, "");
	logonToken.dwSex = 0;
	strcpy(logonToken.szNickName, "");

	std::string uuid = CommonFunction::getUUID();
	std::string Bit32UUid = Crypto::MD5String((void*)uuid.c_str(), strlen(uuid.c_str()));

	Bit32UUid = "mobile" + pystring::slice(Bit32UUid, 0, 26);
	strcpy(logonToken.szMachineID, Bit32UUid.c_str());

	strcpy(logonToken.szAccessToken, token);
	m_GameSocket.SendMsg(MDM_MB_LOGON, SUB_MB_ACCESSTOKEN, &logonToken, sizeof(logonToken));
}

//快速登录
void GameServiceClient::sendQucikLogin(const char* uuid)
{
	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}

	CMD_MB_Quick_Logon qucikLogon;
	qucikLogon.dwOpTerminal = dwOptermianlValue;
	qucikLogon.dwSessionID  = atoi(k_session_id);

	std::string code = "server";
	code.append(k_session_id);
	code.append(k_session_verion);
	code.append("lmyspread");

	std::string md5Code = Crypto::MD5String((void*)code.c_str(), code.length());
	strcpy(qucikLogon.szStatisCode,md5Code.c_str());

	strcpy(qucikLogon.szMachineID,uuid);

	m_GameSocket.SendMsg(MDM_MB_LOGON, SUB_MB_QUICK_LOGIN, &qucikLogon, sizeof(qucikLogon));
}
//注册
void GameServiceClient::onRegister(char* account,char* nick,char* pwd)
{
	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}

	CMD_MB_RegisterAccounts registerAccounts;

	registerAccounts.cbDeviceType = 4;
	registerAccounts.dwPlazaVersion = VERSION_PLAZA;
	registerAccounts.wModuleID[0] = 260; //DNTG
	registerAccounts.wModuleID[1] = 290; //李逵劈鱼
	registerAccounts.wModuleID[2] = 250; //jcpy

	strcpy(registerAccounts.szAccounts,account);

	/*MD5 m;
	m.ComputMd5(pwd, strlen(pwd));
	std::string md5PassWord = m.GetMd5();*/

	std::string md5PassWord = Crypto::MD5String((void*)pwd, strlen(pwd));

	strcpy(registerAccounts.szLogonPass, md5PassWord.c_str());//登录密码
	strcpy(registerAccounts.szInsurePass, md5PassWord.c_str());//银行密码


	strcpy(registerAccounts.szNickName,nick);

	std::string uuid = CommonFunction::getUUID();
	std::string Bit32UUid = Crypto::MD5String((void*)uuid.c_str(), strlen(uuid.c_str()));

	Bit32UUid = "mobile" + pystring::slice(Bit32UUid,0,26);
	strcpy(registerAccounts.szMachineID,Bit32UUid.c_str());
	strcpy(registerAccounts.szMobilePhone, "");

	registerAccounts.wFaceID = 1;//头像标识
	registerAccounts.cbGender = 1;//用户性别

	m_GameSocket.SendMsg(MDM_MB_LOGON, SUB_MB_REGISTER_ACCOUNTS, &registerAccounts, sizeof(registerAccounts));
	
}
//重连
void GameServiceClient::retry()
{

}



void GameServiceClient::cleanup() {
	Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
	closeSoket();
}

void GameServiceClient::logout()
{
	//发登出消息
}

void GameServiceClient::sendGetGameConfig()
{
	CMD_GF_GameOption GameOption;
	memset(&GameOption, 0, sizeof(GameOption));

	GameOption.dwFrameVersion=VERSION_FRAME;
	GameOption.cbAllowLookon=0;
	GameOption.dwClientVersion=VERSION_CLIENT;

	m_GameSocket.SendMsg(MDM_GF_FRAME,SUB_GF_GAME_OPTION, &GameOption, sizeof(CMD_GF_GameOption));
}

void GameServiceClient::sendGetGameMatchConfig()
{
	CMD_GF_GameOptionMatch GameOption;
	memset(&GameOption, 0, sizeof(GameOption));

	GameOption.dwFrameVersion=VERSION_FRAME;
	GameOption.cbAllowLookon=0;
	GameOption.dwClientVersion=VERSION_CLIENT;
	GameOption.cbMatchStatus = SessionManager::shareInstance()->getMatchStatus();
	m_GameSocket.SendMsg(MDM_GF_FRAME,SUB_GF_GAME_OPTION, &GameOption, sizeof(CMD_GF_GameOptionMatch));
}

void GameServiceClient::loginRealRoom(RoomLevelInfo* info)
{

	CMD_GR_LogonMobile logonUserID;
	memset(&logonUserID, 0, sizeof(CMD_GR_LogonMobile));
	logonUserID.wBehaviorFlags = BEHAVIOR_LOGON_IMMEDIATELY;
	logonUserID.wPageTableCount = 10;
	logonUserID.dwProcessVersion= 16973825;
	logonUserID.dwUserID=SessionManager::shareInstance()->getUserId();

	std::string uuid = CommonFunction::getUUID();
	std::string Bit32UUid = Crypto::MD5String((void*)uuid.c_str(), strlen(uuid.c_str()));

	Bit32UUid = "mobile" + pystring::slice(Bit32UUid,0,26);
	
	strcpy(logonUserID.szMachineID,Bit32UUid.c_str());

	LoginUserModel* loginModel = SessionManager::shareInstance()->getLoginModel();

	std::string md5PassWord = Crypto::MD5String((void*)loginModel->getPassword().c_str(), strlen(loginModel->getPassword().c_str()));

	strcpy(logonUserID.szPassword,md5PassWord.c_str());

	logonUserID.wGameID = info->getKindIndex();

	int luidSize=sizeof(CMD_GR_LogonMobile);

	m_GameSocket.SendMsg(MDM_GR_LOGON, SUB_GR_LOGON_MOBILE, &logonUserID, luidSize);

}

void GameServiceClient::loginMatchRoom()
{
	CMD_GR_LogonMobile logonUserID;
	memset(&logonUserID, 0, sizeof(CMD_GR_LogonMobile));
	logonUserID.wBehaviorFlags = BEHAVIOR_LOGON_IMMEDIATELY;
	logonUserID.wPageTableCount = 10;
	logonUserID.dwProcessVersion= 16973825;
	logonUserID.dwUserID=SessionManager::shareInstance()->getUserId();

	std::string uuid = CommonFunction::getUUID();
	std::string Bit32UUid = Crypto::MD5String((void*)uuid.c_str(), strlen(uuid.c_str()));

	Bit32UUid = "mobile" + pystring::slice(Bit32UUid,0,26);

	strcpy(logonUserID.szMachineID,Bit32UUid.c_str());

	LoginUserModel* loginModel = SessionManager::shareInstance()->getLoginModel();

	std::string md5PassWord = Crypto::MD5String((void*)loginModel->getPassword().c_str(), strlen(loginModel->getPassword().c_str()));

	strcpy(logonUserID.szPassword,md5PassWord.c_str());

	logonUserID.wGameID = SessionManager::shareInstance()->getGameMatchKind();

	m_GameSocket.SendMsg(MDM_GR_LOGON, SUB_GR_LOGON_MOBILE, &logonUserID, sizeof(logonUserID));
}

void GameServiceClient::sendUserFire(float angle,int bullet_mulriple)
{
	if (SessionManager::shareInstance()->getChairId() < 3)
	{
		angle +=M_PI;
	}

	CMD_C_UserFire user_fire;
	user_fire.angle = angle;

	user_fire.bullet_mulriple = bullet_mulriple;
	user_fire.lock_fish_id = -1;
	user_fire.tick_count =TimerManager::shareInstance()->GetServerTick();

	m_GameSocket.SendMsg(MDM_GF_GAME,SUB_C_USER_FIRE, &user_fire, sizeof(user_fire));
}

void GameServiceClient::exchangeScore(int score)
{
	CMD_C_ExchangeFishScore exchange_fishscore;
	exchange_fishscore.exchange_score = -1;
	m_GameSocket.SendMsg(MDM_GF_GAME,SUB_C_EXCHANGE_FISHSCORE, &exchange_fishscore, sizeof(exchange_fishscore));
}

void GameServiceClient::SendTimerSync()
{
	CMD_C_TimerSync timer_sync;
	timer_sync.client_tick = SessionManager::shareInstance()->getCurrentTime();
	m_GameSocket.SendMsg(MDM_GF_GAME,SUB_C_TIMER_SYNC,&timer_sync,sizeof(timer_sync));
}

void GameServiceClient::sendStandUp()
{
	CMD_GR_UserStandUp standup;
	UserModel* mode = SessionManager::shareInstance()->getUserById(SessionManager::shareInstance()->getUserId());
	if(mode)
	{
		standup.wTableID = mode->getTableId();
		standup.wChairID = mode->getChairId();
		standup.cbForceLeave = true;
		m_GameSocket.SendMsg(MDM_GR_USER,SUB_GR_USER_STANDUP,&standup,sizeof(standup));
	}
}

void GameServiceClient::SendRequestShop(int type)
{

	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}

	CMD_GP_GetGift getGift;
	getGift.dwOpTerminal = dwOptermianlValue ;
	getGift.dwUserID = SessionManager::shareInstance()->getUserId();
	if (type == 1)
	{
		m_GameSocket.SendMsg(MDM_GP_USER_SERVICE, SUB_GP_GIFT,&getGift,sizeof(getGift));
	}
	else if(type == 2)
	{
		m_GameSocket.SendMsg(MDM_GP_USER_SERVICE, SUB_GP_PROPERTY,&getGift,sizeof(getGift));
	}else if(type == 3)
	{
		m_GameSocket.SendMsg(MDM_GP_USER_SERVICE,SUB_GP_VIPSHOP,&getGift,sizeof(getGift));
	}
}

void  GameServiceClient::sendBuyGiftRequest(int proType, int proNum,int proId)//购买 兑换道具
{

	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}

	 CMD_GP_BuyGift buyGift;
	 buyGift.dwUserID = SessionManager::shareInstance()->getUserId();
	 buyGift.dwBuyMethod = 4;
	 buyGift.dwID = proId;
	 buyGift.dwNum = proNum;
	 buyGift.dwOpTerminal = dwOptermianlValue;
	 buyGift.dwType = proType;

	 LoginUserModel* loginModel = SessionManager::shareInstance()->getLoginModel();
	/* MD5 m;
	 m.ComputMd5(loginModel->getPassword().c_str(),strlen(loginModel->getPassword().c_str()));
	 std::string md5PassWord = m.GetMd5();*/

	 std::string md5PassWord = Crypto::MD5String((void*)loginModel->getPassword().c_str(), strlen(loginModel->getPassword().c_str()));

	 strcpy(buyGift.szLogonPass,md5PassWord.c_str());
	
	 std::string uuid = CommonFunction::getUUID();
	 std::string Bit32UUid = Crypto::MD5String((void*)uuid.c_str(), strlen(uuid.c_str()));

	 Bit32UUid = "mobile" + pystring::slice(Bit32UUid,0,26);
	 strcpy(buyGift.szMachineID,Bit32UUid.c_str());

	 m_GameSocket.SendMsg(MDM_GP_USER_SERVICE,SUB_GP_BUYGIFT,&buyGift,sizeof(buyGift) );
}

void  GameServiceClient::sendUserKnapsackRequset(int id,int num,const char* szNote)//使用背包
{
	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}

	CMD_GP_UseKnapsack userKnap;
	userKnap.dwUserID = SessionManager::shareInstance()->getUserId();
	userKnap.dwID = id;
	userKnap.dwNum = num;
	userKnap.dwOpTerminal = dwOptermianlValue;
	if (strlen(szNote) > 0 )//QQ号码/手机号码
	{
		strcpy(userKnap.szNote,szNote);
	}

	LoginUserModel* loginModel = SessionManager::shareInstance()->getLoginModel();
	/*MD5 m;
	m.ComputMd5(loginModel->getPassword().c_str(),strlen(loginModel->getPassword().c_str()));
	std::string md5PassWord = m.GetMd5();*/

	std::string md5PassWord = Crypto::MD5String((void*)loginModel->getPassword().c_str(), strlen(loginModel->getPassword().c_str()));

	strcpy(userKnap.szPassword,md5PassWord.c_str());

	std::string uuid = CommonFunction::getUUID();
	std::string Bit32UUid = Crypto::MD5String((void*)uuid.c_str(), strlen(uuid.c_str()));

	Bit32UUid = "mobile" + pystring::slice(Bit32UUid,0,26);
	strcpy(userKnap.szMachineID,Bit32UUid.c_str());

	 m_GameSocket.SendMsg(MDM_GP_USER_SERVICE,SUB_GP_USE_KNAPSACKLOG,&userKnap,sizeof(userKnap) );
}


void GameServiceClient::SendAuctionRequset(int subCmd,int pageIndex,int pageCount,WORD userId,int lastday)
{
	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}

	CMD_GP_GetAuctionRecord qAuction;
	qAuction.dwPage = pageIndex;
	qAuction.dwPageSize = pageCount;
	qAuction.dwUserID = userId;
	qAuction.dwLastDay=lastday;
	m_GameSocket.SendMsg(MDM_GP_USER_SERVICE, subCmd, &qAuction, sizeof(qAuction));
	

	
}

//发送获取背包列表请求
void GameServiceClient::sendKnapSackRequest()
{

	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}

	CMD_GP_UserID gpUserID;
	gpUserID.dwUserID = SessionManager::shareInstance()->getUserId();

	/*MD5 m;
	m.ComputMd5(SessionManager::shareInstance()->getPwd().c_str(), SessionManager::shareInstance()->getPwd().length());
	std::string md5PassWord = m.GetMd5();*/

	std::string md5PassWord = Crypto::MD5String((void*)SessionManager::shareInstance()->getPwd().c_str(), strlen(SessionManager::shareInstance()->getPwd().c_str()));

	strcpy(gpUserID.szPassword, md5PassWord.c_str());

	m_GameSocket.SendMsg(MDM_GP_USER_SERVICE, SUB_GP_KNAPSACK, &gpUserID, sizeof(gpUserID));
}
//发送获取上架物品列表请求
void GameServiceClient::SendAuctionListRequset()
{

	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}

	CMD_GP_UserID gpUserID;
	gpUserID.dwUserID = SessionManager::shareInstance()->getUserId();

	/*MD5 m;
	m.ComputMd5(SessionManager::shareInstance()->getPwd().c_str(), SessionManager::shareInstance()->getPwd().length());
	std::string md5PassWord = m.GetMd5();*/

	std::string md5PassWord = Crypto::MD5String((void*)SessionManager::shareInstance()->getPwd().c_str(), strlen(SessionManager::shareInstance()->getPwd().c_str()));
	strcpy(gpUserID.szPassword, md5PassWord.c_str());

	m_GameSocket.SendMsg(MDM_GP_USER_SERVICE, SUB_GP_AUCTION, &gpUserID, sizeof(gpUserID));

}

//发送拍卖物品请求
void GameServiceClient::sendSellAuctionRequest(__Dictionary*info)
{

	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}

	CMD_GP_Sell_Auction sellAuction;
	sellAuction.dwUserID = SessionManager::shareInstance()->getUserId();
	/*MD5 m;
	m.ComputMd5(SessionManager::shareInstance()->getPwd().c_str(), SessionManager::shareInstance()->getPwd().length());
	std::string md5PassWord = m.GetMd5();*/
	std::string md5PassWord = Crypto::MD5String((void*)SessionManager::shareInstance()->getPwd().c_str(), strlen(SessionManager::shareInstance()->getPwd().c_str()));
	strcpy(sellAuction.szLogonPass, md5PassWord.c_str());

	int proId = ((__Integer*)info->objectForKey("index"))->getValue();
	sellAuction.dwID = proId;

	int count = ((__Integer*)info->objectForKey("propNum"))->getValue();
	sellAuction.dwNum = count;

	sellAuction.lGold = ((__Integer*)info->objectForKey("cost"))->getValue();
	sellAuction.dwOpTerminal = dwOptermianlValue;

	std::string uuid = CommonFunction::getUUID();
	std::string Bit32UUid = Crypto::MD5String((void*)uuid.c_str(), strlen(uuid.c_str()));

	Bit32UUid = "mobile" + pystring::slice(Bit32UUid,0,26);
	strcpy(sellAuction.szMachineID,Bit32UUid.c_str());

	m_GameSocket.SendMsg(MDM_GP_USER_SERVICE, SUB_GP_SELL_AUCTION, &sellAuction, sizeof(sellAuction));
}

//发送购买物品请求
void GameServiceClient::sendBuyAuctionRequest(__Dictionary*info)
{

	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}

	CMD_GP_Buy_Auction buyAuction;
	buyAuction.dwUserID = SessionManager::shareInstance()->getUserId();
	//MD5 m;
	//m.ComputMd5(SessionManager::shareInstance()->getPwd().c_str(), SessionManager::shareInstance()->getPwd().length());
	//std::string md5PassWord = m.GetMd5();
	std::string md5PassWord = Crypto::MD5String((void*)SessionManager::shareInstance()->getPwd().c_str(), strlen(SessionManager::shareInstance()->getPwd().c_str()));
	strcpy(buyAuction.szLogonPass, md5PassWord.c_str());

	int proId = ((__Integer*)info->objectForKey("index"))->getValue();
	buyAuction.dwID = proId;

	int count = ((__Integer*)info->objectForKey("buyCount"))->getValue();
	buyAuction.dwNum = count;

	buyAuction.dwOpTerminal = dwOptermianlValue;

	std::string uuid = CommonFunction::getUUID();
	std::string Bit32UUid = Crypto::MD5String((void*)uuid.c_str(), strlen(uuid.c_str()));

	Bit32UUid = "mobile" + pystring::slice(Bit32UUid,0,26);
	strcpy(buyAuction.szMachineID,Bit32UUid.c_str());

	m_GameSocket.SendMsg(MDM_GP_USER_SERVICE, SUB_GP_BUY_AUCTION, &buyAuction, sizeof(buyAuction));
}

//发送撤销拍卖请求
void GameServiceClient::sendCancelAuctionRequest(__Dictionary* info)
{
	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}

	CMD_GP_Cancel_Auction cancelAuction;
	cancelAuction.dwUserID = SessionManager::shareInstance()->getUserId();
	//MD5 m;
	//m.ComputMd5(SessionManager::shareInstance()->getPwd().c_str(), SessionManager::shareInstance()->getPwd().length());
	std::string md5PassWord = Crypto::MD5String((void*)SessionManager::shareInstance()->getPwd().c_str(), strlen(SessionManager::shareInstance()->getPwd().c_str()));
	strcpy(cancelAuction.szLogonPass, md5PassWord.c_str());

	int proId = ((__Integer*)info->objectForKey("index"))->getValue();
	cancelAuction.dwID = proId;

	cancelAuction.dwOpTerminal = dwOptermianlValue;

	std::string uuid = CommonFunction::getUUID();
	std::string Bit32UUid = Crypto::MD5String((void*)uuid.c_str(), strlen(uuid.c_str()));

	Bit32UUid = "mobile" + pystring::slice(Bit32UUid,0,26);
	strcpy(cancelAuction.szMachineID,Bit32UUid.c_str());

	m_GameSocket.SendMsg(MDM_GP_USER_SERVICE, SUB_GP_CANCEL_AUCTION, &cancelAuction, sizeof(cancelAuction));
}

void GameServiceClient::sendQueryAuctionRequest(int pageIndex,int pageCount,char* text,int lastday )
{
	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}

	CMD_GP_Query_Auction qAuction;
	qAuction.dwPage = pageIndex;
	qAuction.dwPageSize = pageCount;
	strcpy(qAuction.szID,text);
	qAuction.dwLastDay=lastday;
	m_GameSocket.SendMsg(MDM_GP_USER_SERVICE, SUB_GP_QUERY_AUCTION, &qAuction, sizeof(qAuction));
}

void GameServiceClient::sendVipPowerRequest()
{
	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}

	CMD_GP_VipUserID vipPower ;
	vipPower.dwUserID = SessionManager::shareInstance()->getUserId();
	vipPower.dwOpTerminal = dwOptermianlValue;
	m_GameSocket.SendMsg(MDM_GP_USER_SERVICE, SUB_GP_VIP_POWER, &vipPower, sizeof(vipPower));
}

void GameServiceClient::sendVipAwardRequest(int awardType)
{
	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}

	CMD_GP_VipAward award;
	award.dwUserID = SessionManager::shareInstance()->getUserId();
	award.dwOpTerminal = dwOptermianlValue;
	award.dwAwardType = awardType;
	/*MD5 m;
	m.ComputMd5(SessionManager::shareInstance()->getPwd().c_str(), SessionManager::shareInstance()->getPwd().length());
	std::string md5PassWord = m.GetMd5();*/
	std::string md5PassWord = Crypto::MD5String((void*)SessionManager::shareInstance()->getPwd().c_str(), strlen(SessionManager::shareInstance()->getPwd().c_str()));
	strcpy(award.szLogonPass, md5PassWord.c_str());
	m_GameSocket.SendMsg(MDM_GP_USER_SERVICE, SUB_GP_VIP_AWARD, &award, sizeof(award));
}

//登录任务服务器
void GameServiceClient::loginTaskServer()
{
	bool ret = m_GameSocket.Connect(SessionManager::shareInstance()->getTaskServer().c_str(),SessionManager::shareInstance()->getTaskPort());
	log("loginTaskServer ret = %d",ret);
	CMD_GL_LogonAccounts LogonAccounts;
	LoginUserModel* loginModel = SessionManager::shareInstance()->getLoginModel();

	if(loginModel->getAccountType() == accountType::accountMachineID)
		strcpy(LogonAccounts.szAccounts, loginModel->getQucikAccount().c_str());
	else
		strcpy(LogonAccounts.szAccounts, loginModel->getAccount().c_str());

	std::string uuid = CommonFunction::getUUID();
	std::string Bit32UUid = Crypto::MD5String((void*)uuid.c_str(), strlen(uuid.c_str()));

	Bit32UUid = "mobile" + pystring::slice(Bit32UUid,0,26);
	strcpy(LogonAccounts.szMachineID,Bit32UUid.c_str());
	/*MD5 m;
	m.ComputMd5(loginModel->getPassword().c_str(), loginModel->getPassword().length());
	std::string md5PassWord = m.GetMd5();*/
	std::string md5PassWord = Crypto::MD5String((void*)loginModel->getPassword().c_str(), strlen(loginModel->getPassword().c_str()));
	strcpy(LogonAccounts.szPassword, md5PassWord.c_str());

	m_GameSocket.SendMsg(MDM_GL_C_DATA, SUB_GL_MB_LOGON_ACCOUNTS, &LogonAccounts, sizeof(LogonAccounts));

}

//发送获取任务列表请求
void GameServiceClient::sendGetTaskList()
{
	CMD_GL_GetTask task;
	task.dwOpTerminal = dwOptermianlValue;
	m_GameSocket.SendMsg(MDM_GL_C_DATA, SUB_GL_C_TASK_LOAD, &task, sizeof(task));
	
}
//发送领取任务奖励请求
void GameServiceClient::sendTaskAwardRequest(int taskId)
{
	CMD_GL_TaskID taskAward;
	taskAward.dwTaskID  = taskId;
	taskAward.dwUserID = SessionManager::shareInstance()->getUserId();
	/*MD5 m;
	m.ComputMd5(SessionManager::shareInstance()->getPwd().c_str(), SessionManager::shareInstance()->getPwd().length());
	std::string md5PassWord = m.GetMd5();*/
	std::string md5PassWord = Crypto::MD5String((void*)SessionManager::shareInstance()->getPwd().c_str(), strlen(SessionManager::shareInstance()->getPwd().c_str()));
	strcpy(taskAward.szPassword, md5PassWord.c_str());

	m_GameSocket.SendMsg(MDM_GL_C_DATA, SUB_GL_C_TASK_REWARD, &taskAward, sizeof(taskAward));
}

//刷新比赛人数请求
void GameServiceClient::sendRefrshMatchPlayers()
{
	tagMatchSerial MatchSerial;
	MatchSerial.dwKindID = SessionManager::shareInstance()->getGameMatchKind();
	m_GameSocket.SendMsg(MDM_GL_C_DATA, SUB_GL_C_MATCH_NUM, &MatchSerial, sizeof(MatchSerial));
}

//比赛报名
void GameServiceClient::sendSignUpRequest(MatchInfoModel* info)
{
	tagMatchID MatchID;
	if (info->getsignUpStatus() != SignUpStatus::SignUp)
	{
		log("len = %d",sizeof(tagMatchID));
		MatchID.dwClientVersion = VERSION_CLIENT;
		MatchID.MatchSerial.dwFullPlayerNum = info->getSerialModel()->getFullPlayerNum();
		MatchID.MatchSerial.dwKindID = info->getSerialModel()->getKindID();
		MatchID.MatchSerial.dwMatchInfoID = info->getSerialModel()->getMatchInfoID();
		MatchID.MatchSerial.dwMatchType = info->getSerialModel()->getMatchType();

		m_GameSocket.SendMsg(MDM_GL_C_DATA,SUB_GL_C_MATCH_SIGNUP,&MatchID,sizeof(MatchID));
	}
}

//取消报名
void GameServiceClient::sendUnSignUpRequest(MatchInfoModel* info)
{
	tagMatchID MatchID;
	if (info->getsignUpStatus() == SignUpStatus::SignUp)
	{
		MatchID.MatchSerial.dwFullPlayerNum = info->getSerialModel()->getFullPlayerNum();
		MatchID.MatchSerial.dwKindID = info->getSerialModel()->getKindID();
		MatchID.MatchSerial.dwMatchInfoID = info->getSerialModel()->getMatchInfoID();
		MatchID.MatchSerial.dwMatchType = info->getSerialModel()->getMatchType();
		m_GameSocket.SendMsg(MDM_GL_C_DATA,SUB_GL_C_MATCH_WITHDRAWAL,&MatchID,sizeof(MatchID));
	}
}

//请求游戏时间
void GameServiceClient::sendCountDownRequest(MatchInfoModel* info)
{
	tagMatchSerial matchSerial;

	matchSerial.dwFullPlayerNum = info->getSerialModel()->getFullPlayerNum();
	matchSerial.dwKindID = info->getSerialModel()->getKindID();
	matchSerial.dwMatchInfoID = info->getSerialModel()->getMatchInfoID();
	matchSerial.dwMatchType = info->getSerialModel()->getMatchType();
	m_GameSocket.SendMsg(MDM_GL_C_DATA,SUB_GL_C_MATCH_COUNTDOWN,&matchSerial,sizeof(matchSerial));	
}

void GameServiceClient::sendMatchUserComeRequest()
{
	m_GameSocket.SendMsg(MDM_GR_USER,SUB_GR_MATCHUSER_COME,NULL,0);	
}

void GameServiceClient::sendLeaveMatchRequest()
{
	m_GameSocket.SendMsg(MDM_GR_MATCH,SUB_GR_LEAVE_MATCH,NULL,0);	
}

void GameServiceClient::sendTryAaginRequest()
{
	m_GameSocket.SendMsg(MDM_GF_GAME,SUB_C_MATCH_START_NEXT,NULL,0);	
}

//获取银行信息
void GameServiceClient::sendQueryBankRequest()
{
	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(), serverPort);
	}

	CMD_GP_QueryInsureInfo insure;
	insure.dwUserID = SessionManager::shareInstance()->getUserId();
	
	//登录密码
	//MD5 m;
	//m.ComputMd5(SessionManager::shareInstance()->getPwd().c_str(), SessionManager::shareInstance()->getPwd().length());
	std::string md5PassWord = Crypto::MD5String((void*)SessionManager::shareInstance()->getPwd().c_str(), strlen(SessionManager::shareInstance()->getPwd().c_str()));
	strcpy(insure.szPassword, md5PassWord.c_str());
	
	
	m_GameSocket.SendMsg(MDM_GP_USER_SERVICE, SUB_GP_QUERY_INSURE_INFO, &insure, sizeof(insure));
}

//存入金币
void GameServiceClient::sendUserSaveScoreRequest(SCORE score)
{
	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}

	CMD_GP_UserSaveScore saveScore;
	saveScore.dwUserID =  SessionManager::shareInstance()->getUserId();
	saveScore.lSaveScore = score;

	std::string uuid = CommonFunction::getUUID();
	std::string Bit32UUid = Crypto::MD5String((void*)uuid.c_str(), strlen(uuid.c_str()));

	Bit32UUid = "mobile" + pystring::slice(Bit32UUid,0,26);
	strcpy(saveScore.szMachineID,Bit32UUid.c_str());

	m_GameSocket.SendMsg(MDM_GP_USER_SERVICE, SUB_GP_USER_SAVE_SCORE, &saveScore, sizeof(saveScore));
}

//提取金币
void GameServiceClient::sendUserTakeScoreRequest(SCORE score)
{
	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}

	CMD_GP_UserTakeScore takeScore;
	takeScore.dwUserID =  SessionManager::shareInstance()->getUserId();
	takeScore.lTakeScore = score;

	std::string uuid = CommonFunction::getUUID();
	std::string Bit32UUid = Crypto::MD5String((void*)uuid.c_str(), strlen(uuid.c_str()));

	Bit32UUid = "mobile" + pystring::slice(Bit32UUid,0,26);
	strcpy(takeScore.szMachineID,Bit32UUid.c_str());
	/*MD5 m;
	m.ComputMd5(SessionManager::shareInstance()->getInsurePass().c_str(), SessionManager::shareInstance()->getInsurePass().length());
	std::string md5PassWord = m.GetMd5();*/

	std::string md5PassWord = Crypto::MD5String((void*)SessionManager::shareInstance()->getInsurePass().c_str(), strlen(SessionManager::shareInstance()->getInsurePass().c_str()));
	strcpy(takeScore.szPassword, md5PassWord.c_str());
	m_GameSocket.SendMsg(MDM_GP_USER_SERVICE, SUB_GP_USER_TAKE_SCORE, &takeScore, sizeof(takeScore));
}

void GameServiceClient::sendModifyInsurePass(const char* pwd)
{
	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}

	CMD_GP_ModifyInsurePass modifyPass;
	modifyPass.dwUserID = SessionManager::shareInstance()->getUserId();
	/*MD5 m;
	m.ComputMd5(SessionManager::shareInstance()->getPwd().c_str(), SessionManager::shareInstance()->getPwd().length());
	std::string md5PassWord = m.GetMd5();*/
	std::string md5PassWord = Crypto::MD5String((void*)SessionManager::shareInstance()->getPwd().c_str(), strlen(SessionManager::shareInstance()->getPwd().c_str()));
	strcpy(modifyPass.szScrPassword, md5PassWord.c_str());
	std::string md5InsuerPassWord = Crypto::MD5String((void*)pwd, strlen(pwd));
	strcpy(modifyPass.szDesPassword, md5InsuerPassWord.c_str());

	m_GameSocket.SendMsg(MDM_GP_USER_SERVICE, SUB_GP_MODIFY_INSURE_PASS, &modifyPass, sizeof(modifyPass));
}

void GameServiceClient::sendModifyPassword(const char* oldPwd,const char *newPwd ,int type)
{

	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}

	if (type == 0)
	{
		CMD_GP_ModifyLogonPass modifyPass;
		modifyPass.dwUserID = SessionManager::shareInstance()->getUserId();
		//MD5 m;
		//m.ComputMd5(oldPwd, strlen(oldPwd));
		std::string md5PassWord = Crypto::MD5String((void*)oldPwd, strlen(oldPwd));
		strcpy(modifyPass.szScrPassword, md5PassWord.c_str());
		//m.ComputMd5(newPwd, strlen(newPwd));
		std::string md5InsuerPassWord = Crypto::MD5String((void*)newPwd, strlen(newPwd));
		strcpy(modifyPass.szDesPassword, md5InsuerPassWord.c_str());

		m_GameSocket.SendMsg(MDM_GP_USER_SERVICE, SUB_GP_MODIFY_LOGON_PASS, &modifyPass, sizeof(modifyPass));
	}else if (type == 1)
	{
		CMD_GP_ModifyInsurePass modifyPass;
		modifyPass.dwUserID = SessionManager::shareInstance()->getUserId();
		//MD5 m;
		//m.ComputMd5(oldPwd, strlen(oldPwd));
		std::string md5PassWord = Crypto::MD5String((void*)oldPwd, strlen(oldPwd));
		strcpy(modifyPass.szScrPassword, md5PassWord.c_str());
		//m.ComputMd5(newPwd, strlen(newPwd));
		std::string md5InsuerPassWord = Crypto::MD5String((void*)newPwd, strlen(newPwd));
		strcpy(modifyPass.szDesPassword, md5InsuerPassWord.c_str());

		m_GameSocket.SendMsg(MDM_GP_USER_SERVICE, SUB_GP_MODIFY_INSURE_PASS, &modifyPass, sizeof(modifyPass));
	}
}

void GameServiceClient::sendVerifyInsurePass(const char* pwd)
{
	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}
	CMD_GP_VerifyInsurePass verifyPass;
	verifyPass.dwUserID = SessionManager::shareInstance()->getUserId();
	
	//MD5 m;
	//m.ComputMd5(pwd, strlen(pwd));
	std::string md5PassWord = Crypto::MD5String((void*)pwd, strlen(pwd));
	strcpy(verifyPass.szInsurePass, md5PassWord.c_str());

	std::string uuid = CommonFunction::getUUID();
	std::string Bit32UUid = Crypto::MD5String((void*)uuid.c_str(), strlen(uuid.c_str()));

	Bit32UUid = "mobile" + pystring::slice(Bit32UUid,0,26);
	strcpy(verifyPass.szMachineID,Bit32UUid.c_str());
	m_GameSocket.SendMsg(MDM_GP_USER_SERVICE, SUB_GP_VERIFY_INSURE_PASS, &verifyPass, sizeof(verifyPass));

}

//发送获取签到列表
void GameServiceClient::sendGetAssignList()
{
	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}

	CMD_GP_GetSignInTask assign ;
	assign.dwUserID = SessionManager::shareInstance()->getUserId();

	assign.dwOpTerminal = dwOptermianlValue;
	m_GameSocket.SendMsg(MDM_GP_USER_SERVICE, SUB_GP_GET_SIGNIN_TASK, &assign, sizeof(assign));
}


//发送签到消息
void GameServiceClient::sendAssignMsg()
{
	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}
	CMD_GP_SignIn signIn;
	signIn.dwUserID = SessionManager::shareInstance()->getUserId();
	signIn.dwOpTerminal = dwOptermianlValue;
	LoginUserModel* loginModel = SessionManager::shareInstance()->getLoginModel();

	//MD5 m;
	//m.ComputMd5(loginModel->getPassword().c_str(),loginModel->getPassword().length());
	std::string md5PassWord =Crypto::MD5String((void*)loginModel->getPassword().c_str(), strlen(loginModel->getPassword().c_str()));
	strcpy(signIn.szLogonPass, md5PassWord.c_str());

	m_GameSocket.SendMsg(MDM_GP_USER_SERVICE, SUB_GP_SIGNIN, &signIn, sizeof(signIn));
}

//获取财富详细请求
void GameServiceClient::sendTreasureRequest()
{
	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}
	CMD_GP_UserID user;

	LoginUserModel* loginModel = SessionManager::shareInstance()->getLoginModel();
	user.dwUserID = SessionManager::shareInstance()->getUserId();

	//MD5 m;
	//m.ComputMd5(loginModel->getPassword().c_str(),loginModel->getPassword().length());
	std::string md5PassWord =Crypto::MD5String((void*)loginModel->getPassword().c_str(), strlen(loginModel->getPassword().c_str()));
	strcpy(user.szPassword, md5PassWord.c_str());

	m_GameSocket.SendMsg(MDM_GP_USER_SERVICE, SUB_GP_TREASURE, &user, sizeof(user));

}

//提取拍卖所得请求
void GameServiceClient::sendConvertAutionScoreRequest()
{
	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}
	CMD_GP_UserID user;

	LoginUserModel* loginModel = SessionManager::shareInstance()->getLoginModel();
	user.dwUserID = SessionManager::shareInstance()->getUserId();

	//MD5 m;
	//m.ComputMd5(loginModel->getPassword().c_str(),loginModel->getPassword().length());
	std::string md5PassWord = Crypto::MD5String((void*)loginModel->getPassword().c_str(), strlen(loginModel->getPassword().c_str()));
	strcpy(user.szPassword, md5PassWord.c_str());

	m_GameSocket.SendMsg(MDM_GP_USER_SERVICE, SUB_GP_CONVERSION_AUCTIONSCORE, &user, sizeof(user));
}

//发送获取验证码请求
void GameServiceClient::sendGetVerifyCode(const char* phone)
{
	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}

	CMD_GP_Get_Captcha getCaptcha;

	getCaptcha.dwUserID = SessionManager::shareInstance()->getUserId();
	LoginUserModel* user = SessionManager::shareInstance()->getLoginModel();

	//MD5 m;
	//m.ComputMd5(user->getPassword().c_str(),user->getPassword().length());
	std::string md5PassWord = Crypto::MD5String((void*)user->getPassword().c_str(), strlen(user->getPassword().c_str()));
	strcpy(getCaptcha.szLogonPass, md5PassWord.c_str());
	
	std::string uuid = CommonFunction::getUUID();
	std::string Bit32UUid = Crypto::MD5String((void*)uuid.c_str(), strlen(uuid.c_str()));

	Bit32UUid = "mobile" + pystring::slice(Bit32UUid,0,26);
	strcpy(getCaptcha.szMachineID,Bit32UUid.c_str());

	strcpy(getCaptcha.szPhone, phone);

	m_GameSocket.SendMsg(MDM_GP_USER_SERVICE, SUB_GP_GET_CAPTCHA, &getCaptcha, sizeof(getCaptcha));
}

//发送绑定手机请求
void GameServiceClient::sendBindPhoneRequest(int code )
{
	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}
	CMD_GP_Send_Captcha sendCaptcha;
	sendCaptcha.dwUserID = SessionManager::shareInstance()->getUserId();
	sendCaptcha.dwCaptcha =code;
	m_GameSocket.SendMsg(MDM_GP_USER_SERVICE, SUB_GP_SEND_CAPTCHA, &sendCaptcha, sizeof(sendCaptcha));
}
// 确认帐号
void GameServiceClient::sendEnsureAccoutRequest(const char* accout)
{
	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}

	
	
	CMD_GP_Accounts checkAccout;
	strcpy(checkAccout.szAccounts,accout);

	m_GameSocket.SendMsg(MDM_GP_USER_SERVICE, SUB_GP_CHECK_ACCOUNT, &checkAccout, sizeof(checkAccout));
	
}

void GameServiceClient::sendGetVerifyCodeByUid()
{
	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}

	CMD_GP_GetCaptchaByUserID getCaptcha;
	getCaptcha.dwUserID = SessionManager::shareInstance()->getUserId();

	m_GameSocket.SendMsg(MDM_GP_USER_SERVICE, SUB_GP_GET_CAPTCHA_BY_ID, &getCaptcha, sizeof(getCaptcha));

}

void GameServiceClient::sendSetPassRequest(int verifyCode ,const char* newPwd)
{
	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}

	CMD_GP_SetPass setPass;
	setPass.dwUserID = SessionManager::shareInstance()->getUserId();
	setPass.dwCaptcha = verifyCode;

	//MD5 m;
	//m.ComputMd5(newPwd,strlen(newPwd));
	std::string md5PassWord = Crypto::MD5String((void*)newPwd, strlen(newPwd));
	strcpy(setPass.szLogonPass, md5PassWord.c_str());

	m_GameSocket.SendMsg(MDM_GP_USER_SERVICE, SUB_GP_SET_LOGIN_PASS, &setPass, sizeof(setPass));
}

//获取银行验证码
void GameServiceClient::sendInsureVerifyRequest()
{
	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}

	CMD_GP_SetInsurePassGetCaptcha getInsureCaptcha;
	LoginUserModel* loginModel = SessionManager::shareInstance()->getLoginModel();
	getInsureCaptcha.dwUserID  =SessionManager::shareInstance()->getUserId();

	//MD5 m;
	//m.ComputMd5(loginModel->getPassword().c_str(),loginModel->getPassword().length());
	std::string md5PassWord =Crypto::MD5String((void*)loginModel->getPassword().c_str(), strlen(loginModel->getPassword().c_str()));
	strcpy(getInsureCaptcha.szLogonPass, md5PassWord.c_str());

	//strcpy(getInsureCaptcha.szLogonPass,loginModel->getPassword().c_str());

	std::string uuid = CommonFunction::getUUID();
	std::string Bit32UUid = Crypto::MD5String((void*)uuid.c_str(), strlen(uuid.c_str()));

	Bit32UUid = "mobile" + pystring::slice(Bit32UUid,0,26);
	strcpy(getInsureCaptcha.szMachineID,Bit32UUid.c_str());

	m_GameSocket.SendMsg(MDM_GP_USER_SERVICE, SUB_GP_SET_INSUREPASS_GET_CAPTCHA, &getInsureCaptcha, sizeof(getInsureCaptcha));
}

//发送银行验证码
void GameServiceClient::sendSetInsuerPass(int verifyCode,const char* newInsurePwd)
{
	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}

	CMD_GP_SetPass setPass;
	setPass.dwUserID = SessionManager::shareInstance()->getUserId();
	setPass.dwCaptcha = verifyCode;

	//MD5 m;
	//m.ComputMd5(newInsurePwd,strlen(newInsurePwd));
	std::string md5PassWord = Crypto::MD5String((void*)newInsurePwd, strlen(newInsurePwd));
	strcpy(setPass.szLogonPass, md5PassWord.c_str());

	m_GameSocket.SendMsg(MDM_GP_USER_SERVICE, SUB_GP_SET_INSUREPASS_SEND_CAPTCHA, &setPass, sizeof(setPass));
}

//元宝兑换积分
void GameServiceClient::sendExchangeIngotRequest(int ingot)
{
	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}

	CMD_GP_UserExchangeIngot exchangeIngot;
	exchangeIngot.dwUserID = SessionManager::shareInstance()->getUserId();
	exchangeIngot.dwIngot = ingot;

	LoginUserModel* loginModel = SessionManager::shareInstance()->getLoginModel();

	std::string uuid = CommonFunction::getUUID();
	std::string Bit32UUid = Crypto::MD5String((void*)uuid.c_str(), strlen(uuid.c_str()));

	Bit32UUid = "mobile" + pystring::slice(Bit32UUid,0,26);
	strcpy(exchangeIngot.szMachineID,Bit32UUid.c_str());
	//MD5 m;
	//m.ComputMd5(loginModel->getPassword().c_str(),loginModel->getPassword().length());
	std::string md5PassWord = Crypto::MD5String((void*)loginModel->getPassword().c_str(), strlen(loginModel->getPassword().c_str()));
	strcpy(exchangeIngot.szPassword, md5PassWord.c_str());
	m_GameSocket.SendMsg(MDM_GP_USER_SERVICE, SUB_GP_EXCHANGE_INGOT, &exchangeIngot, sizeof(exchangeIngot));
}

void GameServiceClient::sendModifyIndividual(bool isMan /* = false */)
{

	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}

	CMD_GP_ModifyIndividual modifyInfo;
	modifyInfo.cbGender = isMan;
	modifyInfo.dwUserID = SessionManager::shareInstance()->getUserId();

	LoginUserModel* loginModel = SessionManager::shareInstance()->getLoginModel();
	//MD5 m;
	//m.ComputMd5(loginModel->getPassword().c_str(),loginModel->getPassword().length());
	std::string md5PassWord = Crypto::MD5String((void*)loginModel->getPassword().c_str(), strlen(loginModel->getPassword().c_str()));
	strcpy(modifyInfo.szPassword, md5PassWord.c_str());

	m_GameSocket.SendMsg(MDM_GP_USER_SERVICE, SUB_GP_MODIFY_INDIVIDUAL, &modifyInfo, sizeof(modifyInfo));
}

void GameServiceClient::sendModifyFaceId(int id)
{
	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}

     CMD_GP_SystemFaceInfo faceInfo;
	 faceInfo.wFaceID= id;
	 faceInfo.dwUserID = SessionManager::shareInstance()->getUserId();

	 LoginUserModel* loginModel = SessionManager::shareInstance()->getLoginModel();
	 //MD5 m;
	 //m.ComputMd5(loginModel->getPassword().c_str(),loginModel->getPassword().length());
	 std::string md5PassWord = Crypto::MD5String((void*)loginModel->getPassword().c_str(), strlen(loginModel->getPassword().c_str()));
	 strcpy(faceInfo.szPassword, md5PassWord.c_str());

	 std::string uuid = CommonFunction::getUUID();
	 std::string Bit32UUid = Crypto::MD5String((void*)uuid.c_str(), strlen(uuid.c_str()));

	 Bit32UUid = "mobile" + pystring::slice(Bit32UUid,0,26);
	 strcpy(faceInfo.szMachineID,Bit32UUid.c_str());

	 m_GameSocket.SendMsg(MDM_GP_USER_SERVICE, SUB_GP_SYSTEM_FACE_INFO, &faceInfo, sizeof(faceInfo));
}

void GameServiceClient::sendMoreGameRequest()
{
	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}
	CMD_GP_GetMoreGame moreGame;
	moreGame.dwOpTerminal = dwOptermianlValue;
	moreGame.wPlatformType = 1;

	m_GameSocket.SendMsg(MDM_GP_USER_SERVICE, SUB_GP_MORE_GAME, &moreGame, sizeof(moreGame));
}

void GameServiceClient::sendFeedBackRequest(const char* content,const char* pQQ)
{
	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}
	CMD_GP_Feedback info;
	LoginUserModel* loginModel = SessionManager::shareInstance()->getLoginModel();
	strcpy(info.szAccounts,CommonFunction::UTF8TOGBK(loginModel->getAccount().c_str()).c_str());

	strcpy(info.szContent,content);

	strcpy(info.szQQ,pQQ);

	m_GameSocket.SendMsg(MDM_GP_USER_SERVICE, SUB_GP_FEEDBACK, &info, sizeof(info));
}

void GameServiceClient::sendDownLoadUrlRequest(int gameKind)
{
	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}
	CMD_GP_GetDownloadResUrl downloadRes;
	downloadRes.dwKindID = gameKind;

	m_GameSocket.SendMsg(MDM_GP_USER_SERVICE, SUB_GP_DOWNLOAD_RESURL, &downloadRes, sizeof(downloadRes));
}


void GameServiceClient::sendRechargeOrder(int ingot,const char* orderInfo, bool isFirstRecharge)
{
	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(),serverPort);
	}
	LoginUserModel* loginModel = SessionManager::shareInstance()->getLoginModel();

	CMD_GP_RechargeOrder recharge;
	recharge.dwOpTerminal = dwOptermianlValue;
	if(loginModel->getAccountType() == accountMachineID)
		strcpy(recharge.szAccounts,loginModel->getQucikAccount().c_str());
	else
		strcpy(recharge.szAccounts,loginModel->getAccount().c_str());

	std::string md5PassWord = Crypto::MD5String((void*)loginModel->getPassword().c_str(), strlen(loginModel->getPassword().c_str()));
	strcpy(recharge.szLogonPass, md5PassWord.c_str());

	recharge.dwFirst = isFirstRecharge;
	recharge.dwOrderAmount = ingot;
	recharge.dwShareID = ChargeByZfb ;

	strcpy(recharge.szRechargeOrder, orderInfo);

	m_GameSocket.SendMsg(MDM_GP_USER_SERVICE, SUB_GP_RECHARGE_ORDER, &recharge, sizeof(recharge));
}

void GameServiceClient::sendGiftConvertRequest(const char* covertCode)
{
	if (!m_GameSocket.IsConnected())
	{
		Connect(SessionManager::shareInstance()->getLoginAddr().c_str(), serverPort);
	}
	LoginUserModel* loginModel = SessionManager::shareInstance()->getLoginModel();

	CMD_GP_GiftChange gift;
	gift.dwOpTerminal = dwOptermianlValue;
	if (loginModel->getAccountType() == accountMachineID)
		strcpy(gift.szAccounts, loginModel->getQucikAccount().c_str());
	else
		strcpy(gift.szAccounts, loginModel->getAccount().c_str());

	strcpy(gift.szGiftChangeOrder, covertCode);
	m_GameSocket.SendMsg(MDM_GP_USER_SERVICE, SUB_GP_GIFT_CHANGE, &gift, sizeof(gift));
}

void GameServiceClient::closeSoket()
{
	if (m_GameSocket.IsConnected())
	{
		m_GameSocket.Destroy();
		OnEventTCPSocketShut(0);
	}
	else
	{
		m_GameSocket.Destroy();
	}
}