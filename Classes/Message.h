/**
 * @file Message
 * @brief 全局消息定义
 * @author TJL
 * @date
 */

#ifndef __Message_H_
#define __Message_H_

//游戏相关消息定义
#define  ExchangeFishScoreMsg  "ExchangeFishScoreMsg" //玩家上分消息
#define  UserFireMsg           "UserFireMsg"//玩家发射炮弹的消息
#define  CreateNewFishMsg	   "CreateNewFishMsg"//产生新的鱼类消息
#define  CatchFishGroupMsg	   "CatchFishGroupMsg"//玩家捕捉到鱼的消息
#define  BulletDoubleTimeoutMsg   "BulletDoubleTimeoutMsg"//发射两次超时消息
#define  ChangeSceneMsg			"ChangeSceneMsg"//切换场景的消息
#define  CatchChainMsg          "CatchChainMsg"
#define  CreateSceneFishMsg		"CreateSceneFishMsg"//创建场景鱼的消息
#define  SceneBulletsMsg		"SceneBulletsMsg"
#define  ForceTimerSyncMsg		"ForceTimerSyncMsg"//强制同步消息
#define  TimerSyncMsg			"TimerSyncMsg"
#define  SubSceneFishMsg		"SubSceneFishMsg"
#define  StockOperateResultMsg	"StockOperateResultMsg"
#define  AdminControlMsg		"AdminControlMsg"

//for lk
#define  ModifyLkMulriple        "ModifyLkMulriple"

//主界面主要处理的消息定义
#define  LoginSuccessMsg		"LoginSuccessMsg"//登录成功返回消息
#define  QucikLoginRspMsg    "QucikLoginRspMsg"
#define  LoginFaildMsg          "LoginFaildMsg"
#define  GetRoomListMsg			"GetRoomListMsg"
#define  GetUserStatusMsg	    "GetUserStatusMsg"
#define  PlayerToSeperaterMsg   "PlayerToSeperaterMsg" //玩家变旁观者消息
#define RemoveGameSoketMsg   "RemoveGameSoketMsg"

//frame
#define GameStatusMsg			 "GameStatusMsg"
#define GameSceneMsg			 "GameSceneMsg"

//系统消息
#define SystemMessage             "SystemMessage"    

//游戏名, zone代表一个游戏
#define XwZoneName      "XwCompilations"
#define XwZoneDntg		"XwFishingTalent"

//chain 相关
#define  FishArriveMsg  "FishArriveMsg"
#define  DealBingoEffectMsg "DealBingoEffectMsg"
#define  DealJettonEffectMsg "DealJettonEffectMsg"

//炼丹炉相关
#define  dealBgLuEffectMsg "dealBgLuEffectMsg"


//商品相关
#define  GetShopListFromServerMessage "GetShopListFromServerMessage"//刷新商品列表消息
#define  buyGifRspMsg        "buyGifRspMsg"

//拍卖相关
#define  getAuctionList   "getAuctionList"					//获取上架物品列表
#define  getAuctoinRecord "getAuctoinRecord"				//获取拍卖行记录
#define  getMyAuctionRecord "getMyAuctionRecord"			//获取我的拍卖
#define  getAuctionHistoryRecord "getAuctionHistoryRecord"	//获取拍卖历史记录
#define  sellAuctionResultMsg  "sellAuctionResultMsg"       //上架物品返回消息
#define  buyAuctionResultMsg   "buyAuctionResultMsg"        //拍卖物品返回消息
#define  cancelAuctionResultMsg "cancelAuctionResultMsg"    //撤销拍卖返回消息
#define  treasureInfoMsg        "treasureInfoMsg"           //财富
#define  converAuctionScoreMsg  "converAuctionScoreMsg"     //提取拍卖行所得

//背包相关
#define  getBagListMsg      "getBagListMsg"
#define  receiveKnapsackLogMsg "receiveKnapsackLogMsg"
#define  useBagMsg           "useBagMsg"
#define  userKnapsackMsg     "userKnapsackMsg"

#define toUserKnascapMsg     "toUserKnascapMsg"//通知背包界面
#define toUserGiftMsg     "toUserGiftMsg"//通知商城界面
#define toBuyAuctionMsg   "toBuyAuctionMsg" //通知拍卖界面
//vip power
#define  getVipPowerInfo     "getVipPowerInfo"
#define  vipAwardResultMsg   "vipAwardResultMsg"
//task 相关
#define  getTaskListMsg      "getTaskListMsg"
#define  getTaskAwardMsg     "getTaskAwardMsg"
//银行相关
#define  queryInsureInfoMsg  "QueryInsureInfoMsg"
#define  insureSuccessMsg        "insureSuccessMsg"
#define  insureFailureMsg        "insureFailureMsg"
//登录相关
#define  RequestFastLoginNotification "RequestLoginNotification" //请求快速登录
#define  RequestRegisterNotification "RequestRegisterNotification" //请求注册

//操作成功，失败返回定义
#define  operateSuccessMsg "operateSuccessMsg"
#define  operateFailureMsg "operateFailureMsg"

//可以登录任务服务器的消息
#define loginTaskServerMsg "loginTaskServerMsg"
//消息界面消息定义
#define refreshMsgListMsg "refreshMsgListMsg"
//签到相关
#define  getAssignListMsg  "getAssignListMsg"
#define  userAssignMsg     "userAssignMsg"

//背包相关
#define  switchBagItemMsg "switchBagItemMsg" //切换背包选项通知

//绑定手机相关
#define  bindPhoneRspMsg   "bindPhoneRspMsg"

//找回密码相关
#define  checkAccountRspMsg  "checkAccountRspMsg"
#define  setPassRspMsg       "setPassRspMsg"
#define  setInsurePassRspMsg  "setInsurePassRspMsg"

//充值相关
#define  exchangeIngotRspMsg  "exchangeIngotRspMsg"
#define  IngotCovertScoreMsg  "IngotCovertScoreMsg"
#define  RechargeOrderRspMsg  "RechargeOrderRspMsg"
#define  toGetRechargeOrderMsg  "toGetRechargeOrderMsg"
#define  RechargeSuccessMsg  " RechargeSuccessMsg"
#define  hasFirstRechargeMsg    "hasFirstRechargeMsg"

//登录游戏房间相关
#define  loginRealRoomSuccess "loginRealRoomSuccess"
#define  loginRealRoomFailure "loginRealRoomFailure"

//刷新用户数据
#define  updateUserInfoMsg "updateUserInfoMsg"
#define  updateHeadMsg      "updateHeadMsg"//刷新头像
#define  updateUserInfoFromAuciontMsg "updateUserInfoFromAuciontMsg"
#define  updateVipLevelMsg "updateVipLevelMsg"
//QQ登录
#define  qqLoginMsg  "qqLoginMsg"
#define  accountLoginMsg "accountLoginMsg"
#define  accountRegisterMsg "accountRegisterMsg"
#define  qucikLoginByAccount "qucikLoginByAccount"//已经保存过快速的帐号和密码后直接登录
#define  loginAtOtherPlace "loginAtOtherPlace"//异地登录

//连接成功相关
#define  connectHallSuccessMsg        "connectHallSuccessMsg" //连接登录服务器返回
#define  connectGameServerSuccessMsg  "connectGameServerSuccessMsg"//连接游戏服务器返回
#define  connectGameServerFaildMsg    "connectGameServerFaildMsg"//边接游戏服务器失败返回
#define  connectTaskServerSuccessMsg  "connectTaskServerSuccessMsg"//连接任务服务器返回

//反馈与更多游戏
#define  feedBackMsg "feedBackMsg"
#define  moreGameMsg "moreGameMsg"

//VIP特权
#define  VipPowerToShopMsg "VipPowerToShopMsg"
#define  VipPowerToChargeMsg "VipPowerToChargeMsg"
#define  ToSendGetAwardMsg "ToSendGetAwardMsg"
#define  DealVipTipsMsg           "DealVipTipsMsg"//播放VIP tips效果
//退出游戏
#define ExitGameMsg "ExitGameMsg"
#define GameSorketCloseMsg "GameSorketCloseMsg"
#define TaskSorketCloseMsg "TaskSorketCloseMsg"

//任务相关
#define  loginTaskServerSuccessMsg "loginTaskServerSuccessMsg"

//下载相关
#define  downLoadReponseMsg "downLoadReponseMsg"//下载返回
//礼包兑换
#define  giftConvertRspMsg  "giftConvertRspMsg"
//redpoint
#define  unReadMessageMsg "unReadMessageMsg"
#define  unGetTaskAwardMsg "unGetTaskAwardMsg"
#define  updateTaskRedPointMsg "updateTaskRedPointMsg"

#define  updateRedPointMsg     "updateRedPointMsg"
//获取验证码返回
#define  getCaptchaRspMsg "getCaptchaRspMsg"
//升级相关
#define  UpdateMsg "UpdateMsg"
#define  CancelUpdateMsg "CancelUpdateMsg"

#define  KeyEscapeMsg "KeyEscapeMsg"

#define  netWorkValidMsg "netWorkValidMsg" //网络连接不可用消息

//比赛相关
#define  refreshMatchListMsg "refreshMatchListMsg"//增加和刷新比赛列表消息
#define  deleteMatchRoomMsg "deleteMatchRoomMsg" //删除过期比赛房间消息
#define  updateRoomSignUpPlayersMsg  "updateRoomSignUpPlayersMsg" //刷新房间人数消息
#define  signUpSuccessMsg "signUpSuccessMsg"
#define  signUpFaildMsg        "signUpFaildMsg"
#define  unSignUpSuccessMsg "unSignUpSuccessMsg"
#define  unSignUpFaildMsg        "unSignUpFaildMsg"
#define  matchCountDownMsg  "matchCountDownMsg" //刷新房间倒计时
#define  matchStartMsg               "matchStartMsg" //比赛开始消息
#define  toSignUpNotice              "toSignUpNotice" //
#define  tounSignUpNotice              "tounSignUpNotice" //

#define  updateMatchStatusMsg       "updateMatchStatusMsg" //刷新房间比赛状态消息
#define  updateMatchInfoMsg	         "updateMatchInfoMsg"    //刷新房间比赛信息消息
#define  matchAwardMsg                   "matchAwardMsg"             //比赛奖励消息
#define  MatchTryAgainMsg              "MatchTryAgainMsg"        //再次挑战消息
#define  showMatchTryMsg               "showMatchTryMsg"
#define  updateMatchRankMsg          "updateMatchRankMsg"
#define  MatchIsBeingStartMsg         "MatchIsBeingStartMsg"
#define  EnterMatchRoomMsg			 "EnterMatchRoomMsg"

#define  MatchPlayCountIsMaxMsg     "MatchPlayCountIsMaxMsg"

#define  closeLoginDialogMsg "closeLoginDialogMsg"
#endif    