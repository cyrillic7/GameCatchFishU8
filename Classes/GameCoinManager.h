
#ifndef COIN_MANAGER_H_
#define COIN_MANAGER_H_

#include "cocos2d.h"
#include "GameFishAction.h"
#include "GameAnimation.h"
USING_NS_CC;
class GameScene;

class CoinManager : public Ref {
 public:
  CoinManager();
  ~CoinManager();
  void purgeInstance();
  void init();
  static CoinManager* shareInstance();
  void BuildCoins(const Vec2& fish_pos, float fish_angle, const Vec2& turret_pos, SCORE score, FishKind fish_kind,GameScene* scene);
  
 private:
	int    mCoinWidth;
	int    mGoldCoinWidth;
};

#endif // COIN_MANAGER_H_
