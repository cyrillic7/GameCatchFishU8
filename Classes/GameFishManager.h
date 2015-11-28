#ifndef _GAME_FISH_MANAGER_H_H_
#define _GAME_FISH_MANAGER_H_H_

#include "cocos2d.h"
#include <vector>
#include"GameGlobalSetting.h"
#include"GameFishAction.h"
#include "GameAnimation.h"
#include "GameBulletManager.h"

#define  YUQUNQUANBGTAG 199
USING_NS_CC;

class  GameScene;

class GameFish :public Node{
 public:
  static GameFish* create(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action);
  GameFish();
  virtual ~GameFish();
  virtual bool init(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action);
  virtual void Update(float delta_time);
  virtual void Render();

  FishKind fish_kind() const { return mFishKind; }
  int fish_id() const { return mFishId; }
  int tag() const { return mTag; }
  Vec2 position() const { return mFishAction->getPosition(); }
  float angle() const { return mFishAction->getAngle(); }
  ActionFishMove* action_fish() const { return mFishAction; }
  GameAnimation*  animation_walk() const {return mFishwalking;}
  float fish_width() const { return mFishwalking->getFrameWidth(); }
  float fish_height() const { return mFishwalking->getFrameHeight(); }
  void set_action_fish(ActionFishMove* action);
  void set_action_walk(GameAnimation* walk);
  virtual Vec2  getEndPos();
  virtual float Scale() const {return mScale;}
  virtual void chainFish();//闪电提前完成
  virtual void SetFishMulriple(int mul){fish_mulriple_ = mul;}//设置李逵的倍数
  enum Status {
    MOVE = 0,
    CHAIN_LOCK,  // 闪电锁定
    DEAD,
    DESTROY
  };
  Status status() const { return mStatus; }
  void ChainLock() { mStatus = CHAIN_LOCK; }
  void set_pause(bool pause) { mBpause = pause; }

 virtual bool BulletHitTest(GameBullet* bullet);//子弹碰撞检测
 virtual void catchFish(WORD view_chair_id, SCORE score);//鱼被捉到后处理
 virtual void onChainPro(GameScene* scene,Vec2 chain_start_pos,Vec2 fish_end_pos,WORD view_chair_id,int fishIndex =0);//闪电处理
 protected:
	 void RenderScoreNum();
	 void removeScore(Node* pSender);
	 void onReadyToMov(Node* pSender);//开始移动
	 void onStartScale(Node* pSender);//开始缩小
	 void changeState(Node* pSender);
	 void createJcSprite();
 protected:
  FishKind mFishKind;
  int mFishId;
  int mTag;
  float mBoundingRadius;
  int mBoundingCount;
  ActionFishMove* mFishAction;//鱼的行走路线动作对象
  GameAnimation* mFishwalking;//鱼的游动动画对象
  GameAnimation* mFishDead;//鱼的死亡动画
  Status mStatus;
  bool mBpause;
  LabelAtlas*       mScoreLabel;
  Vec2   mFishEndPos;
  float  mScale;
  Vec2   mChainStartPos;
  GameAnimation* mChainAnimation;
  GameScene* mScene;
  GameAnimation* mGreenLight;
  ActionScaleTo* mScaleAction;
  Sprite* mCircleSp;
  WORD   mViewChairId;
  bool   mIsArrive;

  LabelAtlas* mScoreLable;
  ActionScore* mActionScore;
  int  fish_mulriple_;//李逵的倍数

  //
  Sprite* mKindJc20Sprite;
};

//神仙船
class GameFishShip : public GameFish
{
public:
	GameFishShip();
	virtual ~GameFishShip();
	static GameFishShip* create(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action);
	virtual bool init(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action);
	virtual void Update(float delta_time);
	virtual void Render();
	void catchFish(WORD view_chair_id, SCORE score);
	void removeScore(Node* pSender);
private:
	__Dictionary* mAnimationDic;
	__Dictionary* mDeadAnimationDic;
};

//鱼群
class GameFishYuqun : public GameFish
{
public:
	GameFishYuqun();
	~GameFishYuqun();
	static GameFishYuqun* create(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action);
	virtual bool init(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action);
	virtual void Update(float delta_time);
	virtual void Render();
	virtual bool BulletHitTest(GameBullet* bullet);
	void removeScore(Node* pSender);
private:
	__Array* mAnimationArray;
	__Array* mRotateArray;
	__Array* mDeadAnimationArray;
	float	mRadius;//直径
};

//大闹天宫
class GameFishDntg : public GameFish
{
public:
	GameFishDntg();
	~GameFishDntg();
	static GameFishDntg* create(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action);
	virtual bool init(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action);
	virtual void Update(float delta_time);
	virtual void Render();
protected:
	void removeScore(Node* pSender);
private:
	GameAnimation* mInnerFishWalking;
	GameAnimation* mInnerFishDead;
};

//一箭双雕
class GameFishYjsd : public GameFish
{
public:
	GameFishYjsd();
	~GameFishYjsd();
	static GameFishYjsd* create(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action);
	virtual bool init(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action);
	virtual void Update(float delta_time);
	virtual void Render();
	void removeScore(Node* pSender);
private:
	__Array* mAnimationArray;
	__Array* mDeadArray;
	float mRadius;
};

//一石三鸟
class GameFishYssn : public GameFish
{
public:
	GameFishYssn();
	~GameFishYssn();
	static GameFishYssn* create(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action);
	virtual bool init(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action);
	virtual void Update(float delta_time);
	virtual void Render();
	void removeScore(Node* pSender);
private:
	__Array* mAnimationArray;
	__Array* mDeadArray;
	float mRadius;
};

//全家福
class GameFishQjf : public GameFish
{
public:
	GameFishQjf();
	~GameFishQjf();
	static GameFishQjf* create(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action);
	virtual bool init(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action);
	virtual void Update(float delta_time);
	virtual void Render();
	GameAnimation* getFishAnimationByIndex(int index);
	void removeScore(Node* pNode);
private:
	__Array* mAnimationArray;
	__Array* mDeadArray;
	float mRadius;//外圈直径
	float mInnerRadius; //内圈直径
	float mScale; //鱼的缩小比例
	float mInnerWidht;
	float mInnerHeight;
	bool  isDealDead;

};

//闪电鱼
class GameFishChain : public GameFish
{
public:
	GameFishChain();
	~GameFishChain();
	static GameFishChain* create(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action);
	virtual bool init(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action);
	virtual void Update(float delta_time);
	virtual void Render();
	virtual void onChainPro(GameScene* scene,Vec2 chain_start_pos,Vec2 fish_end_pos,WORD view_chair_id,int fishIndex  =0);//闪电处理
	virtual Vec2  getEndPos();
	virtual float Scale() const {return mScale;}
	virtual void chainFish();//闪电提前完成
protected:
	void onStartMov(Node* pSender);
private:
	__Array* mAnimationArray;
	__Array* mDeadArray;
	float mScale;
	GameAnimation* mChainAnimation;
	Vec2  mChainStartPos;//闪电链起始坐标
	Vec2  mFishEndPos;//鱼移动终点坐标
	GameScene* mScene;

	ActionScaleTo* mScaleAction;
	GameAnimation* greenLight;
	bool  mIsRotating;
	GameAnimation* mSubFishDead;
	WORD   mViewChairId;
	Sprite* mCircleSp;
};

//炼丹炉
class GameFishBglu : public GameFish
{
public:
	GameFishBglu();
	~GameFishBglu();
	static GameFishBglu* create(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action);
	virtual bool init(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action);
	virtual void Update(float delta_time);
	virtual void Render();
	void removeScore(Node* pSender);
	virtual void catchFish(WORD view_chair_id, SCORE score);//鱼被捉到后处理
private:
	GameAnimation* mFontAnima;
	GameAnimation* mDeadAnima;
};


//李逵
class GameFishLK : public GameFish
{
public:
	GameFishLK();
	~GameFishLK();
	static GameFishLK* create(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action);
	virtual bool init(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action);
	virtual void Update(float delta_time);
	virtual void Render();
	void removeScore(Node* pSender);
private:
	void RenderMulriple(Vec2 pos,float degree);
private:
	LabelAtlas* mulripleLable;
};

//////////////////////////////////////////////////////////////////////////
//鱼王
class GameFishKing : public GameFish
{
public:
	GameFishKing();
	~GameFishKing();
	static GameFishKing* create(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action);
	virtual bool init(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action);
	virtual void Update(float delta_time);
	virtual void Render();
	void removeScore(Node* pSender);
private:
	GameAnimation* mInnerFishWalking;
	GameAnimation* mInnerFishDead;
};

//////////////////////////////////////////////////////////////////////////
//LKPY鱼群
class GameFishYuqunForLK : public GameFish
{
public:
	GameFishYuqunForLK();
	~GameFishYuqunForLK();
	static GameFishYuqunForLK* create(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action);
	virtual bool init(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action);
	virtual void Update(float delta_time);
	virtual void Render();
	virtual bool BulletHitTest(GameBullet* bullet);
	void removeScore(Node* pSender);
private:
	__Array* mAnimationArray;
	__Array* mRotateArray;
	__Array* mDeadAnimationArray;
	float	mRadius;//直径
	int     mFishCnt;
};




//////////////////////////////////////////////////////////////////////////

class GameFishManager : public Ref {
    GameFishManager();
    ~GameFishManager();

public:
    static GameFishManager* sharedInstance();
    static void purgeInstance();
	GameFish* creatFish(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action);

	void freeAllFish();
	void Update(float dt);
	void Render();
	bool InsideScreen(GameFish* fish);
	bool BulletHitTest(GameBullet* bullet);//检测鱼与子弹是否碰撞
	GameFish* getFish(int fish_id);//根据ID 获取鱼类
	GameFish* catchFish(int chairId,FishKind fish_kind ,int fish_id,SCORE fish_score);
	void fishPause(bool swith,float elapsed = 0);
	void modifyLkMulriple(int fish_id, int mulriple);
protected:
	__Array* mFishVector;
};

#endif