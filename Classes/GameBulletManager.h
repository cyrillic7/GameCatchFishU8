#ifndef _GAME_BULLET_MANAGER_H_H_
#define _GAME_BULLET_MANAGER_H_H_

#include "cocos2d.h"
#include "GameGlobalSetting.h"
#include "GameFishAction.h"

class GameBullet : public Sprite
{
public:
	GameBullet();
	~GameBullet();
	static GameBullet* create(BulletKind bullet_kind, float bounding_radius, WORD firer_view_chair_id, float bullet_speed);
	bool init(BulletKind bullet_kind, float bounding_radius, WORD firer_view_chair_id, float bullet_speed);
	void update(float delta);
	void render();
	BulletKind getBulletKind(){return mBulletKind;}
	float      getBulletRadius(){return mBoudingRadius;}
	WORD       getBulletChairID(){return mFirerChairId;}
	float	   getBulletSpeed(){return mBulletSpeed;}
	void       setBulletAction(ActionBulletMove* action);
	ActionBulletMove* getBulletAction() {return mBulletAction;}
	void      setBulletID(DWORD id) {mBulletId = id;}
	DWORD     getBulletID() {return mBulletId;}

	//重新利用
	void   reInit(BulletKind bullet_kind, float bounding_radius, WORD firer_view_chair_id, float bullet_speed);
	enum BulletStatus {
		MOVE = 0,
		EXPLODE,
		DESTROY
	};
	BulletStatus getStatus() const { return mStatus; }
	Vec2 position() {return mBulletAction->getPosition();}
	void Explode();
	void freeBullet(Node* node);
	bool isBReuse() { return mReuseFlag;}
private:
	BulletKind mBulletKind;
	float mBoudingRadius;
	WORD  mFirerChairId;
	float mBulletSpeed;
	DWORD mBulletId;

	ActionBulletMove* mBulletAction; //子弹动作类对象
	BulletStatus mStatus;
	int	   mBulletWidth;
	int	   mBulletHeight;

	bool    mReuseFlag ;

};


class GameBulletManager : public Ref
{
public:
	GameBulletManager();
	~GameBulletManager();
	//static GameBulletManager* create();
	static GameBulletManager* shareInstance();
	void purgeInstance();
	bool init();
	void update(float delta_time);
	void render();

	bool FreeBullet(GameBullet* bullet);
	void FreeAllBullet();
	GameBullet* GetBullet(DWORD bullet_id);
	void Explode(DWORD bullet_id);
	/*args fire_pos子弹的起始位置
	angle :子弹的起始旋转角度
	bullet_kind:子弹种类
	bullet_radius:子弹有效半径
	firer_view_chair_id：在自己视角下的座位ID
	speed : 子弹的速度
	*/
	GameBullet* Fire(const Vec2& fire_pos, float angle, BulletKind bullet_kind, float bounding_radius, WORD firer_view_chair_id, float bullet_speed, int lock_fish_id = -1, ActionInterval* action_fish_move = NULL);
private:
	float BulletMoveDuration(const Vec2& start, const Vec2& end, float bullet_speed);

private:
	__Array*   mBulletArray;
	//__Array*   mFreeBulletArray;//空闲子弹数组
};
#endif //_GAME_BULLET_MANAGER_H_H_