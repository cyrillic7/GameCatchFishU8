#ifndef _GAME_SCENE_H_H_
#define _GAME_SCENE_H_H_

#include "cocos2d.h"
#include "GameServiceClient.h"
#include "GameBaseScene.h"
#include "Message.h"
#include "GameFishManager.h"
#include "GameShooter.h"
#include "GamePlayerManager.h"
#include "GameScenefishManager.h"
#include "LoadingScene.h"
#include "Model/UserModel.h"
#include "Widgets/GameGuideWidget.h"
#include "Model/MatchModel.h"
#include "GameMatchManager.h"

#include "cocos-ext.h"
#include "ui/CocosGUI.h"
using namespace ui;

struct MessageInfo {
	const char* msg[1024];
};

class GameScene :public GameBaseScene
{
public:
	GameScene();
	~GameScene();
	static GameScene* create(int level,int gameKind,bool isMatch = false);
	bool init(int level,int gameKind,bool isMatch);
	virtual void onEnter();
	virtual void onExit();
	virtual void render(Renderer* renderer);
	virtual void loadUI();

	virtual void okCallback();
	virtual bool onTouchBegan(Touch* pTouch, Event* pEvent); 
	virtual void onTouchMoved(Touch* pTouch, Event* pEvent); 
	virtual void onTouchEnded(Touch* pTouch, Event* pEvent);
	//初始化玩家界面元素
	void initPlayer(GamePlayer* player);
	//移除玩家界面元素
	void removePlayer(GamePlayer* player);
public:
	virtual __Array * getResourceNeedLoad();
	virtual void updateProgress(int resourceLeftToLoad);
	void addChain(GameAnimation* chain,WORD view_chair_id);
	void onChainpro(WORD view_chair_id,const CatchFish* fishArr,int cnt);
	void hideChainPro(WORD view_chair_id);
	void addScore(WORD view_chair_id,SCORE score,bool coin_effect,bool bingo_effect);
	Vec2 GetTurretPosition(WORD view_chair_id);
	void addCoin(Node* coinNode);
	GamePlayerManager* getPlayerManager() {return mPlayerManager;}
	GameMatchModel*  getGameMatchModel() {return mGameMatchInfo;}
private:
	void update(float dt);
	void receiveSubDistributeFish(EventCustom* event);//收到分发鱼的消息处理
	void receiveGameSceneMsg(EventCustom* event);//收到切换场景消息处理
	void receiveGameStausMsg(EventCustom* event);//收到游戏状态消息处理
	void receiveUserEnterMsg(EventCustom* event);//收到用户坐下消息处理
	void receivePlayerToSeperaterMsg(EventCustom* event);//收到站立消息处理
	void receiveUserFireMsgMsg(EventCustom* event);//收到用户开火消息处理
	void receiveCatchFishMsg(EventCustom* event);//收到捕捉鱼的消息处理
	void receiveRemoveLoadingMsg(EventCustom* event);//收到移除加载界面消息处理
	void receiveExchangeScoreMsg(EventCustom* event);//玩家金币变动返回消息处理
	void receiveTimerSyncMsg(EventCustom* event);//时间同步返回消息处理
	void receiveForceTimerSyncMsg(EventCustom* event);//强制时间同步消息返回处理
	void receiveSubSceneFishMsg(EventCustom* event);//断线重连，场景恢复消息返回处理
	void receiveBulletDoubleTimeoutMsg(EventCustom* event);//双倍时间到了消息处理
	void receiveChangeSceneMsg(EventCustom* event);////收到切换场景的消息处理
	void receiveModifyLkMulriple(EventCustom* event);//收到李逵改变倍数的消息处理

	//chain 相关
	void receiveCatchChainMsg(EventCustom* event); //接收捉到闪电鱼的消息处理
	void receiveFishArriveMsg(EventCustom* event); //接收到鱼被闪电牵引到位的消息
	void receiveDealBingoEffectMsg(EventCustom* event); //接收开始显示BING 效果消息
	void receiveDealBgluEffectMsg(EventCustom* event);//接收开始显示炼丹炉效果消息
	void receiveDealJettonEffectMsg(EventCustom* event);//接收开始播放筹码效果消息
	void receiveSystemMsg(EventCustom* event);//接受到系统消息

	void receiveGameSorketCloseMsg(EventCustom* event);//网络断开
	void receiveKeyEscapeMsg(EventCustom* evt);

	//比赛相关
	void receiveUpdateMatchStatusMsg(EventCustom* evt);
	void receiveUpdateMatchInfoMsg(EventCustom* evt);
	//收到比赛奖励消息处理
	void receiveMatchAwardMsg(EventCustom* evt);
	//再次挑战
	void toSendTryAgainMatch(EventCustom* evt);
	void showMatchTryAgainMsg(EventCustom* evt);
	void receiveMatchIsBeingStartMsg(EventCustom* evt);


	void loadBg();
	WORD SwitchViewChairID(WORD chair_id);//转换成我的视解下的CHAIR ID
	void createShooter();
	void onBulletFire();//发射子弹
	bool IsSameSide(WORD chair_id1, WORD chair_id2);
	void showLoading();
	void startChain(WORD view_chair_id);
	void removeBingo(Node* pSender);
	void shakeScreen();
	void reset(Node* pSender);//背景层复位
	void changeScenePro(float dt);
	void changeSceneOver(Node* pSender);//场景切换完成
	void createWave(Node* parent,bool isNextScene =false);
	void fishPauseOver(Node* pSender);
	void bgLuDealOver(Node* pSender);
	void preLoadSoundRes();
	void DealBaoJiEffect(WORD chair_id);//播放爆击效果
	void dealbombEffect(Vec2 pos);

	void ExecuteEffect(GameFish* fish);
	void dealPauseStartEffect();//播放开始暂停效果
	void dealPauseEndEffect();

	void fengyinStay(Node* pSend);
	void onBack(Ref* pSender,ui::Widget::TouchEventType type);
	void onClickGuide(Ref* pSender,ui::Widget::TouchEventType type);
	//空闲提示
	void RenderKickoutTips();
	//弹出系统消息
	void popCommonTip(__String* content);
	void showNextTip(Node* pSender);
	//发送站立请求超时处理
	void standupOverTime(Node* node);
	//
	std::string getMusicBackgroudName(int index);

	void showOutTip(FishKind kid);
	void hideOutTip(Node* pSender);
	void exitGameRoom();

	BulletKind calBulletKind();

protected:
	int                mRoomLevel;
	int                mGameKind;
	GamePlayerManager* mPlayerManager;//玩家管理类
	GamePlayer*        me;
	Sprite*            mCursorSp;
	Sprite*            mBgSp;
	ImageView*		   mIceImage;
	int mGameStaus;
	bool mIsLookOn;
	ClientGameConfig mGameConfig;
	Layer* myLayer;//fish 层
	Layer* mBgLayer;//背景层，包括波纹
	DWORD mLastFireTime;
	DWORD mSpcialSceneBeganTime;//特殊场景开始时间
	float   mAngle;
	bool    mBlongPressed;
	int    mLoadResourseCount;
	LoadingScene* mLoadingWidget;
	typedef std::deque<GameBullet*> BulletQueue;
	BulletQueue mBulletQueue;
	bool  mCanShooter;//是否能射击标记
	SceneKind next_scene_kind;//下一个场景ID 
	SceneFishManager* mSceneFishManager;
	bool is_special_scene_;//是特殊鱼阵
	float special_scene_waited_time_;//特殊鱼阵等特时间
	int mCurBgIndex;
	__Array * mWaves;
	int  mWaveIndex;
	__Array* mNextSceneWaves;
	float mWaveInterval;
	int   mDealCnt;//效果播放完记数
	float bubble_elapsed_;
	LabelAtlas* mKickOutLabel;
	__Array*    mMsgArray;

	bool bSend;//是否已发送请求

	ImageView* mTipOutFish;//特殊种类出现的提示图片
	__Array* mParticlesArray;
	SpriteBatchNode* mBulletBatchNode;
	SpriteBatchNode* mCoinBatchNode;

	//向导层
	GameGuideWidget*  mGuideWidget;
	bool  mScoreIsChange ; 
	bool  mIsMatchFlag;

	//比赛相关
	GameMatchModel* mGameMatchInfo;
	GameMatchManager* mMatchManager;
	bool mbEnterMatch;
	bool bShowAward;
};




#endif //_GAME_SCENE_H_H_