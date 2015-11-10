#include"GameFishAction.h"

const int kFPS = 30;              // 帧率
const float kSpeed = 1.f / kFPS;  // 速度

const unsigned long  kGameLoopElasped = 30;

const int kScreenWidth = 1366;    // 屏宽
const int kScreenHeight = 768;    // 屏高

const float kDuration = 1.f;
const float kOffset = 30.f;

//------------------------------------------------------------------------------
static int Factorial(int number) {
  int factorial = 1;
  int temp = number;
  for (int i = 0; i < number; ++i) {
    factorial *= temp;
    --temp;
  }

  return factorial;
}

static int Combination(int count, int r) {
  return Factorial(count) / (Factorial(r) * Factorial(count - r));
}

static float CalcDistance(float x1, float y1, float x2, float y2) {
  return std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

static void BuildBezier(const Vec2 points[], int points_count, MovePointVector& move_points, float distance) {
  assert(points_count >= 2 && points_count <= 4);
  if (!(points_count >= 2 && points_count <= 4))
    return;
  move_points.clear();
  MovePoint point(points[0], 0.f);
  move_points.push_back(point);

  int index = 0;
  MovePoint temp_pos0, temp_pos;
  float t = 0.f;
  int count = points_count - 1;
  float temp_value = 0.f;

  while (t < 1.0f) {
    temp_pos.position_.x = 0.f;
    temp_pos.position_.y = 0.f;
    index = 0;
    while (index <= count) {
      temp_value = std::pow(t, (float)index) * std::pow(1.f - t, float(count - index)) * Combination(count, index);
      temp_pos.position_.x += points[index].x * temp_value;
      temp_pos.position_.y += points[index].y * temp_value;
      ++index;
    }

    MovePoint& back_pos = move_points.back();
    temp_value = CalcDistance(back_pos.position_.x, back_pos.position_.y, temp_pos.position_.x, temp_pos.position_.y);

    if (temp_value >= distance) {
      float temp_dis = CalcDistance(temp_pos.position_.x, temp_pos.position_.y, temp_pos0.position_.x, temp_pos0.position_.y);
      if (temp_dis != 0.f) {
        temp_value = (temp_pos.position_.x - temp_pos0.position_.x) / temp_dis;
        if ((temp_pos.position_.y - temp_pos0.position_.y) >= 0.f) {
          temp_pos.angle_ = -std::acos(temp_value);
        } else {
          temp_pos.angle_ = std::acos(temp_value);
        }
      } else {
        temp_pos.angle_ = 0.f;
      }
      move_points.push_back(temp_pos);
      temp_pos0.position_.x = temp_pos.position_.x;
      temp_pos0.position_.y = temp_pos.position_.y;
    }
    t += 0.001f;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////

ActionFishMove* ActionFishMove::create(float duration)
{
	ActionFishMove *ret = new (std::nothrow) ActionFishMove();
	if (ret && ret->initWithDuration(duration))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

ActionFishMoveLinear* ActionFishMoveLinear::create(float fish_speed, const Vec2& start, const Vec2& end)
{
	ActionFishMoveLinear *ret = new (std::nothrow) ActionFishMoveLinear();

	if (ret && ret->initWithDuration(fish_speed,start,end))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ActionFishMoveLinear::initWithDuration(float fish_speed, const Vec2& start, const Vec2& end)
{
	if(!ActionFishMove::initWithDuration(0.0f))
		return false;
	mCurPos = start;

	speed = fish_speed;
	Vec2::subtract(end,start,&delta);
	startPos = start;
	endPos = end;
	float length = delta.length();
	if(length>0)
	{
		if(delta.y > 0) 
			mAngle = -std::acos(delta.x/ length);
		else
			mAngle = std::acos(delta.x/ length);
	}
	float duration = length/fish_speed;
	setDuration(duration);
	return true;
}

ActionFishMoveLinear::~ActionFishMoveLinear()
{
}

void ActionFishMoveLinear::Start()
{

}

void ActionFishMoveLinear::update(float time)
{
  mCurPos.x = startPos.x + delta.x * time;
  mCurPos.y = startPos.y + delta.y * time;
}

Vec2 ActionFishMoveLinear::FishMoveTo(float elapsed)
{
  float time = MIN(1.0f, (_elapsed + elapsed) / _duration);
  return Vec2(startPos.x + delta.x * time, startPos.y + delta.y * time);
}

 ActionFishMoveBezier* ActionFishMoveBezier::create(float fish_speed, const Vec2& start, const Vec2& c1, const Vec2& end)
 {
	 ActionFishMoveBezier *ret = new (std::nothrow) ActionFishMoveBezier();

	if (ret && ret->initWithDuration(fish_speed,start,c1,end))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
 }
 ActionFishMoveBezier* ActionFishMoveBezier::create(float fish_speed, const Vec2& start, const Vec2& c1, const Vec2& c2, const Vec2& end)
 {
	 ActionFishMoveBezier *ret = new (std::nothrow) ActionFishMoveBezier();

	if (ret && ret->initWithDuration(fish_speed,start,c1,c2,end))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
 }

ActionFishMoveBezier::~ActionFishMoveBezier()
{
}

bool ActionFishMoveBezier::initWithDuration(float fish_speed, const Vec2& start, const Vec2& c1, const Vec2& end)
{
	if(!ActionFishMove::initWithDuration(0.0f))
		return false;
	speed = fish_speed;
	startPos = start;
	mCurPos = startPos;
	endPos = end;
	control1 = c1;

	Vec2 points[3] = { start, c1, end };
	BuildBezier(points, 3, movePoints, speed * kSpeed);
	setDuration(kSpeed * movePoints.size());
	return true;
}

bool ActionFishMoveBezier::initWithDuration(float fish_speed, const Vec2& start, const Vec2& c1, const Vec2& c2, const Vec2& end)
{
	if(!ActionFishMove::initWithDuration(0.0f))
		return false;
	speed = fish_speed;
	startPos = start;
	mCurPos = startPos;
	endPos = end;
	control1 = c1;
	control2 = c2;

	Vec2 points[4] = { start, c1,c2, end };
	BuildBezier(points,4, movePoints, speed * kSpeed);
	setDuration(kSpeed * movePoints.size());

	return true;
}


void ActionFishMoveBezier::Start()
{
}

void ActionFishMoveBezier::update(float time)
{
  float index = time * movePoints.size();
  MovePointVector::size_type idx = static_cast<MovePointVector::size_type>(index);
  float diff = index - idx;
  if (idx >= movePoints.size())
    idx = movePoints.size() - 1;

  if (idx < movePoints.size() - 1) {
    MovePoint point1 = movePoints[idx];
    MovePoint point2 = movePoints[idx + 1];
    mCurPos = point1.position_ * (1.0f - diff) + point2.position_ * diff;
	mAngle = point1.angle_ * (1.0f - diff) + point2.angle_ * diff;
    if (std::abs(point1.angle_ - point2.angle_) > M_PI)
      mAngle = point1.angle_;
  } else {
    mCurPos = movePoints[idx].position_;
  }
}

Vec2 ActionFishMoveBezier::FishMoveTo(float elapsed)
{ 
  float time = MIN(1.0f, (_elapsed + elapsed) / _duration);
  float index = time * movePoints.size();
  MovePointVector::size_type idx = static_cast<MovePointVector::size_type>(index);
  float diff = index - idx;
  if (idx >= movePoints.size())
    idx = movePoints.size() - 1;

  Vec2 move_to;
  if (idx < movePoints.size() - 1) {
    MovePoint point1 = movePoints[idx];
    MovePoint point2 = movePoints[idx + 1];
    move_to = point1.position_ * (1.0f - diff) + point2.position_ * diff;
  } else {
    move_to = movePoints[idx].position_;
  }
  return move_to;
}

//////////////////////////////////////////////////////////////////////////
//佛手移动类
ActionFishMoveFoshou* ActionFishMoveFoshou::create(float fish_speed, const Vec2& start, const Vec2& end)
{
	ActionFishMoveFoshou* pMov = new ActionFishMoveFoshou;
	if (pMov && pMov->initWithDuration(fish_speed,start,end))
	{
		pMov->autorelease();
		return pMov;
	}
	CC_SAFE_DELETE(pMov);
	return nullptr;

}

bool ActionFishMoveFoshou::initWithDuration(float fish_speed, const Vec2& start, const Vec2& end)
{
	if (!ActionFishMove::initWithDuration(0.0f))
		return false;
	mCurPos = start;
	start_ = start;
	end_   = end;
	fish_speed_ = fish_speed;
	Vec2 delta; 
	Vec2::subtract(end_,start_,&delta);

	float length = std::sqrt(delta.x * delta.x + delta.y * delta.y);
	if (length > 0) {
		if (delta.y >= 0) {
			mAngle = std::acos(delta.x / length);
		} else {
			mAngle = -std::acos(delta.x / length);
		}
	}
	dx_ = std::cos(mAngle);
	dy_ = std::sin(mAngle);
	float duration_ = length / fish_speed_;
	setDuration(duration_);

	return true;
}
	

ActionFishMoveFoshou::~ActionFishMoveFoshou() {
}

void ActionFishMoveFoshou::Start() {
	
}

void ActionFishMoveFoshou::step(float delta_time) {

	mCurPos.x += fish_speed_ * delta_time * dx_;
	mCurPos.y += fish_speed_ * delta_time * dy_ ;

	if (mCurPos.x < 0.0f) {
		mCurPos.x = -mCurPos.x;
		dx_ = -dx_;
		mAngle = -mAngle;
	}
	if (mCurPos.x > kScreenWidth) {
		mCurPos.x = kScreenWidth - (mCurPos.x - kScreenWidth);
		dx_ = -dx_;
		mAngle = -mAngle;
	}
	if (mCurPos.y < 0.0f) {
		mCurPos.y = -mCurPos.y;
		dy_ = -dy_;
		mAngle = M_PI - mAngle;
	}
	if (mCurPos.y > kScreenHeight) {
		mCurPos.y = kScreenHeight - (mCurPos.y - kScreenHeight);
		dy_ = -dy_;
		mAngle = M_PI - mAngle;
	}
}

Vec2 ActionFishMoveFoshou::FishMoveTo(float elapsed) {
	Vec2 move_to = mCurPos;
	float dx = dx_, dy = dy_;
	if (_elapsed + elapsed < _duration) {
		move_to.x += fish_speed_ * elapsed * dx * fish_speed_;
		move_to.y += fish_speed_ * elapsed * dy * fish_speed_;
		return move_to;
	} else {
		int count = int(elapsed / kSpeed);
		while ((count--) > 0) {
			move_to.x += fish_speed_ * kSpeed * dx * fish_speed_;
			move_to.y += fish_speed_ * kSpeed * dy * fish_speed_;

			if (move_to.x < 0.0f) {
				move_to.x = -move_to.x;
				dx = -dx;
			}
			if (move_to.x > kScreenWidth) {
				move_to.x = kScreenWidth - (move_to.x - kScreenWidth);
				dx = -dx;
			}
			if (move_to.y < 0.0f) {
				move_to.y = -move_to.y;
				dy = -dy;
			}
			if (move_to.y > kScreenHeight) {
				move_to.y = kScreenHeight - (move_to.y - kScreenHeight);
				dy = -dy;
			}
		}
		return move_to;
	}
}


//////////////////////////////////////////////////////////////////////////
//字弹移动类
ActionBulletMove::ActionBulletMove()
{

}

ActionBulletMove::~ActionBulletMove()
{

}

ActionBulletMove* ActionBulletMove::create(const Vec2& position, float angle, float speed)
{
	ActionBulletMove* pBulletMov = new ActionBulletMove();
	if (pBulletMov && pBulletMov->init(position,angle,speed))
	{
		pBulletMov->autorelease();
		return pBulletMov;
	}
	CC_SAFE_DELETE(pBulletMov);
	return nullptr;
}


bool ActionBulletMove::init(const Vec2& position, float angle, float speed)
{
	if (!ActionInterval::initWithDuration(1.0f))
		return false;
	bullet_speed_ = speed;
	position_ = position;
	angle_ = angle;
	dx_ = std::sin(angle_);
	dy_ = std::cos(angle_);

	return true;

}

void ActionBulletMove::step(float delta_time) {
	position_.x += bullet_speed_ * delta_time * dx_;
	position_.y += bullet_speed_ * delta_time * dy_ ;
	if (position_.x < 0.0f) {
		position_.x = -position_.x;
		dx_ = -dx_;
		angle_ =  -angle_;
	}
	if (position_.x > kScreenWidth) {
		position_.x = kScreenWidth - (position_.x - kScreenWidth);
		dx_ = -dx_;
		angle_ =  -angle_;
	}
	if (position_.y < 0.0f) {
		position_.y = -position_.y;
		dy_ = -dy_;
		angle_ = M_PI - angle_;
	}
	if (position_.y > kScreenHeight) {
		position_.y = kScreenHeight - (position_.y - kScreenHeight);
		dy_ = -dy_;
		angle_ =M_PI  - angle_;
	}
}

//////////////////////////////////////////////////////////////////////////
//气泡移动类
ActionBubbletMove::ActionBubbletMove()
{

}

ActionBubbletMove::~ActionBubbletMove()
{

}

ActionBubbletMove* ActionBubbletMove::create(const Vec2& position, float angle, float speedx, float speedy,float width)
{
	ActionBubbletMove* pBulletMov = new ActionBubbletMove();
	if (pBulletMov && pBulletMov->init(position,angle,speedx,speedy,width))
	{
		pBulletMov->autorelease();
		return pBulletMov;
	}
	CC_SAFE_DELETE(pBulletMov);
	return nullptr;
}


bool ActionBubbletMove::init(const Vec2& position, float angle, float speedx,float speedy,float width)
{
	if (!ActionInterval::initWithDuration(1.0f))
		return false;
	bullet_speed_x = speedx;
	bullet_speed_y = speedy;
	position_ = position;
	angle_ = angle;
	dx_ = std::sin(angle_);
	dy_ = std::cos(angle_);
	width_ = width;
	return true;

}

void ActionBubbletMove::step(float delta_time) {
	position_.x += bullet_speed_x ;//* delta_time * dx_;
	position_.y += bullet_speed_y ;//* delta_time * dy_ ;
	if (position_.x < width_/2) {
		position_.x = width_/2 + 20;
		bullet_speed_x = -bullet_speed_x;
	}
	if (position_.x > kScreenWidth - width_/2) {
		position_.x =  kScreenWidth - width_/2 -20;
		bullet_speed_x = -bullet_speed_x;
	}
	if (position_.y <width_/2) {
		position_.y = width_/2 + 20 ;
		bullet_speed_y = -bullet_speed_y;
	}
	if (position_.y > kScreenHeight - width_/2) {
		position_.y = kScreenHeight - width_/2 -20;
		bullet_speed_y = -bullet_speed_y;
	}
}

void ActionBubbletMove::reverseX()
{
	//dx_ = -dx_;
	bullet_speed_x = -bullet_speed_x;
}

void ActionBubbletMove::reverseY()
{
	bullet_speed_y = -bullet_speed_y;
}

//////////////////////////////////////////////////////////////////////////
ActionRorateBy::ActionRorateBy()
{
	start_angle_ = 0;
	rotate_angle_ =0;
	angle_ = 0;
	isplaying = false;
}
ActionRorateBy* ActionRorateBy::create(float duration, float start, float angle)
{

	ActionRorateBy* pAction = new ActionRorateBy();
	if (pAction && pAction->init(duration,start,angle))
	{
		pAction->autorelease();
		return pAction;
	}
	CC_SAFE_DELETE(pAction);
	pAction =nullptr;
}

ActionRorateBy::~ActionRorateBy() {
}

bool ActionRorateBy::init(float duration, float start, float angle)
{
	if (!ActionInterval::initWithDuration(duration))
		return false;
	start_angle_ = start;
	rotate_angle_ = angle;
	return true;
}

void ActionRorateBy::start()
{
	isplaying = true;
}

void ActionRorateBy::update(float time) {
	if (!isplaying)
		return;
	angle_ = start_angle_ + rotate_angle_ * time;
}

//////////////////////////////////////////////////////////////////////////

ActionScaleTo::ActionScaleTo()
{
	isPlaying = false;
	start_scale_x_ =0;
	start_scale_y_=0;
	end_scale_x_ =0;
	end_scale_y_=0;
	mScale = Vec2(0,0);
	delta_x_ = 0;
	delta_y_ = 0;

}
ActionScaleTo* ActionScaleTo::create(float duration, float start_scale_x, float start_scale_y, float scale)
{
	ActionScaleTo* pAction = new ActionScaleTo();
	if (pAction && pAction->init(duration,start_scale_x,start_scale_y,scale));
	{
		pAction->autorelease();
		return pAction;
	}
	CC_SAFE_DELETE(pAction);
	pAction = nullptr;
}

bool ActionScaleTo::init(float duration, float start_scale_x, float start_scale_y, float scale)
{
	if(!ActionInterval::initWithDuration(duration))
		return false;
	start_scale_x_ =start_scale_x;
	start_scale_y_=start_scale_y;
	end_scale_x_ =scale;
	end_scale_y_=scale;
	mScale.x = start_scale_x;
	mScale.y = start_scale_y;
	delta_x_ = end_scale_x_ - start_scale_x_;
	delta_y_ = end_scale_y_ - start_scale_y_;

	return true;
}



ActionScaleTo::~ActionScaleTo() {
}

void ActionScaleTo::start() {
	isPlaying = true;
	delta_x_ = end_scale_x_ - start_scale_x_;
	delta_y_ = end_scale_y_ - start_scale_y_;
}

void ActionScaleTo::update(float time) {
	if(!isPlaying)
		return;
	mScale.x = start_scale_x_ + delta_x_ * time;
	mScale.y = start_scale_y_ + delta_y_ * time;
}

//////////////////////////////////////////////////////////////////////////

ActionScore::ActionScore()
{
	start_ = Vec2(0,0);
	catch_chair_id_ = -1;
	score_ = "";
}

ActionScore* ActionScore::create(const Vec2& start, int catch_chair_id, std::string score)
{
	ActionScore* pAction = new ActionScore();
	if (pAction && pAction->init(start,catch_chair_id,score))
	{
		pAction->autorelease();
		return pAction;
	}
	CC_SAFE_DELETE(pAction);
	pAction = nullptr;
}


bool ActionScore::init(const Vec2& start, int catch_chair_id, std::string score)
{
	if (!ActionInterval::initWithDuration(kDuration))
		return false; 
	start_ = start;
	catch_chair_id_= catch_chair_id;
	score_ = score;
	position_ = start;
	return true;
}

ActionScore::~ActionScore() {
}

void ActionScore::start() {
	isPlaying = true;
}

void ActionScore::update(float time) {
	if(!isPlaying) return;

	float new_time = 0.f;
	if (time > 0.5f) {
		new_time = (time - 0.5f) / 0.5f;
		if (catch_chair_id_ >= 3) {
			position_.y = start_.y - kOffset * (1.f - new_time);
		} else {
			position_.y = start_.y + kOffset * (1.f - new_time);
		}
	} else {
		new_time = time / 0.5f;
		if (catch_chair_id_ >= 3) {
			position_.y = start_.y - kOffset * new_time;
		} else {
			position_.y = start_.y + kOffset * new_time;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
ActionScene3FishMove::ActionScene3FishMove()
{
	center_ = Vec2(0,0);
	radius_ = 0;
	rotate_duration_ = 0;
	start_angle_ = 0;
	rotate_angle_ = 0;
	fish_speed_ =0;
	stage_ = 0;
}

ActionScene3FishMove::~ActionScene3FishMove() {
}

ActionScene3FishMove* ActionScene3FishMove::create(const Vec2& center, float radius, float rotate_duration, float start_angle, float rotate_angle, float move_duration, float fish_speed)
{
	ActionScene3FishMove* mov = new ActionScene3FishMove();
	if (mov && mov->init(center,radius,rotate_duration,start_angle,rotate_angle,move_duration,fish_speed))
	{
		mov->autorelease();
		return mov;
	}
	CC_SAFE_DELETE(mov);
	return nullptr;
}

bool ActionScene3FishMove::init(const Vec2& center, float radius, float rotate_duration, float start_angle, float rotate_angle, float move_duration, float fish_speed)
{
	if (!ActionFishMove::initWithDuration(rotate_duration))
		return false;
	center_ = center;
	radius_ = radius; 
	rotate_duration_= rotate_duration;
	start_angle_ = start_angle; 
	rotate_angle_=rotate_angle;
	move_duration_=move_duration;
	fish_speed_ = fish_speed;
	mAngle =  start_angle_;

	mCurPos.x = center_.x + radius_ * std::sin(start_angle_);
	mCurPos.y = center_.y + radius_ * std::cos(start_angle_);

	return true;
}

void ActionScene3FishMove::Start() {
}

void ActionScene3FishMove::step(float dt) {
	_elapsed += dt;
	update(MIN(1.0f, _elapsed / _duration));
	if (stage_ == 0 && _elapsed >= _duration) {
		stage_ = 1;
		_elapsed -= _duration;
		_duration = move_duration_;
		delta_.x = std::sin(mAngle+M_PI_2);
		delta_.y = std::cos(mAngle+M_PI_2);
	}
	if (stage_ != 0) {
		mCurPos.x += fish_speed_ * dt * delta_.x ;
		mCurPos.y += fish_speed_ * dt * delta_.y ;
	}
}

void ActionScene3FishMove::update(float time) {
	if (stage_ == 0) {
		float angle = start_angle_ + rotate_angle_ * time;
		mCurPos.x = center_.x + radius_ * std::sin(angle);
		mCurPos.y = center_.y + radius_ * std::cos(angle);
		mAngle = angle;
		if (radius_ ==0)
		{
			mAngle += M_PI_2 ;
		}
	}
}

Vec2 ActionScene3FishMove::FishMoveTo(float elapsed) {
	if (stage_ == 0) {
		if (_elapsed + elapsed >= _duration) {
			float angle = start_angle_ + rotate_angle_;
			Vec2 move_to(center_.x + radius_ * std::cos(angle), center_.y + radius_ * std::cos(angle));
			float el = _elapsed + elapsed - _duration;
			int count = int(el / kSpeed);
			while ((count--) > 0) {
				move_to.x += fish_speed_ * kSpeed * delta_.x;
				move_to.y += fish_speed_ * kSpeed * delta_.y ;
			}
			return move_to;
		} else {
			float time = MIN(1.0f, (_elapsed + elapsed) / _duration);
			float angle = start_angle_ + rotate_angle_ * time;
			return Vec2(center_.x + radius_ * std::cos(angle), center_.y + radius_ * std::cos(angle));
		}
	} else {
		int count = int(elapsed / kSpeed);
		Vec2 move_to = mCurPos;
		while ((count--) > 0) {
			move_to.x += fish_speed_ * kSpeed * delta_.x;
			move_to.y += fish_speed_ * kSpeed * delta_.y;
		}
		return move_to;
	}
}

//------------------------------------------------------------------------------

ActionScene4FishMove::ActionScene4FishMove()
{
	center_ = Vec2(0,0);
	radius_ = 0;
	rotate_duration_ = 0;
	start_angle_ = 0;
	rotate_angle_ = 0;
	fish_speed_ =0;
	stage_ = -1;
}

ActionScene4FishMove::~ActionScene4FishMove() {
}

ActionScene4FishMove* ActionScene4FishMove::create(const Vec2& center, float radius, float rotate_duration, float start_angle, float rotate_angle, float move_duration, float fish_speed)
{
	ActionScene4FishMove* mov = new ActionScene4FishMove();
	if (mov && mov->init(center,radius,rotate_duration,start_angle,rotate_angle,move_duration,fish_speed))
	{
		mov->autorelease();
		return mov;
	}
	CC_SAFE_DELETE(mov);
	return NULL;
}


bool ActionScene4FishMove::init(const Vec2& center, float radius, float rotate_duration, float start_angle, float rotate_angle, float move_duration, float fish_speed)
{
	if (!ActionFishMove::initWithDuration(rotate_duration))
		return false;

	center_ = center;
	radius_ = radius; 
	rotate_duration_= rotate_duration;
	start_angle_ = start_angle; 
	rotate_angle_=rotate_angle;
	move_duration_=move_duration;
	fish_speed_ = fish_speed;
	mAngle =  start_angle_;
	if (radius == 0)
		stage_ = 0;

	mCurPos.x = -500.f;
	mCurPos.y = -500.f;
	save_position_.x = center_.x + radius_ * std::sin(start_angle_);
	save_position_.y = center_.y + radius_ * std::cos(start_angle_);
	if (stage_ != -1)
		mCurPos = save_position_;

	return true;
}


void ActionScene4FishMove::Start() {
}

void ActionScene4FishMove::step(float dt) {
	_elapsed += dt ;
	update(MIN(1.0f, _elapsed / _duration));
	if (stage_ == 0 && _elapsed >= _duration) {
		stage_ = 1;
		_elapsed -= _duration;
		_duration = move_duration_;
		delta_.x = std::sin(mAngle+M_PI_2);
		delta_.y = std::cos(mAngle+M_PI_2);
	}
	if (stage_ == 1) {
		mCurPos.x += fish_speed_ * dt * delta_.x;
		mCurPos.y += fish_speed_ * dt * delta_.y ;
	}
}

void ActionScene4FishMove::update(float time) {
	if (stage_ != 1) {
		float angle = start_angle_ + rotate_angle_ * time;
		save_position_.x = center_.x + radius_ * std::sin(angle);
		save_position_.y = center_.y + radius_ * std::cos(angle);
		if (stage_ == -1 && save_position_.x > center_.x && save_position_.y < center_.y) {
			stage_ = 0;
		}
		if (stage_ != -1)
			mCurPos = save_position_;
		mAngle =  angle;
		if (radius_ ==0)
		{
			mAngle += M_PI ;
		}
	}
}

Vec2 ActionScene4FishMove::FishMoveTo(float elapsed) {
	if (stage_ == 0) {
		if (_elapsed + elapsed >= _duration) {
			float angle = start_angle_ + rotate_angle_;
			Vec2 move_to(center_.x + radius_ * std::cos(angle), center_.y + radius_ * std::cos(angle));
			float el = _elapsed + elapsed - _duration;
			int count = int(el / kSpeed);
			while ((count--) > 0) {
				move_to.x += fish_speed_ * kSpeed * delta_.x ;
				move_to.y += fish_speed_ * kSpeed * delta_.y ;
			}
			return move_to;
		} else {
			float time = MIN(1.0f, (_elapsed + elapsed) / _duration);
			float angle = start_angle_ + rotate_angle_ * time;
			return Vec2(center_.x + radius_ * std::cos(angle), center_.y + radius_ * std::cos(angle));
		}
	} else if (stage_ == 1) {
		int count = int(elapsed / kSpeed);
		Vec2 move_to = mCurPos;
		while ((count--) > 0) {
			move_to.x += fish_speed_ * kSpeed * delta_.x ;
			move_to.y += fish_speed_ * kSpeed * delta_.y ;
		}
		return move_to;
	} else {
		return Vec2(-500.f, -500.f);
	}
}

//////////////////////////////////////////////////////////////////////////
Shake::Shake() : m_strength_x(0), m_strength_y(0)  
{  

}  
  
Shake *Shake::create( float d, float strength )  
{  
    // call other construction method with twice the same strength  
    return createWithStrength( d, strength, strength );  
}  
  
Shake *Shake::createWithStrength(float duration, float strength_x, float strength_y)  
{  
    Shake* pRet = new Shake();  
      
    if (pRet && pRet->initWithDuration(duration, strength_x, strength_y))  
    {  
        pRet->autorelease();  
    }  
    else  
    {  
        CC_SAFE_DELETE(pRet);  
    }  
      
      
    return pRet;  
}  
  
bool Shake::initWithDuration(float duration, float strength_x, float strength_y)  
{  
    if (ActionInterval::initWithDuration(duration))  
    {  
        m_strength_x = strength_x;  
        m_strength_y = strength_y;  
          
        return true;  
    }  
      
    return false;  
}  
  
// Helper function. I included it here so that you can compile the whole file  
// it returns a random value between min and max included  
static float fgRangeRand( float min, float max )  
{  
    float rnd = ((float)rand() / (float)RAND_MAX);  
    return rnd * (max - min) + min; 
}  
  
void Shake::update(float dt)  
{  
    //float randx = fgRangeRand( -m_strength_x, m_strength_x ) * dt;  
    //float randy = fgRangeRand( -m_strength_y, m_strength_y ) * dt;    
	float randx = rand() % 2 == 0 ? (2.f + fgRangeRand(0.f, 3.f)) : (-2.f + fgRangeRand(-3.f, 0.f));
	float randy = rand() % 2 == 1 ? (2.f + fgRangeRand(0.f, 3.f)) : (-2.f + fgRangeRand(-3.f, 0.f));
    // move the target to a shaked position  
    m_pTarget->setPosition(m_StartPosition+Point(randx, randy));  
}  
  
void Shake::startWithTarget(Node *pTarget)  
{  
    ActionInterval::startWithTarget(pTarget);  
    m_pTarget = pTarget;  
    // save the initial position  
    m_StartPosition=pTarget->getPosition();  
}  
  
void Shake::stop(void)  
{  
    // Action is done, reset clip position  
    this->getTarget()->setPosition( m_StartPosition);  
      
    ActionInterval::stop();  
}  
  
ActionInterval* Shake::reverse() const  
{  
    return NULL;  
}  
  
ActionInterval* Shake::clone() const  
{  
    return NULL;  
}  
