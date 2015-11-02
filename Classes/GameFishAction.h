#ifndef _GAME_FISH_ACTION_H_H_
#define _GAME_FISH_ACTION_H_H_

#include "cocos2d.h"
#include <vector>

USING_NS_CC;

class MovePoint {
public:
	
  MovePoint() : angle_(0) {}
  MovePoint(const Vec2& position, float angle) : position_(position), angle_(angle) {}
  ~MovePoint() {}

public:
  Vec2 position_;
  float angle_;
};

typedef std::vector<MovePoint> MovePointVector;


class ActionFishMove : public ActionInterval {
public:
  static ActionFishMove*create(float duration);
  virtual ~ActionFishMove(){}

  virtual Vec2 FishMoveTo(float elapsed){return mCurPos; };//计算FISH在(elapsed)当前时刻的位置并返回,子类实现
  virtual float getAngle(){return mAngle;} 
  virtual Vec2 getPosition(){return mCurPos;}

public:
	Vec2 mCurPos;
	float mAngle;
};

class ActionFishMoveLinear : public ActionFishMove {
 public:
  static ActionFishMoveLinear* create(float fish_speed, const Vec2& start, const Vec2& end);
  virtual ~ActionFishMoveLinear();

  virtual bool initWithDuration(float fish_speed, const Vec2& start, const Vec2& end);
  virtual void Start();

  virtual void update(float time);

  virtual Vec2 FishMoveTo(float elapsed);

 protected:
  Vec2 startPos;
  Vec2 endPos;
  Vec2 delta;
  float speed;
};

class ActionFishMoveBezier : public ActionFishMove {
 public:
  static ActionFishMoveBezier* create(float fish_speed, const Vec2& start, const Vec2& c1, const Vec2& end);
  static ActionFishMoveBezier* create(float fish_speed, const Vec2& start, const Vec2& c1, const Vec2& c2, const Vec2& end);

  bool initWithDuration(float fish_speed, const Vec2& start, const Vec2& c1, const Vec2& end);
  bool initWithDuration(float fish_speed, const Vec2& start, const Vec2& c1, const Vec2& c2, const Vec2& end);
   
  virtual ~ActionFishMoveBezier();

  virtual void Start();

  virtual void update(float time);

  virtual Vec2 FishMoveTo(float elapsed);

 protected:
  Vec2 startPos;
  Vec2 endPos;
  Vec2 control1;
  Vec2 control2;
  float speed;
  MovePointVector movePoints;
};

//佛手移动类
class ActionFishMoveFoshou : public ActionFishMove {
public:
	static ActionFishMoveFoshou* create(float fish_speed, const Vec2& start, const Vec2& end);
	bool initWithDuration(float fish_speed, const Vec2& start, const Vec2& end);
	virtual ~ActionFishMoveFoshou();

	virtual bool IsDone() { return false; }
	virtual void Start();

	virtual void step(float dt);

	virtual Vec2 FishMoveTo(float elapsed);

protected:
	Vec2 start_;
	Vec2 end_;
	float fish_speed_;
	float dx_;
	float dy_;
};



//////////////////////////////////////////////////////////////////////////
//子弹移动类
class ActionBulletMove : public ActionInterval {
public:
	ActionBulletMove();
	static ActionBulletMove* create(const Vec2& position, float angle, float speed);
	virtual bool init(const Vec2& position, float angle, float speed);
	virtual ~ActionBulletMove();

	virtual bool IsDone() { return false; }
	virtual void step(float dt);
	float getAngle() {return angle_;}
	Vec2  getPosition() {return position_;}
private:
	float bullet_speed_;
	float dx_;
	float dy_;
	Vec2  position_;
	float angle_;
};
//////////////////////////////////////////////////////////////////////////
//泡泡移动类，专为小厅
class ActionBubbletMove : public ActionInterval {
public:
	ActionBubbletMove();
	static ActionBubbletMove* create(const Vec2& position, float angle, float speedx, float speedy,float width);
	virtual bool init(const Vec2& position, float angle, float speedx,float speedy,float width);
	virtual ~ActionBubbletMove();

	virtual bool IsDone() { return false; }
	virtual void step(float dt);
	float getAngle() {return angle_;}
	Vec2  getPosition() {return position_;}
	void  reverseX();//X反向
	void  reverseY();//Y反向
	float  getDX() const {return bullet_speed_x;};
	float  getDY() const {return bullet_speed_y;};
private:
	float bullet_speed_x;
	float bullet_speed_y;
	float dx_;
	float dy_;
	Vec2  position_;
	float angle_;
	float width_;
};
   
//////////////////////////////////////////////////////////////////////////


class ActionRorateBy : public ActionInterval {
public:
	static ActionRorateBy* create(float duration, float start, float angle);
	virtual ~ActionRorateBy();
	ActionRorateBy();

	virtual bool  init(float duration, float start, float angle);
	virtual void start();

	virtual void update(float time);

	float angle() const{return angle_;}

private:
	float start_angle_;
	float rotate_angle_;
	float angle_;//当前角度
	bool  isplaying;
};

//////////////////////////////////////////////////////////////////////////
class ActionScaleTo : public ActionInterval {
public:
	static ActionScaleTo* create(float duration, float start_scale_x, float start_scale_y, float scale);
	bool init(float duration, float start_scale_x, float start_scale_y, float scale);
	virtual ~ActionScaleTo();
	ActionScaleTo();

	virtual void start();

	virtual void update(float time);

	Vec2 getCurScale() const {return mScale;}
protected:
	float start_scale_x_;
	float start_scale_y_;
	float end_scale_x_;
	float end_scale_y_;
	float delta_x_;
	float delta_y_;
	bool  isPlaying;
	Vec2  mScale;
};


class ActionScore : public ActionInterval {
public:
	ActionScore();
	virtual ~ActionScore();
	static ActionScore* create(const Vec2& start, int catch_chair_id, std::string score);
	bool virtual init(const Vec2& start, int catch_chair_id, std::string score);

	virtual void start();

	virtual void update(float time);
	virtual int catch_chair_id() const { return catch_chair_id_; }
	virtual std::string score() const { return score_; }
	Vec2    position() const { return position_;}

protected:
	Vec2 start_;
	int catch_chair_id_;
	std::string score_;
	Vec2 position_;
	bool isPlaying;
};

//////////////////////////////////////////////////////////////////////////


class ActionScene3FishMove : public ActionFishMove {
public:
	static ActionScene3FishMove* create(const Vec2& center, float radius, float rotate_duration, float start_angle, float rotate_angle, float move_duration, float fish_speed);
	virtual ~ActionScene3FishMove();
	virtual bool init(const Vec2& center, float radius, float rotate_duration, float start_angle, float rotate_angle, float move_duration, float fish_speed);
	ActionScene3FishMove();
	virtual void Start();

	virtual void step(float dt);
	virtual void update(float time);

	virtual Vec2 FishMoveTo(float elapsed);

private:
	Vec2 center_;
	float radius_;
	float rotate_duration_;
	float start_angle_;
	float rotate_angle_;
	float move_duration_;
	Vec2 delta_;
	float fish_speed_;
	int stage_;
};

//------------------------------------------------------------------------------

class ActionScene4FishMove : public ActionFishMove {
public:
	ActionScene4FishMove();
	static ActionScene4FishMove* create(const Vec2& center, float radius, float rotate_duration, float start_angle, float rotate_angle, float move_duration, float fish_speed);
	virtual ~ActionScene4FishMove();
	virtual bool init(const Vec2& center, float radius, float rotate_duration, float start_angle, float rotate_angle, float move_duration, float fish_speed);
	virtual void Start();

	virtual void step(float dt);
	virtual void update(float time);

	virtual Vec2 FishMoveTo(float elapsed);

private:
	Vec2 center_;
	float radius_;
	float rotate_duration_;
	float start_angle_;
	float rotate_angle_;
	float move_duration_;
	Vec2 delta_;
	float fish_speed_;
	int stage_;
	Vec2 save_position_;
};

//////////////////////////////////////////////////////////////////////////
class Shake : public ActionInterval  
{  
public:  
	Shake();  

	// Create the action with a time and a strength (same in x and y)  
	static Shake *create(float d, float strength );  

	// Create the action with a time and strengths (different in x and y)  
	static Shake *createWithStrength(float d, float strength_x, float strength_y );  

	bool initWithDuration(float d, float strength_x, float strength_y );  

protected:  

	void startWithTarget(cocos2d::Node *pTarget);  
	void update(float time);  
	void stop(void);  

	virtual ActionInterval* reverse() const;  
	virtual ActionInterval* clone() const;  

	Point m_StartPosition;  
	Node* m_pTarget;  

	// Strength of the action  
	float m_strength_x, m_strength_y;  
};  

#endif