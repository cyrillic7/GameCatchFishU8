#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
#include"GameFishAction.h"
#include"GameFishManager.h"
#include"GameResourcesManager.h"
#include "GameServiceClientManager.h"
#include "GameBaseScene.h"
#include "GameBulletManager.h"
#include "Common/MenuLayer.h"


class MainScene : public GameBaseScene
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    //static cocos2d::Scene* createScene();
	MainScene();
	~MainScene();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init(bool isLogin);
    
	virtual void loadUI();

	virtual void onEnterTransitionDidFinish();
	void showLoading();
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

	virtual void render(Renderer* renderer);
    // implement the "static create()" method manually
    static MainScene* create(bool isLogin = true);

	virtual void onEnter();
	virtual void onExit();
	void GetRoomListRsp(EventCustom* event);
	//void onloginTaskServer(EventCustom* evt);
	void refreshUserData(EventCustom* evt);
	void updateHeadInfo(EventCustom* evt);
	void refreshUserDataForAuction(EventCustom* evt);
	void addRedPointForMsg(EventCustom* evt);
	void addRedPointForTask(EventCustom* evt);
	void updateTaskRedPoint(EventCustom* evt);
	void hideFirstChargeMsg(EventCustom* evt);
	void updateVipLevel(EventCustom* evt);
	void receiveUpdateHornMsg(EventCustom* evt);
	void receiveSendhornRspMsg(EventCustom* evt);
	void receiveShowHornMsg(EventCustom* evt);
	void receiveShowLuckSpinMsg(EventCustom* evt);
	void receiveShowVipPowerMsg(EventCustom*evt);
	//刷新红点
	void updateRedPoint(EventCustom* evt);

	char* getWidgetJsonName();
	virtual __Array * getResourceNeedLoad();

	void onClickFishRoom1(Ref *pSender, ui::Widget::TouchEventType eventType);
	void onClickFishRoom2(Ref *pSender, ui::Widget::TouchEventType eventType);
	void onClickFishRoom3(Ref *pSender, ui::Widget::TouchEventType eventType);

	void onClickMsg(Ref *pSender, ui::Widget::TouchEventType eventType);
	void gotoCharge(Ref *pSender, ui::Widget::TouchEventType eventType);
	void onGotoVip(Ref *pSender, ui::Widget::TouchEventType eventType);

	void onClickSetting(Ref *pSender, ui::Widget::TouchEventType eventType);
	void onClickMoreGame(Ref *pSender, ui::Widget::TouchEventType eventType);
	void onClickHead(Ref *pSender, ui::Widget::TouchEventType eventType);
	void onclickBag(Ref *pSender, ui::Widget::TouchEventType eventType);
	void onClickFirstCharge(Ref *pSender, ui::Widget::TouchEventType eventType);
	void initMenuBar();
	void createFish();
	void menuCallback(Ref*pSender, ui::Widget::TouchEventType eventType);
	//比赛按钮
	void onClickMatch(Ref*pSender, ui::Widget::TouchEventType eventType);
	//点击喇叭
	void onClickHorn(Ref*pSender, ui::Widget::TouchEventType eventType);
	void fishIntoWater(Node* pSender);
	float fgRangeRand( float min, float max );

	void playScaleAnimation(bool less, Ref* pSender);
	void playScaleAnimation2(bool less, Ref* pSender);

	void checkAssign();
protected:
	void dealVipEffect();
	void vipDealOver(Node* pSender);
	void reDealVipEffect(Node* pSender);
	//添加跑马灯
	void showClipLabel(Node* parent);
	void resetClipLabelData();
	void onMoveFinsh(Node* pSender);
public:
	void update(float dt);
	Widget* mMainWidget;
	float mCurInterval ;
	bool  mIsLogin;
private:
	Text* mScore;
	ImageView* mFishVip;
	Sprite*  mVipEffectSp;

	CCMenuLayer* menuLayer;
	Button* mBtnMsg;
	ImageView* face;

	ui::TextAtlas* label_vip;
	ImageView*mMsgRedPoint;
	ImageView* mHornBg;//喇叭背景
	Text*      mHornNum;//喇叭个数
	Text*      mLastHornText;//最新跑马灯内容
	std::string mCurHornStr;
	DrawNode* mFront;
};

#endif // __MAIN_SCENE_H__
