
#ifndef GAME_MATHC_MANAGER_H_
#define GAME_MATHC_MANAGER_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "Model/MatchModel.h"
#include "GameGlobalSetting.h"
class GameScene;

using namespace ui;
USING_NS_CC;
USING_NS_CC_EXT;

enum enMatchStatus
{
	MG_NULL_STATUS=0,		//无状态
	MG_SIGNUPING,			//报名中
	MG_MATCHING,			//没有桌子预赛 等待桌子
};

class GameMatchManager : public Node
{
 public:
  GameMatchManager();
  ~GameMatchManager();
  void onEnter();
  void onExit();
  bool init(GameScene* pDelegate);
  static GameMatchManager* create(GameScene* pDelegate);
  void updateMatchRoom(float dt);
 private:
	GameScene* mDelegate;
	Node*            mTimeNode;//倒计时
	Node*            mTimeMatchNode; //定时赛信息节点
	Node*            mForeverMatchNode;//循环赛信息节点
	GameMatchModel* mData;

	Text*            mRankText;
	Text*            mCurScoreText;
	Text*            mMaxScoreText;
	Text*            mTotalScoreText;
	Text*            mPalyCountText;
	Text*            mMatchTimeText;

	ImageView*  mStartOrEnd;
	LabelAtlas*   mStartOrEndTimeLable;
	LabelAtlas*   mRedTimeLable;
	SCORE           mTimeSpan;
};

#endif // COIN_MANAGER_H_
