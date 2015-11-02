//发射器类
#ifndef _GAME_SHOOTER_H_H_
#define _GAME_SHOOTER_H_H_

#include "cocos2d.h"
#include "GameGlobalSetting.h"
#include "GameAnimation.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;

class GameShooter : public cocos2d::Layer 
{
public:
	enum SHootGrade
	{
		Grade_100= 0,
		Grade_1000,
		Grade_10000,
		Grade_Max
	};
	GameShooter();
	virtual ~GameShooter();
	static GameShooter* create(WORD myView_chair_id,BulletKind bKind,float angle,DWORD bulletMulriple,bool isMe =false);
	bool init(WORD myView_chair_id,BulletKind bKind,float angle,DWORD bulletMulriple,bool isMe);
	void setAngle(float angle);
	void fire();
	float getAngle() {return mAnger;}
	void dealFire(cocos2d::Node* pNode);//播放开火动画
	void switchGun(BulletKind kind);//换炮借口
	BulletKind getKind() const {return mBulletKind;}
	void setScore(SCORE score);
	SCORE getScore() const {return mScore;}
	void addScore(SCORE score,bool coin_effect = false ,bool bingo_effect = false);
	void dealBingoEffect(SCORE score);
	void setBulletMulriple(int power);
	int  getBulletMulriple() const {return mBulletMulriple;}
	void setShootGrade(int roomLevel);//设置炮的等级
	void setMulripleArea(int min,int max);//设置炮的倍数的区域
protected:
	void removeBingo(Node* pSender);
	void FireDealOver(Node* pSender);
	std::string getResDir();
	void onClickPlus(Ref *pSender, ui::Widget::TouchEventType eventType);
	void onClickSub(Ref *pSender, ui::Widget::TouchEventType eventType);
private:
	WORD mViewChairId;
	float mAnger;//当前角度值
	BulletKind mBulletKind;//
	Sprite* mGunSp;//炮的精灵
	Sprite* mFireSp;//火的精灵
	GameAnimation* mFireAnima;
	LabelAtlas* mScoreLable; //分数
	LabelAtlas* mMulripleLable; //炮的倍数
	SCORE  mScore;
	Sprite*  mScoreBg;
	int    mBulletMulriple;
	SHootGrade    mShootGrade;//炮的等级 0 表示百炮房，1表示 千炮房 ，2 表示万炮房
	Button*   mBtnPlus;
	Button*  mBtnSub;
	int          mMinMulriple;
	int          mMaxMulriple;

	bool        mIsMatchRoom;
};




#endif //_GAME_SHOOTER_H_H_
